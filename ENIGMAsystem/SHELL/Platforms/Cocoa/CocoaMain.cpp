/*
 * Copyright (C) 2008 IsmAvatar <cmagicj@nni.com>
 * Copyright (C) 2010 Alasdair Morrison <tgmg@g-java.com>
 *
 * This file is part of ENIGMA.
 *
 * ENIGMA is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * ENIGMA is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
 * GNU General Public License (COPYING) for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program. If not, see <http://www.gnu.org/licenses/>.
 */

#include <unistd.h>
#include <stdio.h>
#include "CocoaMain.h"
#include "ObjectiveC.h"
#include <cstdlib>
#include <fstream>
#include <iostream>
#include <fstream>
#include <cassert>

#include "../General/PFwindow.h"
#include "../General/PFfilemanip.h"

#include "Universal_System/CallbackArrays.h"
#include "Universal_System/roomsystem.h"

namespace enigma_user {
  std::string working_directory = "";
}

extern "C" void copy_bundle_cwd(char* res);

int main(int argc,char** argv)
{
    // Re-direct stdout and stderr
    freopen("/Users/sethhetu/Downloads/enigma_cout.txt","w",stdout);
    freopen("/Users/sethhetu/Downloads/enigma_cerr.txt","w",stderr);
    std::cout <<"cout redirect\n";
    printf("printf redirect\n");


{
std::ofstream tmpout("/Users/sethhetu/Downloads/egm_log.txt", std::ofstream::out);
tmpout <<"main() called\n";
}

  // Set the working_directory
  char buffer[1024];
  if (getcwd(buffer, sizeof(buffer)) != NULL)
     fprintf(stdout, "Current working dir: %s\n", buffer);
  else
     perror("getcwd() error");


//NOTE: OSX is different.
copy_bundle_cwd(&buffer[0]);

  enigma_user::working_directory = string( buffer );

{
std::ofstream tmpout("/Users/sethhetu/Downloads/egm_log.txt", std::ofstream::out | std::ofstream::app);
tmpout <<"workind_directory is: " <<enigma_user::working_directory <<"\n";
}
  
	enigma::parameters=new char* [argc];
	for (int i=0; i<argc; i++) {
{
std::ofstream tmpout("/Users/sethhetu/Downloads/egm_log.txt", std::ofstream::out | std::ofstream::app);
tmpout <<"PARAM[" <<i <<"] = ***" <<argv[i] <<"***\n";
}
		enigma::parameters[i]=argv[i];
   }

	return mainO(argc, argv);
}

namespace enigma_user {

  void sleep(int ms) {
    if (ms > 1000) ::sleep(ms/1000);
    usleep((ms % 1000) *1000);
  };
  
  void game_end(int ret) {
    //audiosystem_cleanup();
    exit(ret);
  }

  void action_end_game()
  {
    game_end();
  }
  
  int parameter_count(){
  // TODO
  return 0;
  }

  string parameter_string(int n) {
    // TODO
    return string("");
  }

  string environment_get_variable(string name) {
    // TODO
    return string("");
  }

}
