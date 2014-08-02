/** Copyright (C) 2008-2011 IsmAvatar <cmagicj@nni.com>, Josh Ventura
*** Copyright (C) 2013 Robert B. Colton
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

//This file has been modified beyond recognition by Josh @ Dreamland
//under the pretense that it would be better compatible with ENIGMA

#include <SDL.h>
#include <GL/gl.h>
#include <unistd.h>
#include <stdio.h>
#include <sched.h> //For sched_yield()
#include <string>
#include <iostream>

#include "Platforms/platforms_mandatory.h"

#include "SDLmain.h"
#include "SDLwindow.h"
#include "LINUXjoystick.h"

#include "Universal_System/var4.h"
#include "Universal_System/CallbackArrays.h"
#include "Universal_System/roomsystem.h"
#include "Universal_System/loading.h"

#include <time.h>

namespace enigma_user {
  const int os_type = os_linux; //TODO: Varies based on SDL platform.
  extern int keyboard_key;
  extern int keyboard_lastkey;
  extern string keyboard_lastchar;
  extern string keyboard_string;
}

namespace enigma
{
  int game_return = 0;
  extern char keymap[512];
  void ENIGMA_events(void); //TODO: Synchronize this with Windows by putting these two in a single header.
  bool gameWindowFocused = false;
  extern bool freezeOnLoseFocus;
  unsigned int pausedSteps = 0;

namespace sdl {
SDL_Window* win = NULL;
SDL_DisplayMode main_display;
}
}

using namespace enigma::sdl;

namespace enigma {
  namespace sdl {
    SDL_Event event;

    //Return false if the game should end.
    bool handleEvents()
    {
      unsigned char actualKey = 0;
      char uniStr[2] = {0};
      while (SDL_PollEvent(&event)){
        switch(event.type) {
          case SDL_QUIT:
            return false;

          case SDL_WINDOWEVENT:
            switch (event.window.event) {
              case SDL_WINDOWEVENT_FOCUS_GAINED:
                enigma::gameWindowFocused = true;
                pausedSteps = 0;
                break;
              case SDL_WINDOWEVENT_FOCUS_LOST:
                enigma::gameWindowFocused = false;
                break;
            }
            break;

          case SDL_KEYDOWN:
            if (event.key.keysym.sym&0xFF00) { //TODO: This was from Xlib. See if it still matters.
              actualKey = enigma_user::keyboard_get_map(event.key.keysym.sym&0xFF);
            } else {
              actualKey = enigma_user::keyboard_get_map(event.key.keysym.sym&0x1FF);
            }

            //Set the last-typed letter.
            //TODO
            /*if (event.key.keysym.unicode>0) {
              uniStr[0] = event.key.keysym.unicode;
              enigma_user::keyboard_lastchar = string(1, uniStr[0]);
              enigma_user::keyboard_string += enigma_user::keyboard_lastchar;
              if (enigma_user::keyboard_lastkey == enigma_user::vk_backspace) {
                enigma_user::keyboard_string = enigma_user::keyboard_string.substr(0, enigma_user::keyboard_string.length() - 1);
              } else {
                enigma_user::keyboard_string += enigma_user::keyboard_lastchar;
              }
            }*/

            //Now set the actual key values.
            enigma_user::keyboard_lastkey = actualKey;
            enigma_user::keyboard_key = actualKey;
            if (enigma::last_keybdstatus[actualKey]==1 && enigma::keybdstatus[actualKey]==0) {
              enigma::keybdstatus[actualKey]=1;
              break;
            }
            enigma::last_keybdstatus[actualKey]=enigma::keybdstatus[actualKey];
            enigma::keybdstatus[actualKey]=1;
            break;

          case SDL_KEYUP:
            enigma_user::keyboard_key = 0;
            if ((event.key.keysym.sym&0xFF00)==0) { //TODO: This was from Xlib. See if it still matters.
              actualKey = enigma_user::keyboard_get_map(event.key.keysym.sym&0xFF);
            } else {
              actualKey = enigma_user::keyboard_get_map(event.key.keysym.sym&0x1FF);
            }

            //Now set the actual key values.
            enigma::last_keybdstatus[actualKey]=enigma::keybdstatus[actualKey];
            enigma::keybdstatus[actualKey]=0;
            break;

          case SDL_MOUSEBUTTONDOWN:
            //TODO: Make sure this is right.
            switch (event.button.button) {
              case SDL_BUTTON_LEFT:
                enigma::mousestatus[0] = 1;
                break;
              case SDL_BUTTON_RIGHT:
                enigma::mousestatus[1] = 1;
                break;
              case SDL_BUTTON_MIDDLE:
                enigma::mousestatus[2] = 1;
                break;
            }
            break;

          case SDL_MOUSEBUTTONUP:
            //TODO: Make sure this is right.
            switch (event.button.button) {
              case SDL_BUTTON_LEFT:
                enigma::mousestatus[0] = 0;
                break;
              case SDL_BUTTON_RIGHT:
                enigma::mousestatus[1] = 0;
                break;
              case SDL_BUTTON_MIDDLE:
                enigma::mousestatus[2] = 0;
                break;
            }
            break;

          case SDL_MOUSEWHEEL:
            //TODO: Possibly ignore "touch" events here.
            if (event.button.y != 0) {
              mouse_vscrolls += event.button.y>0 ? 1 : -1;
            }
            if (event.button.x != 0) {
              mouse_hscrolls += event.button.x>0 ? 1 : -1;
            }
            break;

          //TODO: Catch other things like focussed/non-focussed, minimized/maximized, and maybe custom messages?
        }
      }
      return true;
    }

  }
}


