/** 
 * TODO: Currently unsure of how the licensing here works.
**/

#include "GM5Misc.h"

#include <cmath>
#include <list>
#include <algorithm>
#include <cstdio>

#include "Audio_Systems/General/ASbasic.h"
#include "Graphics_Systems/General/GSstdraw.h"
#include "Graphics_Systems/General/GScolors.h"
#include "Graphics_Systems/General/GSsprite.h"
#include "Graphics_Systems/General/GSprimitives.h"

namespace {

//TODO: This likely exists somewhere.
struct IntPt {
  int x;
  int y;
  IntPt(int x, int y) : x(x),y(y) {}
};

//The current polygon's vertices.
std::list<IntPt> currPoly;


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


void draw_triangle(gs_scalar x1, gs_scalar y1, gs_scalar x2, gs_scalar y2, gs_scalar x3, gs_scalar y3)
{
  int lwid = std::max(1,(int)round(pen_size));

  //Fill the shape, if we have the correct brush style.
  //Note: At the moment, we treat all unsupported brush styles as "solid"; only "hollow" avoids drawing.
  if (brush_style != bs_hollow) {
    draw_set_color(brush_color);
    draw_triangle(x1,y1,x2,y2,x3,y3,false);
  }

  //Draw the outline.
  draw_set_color(pen_color);
  draw_line_width(x1,y1, x2,y2, lwid);
  draw_line_width(x2,y2, x3,y3, lwid);
  draw_line_width(x3,y3, x1,y1, lwid);

  //We fake triangle joints with circles.
  //TODO: Triangle coords are still off slightly, not just on the joints.
  draw_circle(x1,y1,lwid/2.0, false);
  draw_circle(x2,y2,lwid/2.0, false);
  draw_circle(x3,y3,lwid/2.0, false);
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

void draw_circle(gs_scalar x, gs_scalar y, float radius)
{
  int lwid = std::max(1,(int)round(pen_size));

  //Shuffle the radius a bit.
  if (lwid%2 == 0) {
    radius -= 1;
  }

  //Fill the shape, if we have the correct brush style.
  //Note: At the moment, we treat all unsupported brush styles as "solid"; only "hollow" avoids drawing.
  if (brush_style != bs_hollow) {
    draw_set_color(brush_color);
    draw_circle(x,y,radius,false);
  }

  //This is borrowed from General drawing code.
  draw_set_color(pen_color);
  double pr = 2 * M_PI / draw_get_circle_precision();
  double oldX = 0.0;
  double oldY = 0.0;
  for (double i = 0; i <= 2*M_PI; i += pr) {
    double xc1=cos(i)*radius;
    double yc1=sin(i)*radius;
    double newX = x+xc1;
    double newY = y+yc1;
    if (i>0) {
      draw_line_width(oldX,oldY, newX, newY, lwid);
    }
    oldX = newX;
    oldY = newY;
  }

  //Similar to the ellipse drawing code, this overlays patches to the circle's shape.
  //It is also inefficient, but necessary for large pen widths.
  oldX = oldY = 0.0;
  for (double i = 0; i <= 2*M_PI; i += pr) {
    double xc1=cos(i+pr/2)*radius;
    double yc1=sin(i+pr/2)*radius;
    double newX = x+xc1;
    double newY = y+yc1;
    if (i>0) {
      draw_line_width(oldX,oldY, newX, newY, lwid);
    }
    oldX = newX;
    oldY = newY;
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

void draw_text_sprite(int x, int y, string str, int sep, int lineWidth, int sprite, int firstChar, int scale)
{
  //Easy lookup of width/height, accounting for scale.
  int w = sprite_get_width(sprite)  * scale;
  int h = sprite_get_height(sprite) * scale;

  //Magic number: "default" line separation height.
  if (sep == -1) {
    sep = h + 2; //Just a guess. 
  }

  //Now, simply draw each letter via sub-images, accounting for the width if required. 
  int offX = 0;
  int offY = 0;
  size_t lastSizeCheck = 0;
  for (size_t i=0; i<str.length(); i++) {
    //Wrap?
    if (lineWidth!=-1 && i>= lastSizeCheck) {
      //Find the next space or hyphen.
      lastSizeCheck=i+1;
      for (; lastSizeCheck<str.length(); lastSizeCheck++) {
        if (str[lastSizeCheck]==' ' || str[lastSizeCheck]=='-') { break; }
      }

      //Can we fit the next word?
      if (offX + (lastSizeCheck-i)*w >= lineWidth) {
        offX = 0;
        offY += sep;
      }
    }

    //Draw, update.
    char c = str[i];
    int subIndex = c - firstChar;
    draw_sprite_stretched(sprite, subIndex, x+offX, y+offY, w, h);
    offX += w;
  }
}

void draw_polygon_begin()
{
  currPoly.clear();
}

void draw_polygon_vertex(int x, int y)
{
  currPoly.push_back(IntPt(x,y));
}

void draw_polygon_end()
{
  int lwid = std::max(1,(int)round(pen_size));
  if (currPoly.size() > 1) {

    //Fill the shape, if we have the correct brush style.
    //Note: At the moment, we treat all unsupported brush styles as "solid"; only "hollow" avoids drawing.
    //Note: It seems that GM5 ignores the brush_style for polygons. More testing needed.
    //if (brush_style != bs_hollow) {
    if (true) {
      draw_set_color(brush_color);
      draw_primitive_begin(pr_trianglefan);
      for (std::list<IntPt>::iterator it = currPoly.begin();it!=currPoly.end(); it++) {
        draw_vertex(it->x, it->y);
      }
      draw_primitive_end();
    }


    //Draw the line around it.
    draw_set_color(pen_color);
    IntPt lastPt = *(--currPoly.end());
    for (std::list<IntPt>::iterator it = currPoly.begin(); it!=currPoly.end(); it++) {
      IntPt currPt = *it;
      draw_line_width(lastPt.x,lastPt.y, currPt.x,currPt.y , lwid);
      lastPt = currPt;
    }

  }

  //Done
  currPoly.clear();
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

