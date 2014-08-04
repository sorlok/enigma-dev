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

#include "Universal_System/scalar.h"
#include "Universal_System/var4.h"


namespace enigma 
{

//Helper base class. Defines various common operations.
class ConglomRefRO {
public:
  explicit ConglomRefRO(var& orig) : orig(orig) {}
  virtual ~ConglomRefRO() {} //Give it a vtable.

public:
  //Retrieve based on index.
  const variant& operator[](int id) const {
    return orig[id];
  }
  const variant& operator()(int id) const {
    return orig(id);
  }

  //Retrieve index zero. 
  //TODO: Needs more testing to see how GM actually implements it; it's probably the first "valid" view.
  operator cs_scalar() const {
    return (*this)[0];
  }
/*  operator double() const {
    return (*this)[0];
  }*/

  //Common operations.
  template <typename T>
  var operator+(const T& rhs) const {
    return orig + rhs;
  }
  template <typename T>
  var operator-(const T& rhs) const {
    return orig - rhs;
  }
  template <typename T>
  var operator*(const T& rhs) const {
    return orig * rhs;
  }
  template <typename T>
  var operator/(const T& rhs) const {
    return orig / rhs;
  }

protected:
  var& orig; //The "primary" value we are representing.
};


//Let's make things even more complicated.
class InterpHolder {
public:
  InterpHolder(variant& origVal, const variant& span) : origVal(origVal), newVal(origVal), span(span) {}
  ~InterpHolder() {
    if (newVal != origVal) {
      origVal = newVal;
    }
  }

  template <typename T>
  InterpHolder& operator=(const T& val) {
    newVal = val;
    return *this;
  }

  operator cs_scalar() const {
    return newVal;
  }
/*  operator double() const {
    return newVal;
  }*/
/*  operator variant() const {
    return newVal;
  }*/


  //Common operations.
  template <typename T>
  var operator+(const T& rhs) const {
    return newVal + rhs;
  }
  template <typename T>
  var operator-(const T& rhs) const {
    return newVal - rhs;
  }
  template <typename T>
  var operator*(const T& rhs) const {
    return newVal * rhs;
  }
  template <typename T>
  var operator/(const T& rhs) const {
    return newVal / rhs;
  }
private:
  variant& origVal;
  variant newVal;
  const variant& span;
};


class ConglomInterp : public ConglomRefRO {
public:
  explicit ConglomInterp(var& orig, const var& span) : ConglomRefRO(orig), span(span) {}

  //Retrieve based on index.
  InterpHolder operator[](int id) {
    return InterpHolder(orig[id], span[id]);
  }
  InterpHolder operator()(int id) {
    return (*this)[id];
  }

  //Set it (index 0)
  template <typename T>
  ConglomInterp& operator=(const T& val) {
    (*this)[0] = val;
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
extern enigma::ConglomRefRO view_x;      //View's X position. Mapped to view_xview
extern enigma::ConglomRefRO view_y;      //View's Y position. Mapped to view_yview
extern enigma::ConglomRefRO view_width;  //View width. Mapped to view_wview/view_wport
extern enigma::ConglomRefRO view_height; //View height. Mapped to view_hview/view_hport
extern enigma::ConglomInterp view_left;   //Left side of the view. Mapped to view_x+view_width/2
extern enigma::ConglomInterp view_top;    //Top side of the view. Mapped to view_y+view_height/2


}

#endif // ENIGMA_GM5COMPAT_VIEWS__H

