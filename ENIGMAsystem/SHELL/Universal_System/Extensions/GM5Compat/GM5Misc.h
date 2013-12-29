/** 
 * TODO: Currently unsure of how the licensing here works.
**/

#ifndef ENIGMA_GM5COMPAT_MISC__H
#define ENIGMA_GM5COMPAT_MISC__H

#include "Universal_System/var4.h"


namespace enigma_user
{

//General graphics
void set_graphics_mode(bool exclusive, int horizRes, int colorDepth, int freq, bool fullscreen, int winScale, int fullScale);
void screen_gamma(double r, double g, double b);

//Drawing - TextSprite
void draw_text_sprite(int x, int y, string str, int sep, int w, int sprite, int firstChar, int scale);

//Drawing - Polygons
void draw_polygon_begin();
void draw_polygon_vertex(int x, int y);
void draw_polygon_end();

//Sound
void sound_frequency(int index, double value);

}

#endif // ENIGMA_GM5COMPAT_MISC__H

