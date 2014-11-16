/********************************************************************************\
**                                                                              **
**  Copyright (C) 2008 Josh Ventura                                             **
**  Copyright (C) 2014 Seth N. Hetu                                             **
**                                                                              **
**  This file is a part of the ENIGMA Development Environment.                  **
**                                                                              **
**                                                                              **
**  ENIGMA is free software: you can redistribute it and/or modify it under the **
**  terms of the GNU General Public License as published by the Free Software   **
**  Foundation, version 3 of the license or any later version.                  **
**                                                                              **
**  This application and its source code is distributed AS-IS, WITHOUT ANY      **
**  WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS   **
**  FOR A PARTICULAR PURPOSE. See the GNU General Public License for more       **
**  details.                                                                    **
**                                                                              **
**  You should have recieved a copy of the GNU General Public License along     **
**  with this code. If not, see <http://www.gnu.org/licenses/>                  **
**                                                                              **
**  ENIGMA is an environment designed to create games and other programs with a **
**  high-level, fully compilable language. Developers of ENIGMA or anything     **
**  associated with ENIGMA are in no way responsible for its users or           **
**  applications created by its users, or damages caused by the environment     **
**  or programs made in the environment.                                        **
**                                                                              **
\********************************************************************************/

#include "makedir.h"
#include <stdio.h>
#include <iostream>
#include <fstream>

using namespace std;

#include "syntax/syncheck.h"
#include "general/estring.h"
#include "parser/parser.h"

#include "backend/EnigmaStruct.h" //LateralGM interface structures
#include "compiler/compile_common.h"

#include "languages/lang_CPP.h"

int global_script_argument_count = 0;

static string esc(string str) {
  string res;
  res.reserve(str.length());
  for (size_t i = 0; i < str.length(); ++i) {
    char c = str[i];
    if (c == '\n') { res += "\\n"; continue; }
    if (c == '\r') { res += "\\r"; continue; }
    if (c == '\\') { res += "\\\\"; continue; }
	if (c == '\"') { res += "\\\""; continue; }
    res.append(1, c);
  }
  return res;
}

