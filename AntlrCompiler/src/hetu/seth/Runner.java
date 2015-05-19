package hetu.seth;

import hetu.seth.res.Res;

import java.io.BufferedReader;
import java.io.BufferedWriter;
import java.io.FileReader;
import java.io.FileWriter;
import java.io.IOException;
import java.util.ArrayList;
import java.util.HashMap;
import java.util.Map;

public class Runner {
	//Toy example. Used to test various new and weird parse situations.
	private static void toy_example() {		
		EgmCompiler compiler = new EgmCompiler();
		
		//Resource names include everything, and are purely for syntax.
		compiler.addResource("sprites", new Res(1, "spr_player"));
		compiler.addResource("sprites", new Res(2, "spr_enemy"));
		compiler.addResource("scripts", new Res(1, "scr_jump"));
		compiler.addResource("objects", new Res(1, "obj_player"));
		
		//Scripts, objects, and timelines are defined in much greater details, and 
		// have interacting semantics.
		HashMap<String, String> scrDef1 = new HashMap<>();
		scrDef1.put("name", "scr_jump");
		scrDef1.put("id", "1");
		scrDef1.put("code", "//Init \n grav = 0.2; \n hsp = 0; \n vsp = hsp + self.spd; " + 
                "\n { jumpspeed = \"Hey there!\"; } \n movespeed = 4; \n with(obj_other) { movespeed = 6; } " +
	            "\n test2 = 1; \n go_here(); \n if (true) { obj_other.test3 = go_there(1,2,qsp); }");
		compiler.readScript(scrDef1); //TODO: "if" functionality
		
		HashMap<String, String> objDef1 = new HashMap<>();
		objDef1.put("name", "obj_player");
		objDef1.put("id", "1");
		objDef1.put("event-1-1", "//TODO");
		objDef1.put("event-2-2", "//TODO");
		compiler.readObject(objDef1);
		System.out.println("Done");
	}
	
	public static void main(String[] args) {
		System.out.println("New parser called with " + args.length + " args.");
		if (args.length==0) {
			//Just run some testing code.
			toy_example();
			return;
		}

		//We're running the actual parser.
		String main_dir = args[0];
		String editable_dir = main_dir + "Preprocessor_Environment_Editable/";
		System.out.println("main_dir is " + main_dir);
		System.out.println("editable_dir is " + editable_dir);
		try {
			EgmCompiler compiler = new EgmCompiler();
			//Parse the resource list
			BufferedReader resList = new BufferedReader(new FileReader(main_dir + "parser_resources.txt"));
			ReadResourceList(resList, compiler);
			resList.close();
			
			//Parse scripts
			//TODO
			
			//Parse timelines
			BufferedReader tmList = new BufferedReader(new FileReader(main_dir + "parser_timelines.txt"));
			ArrayList<HashMap<String, String>> tmlines = ReadSpecialList(tmList, compiler, "timeline_done", "moment-");
			tmList.close();
			for (HashMap<String,String> props : tmlines) {
				compiler.readTimeline(props);
			}
			
			//Parse objects
			BufferedReader objList = new BufferedReader(new FileReader(main_dir + "parser_objects.txt"));
			ArrayList<HashMap<String, String>> objs = ReadSpecialList(objList, compiler, "object_done", "event-");
			objList.close();
			for (HashMap<String,String> props : objs) {
				compiler.readObject(props);
			}
			
			//TODO
			
			//Now, write the output files.
			BufferedWriter file;
			
			//The switch is simple; let's do that one first!
			file = new BufferedWriter(new FileWriter(editable_dir + "IDE_EDIT_object_switch.h"));
			file.write(EgmLicense.Text);
			compiler.writeObjectSwitch(file);
			file.close();
			
			//This one's also pretty easy.
			file = new BufferedWriter(new FileWriter(editable_dir + "IDE_EDIT_resourcenames.h"));
			file.write(EgmLicense.Text);
			compiler.writeResourceNames(file);
			file.close();


			
			//TODO: Actually write them.
			file = new BufferedWriter(new FileWriter(editable_dir + "IDE_EDIT_events.h"));
			file.close();
			file = new BufferedWriter(new FileWriter(editable_dir + "IDE_EDIT_evparent.h"));
			file.close();
			file = new BufferedWriter(new FileWriter(editable_dir + "IDE_EDIT_globals.h"));
			file.close();
			file = new BufferedWriter(new FileWriter(editable_dir + "IDE_EDIT_objectaccess.h"));
			file.close();
			file = new BufferedWriter(new FileWriter(editable_dir + "IDE_EDIT_objectdeclarations.h"));
			file.close();
			file = new BufferedWriter(new FileWriter(editable_dir + "IDE_EDIT_objectfunctionality.h"));
			file.close();
			
			file = new BufferedWriter(new FileWriter(editable_dir + "IDE_EDIT_roomarrays.h"));
			file.close();
			file = new BufferedWriter(new FileWriter(editable_dir + "IDE_EDIT_roomcreates.h"));
			file.close();
			file = new BufferedWriter(new FileWriter(editable_dir + "IDE_EDIT_timelines.h"));
			file.close();
			file = new BufferedWriter(new FileWriter(editable_dir + "IDE_EDIT_whitespace.h"));
			file.close();
			file = new BufferedWriter(new FileWriter(editable_dir + "LIBINCLUDE.h"));
			file.close();
			
			
		} catch (Exception ex) {
			System.out.println("Error: " + ex.getClass().getName() + " : " + ex.getMessage());
			ex.printStackTrace();
			System.exit(1);
		}
		
		System.out.println("Done");
	}

