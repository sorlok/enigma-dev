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


#ifndef ENIGMA_GM5COMPAT_VIEWS__H
#define ENIGMA_GM5COMPAT_VIEWS__H


///This file contains some missing views-based variables for GM5.

#include "Universal_System/roomsystem.h"

namespace enigma 
{

//Helper base class. Defines various common operations.
class RvtBase {
protected:
  explicit RvtBase(var& orig) : orig(orig) {} //Always use sub-classes.
  virtual ~RvtBase() {} //Give it a vtable.
  var& orig; //The "primary" value we are representing.

public:
  template <typename T>
  var operator+(const T& rhs) const {
    return orig + rhs;
  }
  template <typename T>
  RvtBase& operator+= (const T& rhs) {
    (*this) = orig + rhs;
    return *this;
  }
  template <typename T>
  var operator-(const T& rhs) const {
    return orig - rhs;
  }
  template <typename T>
  RvtBase& operator-= (const T& rhs) {
    (*this) = orig - rhs;
    return *this;
  }
  template <typename T>
  var operator*(const T& rhs) const {
    return orig * rhs;
  }
  template <typename T>
  RvtBase& operator*= (const T& rhs) {
    (*this) = orig * rhs;
    return *this;
  }
  template <typename T>
  var operator/(const T& rhs) const {
    return orig / rhs;
  }
  template <typename T>
  RvtBase& operator/= (const T& rhs) {
    (*this) = orig / rhs;
    return *this;
  }

  //TODO: This won't work for RvtBind2. 
  //      I think we might want to re-think how indexed and non-indexed var references work.
  variant& operator()(int id) {
    return orig(id);
  }
};

//Helper class: Bind an rvt setter/getter to an existing variable in enigma_user.
class RvtBind : public RvtBase{
public:
  explicit RvtBind(var& orig) : RvtBase(orig) {}

  operator var() const {
    return orig;
  }

  RvtBind& operator=(var value) {
    orig = value;
    return *this;
  }
};
class RvtBind2 : public RvtBase{
public:
  explicit RvtBind2(var& orig1, var& orig2) : RvtBase(orig1), orig2(orig2) {}

  operator var() const {
    return orig;
  }

  RvtBind2& operator=(var value) {
    orig = value;
    orig2 = value;
    return *this;
  }

private:
  var& orig2;
};
class RvtBind3 : public RvtBase{
public:
  explicit RvtBind3(var& orig, const var& span) : RvtBase(orig), span(span) {}

  operator var() const {
    return orig + span/2;
  }

  RvtBind3& operator=(var value) {
    orig += (value-orig);
    return *this;
  }

private:
  const var& span;
};

}


namespace enigma_user
{

//View variables that become ambiguous with the introduction of view+port notation.
//Our approach to these is on a case-by-case basis. As a guiding principle, if there
//are corresponding view_Xport and view_Xview variables, we always SET both, but only
//GET the *view variant.
extern enigma::RvtBind3 view_left;   //Left side of the view. Mapped to view_x+view_width/2
extern enigma::RvtBind3 view_top;    //Top side of the view. Mapped to view_y+view_height/2
extern enigma::RvtBind2 view_width;  //View width. Mapped to view_wview/view_wport
extern enigma::RvtBind2 view_height; //View height. Mapped to view_hview/view_hport
extern enigma::RvtBind view_x;      //View's X position. Mapped to view_xview
extern enigma::RvtBind view_y;      //View's Y position. Mapped to view_yview


}

#endif // ENIGMA_GM5COMPAT_VIEWS__H

