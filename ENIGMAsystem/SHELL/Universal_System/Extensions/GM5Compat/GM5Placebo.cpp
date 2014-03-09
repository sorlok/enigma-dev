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


#include "GM5Placebo.h"
#include "Audio_Systems/General/ASbasic.h"


namespace enigma_user
{

void sprite_discard(int num)
{
}

void sprite_restore(int num)
{
}

void background_discard(int num)
{
}

void background_restore(int num)
{
}

void sound_discard(int num)
{
}

void sound_restore(int num)
{
}

void discard_all()
{
}

void set_graphics_mode(bool exclusive, int horizRes, int colorDepth, int freq, bool fullscreen, int winScale, int fullScale)
{
}

void screen_gamma(double r, double g, double b)
{
}

void sound_frequency(int index, double value)
{
}

int sound_add(string fName, int buffers, bool useEffects, bool loadOnUse)
{
  //NOTE: This is super-experimental
  //0 = "normal" sound
  //!loadOnUse is for "preload", and might work?
  return sound_add(fName, 0, !loadOnUse);
}



}

