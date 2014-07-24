/** Copyright (C) 2008-2011 Josh Ventura
*** Copyright (C) 2010 Alasdair Morrison <tgmg@g-java.com>
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

#include <string>
#include <stdio.h>
using namespace std;

#include "backgroundstruct.h"
#include "Graphics_Systems/graphics_mandatory.h"
#include "Widget_Systems/widgets_mandatory.h"
#include "Platforms/platforms_mandatory.h"
#include "libEGMstd.h"
#include "zlib.h"
#include "resinit.h"


namespace enigma
{
  void exe_loadbackgrounds(FILE *exe)
  {
    int nullhere=0;
    unsigned bkgid=0, width=0, height=0,transparent=0,smoothEdges=0,preload=0,useAsTileset=0,tileWidth=0,tileHeight=0,hOffset=0,vOffset=0,hSep=0,vSep=0;

    if (!fread(&nullhere,4,1,exe) or nullhere != *(int*)"BKG ")
      return;

    // Determine how many backgrounds we have
    int bkgcount=0;
    if (!fread(&bkgcount,4,1,exe))
      return;


	  // Fetch the highest ID we will be using
	  int bkg_highid=0;
	  if (!fread(&bkg_highid,4,1,exe))
	    return;

	  for (int i = 0; i < bkgcount; i++)
	  {
		  int unpacked=0;
		  if (!fread(&bkgid, 4,1,exe)) return;
		  if (!fread(&width, 4,1,exe)) return;
		  if (!fread(&height,4,1,exe)) return;
		  if (!fread(&transparent,4,1,exe)) return;
		  if (!fread(&smoothEdges,4,1,exe)) return;
		  if (!fread(&preload,4,1,exe)) return;
		  if (!fread(&useAsTileset,4,1,exe)) return;
		  if (!fread(&tileWidth,4,1,exe)) return;
		  if (!fread(&tileHeight,4,1,exe)) return;
		  if (!fread(&hOffset,4,1,exe)) return;
		  if (!fread(&vOffset,4,1,exe)) return;
		  if (!fread(&hSep,4,1,exe)) return;
		  if (!fread(&vSep,4,1,exe)) return;

		  //need to add: transparent, smooth, preload, tileset, tileWidth, tileHeight, hOffset, vOffset, hSep, vSep

		  unpacked = width*height*4;

		  unsigned int size=0;
		  if (!fread(&size,4,1,exe)){};

		  unsigned char* cpixels=new unsigned char[size+1];
		  if (!cpixels)
		  {
			  show_error("Failed to load background: Cannot allocate enough memory "+toString(unpacked),0);
			  break;
		  }
		  unsigned int sz2=fread(cpixels,1,size,exe);
		  if (size!=sz2) {
			  show_error("Failed to load background: Data is truncated before exe end. Read "+toString(sz2)+" out of expected "+toString(size),0);
			  return;
		  }
		  unsigned char* pixels=new unsigned char[unpacked+1];
		  if (zlib_decompress(cpixels,size,unpacked,pixels) != unpacked)
		  {
			  show_error("Background load error: Background does not match expected size",0);
			  continue;
		  }
		  delete[] cpixels;

		  background_new(bkgid, width, height, pixels, false, false, true, false, 32, 32, 0, 0, 1,1);

		  delete[] pixels;
	  }
  }
}
