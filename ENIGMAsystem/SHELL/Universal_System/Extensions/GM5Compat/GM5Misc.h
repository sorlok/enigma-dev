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
//BUG: Invalid statements such as "brush_color = dafadf;" are valid for these.
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
void draw_triangle(gs_scalar x1, gs_scalar y1, gs_scalar x2, gs_scalar y2, gs_scalar x3, gs_scalar y3);
void draw_ellipse(gs_scalar x1, gs_scalar y1, gs_scalar x2, gs_scalar y2);
void draw_circle(gs_scalar x, gs_scalar y, float radius);

//Drawing - Related (TODO: remove)
void draw_set_pen_color(int clr);
void draw_set_pen_size(gs_scalar sz);
void draw_set_brush_color(int clr);
void draw_set_brush_style(int sty);


//Drawing - TextSprite
void draw_text_sprite(int x, int y, string str, int sep, int lineWidth, int sprite, int firstChar, int scale);

//Drawing - Polygons
void draw_polygon_begin();
void draw_polygon_vertex(int x, int y);
void draw_polygon_end();

//Sound
void sound_frequency(int index, double value);
int sound_add(string fName, int buffers, bool useEffects, bool loadOnUse);

}

#endif // ENIGMA_GM5COMPAT_MISC__H

