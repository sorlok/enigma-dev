/** Copyright (C) 2008-2011 IsmAvatar <cmagicj@nni.com>, Josh Ventura
***
*** This file is a part of the ENIGMA Development Environment.
***
*** ENIGMA is free software: you can redistribute it and/or modify it under the
*** terms of the GNU General Public License as published by the Free Software
*** Foundation, version 3 of the license or any later version.
***
*** This application and its source code is distributed AS-IS, WITHOUT ANY
*** WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS
*** FOR A PARTICULAR PURPOSE. See the GNU General Public License for more
*** details.
***
*** You should have received a copy of the GNU General Public License along
*** with this code. If not, see <http://www.gnu.org/licenses/>
**/

 //File consists of Ism's code glued together and set to work with ENIGMA
 //(Josh's doing)

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <time.h>
#include <string>
#include <map>

using namespace std;

#include "Universal_System/CallbackArrays.h"
#include "Universal_System/roomsystem.h"
#include "Platforms/platforms_mandatory.h"
#include "GameSettings.h"
#include "SDLwindow.h"
#include "SDLmain.h"
#undef sleep

using namespace enigma::sdl;

namespace enigma {
  extern bool freezeOnLoseFocus;
}

//////////
// INIT //
//////////

void Sleep(int ms)
{
	if(ms>=1000) sleep(ms/1000);
	if(ms>0)	usleep(ms%1000*1000);
}

int visx = -1;
int visy = -1;

namespace enigma_user
{

void window_set_visible(bool visible)
{
  if(visible) {
    SDL_HideWindow(win);
  } else {
    SDL_ShowWindow(win);
  }
}

int window_get_visible()
{
  return SDL_GetWindowFlags(win)&SDL_WINDOW_SHOWN;
}

void window_set_caption(string caption) 
{
  SDL_SetWindowTitle(win, caption.c_str());
}

string window_get_caption()
{
  return SDL_GetWindowTitle(win);
}

}

namespace enigma_user
{

int display_mouse_get_x() { 
  //TODO: This might not work.
  int origin=0;
  SDL_GetWindowPosition(win, &origin, NULL);
  int res=0;
  SDL_GetMouseState(&res, NULL);
  return res + origin;
}

int display_mouse_get_y() { 
  //TODO: This might not work.
  int origin=0;
  SDL_GetWindowPosition(win, NULL, &origin);
  int res=0;
  SDL_GetMouseState(NULL, &res);
  return res + origin;
}

int window_mouse_get_x()  { 
  int res=0;
  SDL_GetMouseState(&res, NULL);
  return res;
}

int window_mouse_get_y()  { 
  int res=0;
  SDL_GetMouseState(NULL, &res);
  return res;
}

void window_set_stayontop(bool stay) {
}

bool window_get_stayontop() {
  return false;
}

void window_set_sizeable(bool sizeable) {
}

bool window_get_sizeable() {
  return false;
}

void window_set_showborder(bool show) {
}

bool window_get_showborder() {
  return true;
}

void window_set_showicons(bool show) {
}

bool window_get_showicons() {
  return true;
}

void window_set_minimized(bool minimized) {
  //TODO: Not sure.
}

bool window_get_minimized(){ 
  //TODO: May have to track events.
  return false; 
};

void window_default(bool center_size)
{
  int xm = room_width;
  int ym = room_height;
  if (view_enabled) {
    int tx = 0, ty = 0;
    for (int i = 0; i < 8; i++) {
      if (view_visible[i]) {
        if (view_xport[i]+view_wport[i] > tx) {
          tx = (int)(view_xport[i]+view_wport[i]);
        }
        if (view_yport[i]+view_hport[i] > ty) {
          ty = (int)(view_yport[i]+view_hport[i]);
        }
      }
    }
    if (tx and ty) {
      xm = tx, ym = ty;
    }
  } else {
    // By default if the room is too big instead of creating a gigantic ass window
    // make it not bigger than the screen to full screen it, this is what 8.1 and Studio
    // do, if the user wants to manually override this they can using
    // views/screen_set_viewport or window_set_size/window_set_region_size
    // We won't limit those functions like GM, just the default.
    if (xm > main_display.w) {
      xm = main_display.w;
    }
    if (ym > main_display.h) {
      ym = main_display.h;
    }
  }
  bool center = true;
  if (center_size) {
    center = (xm != enigma_user::window_get_width() || ym != enigma_user::window_get_height());
  }
  window_set_size(xm, ym);
  if (center) {
    enigma_user::window_center();
  }
}

void window_mouse_set(int x,int y) {
  //TODO: Is this still in SDL 2?
  //SDL_WarpMouse(x, y);
}

void window_view_mouse_set(int id, int x,int y) {
  //TODO: Is this still in SDL 2?
  //SDL_WarpMouse(view_xview[id] + x, view_yview[id] + y);
}

void display_mouse_set(double x,double y) {
  //TODO: SDL's not great with global display stuff.
}

}

