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


#ifndef ENIGMA_SUPERSOUND_ADVANCED__H
#define ENIGMA_SUPERSOUND_ADVANCED__H

#include "Universal_System/var4.h"


///This file contains advanced "Super Sound" audio functionality.

namespace enigma 
{
int translate_ss_id(const string& ssid); //Convert a super-sound ID to an internal ID. Returns -1 on failure.
}


namespace enigma_user
{

//Check if the specified sound is currently playing/looping.
//Returns 1 or 0 for true/false, and -1 if the sound doesn't exist (or some other error).
int SS_IsSoundPlaying(const string& sndHandle);

//Check if the specified sound is currently looping. 
//Returns 1 or 0 for true/false, and -1 if the sound doesn't exist (or some other error).
int SS_IsSoundLooping(const string& sndHandle);

//Returns the specified sound's current volume. Returns -1 on error.
int SS_GetSoundVol(const string& sndHandle);

//Returns the specified sound's current pan. Returns -1 on error.
int SS_GetSoundPan(const string& sndHandle);

//Returns the specified sound's current frequency. Returns -1 on error.
int SS_GetSoundFreq(const string& sndHandle);

//Returns the sound's "current position in bytes". Not sure what this is; maybe how much of it we've currently played?
int SS_GetSoundPosition(const string& sndHandle);

//Returns the sound's "length in bytes". Again, most likely this is used to get a % completion of the current sound.
int SS_GetSoundLength(const string& sndHandle);

//Sets the sound's "position in bytes". See above; probably used for finer control over looping.
void SS_SetSoundPosition(const string& sndHandle, int newPos);

//Retrieves the number of bytes per second of sound in this sample.
int SS_GetSoundBytesPerSecond(const string& sndHandle);

//Returns true if the current handle is valid.
bool SS_IsHandleValid(const string& sndHandle);

//Returns 1/0 if the sound is paused or not, and -1 on error.
int SS_IsSoundPaused(const string& sndHandle);

//Pauses the given sound.
void SS_PauseSound(const string& sndHandle);

//Resumes playing the given sound.
//If the sound was stopped it will start playing from the start.
//If the sound was just paused, it will continue from where it left off.
void SS_ResumeSound(const string& sndHandle);


}

#endif // ENIGMA_SUPERSOUND_ADVANCED__H

