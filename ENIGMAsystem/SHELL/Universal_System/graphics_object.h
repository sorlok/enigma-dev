/** Copyright (C) 2010-2011 Josh Ventura
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

/**
  @file    graphics_object.h
  @summary Adds a graphics-related tier following the timelines tier.
*/

#ifndef _GRAPHICS_OBJECT_H
#define _GRAPHICS_OBJECT_H

#ifndef INCLUDED_FROM_SHELLMAIN
  #include "var4.h"
  #include "spritestruct.h"
  #include <cstdlib>
  #include <cmath>
#endif

#include "scalar.h"
#include "timelines_object.h"
#include "multifunction_variant.h"

namespace enigma
{
  class ImageIndex {
  public:
    ImageIndex() : value(0), manual(true) {}

    ImageIndex& operator=(int value) { //Using "int" will always reset the substep.
      this->value = value;
      manual = true;
      return *this;
    }

    void incr(gs_scalar speed, int sprite_num) { //Called in the event loop.
      value = fmod((speed < 0)?(sprite_num + value - fmod(abs(speed),sprite_num)):(value + speed), sprite_num);
    }

    void set(gs_scalar value, bool manual) { //To be called by image_single.
      this->value = value;
      this->manual = manual;
    }

    bool isManual() const { //To be called by image_single.
      return manual;
    }

    operator gs_scalar() {
      return value;
    }

  private:
    gs_scalar value;   //The value returned by operator gs_scalar(). It includes the "substep" after the decimal point.
    bool manual; //Was this value set manually by the user? (else, image_single set it).
  };


  class ImageSingle {
  public:
    ImageSingle(ImageIndex& img_ind) : img_ind(img_ind), value(0), oldImgIndex(0) {}

    ImageSingle& operator=(int value) {
      //First, handle the linked image_index.
      if (value>=0) {
        if (img_ind.isManual()) { oldImgIndex = img_ind; } //Save if it was manually set.
        img_ind.set(value, false);
      } else {
        img_ind.set(oldImgIndex, true);
      }

      //Now, set the value.
      this->value = value;
      return *this;
    }

   operator int() {
      return value;
   }

  private:
    ImageIndex& img_ind; //The image_index reference we are monitoring.
    int value;        //The value returned by operator int()
    gs_scalar oldImgIndex;  //The value to return ImageIndex to when ImageSingle is set to -1.
  };




  extern bool gui_used;
  struct depthv: multifunction_variant {
    INHERIT_OPERATORS(depthv)
    struct inst_iter *myiter;
    void function(variant oldval);
    void init(gs_scalar depth, object_basic* who);
    void remove();
    ~depthv();
  };
  struct object_graphics: object_timelines
  {
    //Sprites: these are mostly for higher tiers...
      int sprite_index;
      gs_scalar image_speed;

      ImageIndex image_index;
      ImageSingle image_single; //Older combination of image_index and image_speed.

      //Depth
      enigma::depthv  depth;
      bool visible;

    //Transformations: these are mostly for higher tiers...
      gs_scalar image_xscale;
      gs_scalar image_yscale;
      gs_scalar image_angle;

    //Accessors
      #ifdef JUST_DEFINE_IT_RUN
        int sprite_width, sprite_height;
        int sprite_xoffset, sprite_yoffset;
        int image_number;
      #else
        int $sprite_width() const;
        int $sprite_height() const;
        int $sprite_xoffset() const;
        int $sprite_yoffset() const;
        int $image_number() const;
        #define sprite_width $sprite_width()
        #define sprite_height $sprite_height()
        #define sprite_xoffset $sprite_xoffset()
        #define sprite_yoffset $sprite_yoffset()
        #define image_number $image_number()
      #endif

    //Constructors
      object_graphics();
      object_graphics(unsigned x, int y);
      virtual ~object_graphics();
  };
}

#endif
