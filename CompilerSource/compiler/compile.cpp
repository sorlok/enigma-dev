/** Copyright (C) 2011 Josh Ventura
*** Copyright (C) 2014 Seth N. Hetu
***
*** This file is a part of the ENIGMA Development Environment.
***
*** ENIGMA is free software: you can redistribute it and/or modify it under the
*** terms of the GNU General Public License as published by the Free Software
*** Foundation, version 3 of the license or any later version.
***
*** This application and its source code is distributed AS-IS, WITHOUT ANY
*** WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS
*** FOR A PARTICULAR PURPOSE. See the GNU General Public License for more
*** details.
***
*** You should have received a copy of the GNU General Public License along
*** with this code. If not, see <http://www.gnu.org/licenses/>
**/

#include "makedir.h"
#include "OS_Switchboard.h" //Tell us where the hell we are
#include "backend/EnigmaStruct.h" //LateralGM interface structures

#include "general/darray.h"

 #include <cstdio>

#ifdef _WIN32
 #define dllexport extern "C" __declspec(dllexport)
 #include <windows.h>
 #define sleep Sleep
#else
 #define dllexport extern "C"
 #include <unistd.h>
 #define sleep(x) usleep(x * 1000)
#endif

#define idpr(x,y) \
  ide_dia_progress_text(x); \
  ide_dia_progress(y);

#include <map>
#include <set>
#include <string>
#include <vector>
#include <iostream>
#include <sstream>
#include <algorithm>
#include <fstream>
//#include <dirent.h>
#include <sys/stat.h>
#include "backend/ideprint.h"

using namespace std;

#include "backend/JavaCallbacks.h"
#include "syntax/syncheck.h"
#include "parser/parser.h"
#include "compile_includes.h"
#include "compile_common.h"

#include "settings-parse/crawler.h"
#include "settings-parse/eyaml.h"

#include "components/components.h"
#include "gcc_interface/gcc_backend.h"

#include "general/bettersystem.h"
#include "event_reader/event_parser.h"

#include "languages/lang_CPP.h"

#include "compiler/jdi_utility.h"

#ifdef WRITE_UNIMPLEMENTED_TXT
std::map <string, char> unimplemented_function_list;
#endif

inline void writei(int x, FILE *f) {
  fwrite(&x,4,1,f);
}
inline void writef(float x, FILE *f) {
  fwrite(&x,4,1,f);
}

inline string string_replace_all(string str,string substr,string nstr)
{
  pt pos=0;
  while ((pos=str.find(substr,pos)) != string::npos)
  {
    str.replace(pos,substr.length(),nstr);
    pos+=nstr.length();
  }
  return str;
}

inline string fc(const char* fn)
{
  FILE *file = fopen(fn,"rb");
  if (!file) return "";

  fseek(file, 0, SEEK_END);
  size_t sz = ftell(file);
  fseek(file, 0, SEEK_SET);

  char *a = new char[sz];
  sz = fread(a, 1, sz, file);
  fclose(file);

  string res(a, sz);
  delete[] a;
  return res;
}

#include "System/builtins.h"

dllexport int compileEGMf(EnigmaStruct *es, const char* exe_filename, int mode) {
  return current_language->compile(es, exe_filename, mode);
}

//Error macro, relies on "res".
#define irrr() if (res) { idpr("Error occurred; see scrollback for details.",-1); return res; }


int parse_new(EnigmaStruct *es) 
{
  //Our command is pretty simple.
  std::string parseCmd = std::string("java -cp \"/home/sethhetu/source/enigma-dev/AntlrCompiler/bin:/home/sethhetu/bin/antlr-4.5-complete.jar\" hetu.seth.Runner  " + makedir);

  edbg << "Running NEW parser as \"" <<parseCmd << "\"" << flushl;

  // Pick a file and flush it
  const string redirfile = (makedir + "parser.log");
  fclose(fopen(redirfile.c_str(),"wb"));

  // Redirect it
  ide_output_redirect_file(redirfile.c_str()); //TODO: If you pass this function the address it will screw up the value; most likely a JNA/Plugin bug.
  int parseres = e_execs(parseCmd,"&> \"" + redirfile + "\"");

  // Stop redirecting output
  ide_output_redirect_reset();

  return parseres;
}