int lang_CPP::compile_writeGlobals(EnigmaStruct* es, parsed_object* global)
{
  ofstream wto;
  wto.open((makedir +"Preprocessor_Environment_Editable/IDE_EDIT_globals.h").c_str(),ios_base::out);
    wto << license;

    global_script_argument_count=16; //write all 16 arguments
    if (global_script_argument_count) {
      wto << "// Script arguments\n";
      wto << "variant argument0 = 0";
      for (int i = 1; i < global_script_argument_count; i++)
        wto << ", argument" << i << " = 0";
      wto << ";\n\n";
    }
	
    wto << "namespace enigma_user { " << endl;
    //wto << "  string working_directory = \"\";" << endl; // moved over to PFmain.h
    wto << "  unsigned int game_id = " << es->gameSettings.gameId << ";" << endl;
    wto << "}" << endl <<endl;

    wto << "namespace enigma_user {" << endl;

//TEMP - constants are broken.
std::cerr <<"WRITING: " <<es->constantCount <<" constants\n";
wto<<"  #define VERNUM 117\n";
wto<<"  #define HRT_MAX 95\n";
wto<<"  #define AB_MAX 24\n";
wto<<"  #define c_undertomb make_color(130,130,30)\n";
wto<<"  #define c_strange make_color(175,255,140)\n";
wto<<"  #define c_ability make_color(64,64,255)\n";
wto<<"  #define c_thecurtain make_color(85,120,125)\n";
wto<<"  #define c_cave make_color(96,49,2)\n";
wto<<"  #define c_dkcave make_color(64,33,1)\n";
wto<<"  #define c_climb make_color(0,128,0)\n";
wto<<"  #define c_gold make_color(249,190,70)\n";
wto<<"  #define c_pink make_color(255,128,128)\n";
wto<<"  #define c_tree make_color(225,116,6)\n";
wto<<"  #define c_deepdive make_color(88,114,91)\n";
wto<<"  #define c_firecage c_red\n";
wto<<"  #define c_ghost make_color(82,48,81)\n";
wto<<"  #define c_eye make_color(255,64,255)\n";
wto<<"  #define c_skysand make_color(218,111,3)\n";
wto<<"  #define c_rainbowdive make_color(192,255,255)\n";
wto<<"  #define c_library make_color(138,140,62)\n";
wto<<"  #define c_highland make_color(191,193,115)\n";
wto<<"  #define mus_area1 mus_nightwalk\n";
wto<<"  #define mus_area2 mus_grotto\n";
wto<<"  #define mus_area3 mus_deeptower\n";
wto<<"  #define mus_area4 mus_coldkeep\n";
wto<<"  #define mus_area5 mus_skytown\n";
wto<<"  #define mus_area6 mus_nightclimb\n";
wto<<"  #define mus_area7 mus_farfall\n";
wto<<"  #define directory \"BGs\"\n";
wto<<"  #define snd_break2 snd_breakice\n";
wto<<"  #define snd_button snd_land\n";

/*    for (int i=0; i<es->constantCount; i++) {
      const Constant& con = es->constants[i];
      wto << "  #define " << con.name << " " << con.value << endl;
    }*/
//END TEMP

    wto << "}" << endl;

    wto << "//Default variable type: \"undefined\" or \"real\"" <<endl;
    wto << "const int variant::default_type = " <<(es->gameSettings.treatUninitializedAs0 ? "enigma::vt_real" : "-1") <<";" <<endl <<endl;

    wto << "namespace enigma {" << endl;
    wto << "  bool interpolate_textures = " << es->gameSettings.interpolate << ";" << endl;
    wto << "  bool forceSoftwareVertexProcessing = " << es->gameSettings.forceSoftwareVertexProcessing << ";" << endl;
    wto << "  bool isSizeable = " << es->gameSettings.allowWindowResize << ";" << endl;
    wto << "  bool showBorder = " << !es->gameSettings.dontDrawBorder << ";" << endl;
    wto << "  bool showIcons = " << !es->gameSettings.dontShowButtons << ";" << endl;
    wto << "  bool freezeOnLoseFocus = " << es->gameSettings.freezeOnLoseFocus << ";" << endl;
    wto << "  bool treatCloseAsEscape = " << es->gameSettings.treatCloseAsEscape << ";" << endl;
    wto << "  bool isFullScreen = " << es->gameSettings.startFullscreen << ";" << endl;
    wto << "  string gameInfoText = \"" << esc(es->gameInfo.gameInfoStr) << "\";" << endl;
    wto << "  string gameInfoCaption = \"" << es->gameInfo.formCaption << "\";" << endl;
    wto << "  int gameInfoBackgroundColor = " << javaColor(es->gameInfo.backgroundColor) << ";" << endl;
    wto << "  int gameInfoLeft = " << es->gameInfo.left << ";" << endl;
    wto << "  int gameInfoTop = " << es->gameInfo.top << ";" << endl;
    wto << "  int gameInfoWidth = " << es->gameInfo.width << ";" << endl;
    wto << "  int gameInfoHeight = " << es->gameInfo.height << ";" << endl;
    wto << "  bool gameInfoEmbedGameWindow = " << es->gameInfo.embedGameWindow << ";" << endl;
    wto << "  bool gameInfoShowBorder = " << es->gameInfo.showBorder << ";" << endl;
    wto << "  bool gameInfoAllowResize = " << es->gameInfo.allowResize << ";" << endl;
    wto << "  bool gameInfoStayOnTop = " << es->gameInfo.stayOnTop << ";" << endl;
    wto << "  bool gameInfoPauseGame = " << es->gameInfo.pauseGame << ";" << endl;
    wto << "  int viewScale = " << es->gameSettings.scaling << ";" << endl;
    wto << "  int windowColor = " << javaColor(es->gameSettings.colorOutsideRoom) << ";" << endl;
    wto << "}" << endl;

    for (parsed_object::globit i = global->globals.begin(); i != global->globals.end(); i++)
      wto << i->second.type << " " << i->second.prefix << i->first << i->second.suffix << ";" << endl;
    //This part needs written into a global object_parent class instance elsewhere.
    //for (globit i = global->dots.begin(); i != global->globals.end(); i++)
    //  wto << i->second->type << " " << i->second->prefixes << i->second->name << i->second->suffixes << ";" << endl;
    wto << endl;

    wto << "namespace enigma" << endl << "{" << endl << "  struct ENIGMA_global_structure: object_locals" << endl << "  {" << endl;
    for (deciter i = dot_accessed_locals.begin(); i != dot_accessed_locals.end(); i++) // Dots are vars that are accessed as something.varname.
      wto << "    " << i->second.type << " " << i->second.prefix << i->first << i->second.suffix << ";" << endl;

    wto << "    ENIGMA_global_structure(const int _x, const int _y): object_locals(_x,_y) {}" << endl << "  };" << endl << "  object_basic *ENIGMA_global_instance = new ENIGMA_global_structure(global,global);" << endl << "}";
    wto << endl;
  wto.close();
  return 0;
}
