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


#include "GM5Views.h"


namespace enigma_user
{

enigma::RvtBind2 view_width(view_wview, view_wport);
enigma::RvtBind2 view_height(view_hview, view_hport);
enigma::RvtBind view_x(view_xview);
enigma::RvtBind view_y(view_yview);
enigma::RvtBind3 view_left(view_xview, view_wview);
enigma::RvtBind3 view_top(view_yview, view_hview);

}

