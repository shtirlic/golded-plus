//  This may look like C code, but it is really -*- C++ -*-

//  ------------------------------------------------------------------
//  The Goldware Library
//  Copyright (C) 1990-1999 Odinn Sorensen
//  Copyright (C) 1999-2000 Alexander S. Aganichev
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
//  Memory handling.
//  ------------------------------------------------------------------

#ifndef __gmemall_h
#define __gmemall_h


//  ------------------------------------------------------------------

#include <stdlib.h>
#include <gdefs.h>


#define HEX_DUMP1 1
#define HEX_DUMP2 2

char* HexDump16(char* strbuf, const char* memptr, int limit, const char* fmt, int fmtno=0);
inline char* HexDump16(char* strbuf, const char* memptr, int limit, int fmtno)
{
    return HexDump16(strbuf, memptr, limit, NULL, fmtno);
}

//  ------------------------------------------------------------------

#endif

//  ------------------------------------------------------------------
