/** Copyright (C) 2014 Robert B. Colton
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

#include "../General/PFsystem.h"

namespace enigma {
	extern bool gameWindowFocused;
}

namespace enigma_user {

string os_get_config() {
  return "";
}

int os_get_info() {
  return -1;
}

string os_get_language() {
  return "";
}

string os_get_region() {
  return "";
}

bool os_is_network_connected() {
  return false;
}

bool os_is_paused() {
	return !enigma::gameWindowFocused;
}

void os_lock_orientation(bool enable) {

}

void os_powersave_enable(bool enable) {

}

}
