package hetu.seth;

import hetu.seth.WalkTagVariables;
import hetu.seth.gen.EgmLexer;
import hetu.seth.gen.EgmParser;

import java.io.BufferedWriter;
import java.io.ByteArrayInputStream;
import java.io.FileNotFoundException;
import java.io.FileWriter;
import java.io.IOException;
import java.util.ArrayList;
import java.util.BitSet;
import java.util.HashMap;
import java.util.Map;
import java.util.TreeMap;
import java.util.TreeSet;

import org.antlr.v4.runtime.ANTLRErrorListener;
import org.antlr.v4.runtime.ANTLRInputStream;
import org.antlr.v4.runtime.CommonTokenStream;
import org.antlr.v4.runtime.Parser;
import org.antlr.v4.runtime.RecognitionException;
import org.antlr.v4.runtime.Recognizer;
import org.antlr.v4.runtime.atn.ATNConfigSet;
import org.antlr.v4.runtime.dfa.DFA;
import org.antlr.v4.runtime.tree.ParseTree;
import org.antlr.v4.runtime.tree.ParseTreeWalker;

public class EgmCompiler {
	public void readResource(String resourceName) {
		if (resources.get(resourceName)!=null) { throw new RuntimeException("Resource with duplicate name."); }
		resources.put(resourceName, new TreeMap<String,String>());
	}
	
	public void readScript(String scriptName, String scriptSource) {
		if (scripts.get(scriptName)!=null) { throw new RuntimeException("Script with duplicate name."); }
		scripts.put(scriptName, new CodeGen());
		parseCode(scriptSource, scripts.get(scriptName));
	}
	
	//objectSource is {eventName => eventSource}
	public void readObject(HashMap<String, String> objectProps) {
		//Basic properties
		String name = objectProps.get("name");
		String idStr = objectProps.get("id");
		if (name==null) { throw new RuntimeException("Object missing required property: name"); }
		if (idStr==null) { throw new RuntimeException("Object missing required property: id"); }
		int id = Integer.parseInt(idStr);
		
		//Save it.
		if (resources.get(name)==null) { throw new RuntimeException("Object not specified in the resource tree."); }
		if (objects.get(id)!=null) { throw new RuntimeException("Object with duplicate id specified."); }
		objects.put(id, new TreeMap<String, CodeGen>());
		
		//Save and parse each of its events
		for (Map.Entry<String,String> entry : objectProps.entrySet()) {
			if (entry.getKey().startsWith("event-")) {
				parseCode(entry.getValue(), objects.get(id).get(entry.getKey()));
			} else {
				//Save non-code props for later.
				if (resources.get(name).get(entry.getKey())!=null) { throw new RuntimeException("Object has duplicate property."); }
				resources.get(name).put(entry.getKey(), entry.getValue());
			}
		}
	}
	
	//Write output file.
	public void writeObjectSwitch(BufferedWriter file) throws IOException {
		//TODO: We need objects in order by ID, but accessible by name/prop. 
		//      Our current way of storing data overlaps; for now we just force it.
		TreeMap<Integer, String> objs = new TreeMap<>();
		for (Map.Entry<String, TreeMap<String, String>> entry : resources.entrySet()) { //TODO: This will break once we add scripts/timelines!
			if (entry.getValue().isEmpty()) { continue; }
			if (!entry.getKey().startsWith("obj_")) { throw new RuntimeException("TODO: Better storage of objects in transit."); }
			Integer key = Integer.parseInt(entry.getValue().get("id"));
			String value = entry.getValue().get("name");
			objs.put(key, value);
		}
		
		file.write("#ifndef NEW_OBJ_PREFIX\n#  define NEW_OBJ_PREFIX\n#endif\n\n");
		for (Map.Entry<Integer, String> entry : objs.entrySet()) {
			file.write("case " + entry.getKey() + ":\n");
			file.write("    NEW_OBJ_PREFIX new enigma::OBJ_" + entry.getValue() + "(x,y,idn);\n");
			file.write("  break;\n");
		}
		
	    file.write("\n\n#undef NEW_OBJ_PREFIX\n");
	}
	
	
	private void parseCode(String source, CodeGen codeGen) {
		//Set up input.
		ANTLRInputStream input = null;
		try {
			input = new ANTLRInputStream(new ByteArrayInputStream(source.getBytes("UTF-8")));
		} catch (FileNotFoundException ex) {
			System.out.println("File not found!");
			return;
		} catch (IOException ex) {
			System.out.println("I/O error!");
			return;
		}
        EgmLexer lexer = new EgmLexer(input);
        lexer.addErrorListener(new ANTLRErrorListener() {
			public void syntaxError(Recognizer<?, ?> arg0, Object arg1, int arg2, int arg3, String arg4, RecognitionException arg5) { throw new RuntimeException("Lexer error 1"); }
			public void reportContextSensitivity(Parser arg0, DFA arg1, int arg2, int arg3, int arg4, ATNConfigSet arg5) { throw new RuntimeException("Lexer error 2"); }
			public void reportAttemptingFullContext(Parser arg0, DFA arg1, int arg2, int arg3, BitSet arg4, ATNConfigSet arg5) { throw new RuntimeException("Lexer error 3"); }
			public void reportAmbiguity(Parser arg0, DFA arg1, int arg2, int arg3, boolean arg4, BitSet arg5, ATNConfigSet arg6) { throw new RuntimeException("Lexer error 4"); }
		});
        
        //Tokenize.
        CommonTokenStream tokens = new CommonTokenStream(lexer);
        
        //Parse to a tree.
        EgmParser parser = new EgmParser(tokens);
        ParseTree tree = parser.rootRule();

        //Walk the tree, tagging all variables.
        ParseTreeWalker walker = new ParseTreeWalker();
        walker.walk(new WalkTagVariables(codeGen), tree);
	}
	
	
	//A list of all valid resource names. This includes sprites, sounds, etc.
	//The value is a key-value property set (may be empty) that is used primarily for complex resources like objects.
	private TreeMap<String, TreeMap<String, String>> resources = new TreeMap<>();
	
	//A list of all scripts and their associated CodeGen objects.
	private TreeMap<String, CodeGen> scripts = new TreeMap<>();
	
	//A list of all objects and their associated CodeGen objects. (second key is event-mainEvId-subEvId)
	private TreeMap<Integer, TreeMap<String, CodeGen>> objects = new TreeMap<>();
	
	//Book-keeping structure for code that is being generated for a script or object-event.
	public static class CodeGen {
		//List of variables required by each scope. The key can be "self", "other", "obj_player", etc.
		public TreeMap<String, ArrayList<VarDeclare>> variables = new TreeMap<>();
		
	}
	
	//Represents a variable required by a given object.
	public static class VarDeclare {
		public String name;
		public boolean isLHS; //If false, it should be defined elsewhere (or the "allow uninitialized variables" flag can be set).
	}
}
