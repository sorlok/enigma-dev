package hetu.seth;

import hetu.seth.WalkTagVariables;
import hetu.seth.gen.EgmLexer;
import hetu.seth.gen.EgmParser;
import hetu.seth.res.Game;
import hetu.seth.res.Res;

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
	public void addResource(String category, Res res) {		
		//Add it to the resource list.
		if (resourceNames.contains(res.name)) { System.out.println("Warning: Resource with duplicate name \"" + res.name + "\""); }
		resourceNames.add(res.name);
		
		//Add it to the right category.
		TreeMap<Integer, Res> mapping = game.Lookup.get(category);
		if (mapping == null) { throw new RuntimeException("Unexpected category: " + category); }
		if (mapping.containsKey(res.id)) { throw new RuntimeException("Duplicate id for: " + res.name); }
		mapping.put(new Integer(res.id), res);
	}
	
	private static int GetId(HashMap<String, String> objectProps) {
		String idStr = objectProps.get("id");
		if (idStr==null) { throw new RuntimeException("Resource missing required property: id"); }
		return Integer.parseInt(idStr);
	}
	
	public void readScript(HashMap<String, String> props) {
		//TODO: Custom script type, and add a "parsed" flag to check for double-setting.
		Res script = game.scripts.get(GetId(props));
		if (script == null) { throw new RuntimeException("Undefined script"); }
		
		parseCode("//source", new CodeGen()); //TODO
	}
	
	//objectSource is {eventName => eventSource}
	public void readObject(HashMap<String, String> props) {
		//TODO: Custom object type, and add a "parsed" flag to check for double-setting.
		Res obj = game.objects.get(GetId(props));
		if (obj == null) { throw new RuntimeException("Undefined object"); }

		//Save and parse each of its events
		//TODO
		/*for (Map.Entry<String,String> entry : objectProps.entrySet()) {
			if (entry.getKey().startsWith("event-")) {
				//parseCode(entry.getValue(), objects.get(id).get(entry.getKey()));
				parseCode("//source", new CodeGen()); //TODO
			} else {
				//Save non-code props for later.
				if (resources.get(name).get(entry.getKey())!=null) { throw new RuntimeException("Object has duplicate property."); }
				resources.get(name).put(entry.getKey(), entry.getValue());
			}
		}*/
	}
	
	public void readTimeline(HashMap<String, String> props) {
		//TODO: Custom timeline type, and add a "parsed" flag to check for double-setting.
		Res tm = game.timelines.get(GetId(props));
		if (tm== null) { throw new RuntimeException("Undefined timeline"); }

		//Save and parse each of its events
		//TODO
		/*for (Map.Entry<String,String> entry : objectProps.entrySet()) {
			if (entry.getKey().startsWith("event-")) {
				//parseCode(entry.getValue(), objects.get(id).get(entry.getKey()));
				parseCode("//source", new CodeGen()); //TODO
			} else {
				//Save non-code props for later.
				if (resources.get(name).get(entry.getKey())!=null) { throw new RuntimeException("Object has duplicate property."); }
				resources.get(name).put(entry.getKey(), entry.getValue());
			}
		}*/
	}
	
	//Write output file.
	public void writeObjectSwitch(BufferedWriter file) throws IOException {
		file.write("#ifndef NEW_OBJ_PREFIX\n#  define NEW_OBJ_PREFIX\n#endif\n\n");
		for (Res entry : game.objects.values()) {
			file.write("case " + entry.id + ":\n");
			file.write("    NEW_OBJ_PREFIX new enigma::OBJ_" + entry.name + "(x,y,idn);\n");
			file.write("  break;\n");
		}
		
	    file.write("\n\n#undef NEW_OBJ_PREFIX\n");
	}
		
	//Write output file.
	public void writeResourceNames(BufferedWriter file) throws IOException {
		writeSingleResource(file, "object", game.objects, true);
		writeSingleResource(file, "sprite", game.sprites, true);
		writeSingleResource(file, "background", game.backgrounds, true);
		writeSingleResource(file, "font", game.fonts, true);
		writeSingleResource(file, "timeline", game.timelines, true);
		writeSingleResource(file, "path", game.paths, true);
		writeSingleResource(file, "sound", game.sounds, true);
		writeSingleResource(file, "script", game.scripts, true);
		writeSingleResource(file, "shader", game.shaders, true);
		writeSingleResource(file, "room", game.rooms, false);
	}
	
	private void writeSingleResource(BufferedWriter file, String category, TreeMap<Integer, Res> entries, boolean secondHalf) throws IOException {
		int max = 0;
		StringBuffer ss = new StringBuffer();
		file.write("namespace enigma_user {\nenum //" + category + " names\n{\n");
		for (Res res : entries.values()) {
			if (res.id >= max) { max = res.id + 1; }
			file.write("  " + res.name + " = " + res.id + ",\n");
			if (secondHalf) {
				ss.append("    case " + res.id + ": return \"" + res.name + "\"; break;\n");
			}
		}
		file.write("};\n}\nnamespace enigma { size_t " + category + "_idmax = " + max + "; }\n\n");
		
		if (secondHalf) {
			file.write("namespace enigma_user {\nstring " + category + "_get_name(int i) {\n switch (i) {\n");
			file.write(ss.toString() + " default: return \"<undefined>\";}};}\n\n");
		}
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
	
	//List of all resource names. 
	TreeSet<String> resourceNames = new TreeSet<>();
	
	//The current game state.
	Game game = new Game();
	
	//A list of all valid resource names. This includes sprites, sounds, etc.
	//The value is a key-value property set (may be empty) that is used primarily for complex resources like objects.
	//private TreeMap<String, TreeMap<String, String>> resources = new TreeMap<>();
	
	//A list of all scripts and their associated CodeGen objects.
	//private TreeMap<String, CodeGen> scripts = new TreeMap<>();
	
	//A list of all objects and their associated CodeGen objects. (second key is event-mainEvId-subEvId)
	//private TreeMap<Integer, TreeMap<String, CodeGen>> objects = new TreeMap<>();
	
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
