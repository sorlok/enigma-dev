/** Copyright (C) 2008, 2012 Josh Ventura
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

#include <string>

namespace enigma_user
{

//A "filestr" is a string that converts "\" to "/" on Linux 
// (and optionally Windows). It is used to allow Windows-separated filenames on Linux.
class filestr {
public:
  //A one-argument constructor allows the class to silently convert strings into filstrs
  filestr(const char* fname);
  filestr(const std::string& fname);

  //Use to retrieve the underlying c_str(); the only thing that filestrs are used for (usually).
  const char* c_str() const { return data.c_str(); }

private:
  std::string data;
};



// File attribute constants
enum {
  fa_readonly  = 1,
  fa_hidden    = 2,
  fa_sysfile   = 4,
  fa_volumeid  = 8,
  fa_directory = 16,
  fa_archive   = 32
};

long long file_size(std::string fname);
time_t file_access_time(std::string fname);
time_t file_modified_time(std::string fname);

int file_exists(const filestr& fname);
int file_delete(std::string fname);
int file_rename(std::string oldname,std::string newname);
int file_copy(std::string fname,std::string newname);
int directory_exists(std::string dname);
int directory_create(std::string dname);

std::string file_find_next();
int file_find_close();
std::string file_find_first(std::string mask,int attr);

bool file_attributes(std::string fname,int attributes);

void export_include_file(std::string fname);
void export_include_file_location(std::string fname,std::string location);
void discard_include_file(std::string fname);

extern unsigned game_id;
extern std::string working_directory;
extern std::string program_directory;
extern std::string temp_directory;

}