////////////
// WINDOW //
////////////

namespace enigma_user
{

//Getters
int window_get_x() { 
  int res;
  SDL_GetWindowPosition(win, &res, 0);
  return res;
}

int window_get_y() { 
  int res;
  SDL_GetWindowPosition(win, 0, &res);
  return res;
}

int window_get_width() { 
  int res;
  SDL_GetWindowSize(win, &res, 0);
  return res;
}

int window_get_height() { 
  int res;
  SDL_GetWindowSize(win, 0, &res);
  return res;
}

//Setters
void window_set_position(int x,int y) {
  SDL_SetWindowPosition(win, x, y);
}

void window_set_size(unsigned int w,unsigned int h) {
  SDL_SetWindowSize(win, w, h);
}

void window_set_rectangle(int x,int y,int w,int h) {
  window_set_position(x, y);
  window_set_size(w, h);
}

//Center
void window_center() {
  window_set_position(display_get_width()/2-window_get_width()/2, display_get_height()/2-window_get_height()/2);
}

}

////////////////
// FULLSCREEN //
////////////////

namespace enigma_user
{

void window_set_freezeonlosefocus(bool freeze)
{
  //TODO
}

bool window_get_freezeonlosefocus()
{
  return false;
}

void window_set_fullscreen(bool full)
{
  SDL_SetWindowFullscreen(win, full?SDL_WINDOW_FULLSCREEN:0);
}

bool window_get_fullscreen()
{
  //TODO: track and return fullscreen flag.
  return false;
}

}

namespace enigma
{
  //Replacing usermap array with keybdmap map, to align code with Windows implementation.
  std::map<int,int> keybdmap;
  //unsigned char usermap[256];

  unsigned char keymap[512];
  void initkeymap() {
    using namespace enigma_user;
    // Pretend this part doesn't exist
    keymap[0x151] = vk_left;
    keymap[0x153] = vk_right;
    keymap[0x152] = vk_up;
    keymap[0x154] = vk_down;
    keymap[0x1E3] = vk_control;
    keymap[0x1E4] = vk_control;
    keymap[0x1E9] = vk_alt;
    keymap[0x1EA] = vk_alt;
    keymap[0x1E1] = vk_shift;
    keymap[0x1E2] = vk_shift;
    keymap[0x10D] = vk_enter;
    keymap[0x185] = vk_lsuper;
    keymap[0x186] = vk_rsuper;
    keymap[0x117] = vk_tab;
    keymap[0x142] = vk_caps;
    keymap[0x14E] = vk_scroll;
    keymap[0x17F] = vk_pause;
    keymap[0x19E] = vk_numpad0;
    keymap[0x19C] = vk_numpad1;
    keymap[0x199] = vk_numpad2;
    keymap[0x19B] = vk_numpad3;
    keymap[0x196] = vk_numpad4;
    keymap[0x19D] = vk_numpad5;
    keymap[0x198] = vk_numpad6;
    keymap[0x195] = vk_numpad7;
    keymap[0x197] = vk_numpad8;
    keymap[0x19A] = vk_numpad9;
    keymap[0x1AF] = vk_divide;
    keymap[0x1AA] = vk_multiply;
    keymap[0x1AD] = vk_subtract;
    keymap[0x1AB] = vk_add;
    keymap[0x19F] = vk_decimal;
    keymap[0x1BE] = vk_f1;
    keymap[0x1BF] = vk_f2;
    keymap[0x1C0] = vk_f3;
    keymap[0x1C1] = vk_f4;
    keymap[0x1C2] = vk_f5;
    keymap[0x1C3] = vk_f6;
    keymap[0x1C4] = vk_f7;
    keymap[0x1C5] = vk_f8;
    keymap[0x1C6] = vk_f9;
    keymap[0x1C7] = vk_f10;
    keymap[0x1C8] = vk_f11;
    keymap[0x1C9] = vk_f12;
    keymap[0x108] = vk_backspace;
    keymap[0x11B] = vk_escape;
    keymap[0x150] = vk_home;
    keymap[0x157] = vk_end;
    keymap[0x155] = vk_pageup;
    keymap[0x156] = vk_pagedown;
    keymap[0x1FF] = vk_delete;
    keymap[0x163] = vk_insert;

    // Set up identity map...
    //for (int i = 0; i < 255; i++)
    //  usermap[i] = i;

    for (int i = 0; i < 255; i++) {
      keymap[i] = i;
    }
    for (int i = 'a'; i <= 'z'; i++) { // 'a' to 'z' wrap to 'A' to 'Z'
      keymap[i] = i + 'A' - 'a';
    }
    for (int i = 'z'+1; i < 255; i++) {
      keymap[i] = i;
    }
  }
}

