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


#ifndef ENIGMA_SUPERSOUND_BASIC__H
#define ENIGMA_SUPERSOUND_BASIC__H

#include "Universal_System/var4.h"


///This file contains basic "Super Sound" audio functionality.


namespace enigma_user
{

//Lifecycle no-ops. Exists for compatibility with the SuperSound API.
void SS_Init();
void SS_Unload();

//Load a sound from a file into memory. "Stream" mode is ignored.
//Returns the sound handle as a string; "0" if the sound couldn't be loaded.
string SS_LoadSound(const string& filename, bool stream);

//Play a sound (once).
void SS_PlaySound(const string& sndHandle);

//Start looping a sound. It will play until you stop it.
void SS_LoopSound(const string& sndHandle);

//Stop playing a sound.
void SS_StopSound(const string& sndHandle);

//Set the frequency of a sound (in Hz).
//Frequency can be a string or int, and range from 1000 Hz to 100 000 Hz.
void SS_SetSoundFreq(const string& sndHandle, const var& frequency);

//Set the panning of a sound.
//Pan can be a string or int, and range from -10 000 (full left) to 10 000 (full right).
void SS_SetSoundPan(const string& sndHandle, const var& panning);

//Set the volume of a sound.
//Volumes range from 0 (mute) to 10 000 (full volume).
void SS_SetSoundVol(const string& sndHandle, const var& volume);

//Unload a sound from memory. Normally not used.
void SS_FreeSound(const string& sndHandle);

}

#endif // ENIGMA_SUPERSOUND_BASIC__H

