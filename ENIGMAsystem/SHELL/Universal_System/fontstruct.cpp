/********************************************************************************\
**                                                                              **
**  Copyright (C) 2008 Josh Ventura                                             **
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

#include <list>
#include <string>
#include <string.h>
#include <stdio.h>
using namespace std;

#include "../Graphics_Systems/graphics_mandatory.h"
#include "../libEGMstd.h"

#include "spritestruct.h"
#include "fontstruct.h"
#include "rectpack.h"

namespace enigma
{
  font **fontstructarray = NULL;
  static size_t fontcount = 0;

  int font_new(unsigned char gs, unsigned char gc) // Creates a new font, allocating 'gc' glyphs
  {
    font *ret = new font;
    ret->glyphstart = gs;
    ret->glyphcount = gc;
    ret->glyphs = new fontglyph[gc];
    ret->height = 0;
    
    font **fsan = new font*[fontcount+1];
    for (unsigned i = 0; i < fontcount; i++)
      fsan[i] = fontstructarray[i];
    delete fontstructarray;
    fontstructarray = fsan;
    
    fontstructarray[fontcount] = ret;
    return fontcount++;
  }
}

int font_add_sprite(int spr, unsigned char first, bool prop, int sep)
{
  enigma::sprite *sspr = enigma::spritestructarray[spr];
  if (!sspr) return -1;
  
  unsigned char gcount = sspr->subcount;
  int idfont = enigma::font_new(first, gcount);
  enigma::font *font = enigma::fontstructarray[idfont];
  
  // Now, what we'll implement is a packing algorithm.
  // This algorithm will try to fit as many glyphs as possible into
  // a square space based on the max height of the font.
  
  unsigned char* glyphdata[gcount];
  enigma::rect_packer::pvrect glyphmetrics[gcount];
  int glyphx[gcount], glyphy[gcount];
  
  int gwm = sspr->width, // Glyph width max: sprite width
      ghm = sspr->height, // Glyph height max: sprite height
      gtw = int((double)sspr->width / sspr->texbordy);
  
  font->height = ghm;
  
  for (int i = 0; i < gcount; i++)
  {
    unsigned char* data = enigma::graphics_get_texture_rgba(sspr->texturearray[i]);
    glyphdata[i] = data;
    
    // Here we calculate the bbox
    if (!prop)
       glyphmetrics[i].x = 0,   glyphmetrics[i].y = 0,
       glyphmetrics[i].w = gwm-1, glyphmetrics[i].h = ghm-1;
    else
    {
      glyphmetrics[i].x = gwm, glyphmetrics[i].y = ghm,
      glyphmetrics[i].w = 0,   glyphmetrics[i].h = 0;
      for (int bx = 0; bx < gwm; bx++)
      for (int by = 0; by < ghm; by++)
      {
        if (data[(by*gtw + bx)<<2]) // If this pixel isn't completely transparent
        {
          if (bx < glyphmetrics[i].x) glyphmetrics[i].x = bx;
          if (bx > glyphmetrics[i].w) glyphmetrics[i].w = bx; // Treat width as right for now
          if (by < glyphmetrics[i].y) glyphmetrics[i].y = by;
          if (by > glyphmetrics[i].h) glyphmetrics[i].h = by; // Treat height as bottom for now
        }
      }
      if (glyphmetrics[i].x > glyphmetrics[i].w)
        glyphmetrics[i].x = 0, glyphmetrics[i].y = 0,
        glyphmetrics[i].w = 0, glyphmetrics[i].h = 0;
    }
    font->glyphs[i].x = glyphmetrics[i].x; // Save these metrics while x and y are still relative to each glyph
    font->glyphs[i].y = glyphmetrics[i].y;
    font->glyphs[i].x2 = glyphmetrics[i].w + 1; // And while w and h are still the right and bottom edge coordinates
    font->glyphs[i].y2 = glyphmetrics[i].h + 1;
    
    font->glyphs[i].xs = glyphmetrics[i].w + sep; // This is just user-specified for sprite-loaded fonts
    
    glyphmetrics[i].w -= glyphmetrics[i].x - 1; // Fix width and height to be such
    glyphmetrics[i].h -= glyphmetrics[i].y - 1; // instead of right and bottom
    glyphx[i] = glyphmetrics[i].x, glyphy[i] = glyphmetrics[i].y;
    glyphmetrics[i].placed = -1;
  }
  
  list<unsigned int> boxes;
  for (int i = 0; i < gcount; i++)
    boxes.push_back((glyphmetrics[i].w * glyphmetrics[i].h << 8) + i);
  boxes.sort();
  
  int w = 64, h = 64;
  enigma::rect_packer::rectpnode *rectplane = new enigma::rect_packer::rectpnode(0,0,w,h);
  for (list<unsigned int>::reverse_iterator i = boxes.rbegin(); i != boxes.rend() and w and h; )
  {
    printf("Add rectangle %d, which is of size %d x %d, to main cell of size %d x %d (%d, %d)\n", *i & 255, glyphmetrics[*i & 255].w, glyphmetrics[*i & 255].h, rectplane->wid, rectplane->hgt, w, h);
    enigma::rect_packer::rectpnode *nn = enigma::rect_packer::rninsert(rectplane, *i & 0xFF, glyphmetrics);
    if (nn)
      enigma::rect_packer::rncopy(nn, glyphmetrics, *i & 0xFF),
      i++;
    else
    {
      w > h ? h <<= 1 : w <<= 1,
      rectplane = enigma::rect_packer::expand(rectplane, w, h);
      printf("Expanded to %d by %d\n", w, h);
      if (!w or !h) return -1;
    }
  }
  
  int bigtex[w*h];
  for (int i = 0; i < gcount; i++)
  {
    for (int yy = 0; yy < glyphmetrics[i].h; yy++)
      for (int xx = 0; xx < glyphmetrics[i].w; xx++)
        bigtex[w*(glyphmetrics[i].y + yy) + glyphmetrics[i].x + xx] = ((unsigned int*)glyphdata[i])[gtw*(glyphy[i] + yy) + xx + glyphx[i]];
    delete[] glyphdata[i];
    
    font->glyphs[i].tx = glyphmetrics[i].x / double(w);
    font->glyphs[i].ty = glyphmetrics[i].y / double(h);
    font->glyphs[i].tx2 = (glyphmetrics[i].x + glyphmetrics[i].w) / double(w);
    font->glyphs[i].ty2 = (glyphmetrics[i].y + glyphmetrics[i].h) / double(h);
  }
  
  font->texture = enigma::graphics_create_texture(w,h,bigtex);
  return idfont;
}