namespace enigma
{
  //initialize_everything() calls this at the appropriate time. The goal is to null out all input in preparation for the main loop.
  void input_initialize()
  {
    //Clear the input arrays
    for(int i=0;i<3;i++){
      last_mousestatus[i]=0;
      mousestatus[i]=0;
    }
    for(int i=0;i<256;i++){
      last_keybdstatus[i]=0;
      keybdstatus[i]=0;
    }

    init_joysticks();
  }

  //Push the old set of inputs into last_*, and then reset mouse scrolls.
  void input_push()
  {
    for(int i=0;i<3;i++){
      last_mousestatus[i] = mousestatus[i];
    }
    for(int i=0;i<256;i++){
      last_keybdstatus[i] = keybdstatus[i];
    }
    mouse_hscrolls = mouse_vscrolls = 0;
  }

  int game_ending();
}

//TODO: Implement pause events
unsigned long current_time_mcs = 0; // microseconds since the start of the game

namespace enigma_user {
  std::string working_directory = "";
  extern double fps;
  unsigned long current_time = 0; // milliseconds since the start of the game
  unsigned long delta_time = 0; // microseconds since the last step event

  unsigned long get_timer() {  // microseconds since the start of the game
    return current_time_mcs;
  }
}

static inline long clamp(long value, long min, long max)
{
  if (value < min) return min;
  if (value > max) return max;
  return value;
}

#include <unistd.h>
static bool game_isending = false;