#include <sys/time.h>

namespace enigma_user {
  extern double fps;
}

namespace enigma {
  string* parameters;
  unsigned int parameterc;
  int current_room_speed;
  void windowsystem_write_exename(char* x) {
    unsigned irx = 0;
    if (enigma::parameterc) {
      for (irx = 0; enigma::parameters[0][irx] != 0; irx++) {
        x[irx] = enigma::parameters[0][irx];
      }
    }
    x[irx] = 0;
  }

  #define hielem 9
  void set_room_speed(int rs) {
    current_room_speed = rs;
  }
}

#include "Universal_System/globalupdate.h"

namespace enigma_user
{

void io_handle()
{
  enigma::input_push();
  if (!handleEvents()) {
    exit(0);
  }
  enigma::update_mouse_variables();
}

int window_set_cursor(int c)
{
  //TODO: setup cursors.
  if (c==cr_none) {
  } else {
  }
  return 0;
}

void keyboard_wait()
{
  io_clear();
  for (;;) {
    io_handle();
    for (int i = 0; i < 255; i++) {
      if (enigma::keybdstatus[i]) {
        return;
      }
    }
    usleep(10000); // Sleep 1/100 second
  }
}

void keyboard_set_map(int key1, int key2)
{
  std::map< int, int >::iterator it = enigma::keybdmap.find( key1 );
  if ( enigma::keybdmap.end() != it ) {
    it->second = key2;
  } else {
    enigma::keybdmap.insert( map< int, int >::value_type(key1, key2) );
  }
}

int keyboard_get_map(int key)
{
  std::map< int, int >::iterator it = enigma::keybdmap.find( key );
  if ( enigma::keybdmap.end() != it ) {
    return it->second;
  } else {
    return key;
  }
}

void keyboard_unset_map()
{
  enigma::keybdmap.clear();
}

void keyboard_clear(const int key)
{
  enigma::keybdstatus[key] = enigma::last_keybdstatus[key] = 0;
}


void window_set_region_scale(double scale, bool adaptwindow) 
{
}

double window_get_region_scale() 
{
  return 1;
}

void window_set_region_size(int w, int h, bool adaptwindow) 
{
}

int window_get_region_width()
{
  //TODO
  return enigma_user::window_get_width();
}

int window_get_region_height()
{
  //TODO
  return enigma_user::window_get_height();
}

int window_get_region_width_scaled()
{
  //TODO
  return enigma_user::window_get_width();
}

int window_get_region_height_scaled()
{
  //TODO
  return enigma_user::window_get_height();
}

string parameter_string(unsigned num) {
  return num < enigma::parameterc ? enigma::parameters[num] : "";
}
int parameter_count() {
  return enigma::parameterc;
}

}

