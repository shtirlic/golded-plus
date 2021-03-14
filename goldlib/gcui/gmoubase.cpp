//  This may look like C code, but it is really -*- C++ -*-

//  ------------------------------------------------------------------
//  The Goldware Library
//  Copyright (C) 1990-1999 Odinn Sorensen
//  ------------------------------------------------------------------
//  This library is free software; you can redistribute it and/or
//  modify it under the terms of the GNU Library General Public
//  License as published by the Free Software Foundation; either
//  version 2 of the License, or (at your option) any later version.
//
//  This library is distributed in the hope that it will be useful,
//  but WITHOUT ANY WARRANTY; without even the implied warranty of
//  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
//  Library General Public License for more details.
//
//  You should have received a copy of the GNU Library General Public
//  License along with this program; if not, write to the Free
//  Software Foundation, Inc., 59 Temple Place, Suite 330, Boston,
//  MA 02111-1307, USA
//  ------------------------------------------------------------------
//  $Id$
//  ------------------------------------------------------------------
//  GCUI: Golded+ Character-oriented User Interface.
//  Mousing: class GMou member functions implementation.
//  ------------------------------------------------------------------

#include <gmemall.h>
#include <gvidall.h>
#include <gmoubase.h>


//  ------------------------------------------------------------------

#ifdef GOLD_MOUSE

//  ------------------------------------------------------------------

GMou gmou;


//  ------------------------------------------------------------------

GMou::GMou()
{

    detected = false;
    Reset();
}


//  ------------------------------------------------------------------

GMou::~GMou()
{

    Reset();
}


//  ------------------------------------------------------------------

void GMou::SetLevel(int __level)
{

    if(detected)
    {
        level = __level;
        if(level < GMOU_LEVEL_NONE)
            level = GMOU_LEVEL_NONE;
        else if(level > GMOU_LEVEL_FULL)
            level = GMOU_LEVEL_FULL;
    }
    else
    {
        level = GMOU_LEVEL_NONE;
    }
}


//  ------------------------------------------------------------------

void GMou::Reset()
{

    level = GMOU_LEVEL_NONE;
    hidden = 1;
    hit.button = 0;
    hit.count = 0;
    hit.row = 0;
    hit.column = 0;
    if(detected)
        Init();
}

//  ------------------------------------------------------------------
//  Dummy definitions when mouse support is not defined

void GMou::Init() {}
void GMou::ClearEvents() {}
void GMou::GetStatus() {}
void GMou::GetPress(int) {}
void GMou::GetRelease(int) {}
void GMou::SetCursor(int, int, int) {}
void GMou::SetPosition(int, int) {}
void GMou::HideCursor() {}
void GMou::ShowCursor() {}


//  ------------------------------------------------------------------

#endif // GOLD_MOUSE

//  ------------------------------------------------------------------