int parse_old(EnigmaStruct *es, int mode) //Mode used for "debug", etc. information
{
  /// Next we do a simple parse of the code, scouting for some variable names and adding semicolons.

  idpr("Checking Syntax and performing Preliminary Parsing",2);

  edbg << "SYNTAX CHECKING AND PRIMARY PARSING:" << flushl;

  edbg << es->scriptCount << " Scripts:" << flushl;
  parsed_script *parsed_scripts[es->scriptCount];

  //parsed timelines involve N timelines with M moments each. So we just store them in a large vector rather than a messy 2-D array of pointers.
  vector<parsed_script*> parsed_tlines;

  scr_lookup.clear();
  used_funcs::zero();

  int res;

  //The parser (and, to some extent, the compiler) needs knowledge of script names for various optimizations.
  std::set<std::string> script_names;
  for (int i = 0; i < es->scriptCount; i++)
    script_names.insert(es->scripts[i].name);

  res = current_language->compile_parseAndLink(es,parsed_scripts, parsed_tlines, script_names);
  irrr();


  //Export resources to each file.

  ofstream wto;
  idpr("Outputting Resources in Various Places...",10);

  //NEXT FILE ----------------------------------------
  //Object switch: A listing of all object IDs and the code to allocate them.
  edbg << "Writing object switch" << flushl;
  wto.open((makedir +"Preprocessor_Environment_Editable/IDE_EDIT_object_switch.h").c_str(),ios_base::out);
    wto << license;
    wto << "#ifndef NEW_OBJ_PREFIX\n#  define NEW_OBJ_PREFIX\n#endif\n\n";
    for (po_i i = parsed_objects.begin(); i != parsed_objects.end(); i++)
    {
      wto << "case " << i->second->id << ":\n";
      wto << "    NEW_OBJ_PREFIX new enigma::OBJ_" << i->second->name <<"(x,y,idn);\n";
      wto << "  break;\n";
    }
    wto << "\n\n#undef NEW_OBJ_PREFIX\n";
  wto.close();


  //NEXT FILE ----------------------------------------
  //Resource names: Defines integer constants for all resources.
  int max;
  edbg << "Writing resource names and maxima" << flushl;
  wto.open((makedir +"Preprocessor_Environment_Editable/IDE_EDIT_resourcenames.h").c_str(),ios_base::out);
    wto << license;

stringstream ss;

    max = 0;
    wto << "namespace enigma_user {\nenum //object names\n{\n";
    for (po_i i = parsed_objects.begin(); i != parsed_objects.end(); i++) {
      if (i->first >= max) max = i->first + 1;
      wto << "  " << i->second->name << " = " << i->first << ",\n";
      ss << "    case " << i->first << ": return \"" << i->second->name << "\"; break;\n";
    } wto << "};\n}\nnamespace enigma { size_t object_idmax = " << max << "; }\n\n";

    wto << "namespace enigma_user {\nstring object_get_name(int i) {\n switch (i) {\n";
     wto << ss.str() << " default: return \"<undefined>\";}};}\n\n";
     ss.str( "" );

    max = 0;
    wto << "namespace enigma_user {\nenum //sprite names\n{\n";
    for (int i = 0; i < es->spriteCount; i++) {
      if (es->sprites[i].id >= max) max = es->sprites[i].id + 1;
      wto << "  " << es->sprites[i].name << " = " << es->sprites[i].id << ",\n";
      ss << "    case " << es->sprites[i].id << ": return \"" << es->sprites[i].name << "\"; break;\n";
    } wto << "};}\nnamespace enigma { size_t sprite_idmax = " << max << "; }\n\n";

     wto << "namespace enigma_user {\nstring sprite_get_name(int i) {\n switch (i) {\n";
     wto << ss.str() << " default: return \"<undefined>\";}};}\n\n";
     ss.str( "" );

    max = 0;
    wto << "namespace enigma_user {\nenum //background names\n{\n";
    for (int i = 0; i < es->backgroundCount; i++) {
      if (es->backgrounds[i].id >= max) max = es->backgrounds[i].id + 1;
      wto << "  " << es->backgrounds[i].name << " = " << es->backgrounds[i].id << ",\n";
      ss << "    case " << es->backgrounds[i].id << ": return \"" << es->backgrounds[i].name << "\"; break;\n";
    } wto << "};}\nnamespace enigma { size_t background_idmax = " << max << "; }\n\n";

     wto << "namespace enigma_user {\nstring background_get_name(int i) {\n switch (i) {\n";
     wto << ss.str() << " default: return \"<undefined>\";}};}\n\n";
     ss.str( "" );

    max = 0;
    wto << "namespace enigma_user {\nenum //font names\n{\n";
    for (int i = 0; i < es->fontCount; i++) {
      if (es->fonts[i].id >= max) max = es->fonts[i].id + 1;
      wto << "  " << es->fonts[i].name << " = " << es->fonts[i].id << ",\n";
      ss << "    case " << es->fonts[i].id << ": return \"" << es->fonts[i].name << "\"; break;\n";
    } wto << "};}\nnamespace enigma { size_t font_idmax = " << max << "; }\n\n";

     wto << "namespace enigma_user {\nstring font_get_name(int i) {\n switch (i) {\n";
     wto << ss.str() << " default: return \"<undefined>\";}};}\n\n";
     ss.str( "" );

    max = 0;
	wto << "namespace enigma_user {\nenum //timeline names\n{\n";
	for (int i = 0; i < es->timelineCount; i++) {
	    if (es->timelines[i].id >= max) max = es->timelines[i].id + 1;
        wto << "  " << es->timelines[i].name << " = " << es->timelines[i].id << ",\n";
        ss << "    case " << es->timelines[i].id << ": return \"" << es->timelines[i].name << "\"; break;\n";
	} wto << "};}\nnamespace enigma { size_t timeline_idmax = " << max << "; }\n\n";

wto << "namespace enigma_user {\nstring timeline_get_name(int i) {\n switch (i) {\n";
     wto << ss.str() << " default: return \"<undefined>\";}};}\n\n";
     ss.str( "" );

    max = 0;
	wto << "namespace enigma_user {\nenum //path names\n{\n";
	for (int i = 0; i < es->pathCount; i++) {
	    if (es->paths[i].id >= max) max = es->paths[i].id + 1;
        wto << "  " << es->paths[i].name << " = " << es->paths[i].id << ",\n";
        ss << "    case " << es->paths[i].id << ": return \"" << es->paths[i].name << "\"; break;\n";
	} wto << "};}\nnamespace enigma { size_t path_idmax = " << max << "; }\n\n";

wto << "namespace enigma_user {\nstring path_get_name(int i) {\n switch (i) {\n";
     wto << ss.str() << " default: return \"<undefined>\";}};}\n\n";
     ss.str( "" );

    max = 0;
    wto << "namespace enigma_user {\nenum //sound names\n{\n";
    for (int i = 0; i < es->soundCount; i++) {
      if (es->sounds[i].id >= max) max = es->sounds[i].id + 1;
      wto << "  " << es->sounds[i].name << " = " << es->sounds[i].id << ",\n";
      ss << "    case " << es->sounds[i].id << ": return \"" << es->sounds[i].name << "\"; break;\n";
    } wto << "};}\nnamespace enigma { size_t sound_idmax = " <<max << "; }\n\n";

wto << "namespace enigma_user {\nstring sound_get_name(int i) {\n switch (i) {\n";
     wto << ss.str() << " default: return \"<undefined>\";}};}\n\n";
     ss.str( "" );

    max = 0;
    wto << "namespace enigma_user {\nenum //script names\n{\n";
    for (int i = 0; i < es->scriptCount; i++) {
      if (es->scripts[i].id >= max) max = es->scripts[i].id + 1;
      wto << "  " << es->scripts[i].name << " = " << es->scripts[i].id << ",\n";
      ss << "    case " << es->scripts[i].id << ": return \"" << es->scripts[i].name << "\"; break;\n";
    } wto << "};}\nnamespace enigma { size_t script_idmax = " <<max << "; }\n\n";

wto << "namespace enigma_user {\nstring script_get_name(int i) {\n switch (i) {\n";
     wto << ss.str() << " default: return \"<undefined>\";}};}\n\n";
     ss.str( "" );

    max = 0;
    wto << "namespace enigma_user {\nenum //shader names\n{\n";
    for (int i = 0; i < es->shaderCount; i++) {
      if (es->shaders[i].id >= max) max = es->shaders[i].id + 1;
      wto << "  " << es->shaders[i].name << " = " << es->shaders[i].id << ",\n";
      ss << "    case " << es->shaders[i].id << ": return \"" << es->shaders[i].name << "\"; break;\n";
    } wto << "};}\nnamespace enigma { size_t shader_idmax = " <<max << "; }\n\n";

wto << "namespace enigma_user {\nstring shader_get_name(int i) {\n switch (i) {\n";
     wto << ss.str() << " default: return \"<undefined>\";}};}\n\n";
     ss.str( "" );
	 
    max = 0;
    wto << "namespace enigma_user {\nenum //room names\n{\n";
    for (int i = 0; i < es->roomCount; i++) {
      if (es->rooms[i].id >= max) max = es->rooms[i].id + 1;
      wto << "  " << es->rooms[i].name << " = " << es->rooms[i].id << ",\n";
    }
    wto << "};}\nnamespace enigma { size_t room_idmax = " <<max << "; }\n\n";
  wto.close();


  //NEXT FILE ----------------------------------------
  //Timelines: Defines "moment" lookup structures for timelines.
  edbg << "Writing timeline control information" << flushl;
  wto.open((makedir +"Preprocessor_Environment_Editable/IDE_EDIT_timelines.h").c_str(),ios_base::out);
  {
    wto << license;
    wto <<"namespace enigma {\n\n";

    //Each timeline has a lookup structure (in this case, a map) which allows easy forward/backward lookup.
    //This is currently constructed rather manually; there are probably more efficient 
    // construction techniques, but none come to mind.
    wto <<"void timeline_system_initialize() {\n";
    wto <<"  std::vector< std::map<int, int> >& res = object_timelines::timeline_moments_maps;\n";
    wto <<"  res.reserve(" <<es->timelineCount <<");\n";
    wto <<"  std::map<int, int> curr;\n\n";
    for (int i=0; i<es->timelineCount; i++) {
      wto <<"  curr.clear();\n";
      for (int j=0; j<es->timelines[i].momentCount; j++) {
        wto <<"  curr[" <<es->timelines[i].moments[j].stepNo <<"] = " <<j <<";\n";
      }
      wto <<"  res.push_back(curr);\n\n";
    }
    wto <<"}\n\n";

    wto <<"}\n"; //namespace 
  }
  wto.close();


  idpr("Performing Secondary Parsing and Writing Globals",25);

  parsed_object EGMglobal;

  edbg << "Linking globals and ambiguous variables" << flushl;
  res = current_language->link_globals(&EGMglobal,es,parsed_scripts, parsed_tlines);
  res = current_language->link_ambiguous(&EGMglobal,es,parsed_scripts, parsed_tlines);
  irrr();

  edbg << "Running Secondary Parse Passes" << flushl;
  res = current_language->compile_parseSecondary(parsed_objects,parsed_scripts,es->scriptCount, parsed_tlines, parsed_rooms,&EGMglobal, script_names);
  
  edbg << "Writing events" << flushl;
  res = current_language->compile_writeDefraggedEvents(es);
  irrr();

  edbg << "Writing object data" << flushl;
  res = current_language->compile_writeObjectData(es,&EGMglobal,mode);
  irrr();

  edbg << "Writing local accessors" << flushl;
  res = current_language->compile_writeObjAccess(parsed_objects, &EGMglobal, es->gameSettings.treatUninitializedAs0);
  irrr();

  edbg << "Writing room data" << flushl;
  res = current_language->compile_writeRoomData(es,&EGMglobal,mode);
  irrr();

  // Write the global variables to their own file to be included before any of the objects
  res = current_language->compile_writeGlobals(es,&EGMglobal);
  irrr();


  // Now we write any additional templates requested by the window system.
  // compile_handle_templates(es);

#ifdef WRITE_UNIMPLEMENTED_TXT
    printf("write unimplemented functions %d",0);
    ofstream outputFile;
    outputFile.open("unimplementedfunctionnames.txt");

    for ( std::map< string, char, std::less< char > >::const_iterator iter = unimplemented_function_list.begin();
         iter != unimplemented_function_list.end(); ++iter )
    {
        outputFile << iter->first << '\t' << iter->second << '\n';
    }

    outputFile << endl;
    outputFile.close();
#endif

  return 0;
}


