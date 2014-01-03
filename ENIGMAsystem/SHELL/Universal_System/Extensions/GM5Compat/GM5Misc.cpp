/** 
 * TODO: Currently unsure of how the licensing here works.
**/

#include "GM5Misc.h"

#include <cmath>
#include <algorithm>

#include "Graphics_Systems/General/GSstdraw.h"
#include "Graphics_Systems/General/GScolors.h"

namespace {


template <typename T>
void swap(T& val1, T& val2){
  T temp = val1;
  val1 = val2;
  val2 = temp;
}
}


namespace enigma_user
{

//GM5 draw state.
gs_scalar pen_size = 1;
int brush_style = enigma_user::bs_solid; 
int pen_color = enigma_user::c_black;
int brush_color = enigma_user::c_black;


void set_graphics_mode(bool exclusive, int horizRes, int colorDepth, int freq, bool fullscreen, int winScale, int fullScale)
{
}

void screen_gamma(double r, double g, double b)
{
}

void draw_rectangle(gs_scalar x1, gs_scalar y1, gs_scalar x2, gs_scalar y2)
{
  //lwid/hwid are the width and half-width, respectively. shft is used to nudge odd-width penned shapes to the right.
  int lwid = std::max(1,(int)round(pen_size));
  int hwid = lwid/2;
  int shft = (lwid%2==0)?0:1;

  //Ensure (x1,y1) are the smallest components, and (x2,y2) are the largest.
  if (x2<x1) { swap(x1,x2); }
  if (y2<y1) { swap(y1,y2); }

  //Nudge the shape into position.
  x1 = int(x1) + shft;
  x2 = int(x2) + shft - 1 ;
  y1 = int(y1);
  y2 = int(y2) - 1 ;

  //Fill the shape, if we have the correct brush style.
  //Note: At the moment, we treat all unsupported brush styles as "solid"; only "hollow" avoids drawing.
  if (brush_style != bs_hollow) {
    draw_set_color(brush_color);
    draw_rectangle(x1,y1,x2,y2,false);
  }

  //Draw the line (this always happens; a pen_size of 0 still draws a 1px line).
  //Note: So many off-by-one errors worries me; will have to confirm on Windows that this
  //      is not a bug in the OpenGL code.
  draw_set_color(pen_color);
  draw_line_width(x1,           y1-hwid, x1,      y2+hwid, lwid);
  draw_line_width(x1-hwid-shft, y2,      x2+hwid, y2,      lwid);
  draw_line_width(x2,           y2+hwid, x2,      y1-hwid, lwid);
  draw_line_width(x2+hwid,      y1,      x1-hwid, y1,      lwid);
}


void draw_ellipse(gs_scalar x1, gs_scalar y1, gs_scalar x2, gs_scalar y2)
{
  int lwid = std::max(1,(int)round(pen_size));

  //Ensure (x1,y1) are the smallest components, and (x2,y2) are the largest.
  if (x2<x1) { swap(x1,x2); }
  if (y2<y1) { swap(y1,y2); }

  //Nudge the shape into position.
  gs_scalar shft = (lwid%2 == 0) ? 1 : 0.5;
  x1 += shft;
  x2 -= shft;
  y1 += shft;
  y2 -= shft;

  //Fill the shape, if we have the correct brush style.
  //Note: At the moment, we treat all unsupported brush styles as "solid"; only "hollow" avoids drawing.
  if (brush_style != bs_hollow) {
    draw_set_color(brush_color);
    draw_ellipse(x1,y1,x2,y2,false);
  }

  //This is borrowed from General drawing code.
  draw_set_color(pen_color);
  gs_scalar x = (x1+x2)/2,y=(y1+y2)/2;
  gs_scalar hr = fabs(x2-x),vr=fabs(y2-y);
  gs_scalar pr = 2*M_PI/draw_get_circle_precision();
  for(gs_scalar i=pr;i<M_PI;i+=pr) {
    gs_scalar xc1 = cos(i)*hr;
    gs_scalar yc1 = sin(i)*vr;
    i += pr;
    gs_scalar xc2=cos(i)*hr;
    gs_scalar yc2=sin(i)*vr;
    draw_line_width(x+xc1,y+yc1  ,  x+xc2,y+yc2  , lwid);
    draw_line_width(x-xc1,y+yc1  ,  x-xc2,y+yc2  , lwid);
    draw_line_width(x+xc1,y-yc1  ,  x+xc2,y-yc2  , lwid);
    draw_line_width(x-xc1,y-yc1  ,  x-xc2,y-yc2  , lwid);
  }

  //These are needed to prevent the top half-width of the line from overlapping for wide lines.
  //This is clearly massively inefficient; currently it rotates the entire shape through each half-width
  //  in an attempt to cover blank spots. 
  for(gs_scalar i=pr;i<M_PI;i+=pr) {
    gs_scalar xc1 = cos(i+pr/2)*hr;
    gs_scalar yc1 = sin(i+pr/2)*vr;
    i += pr;
    gs_scalar xc2=cos(i+pr/2)*hr;
    gs_scalar yc2=sin(i+pr/2)*vr;
    draw_line_width(x+xc1,y+yc1  ,  x+xc2,y+yc2  , lwid);
    draw_line_width(x-xc1,y+yc1  ,  x-xc2,y+yc2  , lwid);
    draw_line_width(x+xc1,y-yc1  ,  x+xc2,y-yc2  , lwid);
    draw_line_width(x-xc1,y-yc1  ,  x-xc2,y-yc2  , lwid);
  }
  for(gs_scalar i=pr;i<M_PI;i+=pr) {
    gs_scalar xc1 = cos(i-pr/2)*hr;
    gs_scalar yc1 = sin(i-pr/2)*vr;
    i += pr;
    gs_scalar xc2=cos(i-pr/2)*hr;
    gs_scalar yc2=sin(i-pr/2)*vr;
    draw_line_width(x+xc1,y+yc1  ,  x+xc2,y+yc2  , lwid);
    draw_line_width(x-xc1,y+yc1  ,  x-xc2,y+yc2  , lwid);
    draw_line_width(x+xc1,y-yc1  ,  x+xc2,y-yc2  , lwid);
    draw_line_width(x-xc1,y-yc1  ,  x-xc2,y-yc2  , lwid);
  }
}


void draw_set_pen_color(int clr)
{
  pen_color = clr;
}

void draw_set_pen_size(gs_scalar sz)
{
  pen_size = sz;
}

void draw_set_brush_color(int clr)
{
  brush_color = clr;
}

void draw_set_brush_style(int sty)
{
  brush_style = sty;
}

void draw_text_sprite(int x, int y, string str, int sep, int w, int sprite, int firstChar, int scale)
{
}

void draw_polygon_begin()
{
}

void draw_polygon_vertex(int x, int y)
{
}

void draw_polygon_end()
{
}

void sound_frequency(int index, double value)
{
}


}

