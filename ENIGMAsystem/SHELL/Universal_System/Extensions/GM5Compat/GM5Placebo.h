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

#ifndef ENIGMA_GM5COMPAT_PLACEBO__H
#define ENIGMA_GM5COMPAT_PLACEBO__H

#include "Universal_System/var4.h"


///This file contains functions which are no longer used by GM or ENIGMA,
/// but which GM5 scripts may expect to see. All of them do absolutely
/// no processing.
///In particular, *_restore() is used to pull a sprite into video memory,
/// to prevent stuttering (and *_discard() will remove it). This type of
/// fine-grained control is not needed by modern versions of GM or by ENIGMA.


namespace enigma_user
{

class filestr;

//Sprites
void sprite_discard(int num);
void sprite_restore(int num);

//Backgrounds
void background_discard(int num);
void background_restore(int num);

//Sounds
void sound_discard(int num);
void sound_restore(int num);

//General
void discard_all();

//TODO: These are not properly implemented yet.
void set_graphics_mode(bool exclusive, int horizRes, int colorDepth, int freq, bool fullscreen, int winScale, int fullScale);
void screen_gamma(double r, double g, double b);
void sound_frequency(int index, double value);
int sound_add(const filestr& fname, int buffers, bool useEffects, bool loadOnUse);

}

#endif // ENIGMA_GM5COMPAT_PLACEBO__H