int lang_CPP::compile(EnigmaStruct *es, const char* exe_filename, int mode)
{ 
  cout << "Initializing dialog boxes" << endl;
    ide_dia_clear();
    ide_dia_open();
  cout << "Initialized." << endl;

  if (mode == emode_rebuild)
  {
    edbg << "Cleaning..." << flushl;

	string make = "clean-game ";
	string compilepath = CURRENT_PLATFORM_NAME "/" + extensions::targetOS.identifier;
	make += "COMPILEPATH=\"" + compilepath + "\" ";
	make += "WORKDIR=\"" + makedir + "\" ";
	make += "eTCpath=\"" + MAKE_tcpaths + "\"";

	edbg << "Full command line: " << MAKE_location << " " << make << flushl;
    e_execs(MAKE_location,make);

    edbg << "Done.\n" << flushl;
	idpr("Done.", 100);
	return 0;
  }
  edbg << "Building for mode (" << mode << ")" << flushl;
 
  // CLean up from any previous executions.

  edbg << "Cleaning up from previous executions" << flushl;
    parsed_objects.clear(); //Make sure we don't dump in any old object code...
    edbg << " - Cleared parsed objects" << flushl;
    parsed_rooms.clear();   //Or that we dump any room code, for that matter...
    edbg << " - Cleared room entries" << flushl;
    shared_locals_clear();  //Forget inherited locals, we'll reparse them
    edbg << " - Cleared shared locals list" << flushl;
    event_info_clear();     //Forget event definitions, we'll re-get them
    edbg << " - Cleared event info" << flushl;

  // Re-establish ourself
    // Read the global locals: locals that will be included with each instance
    {
      vector<string> extnp;
      for (int i = 0; i < es->extensionCount; i++) {
        cout << "Adding extension " << flushl << "extension " << flushl << es->extensions[i].path << flushl << ":" << endl << es->extensions[i].name << flushl;
        extnp.push_back(string(es->extensions[i].path) + es->extensions[i].name);
      }
      edbg << "Loading shared locals from extensions list" << flushl;
      if (shared_locals_load(extnp) != 0) {
        user << "Failed to determine locals; couldn't determine bottom tier: is ENIGMA configured correctly?";
        idpr("ENIGMA Misconfiguration",-1); return E_ERROR_LOAD_LOCALS;
      }
    }

  //Read the types of events
  event_parse_resourcefile();

  // Pick apart the sent resources
  edbg << "Location in memory of structure: " << (void*)es << flushl;
  if (es == NULL) {
    idpr("Java ENIGMA plugin dropped its ass.",-1);
    return E_ERROR_PLUGIN_FUCKED_UP;
  }


  /**** Segment One: This segment of the compile process is responsible for
  * @ * translating the code into C++. Basically, anything essential to the
  *//// compilation of said code is dealt with during this segment.

  ///The segment begins by adding resource names to the collection of variables that should not be automatically re-scoped.

  //Prepare a copy of all resource names, for the new parser.
  std::ofstream sidelist(makedir+"parser_resources.txt");

  //First, we make a space to put our globals.
  jdi::using_scope globals_scope("<ENIGMA Resources>", main_context->get_global());

  idpr("Copying resources",1);

  //Next, add the resource names to that list
  edbg << "Copying resources:" << flushl;
  edbg << "Copying sprite names [" << es->spriteCount << "]" << flushl;
  sidelist <<"sprites:" <<es->spriteCount <<"\n";
  for (int i = 0; i < es->spriteCount; i++) {
    sidelist <<es->sprites[i].id <<":" <<es->sprites[i].name <<"\n";
    quickmember_variable(&globals_scope,jdi::builtin_type__int,es->sprites[i].name);
  }

  edbg << "Copying sound names [" << es->soundCount << "]" << flushl;
  sidelist <<"sounds:" <<es->soundCount <<"\n";
  for (int i = 0; i < es->soundCount; i++) {
    sidelist <<es->sounds[i].id <<":" <<es->sounds[i].name <<"\n";
    quickmember_variable(&globals_scope,jdi::builtin_type__int,es->sounds[i].name);
  }

  edbg << "Copying background names [" << es->backgroundCount << "]" << flushl;
  sidelist <<"backgrounds:" <<es->backgroundCount <<"\n";
  for (int i = 0; i < es->backgroundCount; i++) {
    sidelist <<es->backgrounds[i].id <<":" <<es->backgrounds[i].name <<"\n";
    quickmember_variable(&globals_scope,jdi::builtin_type__int,es->backgrounds[i].name);
  }

  edbg << "Copying path names [" << es->pathCount << "]" << flushl;
  sidelist <<"paths:" <<es->pathCount <<"\n";
  for (int i = 0; i < es->pathCount; i++) {
    sidelist <<es->paths[i].id <<":" <<es->paths[i].name <<"\n";
    quickmember_variable(&globals_scope,jdi::builtin_type__int,es->paths[i].name);
  }

  edbg << "Copying script names [" << es->scriptCount << "]" << flushl;
  sidelist <<"scripts:" <<es->scriptCount <<"\n";
  for (int i = 0; i < es->scriptCount; i++) {
    sidelist <<es->scripts[i].id <<":" <<es->scripts[i].name <<"\n";
    quickmember_script(&globals_scope,es->scripts[i].name);
  }

  edbg << "Copying shader names [" << es->shaderCount << "]" << flushl;
  sidelist <<"shaders:" <<es->shaderCount <<"\n";
  for (int i = 0; i < es->shaderCount; i++) {
    sidelist <<es->shaders[i].id <<":" <<es->shaders[i].name <<"\n";
    quickmember_variable(&globals_scope,jdi::builtin_type__int,es->shaders[i].name);
  }

  edbg << "Copying font names [" << es->fontCount << "]" << flushl;
  sidelist <<"fonts:" <<es->fontCount <<"\n";
  for (int i = 0; i < es->fontCount; i++) {
    sidelist <<es->fonts[i].id <<":" <<es->fonts[i].name <<"\n";
    quickmember_variable(&globals_scope,jdi::builtin_type__int,es->fonts[i].name);
  }

  edbg << "Copying timeline names [" << es->timelineCount << "]" << flushl;
  sidelist <<"timelines:" <<es->timelineCount <<"\n";
  for (int i = 0; i < es->timelineCount; i++) {
    sidelist <<es->timelines[i].id <<":" <<es->timelines[i].name <<"\n";
    quickmember_variable(&globals_scope,jdi::builtin_type__int,es->timelines[i].name);
  }

  edbg << "Copying object names [" << es->gmObjectCount << "]" << flushl;
  sidelist <<"objects:" <<es->gmObjectCount <<"\n";
  for (int i = 0; i < es->gmObjectCount; i++) {
    sidelist <<es->gmObjects[i].id <<":" <<es->gmObjects[i].name <<"\n";
    quickmember_variable(&globals_scope,jdi::builtin_type__int,es->gmObjects[i].name);
  }

  edbg << "Copying room names [" << es->roomCount << "]" << flushl;
  sidelist <<"rooms:" <<es->roomCount <<"\n";
  for (int i = 0; i < es->roomCount; i++) {
    sidelist <<es->rooms[i].id <<":" <<es->rooms[i].name <<"\n";
    quickmember_variable(&globals_scope,jdi::builtin_type__int,es->rooms[i].name);
  }

  int fakeId = 0;
  edbg << "Copying constant names [" << es->constantCount << "]" << flushl;
  sidelist <<"constants:" <<es->constantCount <<"\n";
  for (int i = 0; i < es->constantCount; i++) {
    sidelist <<fakeId++ <<":" <<es->constants[i].name <<"\n";
    quickmember_variable(&globals_scope,jdi::builtin_type__int,es->constants[i].name);
  }

  //Done with the resource list.
  sidelist.close();

  //The details of each game object matter.
  std::ofstream objlist(makedir+"parser_objects.txt");
  for (int i = 0; i < es->gmObjectCount; i++) {
    //Save basic properties.
    objlist <<"id:" <<es->gmObjects[i].id <<"\n";
    objlist <<"name:" <<es->gmObjects[i].name <<"\n";
    objlist <<"spriteId:" <<es->gmObjects[i].spriteId <<"\n";
    objlist <<"maskId:" <<es->gmObjects[i].maskId <<"\n";
    objlist <<"parentId:" <<es->gmObjects[i].parentId <<"\n";
    objlist <<"visible:" <<es->gmObjects[i].visible <<"\n";
    objlist <<"solid:" <<es->gmObjects[i].solid <<"\n";
    objlist <<"depth:" <<es->gmObjects[i].depth <<"\n";
    objlist <<"persistent:" <<es->gmObjects[i].persistent <<"\n";

    //Save Event (Step) and Sub-Event (Begin Step) code.
    for (int ii=0; ii<es->gmObjects[i].mainEventCount; ii++) {
      if (es->gmObjects[i].mainEvents[ii].eventCount) {
        for (int iii = 0; iii < es->gmObjects[i].mainEvents[ii].eventCount; iii++) {
          //Make sure this event's code ends in a newline.
          std::string code = es->gmObjects[i].mainEvents[ii].events[iii].code;
          if (code[code.size()-1] != '\n') { code += "\n"; }

          //Write events as event-mainEvId-subEvId:X, followed by X lines of code.
          const int mainEvId = es->gmObjects[i].mainEvents[ii].id;
          const int subEvId = es->gmObjects[i].mainEvents[ii].events[iii].id;
          const size_t nlCount = std::count(code.begin(), code.end(), '\n');
          objlist <<"event-" <<mainEvId <<"-" <<subEvId <<":" <<nlCount <<"\n" <<code;
        }
      }
    }
    objlist <<"object_done\n";
  }
  objlist.close();

  //The details of the timelines matter.
  std::ofstream tmlist(makedir+"parser_timelines.txt");
  for (int i = 0; i < es->timelineCount; i++) {
    tmlist <<"id:" <<es->timelines[i].id <<"\n";
    tmlist <<"name:" <<es->timelines[i].name <<"\n";

    //Save Moment (Step) specific code.
    for (int j=0; j<es->timelines[i].momentCount; j++) {
      //Make sure this moment's code ends in a newline.
      std::string code = es->timelines[i].moments[j].code;
      if (code[code.size()-1] != '\n') { code += "\n"; }

      //Save as moment-stepNo:X, followed by X lines of code.
      const size_t nlCount = std::count(code.begin(), code.end(), '\n');
      tmlist <<"moment-" <<es->timelines[i].moments[j].stepNo <<":" <<nlCount <<"\n" <<code;
    }

    tmlist <<"timeline_done\n";
  }
  tmlist.close();



  // Some files don't need the new parser.
  GameSettings gameSet = es->gameSettings;
  edbg << "Writing executable information and resources." << flushl;
  ofstream wto;
  wto.open((makedir +"Preprocessor_Environment_Editable/Resources.rc").c_str(),ios_base::out);
    wto << license;
    wto << "#include <windows.h>\n";
	if (gameSet.gameIcon != NULL && strlen(gameSet.gameIcon) > 0) {
		wto << "IDI_MAIN_ICON ICON          \"" << string_replace_all(gameSet.gameIcon,"\\","/")  << "\"\n";
	}
	wto << "VS_VERSION_INFO VERSIONINFO\n";
	wto << "FILEVERSION " << gameSet.versionMajor << "," << gameSet.versionMinor << "," << gameSet.versionRelease << "," << gameSet.versionBuild << "\n";
	wto << "PRODUCTVERSION " << gameSet.versionMajor << "," << gameSet.versionMinor << "," << gameSet.versionRelease << "," << gameSet.versionBuild << "\n";
	wto << "BEGIN\n" << "BLOCK \"StringFileInfo\"\n" << "BEGIN\n" << "BLOCK \"040904E4\"\n" << "BEGIN\n";
	wto << "VALUE \"CompanyName\",         \"" << gameSet.company << "\"\n";
	wto << "VALUE \"FileDescription\",     \"" << gameSet.description << "\"\n";
	wto << "VALUE \"FileVersion\",         \"" << gameSet.version << "\\0\"\n";
	wto << "VALUE \"ProductName\",         \"" << gameSet.product << "\"\n";
	wto << "VALUE \"ProductVersion\",      \"" << gameSet.version << "\\0\"\n";
	wto << "VALUE \"LegalCopyright\",      \"" << gameSet.copyright << "\"\n";
	if (es->filename != NULL && strlen(es->filename) > 0) {
		wto << "VALUE \"OriginalFilename\",         \"" << string_replace_all(es->filename,"\\","/") << "\"\n";
	} else {
		wto << "VALUE \"OriginalFilename\",         \"\"\n";
	}
	wto << "END\nEND\nBLOCK \"VarFileInfo\"\nBEGIN\n";
	wto << "VALUE \"Translation\", 0x409, 1252\n";
	wto << "END\nEND";
  wto.close();

  edbg << "Writing modes and settings" << flushl;
  wto.open((makedir +"Preprocessor_Environment_Editable/GAME_SETTINGS.h").c_str(),ios_base::out);
    wto << license;
    wto << "#define ASSUMEZERO 0\n";
    wto << "#define PRIMBUFFER 0\n";
    wto << "#define PRIMDEPTH2 6\n";
    wto << "#define AUTOLOCALS 0\n";
    wto << "#define MODE3DVARS 0\n";
    wto << "void ABORT_ON_ALL_ERRORS() { " << (false?"game_end();":"") << " }\n";
    wto << '\n';
  wto.close();

  wto.open((makedir +"Preprocessor_Environment_Editable/IDE_EDIT_modesenabled.h").c_str(),ios_base::out);
    wto << license;
    wto << "#define BUILDMODE " << 0 << "\n";
    wto << "#define DEBUGMODE " << 0 << "\n";
    wto << '\n';
  wto.close();

  wto.open((makedir +"Preprocessor_Environment_Editable/IDE_EDIT_inherited_locals.h").c_str(),ios_base::out);
  wto.close();

  int res = 0;

  edbg << "Writing font data" << flushl;
  res = current_language->compile_writeFontInfo(es);
  irrr();

  edbg << "Writing shader data" << flushl;
  res = current_language->compile_writeShaderData(es,NULL);
  irrr();

  // Next step depends on whether we are using the old or new-style parser
  const bool NEW_PARSER = true;
  int parse_res = 0;
  if (NEW_PARSER) {
    parse_res = parse_new(es);
  } else {
    parse_res = parse_old(es, mode);
  }
  if (parse_res) {
    idpr("(New) parser failed to parse code.",-1);
    return E_ERROR_BUILD;
  }
  user << "******** New Parser Completed Successfully ******** \n";


  /**  * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
    Segment two: Now that the game has been exported as C++ and raw
    resources, our task is to compile the game itself via GNU Make.
  * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * **/

  idpr("Adding resources...",90);
  string desstr = "./ENIGMAsystem/SHELL/design_game" + extensions::targetOS.buildext;
  string gameFname = mode == emode_design ? desstr.c_str() : (desstr = exe_filename, exe_filename); // We will be using this first to write, then to run

  idpr("Starting compile (This may take a while...)", 30);

  string make = "Game ";

  make += "WORKDIR=\"" + makedir + "\" ";
  make += mode == emode_debug? "GMODE=Debug ": mode == emode_design? "GMODE=Design ": mode == emode_compile?"GMODE=Compile ": "GMODE=Run ";
  make += "GRAPHICS=" + extensions::targetAPI.graphicsSys + " ";
  make += "AUDIO=" + extensions::targetAPI.audioSys + " ";
  make += "COLLISION=" + extensions::targetAPI.collisionSys + " ";
  make += "WIDGETS="  + extensions::targetAPI.widgetSys + " ";
  make += "NETWORKING="  + extensions::targetAPI.networkSys + " ";
  make += "PLATFORM=" + extensions::targetAPI.windowSys + " ";

  if (CXX_override.length()) make += "CXX=" + CXX_override + " ";
  if (CC_override.length()) make += "CC=" + CC_override + " ";
  if (WINDRES_location.length()) make += "WINDRES=" + WINDRES_location + " ";

  if (mode != emode_debug) {
    if (TOPLEVEL_cflags.length()) make += "CFLAGS=\"" + TOPLEVEL_cflags + "\" ";
    if (TOPLEVEL_cppflags.length()) make += "CPPFLAGS=\"" + TOPLEVEL_cppflags + "\" ";
    if (TOPLEVEL_cxxflags.length()) make += "CXXFLAGS=\"" + TOPLEVEL_cxxflags + "\" ";
    if (TOPLEVEL_ldflags.length()) make += "LDFLAGS=\"" + TOPLEVEL_ldflags + "\" ";
  }
  else {
    if (TOPLEVEL_cflags.length()) make += "CFLAGS=\"" + TOPLEVEL_cflags + " -g -DDEBUG_MODE\" ";
    if (TOPLEVEL_cppflags.length()) make += "CPPFLAGS=\"" + TOPLEVEL_cppflags + "\" ";
    if (TOPLEVEL_cxxflags.length()) make += "CXXFLAGS=\"" + TOPLEVEL_cxxflags + " -g -DDEBUG_MODE\" ";
    if (TOPLEVEL_ldflags.length()) make += "LDFLAGS=\"" + TOPLEVEL_ldflags + "\" ";
  }

  string compilepath = CURRENT_PLATFORM_NAME "/" + extensions::targetOS.identifier;
  make += "COMPILEPATH=\"" + compilepath + "\" ";

  string extstr = "EXTENSIONS=\"";
  for (unsigned i = 0; i < parsed_extensions.size(); i++)
  	extstr += " " + parsed_extensions[i].pathname;
  make += extstr + "\"";

  string mfgfn = gameFname;
  for (size_t i = 0; i < mfgfn.length(); i++)
    if (mfgfn[i] == '\\') mfgfn[i] = '/';
  make += string(" OUTPUTNAME=\"") + mfgfn + "\" ";
  make += "eTCpath=\"" + MAKE_tcpaths + "\"";

  edbg << "Running make from `" << MAKE_location << "'" << flushl;
  edbg << "Full command line: " << MAKE_location << " " << make << flushl;

//  #if CURRENT_PLATFORM_ID == OS_MACOSX
  //int makeres = better_system("cd ","/MacOS/");
//  int makeres = better_system(MAKE_location,"MacOS");

  // Pick a file and flush it
  const string redirfile = (makedir + "enigma_compile.log");
  fclose(fopen(redirfile.c_str(),"wb"));

  // Redirect it
  ide_output_redirect_file(redirfile.c_str()); //TODO: If you pass this function the address it will screw up the value; most likely a JNA/Plugin bug.
  int makeres = e_execs(MAKE_location,make,"&> \"" + redirfile + "\"");

  // Stop redirecting GCC output
  ide_output_redirect_reset();

  if (makeres) {
    idpr("Compile failed at C++ level.",-1);
    return E_ERROR_BUILD;
  }
  user << "******** Make Completed Successfully ******** \n";

  /**  * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
    Segment three: Add resources into the game executable. They are
    literally tacked on to the end of the file for now. They should
    have an option in the config file to pass them to some resource
    linker sometime in the future.
  * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * **/

  #ifdef OS_ANDROID
    "Platforms/Android/EnigmaAndroidGame/libs/armeabi/libndkEnigmaGame.so";
  #endif

  FILE *gameModule;
  int resourceblock_start = 0;
  cout << "`" << extensions::targetOS.resfile << "` == '$exe': " << (extensions::targetOS.resfile == "$exe"?"true":"FALSE") << endl;
  if (extensions::targetOS.resfile == "$exe")
  {
    gameModule = fopen(gameFname.c_str(),"ab");
    if (!gameModule) {
      user << "Failed to append resources to the game. Did compile actually succeed?" << flushl;
      idpr("Failed to add resources.",-1); return 12;
    }

    fseek(gameModule,0,SEEK_END); //necessary on Windows for no reason.
    resourceblock_start = ftell(gameModule);

    if (resourceblock_start < 128) {
      user << "Compiled game is clearly not a working module; cannot continue" << flushl;
      idpr("Failed to add resources.",-1); return 13;
    }
  }
  else
  {
    string resname = extensions::targetOS.resfile;
    for (size_t p = resname.find("$exe"); p != string::npos; p = resname.find("$game"))
      resname.replace(p,4,gameFname);
    gameModule = fopen(resname.c_str(),"wb");
    if (!gameModule) {
      user << "Failed to write resources to compiler-specified file, `" << resname << "`. Write permissions to valid path?" << flushl;
      idpr("Failed to write resources.",-1); return 12;
    }
  }

  // Start by setting off our location with a DWord of NULLs
  fwrite("\0\0\0",1,4,gameModule);

  idpr("Adding Sprites",90);

  res = current_language->module_write_sprites(es, gameModule);
  irrr();

  edbg << "Finalized sprites." << flushl;
  idpr("Adding Sounds",93);

  current_language->module_write_sounds(es,gameModule);

  current_language->module_write_backgrounds(es,gameModule);

  current_language->module_write_fonts(es,gameModule);

  current_language->module_write_paths(es,gameModule);

  // Tell where the resources start
  fwrite("\0\0\0\0res0",8,1,gameModule);
  fwrite(&resourceblock_start,4,1,gameModule);

  // Close the game module; we're done adding resources
  idpr("Closing game module and running if requested.",99);
  edbg << "Closing game module and running if requested." << flushl;
  fclose(gameModule);

  // Run the game if requested
  if (mode == emode_run or mode == emode_debug or mode == emode_design)
  {
    // The games working directory, in run/debug it is the GMK/GMX location where the IDE is working with the project,
    // in compile mode it is the same as program_directory, or where the (*.exe executable) is located.
    // The working_directory global is set in the main() of each platform using the platform specific function.
    // This the exact behaviour of GM8.1
    char prevdir[4096];
    string newdir = (es->filename != NULL && strlen(es->filename) > 0) ? string(es->filename) : string( exe_filename );
    #if CURRENT_PLATFORM_ID == OS_WINDOWS
      if (newdir[0] == '/' || newdir[0] == '\\') {
        newdir = newdir.substr(1, newdir.size());
      }
    #endif
    newdir = newdir.substr( 0, newdir.find_last_of( "\\/" ));

    #if CURRENT_PLATFORM_ID == OS_WINDOWS
    GetCurrentDirectory( 4096, prevdir );
    SetCurrentDirectory(newdir.c_str());
    #else
    getcwd (prevdir, 4096);
    chdir(newdir.c_str());
    #endif
    
    string rprog = extensions::targetOS.runprog, rparam = extensions::targetOS.runparam;
    rprog = string_replace_all(rprog,"$game",gameFname);
    rparam = string_replace_all(rparam,"$game",gameFname);
    user << "Running \"" << rprog << "\" " << rparam << flushl;
    int gameres = e_execs(rprog, rparam);
    user << "\n\nGame returned " << gameres << "\n";
    
    // Restore the compilers original working directory.
    #if CURRENT_PLATFORM_ID == OS_WINDOWS
    SetCurrentDirectory(prevdir);
    #else
    chdir(prevdir);
    #endif
  }

  idpr("Done.", 100);
  return 0;
}