int main(int argc,char** argv)
{
////////////////////////////////////////////////////////////////////////
// Startup
////////////////////////////////////////////////////////////////////////

  // Set the working_directory
  char buffer[1024];
  if (getcwd(buffer, sizeof(buffer))) {
     fprintf(stdout, "Current working dir: %s\n", buffer);
  } else {
     perror("getcwd() error");
  }
  enigma_user::working_directory = buffer;

  // Copy our parameters
  enigma::parameters = new string[argc];
  enigma::parameterc = argc;
  for (int i=0; i<argc; i++) {
    enigma::parameters[i]=argv[i];
  }
  enigma::initkeymap();

  //Initialize SDL.
  if (SDL_Init(SDL_INIT_EVERYTHING) != 0){
    std::cout <<"SDL_Init failed: " <<SDL_GetError() <<"\n";
    return -1;
  }

  //Query displays to get the maximum width/height of any attached display (this is always display 0).
  int display_count = 0;
  if ((display_count = SDL_GetNumVideoDisplays()) < 1) {
    std::cout <<"SDL_GetNumVideoDisplays failed: " <<display_count <<" displays only.\n";
    SDL_Quit();
    return -1;
  }

  if (SDL_GetCurrentDisplayMode(0, &main_display) != 0) {
    std::cout <<"SDL_GetDesktopDisplayMode failed: " <<SDL_GetError() <<"\n";
    SDL_Quit();
    return -1;
  }

  //Default screen size (avoid creating massive windows if views are undefined; this matches GM and can always be over-ridden with views).
  int win_width = std::min(enigma_user::room_width, main_display.w);
  int win_height = std::min(enigma_user::room_height, main_display.h);

  //Get information about the current video settings.
/*  const SDL_VideoInfo* info = SDL_GetVideoInfo();
  if (!info) {
    std::cout <<"SDL_GetVideoInfo failed: " <<SDL_GetError() <<"\n";
    SDL_Quit();
    return -1;
  }*/

  //Set relevant OpenGL attributes
  //TODO: These might be high; XLib seems to start high and then re-calculate to low, while SDL will just fail.
  SDL_GL_SetAttribute(SDL_GL_RED_SIZE,     8);
  SDL_GL_SetAttribute(SDL_GL_GREEN_SIZE,   8);
  SDL_GL_SetAttribute(SDL_GL_BLUE_SIZE,    8);
  SDL_GL_SetAttribute(SDL_GL_BUFFER_SIZE, 32);
  SDL_GL_SetAttribute(SDL_GL_DEPTH_SIZE,  24);
  SDL_GL_SetAttribute(SDL_GL_DOUBLEBUFFER, 1);

  //Create a Window.
  win = SDL_CreateWindow("", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, win_width, win_height, SDL_WINDOW_OPENGL);
  if (!win){
    std::cout <<"SDL_CreateWindow failed: " <<SDL_GetError() <<"\n";
    SDL_Quit();
    return -1;
  }

  //Clear everything.
  glClear(GL_COLOR_BUFFER_BIT|GL_DEPTH_BUFFER_BIT|GL_ACCUM_BUFFER_BIT|GL_STENCIL_BUFFER_BIT);

  //Call ENIGMA system initializers; sprites, audio, and what have you
  enigma::initialize_everything();


////////////////////////////////////////////////////////////////////////
// Main Loop
////////////////////////////////////////////////////////////////////////


  struct timespec time_offset;
  struct timespec time_offset_slowing;
  struct timespec time_current;
  clock_gettime(CLOCK_MONOTONIC, &time_offset);
  time_offset_slowing.tv_sec = time_offset.tv_sec;
  time_offset_slowing.tv_nsec = time_offset.tv_nsec;
  int frames_count = 0;

  while (!game_isending) {
    //Update timer, FPS
    clock_gettime(CLOCK_MONOTONIC, &time_current);
    {
      long passed_mcs = (time_current.tv_sec - time_offset.tv_sec)*1000000 + (time_current.tv_nsec/1000 - + time_offset.tv_nsec/1000);
      passed_mcs = clamp(passed_mcs, 0, 1000000);
      if (passed_mcs >= 1000000) { // Handle resetting.
        enigma_user::fps = frames_count;
        frames_count = 0;
        time_offset.tv_sec += passed_mcs/1000000;
        time_offset_slowing.tv_sec = time_offset.tv_sec;
        time_offset_slowing.tv_nsec = time_offset.tv_nsec;
      }
    }

    //Update room if room_speed is positive.
    long spent_mcs = 0;
    long last_mcs = 0;
    if (enigma::current_room_speed > 0) {
      spent_mcs = (time_current.tv_sec - time_offset_slowing.tv_sec)*1000000 + (time_current.tv_nsec/1000 - time_offset_slowing.tv_nsec/1000);
      spent_mcs = clamp(spent_mcs, 0, 1000000);
      long remaining_mcs = 1000000 - spent_mcs;
      long needed_mcs = long((1.0 - 1.0*frames_count/enigma::current_room_speed)*1e6);
      const int catchup_limit_ms = 50;
      if (needed_mcs > remaining_mcs + catchup_limit_ms*1000) {
        // If more than catchup_limit ms is needed than is remaining, we risk running too fast to catch up.
        // In order to avoid running too fast, we advance the offset, such that we are only at most catchup_limit ms behind.
        // Thus, if the load is consistently making the game slow, the game is still allowed to run as fast as possible
        // without any sleep.
        // And if there is very heavy load once in a while, the game will only run too fast for catchup_limit ms.
        time_offset_slowing.tv_nsec += 1000*(needed_mcs - (remaining_mcs + catchup_limit_ms*1000));
        spent_mcs = (time_current.tv_sec - time_offset_slowing.tv_sec)*1000000 + (time_current.tv_nsec/1000 - time_offset_slowing.tv_nsec/1000);
        spent_mcs = clamp(spent_mcs, 0, 1000000);
        remaining_mcs = 1000000 - spent_mcs;
        needed_mcs = long((1.0 - 1.0*frames_count/enigma::current_room_speed)*1e6);
      }

      //Sleep if there's
      if (remaining_mcs > needed_mcs) {
        const long sleeping_time = std::min((remaining_mcs - needed_mcs)/5, long(999999));
        usleep(std::max(long(1), sleeping_time));
        continue;
      } else {
        sched_yield(); //Otherwise we can freeze the system. (TODO: Lots of code can freeze the system; maybe we should re-evaluate this.)
      }
    }

    //TODO: The placement of this code is inconsistent with Win32 because events are handled after, ask Josh.
    unsigned long dt = 0;
    if (spent_mcs > last_mcs) {
        dt = (spent_mcs - last_mcs);
    } else {
        //TODO: figure out what to do here this happens when the fps is reached and the timers start over
        dt = enigma_user::delta_time;
    }
    last_mcs = spent_mcs;
    enigma_user::delta_time = dt;
    current_time_mcs += enigma_user::delta_time;
    enigma_user::current_time += enigma_user::delta_time / 1000;

    //Handle SDL events.
    if (!handleEvents()) {
      break;
    }

    if (!enigma::gameWindowFocused && enigma::freezeOnLoseFocus) { 
      if (enigma::pausedSteps < 1) {
        enigma::pausedSteps += 1;
      } else {
        usleep(100000); 
        continue; 
      }
    }

    enigma::handle_joysticks();
    enigma::ENIGMA_events();
    enigma::input_push();

    frames_count++;
  }

////////////////////////////////////////////////////////////////////////
// Teardown
////////////////////////////////////////////////////////////////////////

  enigma::game_ending();
  SDL_DestroyWindow(win);
  SDL_Quit();

  return enigma::game_return;
}

namespace enigma_user
{

void game_end(int ret) {
  game_isending = true;
  enigma::game_return = ret;
}

void action_end_game() {
  game_end();
}

int display_get_width() {
  return main_display.w;
}
int display_get_height() {
  return main_display.h;
}

}

