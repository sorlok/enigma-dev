// 
// Copyright (C) 2014 Seth N. Hetu
// 
// This file is a part of the ENIGMA Development Environment.
// 
// ENIGMA is free software: you can redistribute it and/or modify it under the
// terms of the GNU General Public License as published by the Free Software
// Foundation, version 3 of the license or any later version.
// 
// This application and its source code is distributed AS-IS, WITHOUT ANY
// WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS
// FOR A PARTICULAR PURPOSE. See the GNU General Public License for more
// details.
// 
// You should have received a copy of the GNU General Public License along
// with this code. If not, see <http://www.gnu.org/licenses/>
//

#include "SS_Basic.h"

#include <map>
#include <sstream>

#include "Audio_Systems/General/ASbasic.h"
#include "Universal_System/estring.h"

namespace {

//Map of Super-Sound IDs to internal IDs.
std::map<string, int> ss_map;

//Next ID to assign.
int nextId = 1;

int get_int(const var& val) {
  int res = 0;
  if (enigma_user::is_real(static_cast<const variant&>(val))) {
    res = static_cast<int>(val);
  } else if (enigma_user::is_string(static_cast<const variant&>(val))) {
    std::istringstream buffer(static_cast<string>(val));
    buffer >>res;
  }
  return res;
}

}

namespace enigma  
{

//Convert a super-sound ID to an internal ID. Returns -1 on failure.
int translate_ss_id(const string& ssid)
{
  std::map<string, int>::const_iterator it=ss_map.find(ssid);
  if (it==ss_map.end()) {
    return -1;
  }

  return it->second;
}

}


namespace enigma_user
{

void SS_Init() {}
void SS_Unload() {}

string SS_LoadSound(const string& filename, bool stream)
{
  //Try loading it; return "0" on failure.
  int resId = sound_add(filename, false, false);
  if (resId==-1) {
    return "0";
  }

  //Our IDs will be "SS:X", where X is a monotonically increasing count, for easy reference.
  std::stringstream ssId;
  ssId <<"SS:" <<nextId++;
  ss_map[ssId.str()] = resId;

  return ssId.str();
}

void SS_PlaySound(const string& sndHandle)
{
  int id = enigma::translate_ss_id(sndHandle);
  if (id==-1) {
    return;
  }

  sound_play(id);
}

void SS_LoopSound(const string& sndHandle)
{
  int id = enigma::translate_ss_id(sndHandle);
  if (id==-1) {
    return;
  }

  sound_loop(id);
}

void SS_StopSound(const string& sndHandle)
{
  int id = enigma::translate_ss_id(sndHandle);
  if (id==-1) {
    return;
  }

  sound_stop(id);
}

void SS_SetSoundFreq(const string& sndHandle, const var& frequency)
{
  int id = enigma::translate_ss_id(sndHandle);
  if (id==-1) {
    return;
  }

  //NOTE: This is currently not supported (we have pitch though). Might want to deal with this later.
}

void SS_SetSoundPan(const string& sndHandle, const var& panning)
{
  int id = enigma::translate_ss_id(sndHandle);
  if (id==-1) {
    return;
  }

  //Scale it, set it.
  float val = get_int(panning) / 10000.0F;
  sound_pan(id, val);
}

void SS_SetSoundVol(const string& sndHandle, const var& volume)
{
  int id = enigma::translate_ss_id(sndHandle);
  if (id==-1) {
    return;
  }

  //Scale it, set it.
  float val = get_int(volume) / 10000.0F;
  sound_volume(id, val);
}

void SS_FreeSound(const string& sndHandle)
{
  //No-op?
}


}

