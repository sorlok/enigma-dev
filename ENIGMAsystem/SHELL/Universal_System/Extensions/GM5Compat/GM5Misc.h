/** 
 * TODO: Currently unsure of how the licensing here works.
**/

#ifndef ENIGMA_GM5COMPAT_MISC__H
#define ENIGMA_GM5COMPAT_MISC__H

#include "Universal_System/var4.h"
#include "Universal_System/scalar.h"


namespace enigma_user
{

//Placing these here allows Enigma to identify them as globals, so 
// they can be set properly from any script.
extern gs_scalar pen_size;
extern int brush_style;
extern int brush_color;
extern int pen_color;


//Brush styles.
enum {
  bs_solid  = 0,
  bs_hollow,

  //Currently not supported.
  bs_bdiagonal,
  bs_fdiagonal,
  bs_cross,
  bs_diagcross,
  bs_horizontal,
  bs_vertical,
};


//General graphics
void set_graphics_mode(bool exclusive, int horizRes, int colorDepth, int freq, bool fullscreen, int winScale, int fullScale);
void screen_gamma(double r, double g, double b);

//Drawing - Wrappers.
void draw_rectangle(gs_scalar x1, gs_scalar y1, gs_scalar x2, gs_scalar y2);
void draw_ellipse(gs_scalar x1, gs_scalar y1, gs_scalar x2, gs_scalar y2);

//Drawing - Related (TODO: remove)
void draw_set_pen_color(int clr);
void draw_set_pen_size(gs_scalar sz);
void draw_set_brush_color(int clr);
void draw_set_brush_style(int sty);


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

