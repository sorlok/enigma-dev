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

#include "Graphics_Systems/General/GSsprite.h"


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

void sound_background_tempo(float f)
{
}

int sprite_create_from_screen(int x, int y, int w, int h, bool b1, bool b2, bool b3, bool b4, int xorig, int yorig)
{
  return sprite_create_from_screen(x, y, w, h, false, false, xorig, yorig);
}

void registry_write_string(std::string name, std::string str) {}

void registry_write_real(std::string name, int x) {}


}