	private static void ReadResourceList(BufferedReader resList, EgmCompiler compiler) throws IOException {
		String resType = null;
		int resLeft = 0;
		for (String line=resList.readLine();line!=null;line=resList.readLine()) {
			//Skip WS
			line = line.trim();
			if (line.isEmpty()) { continue; }
			
			//Always expect two parts
			String[] parts = line.split(":");
			if (parts.length != 2) {
				throw new RuntimeException("Expected A:B, but received: \"" + line + "\"");
			}
			
			//New resource type?
			if (resLeft == 0) {
				resType = parts[0];
				resLeft = Integer.parseInt(parts[1]);
				if (resLeft < 0) {
					throw new RuntimeException("Bad resource count: \"" + line + "\"");
				}
			} else {
				int id = Integer.parseInt(parts[0]);
				String name = parts[1];
				
				Res res = new Res(id, name);
				//if (resType.equals("object")) {} //TODO: Different types for object, scripts, etc.
				
				compiler.addResource(resType, res);
				resLeft -= 1;
			}
		}
	}
	
	private static ArrayList<HashMap<String, String>> ReadSpecialList(BufferedReader srcFile, EgmCompiler compiler, String doneStr, String multiLineStr) throws IOException {
		ArrayList<HashMap<String, String>> res = new ArrayList<HashMap<String,String>>();
		HashMap<String, String> props = new HashMap<>(); //Key/value for the current entry.
		for (String line=srcFile.readLine();line!=null;line=srcFile.readLine()) {
			//Skip WS
			line = line.trim();
			if (line.isEmpty()) { continue; }
			
			//Done?
			if (line.equals(doneStr)) {
				//Save, restart.
				res.add(props);
				props = new HashMap<>();
			} else {
				String[] parts = line.split(":");
				if (parts.length != 2) {
					throw new RuntimeException("Expected special property, but received: \"" + line + "\"");
				}
				//Events involve reading multiple additional lines.
				if (parts[0].startsWith(multiLineStr)) {
					int numLines = Integer.parseInt(parts[1]);
					StringBuilder sb = new StringBuilder();
					for (int i=0; i<numLines; i++) {
						line=srcFile.readLine();
						if (line == null) {
							throw new RuntimeException("Ran out of lines parsing special with " + numLines+ " lines.");
						}
						sb.append(line.trim()).append("\n");
					}
					props.put(parts[0],  sb.toString());
				} else {
					//Simple props are simple
					props.put(parts[0],  parts[1]);
				}
			}
		}
		return res;
	}

}
