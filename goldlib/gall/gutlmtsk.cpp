//  This may look like C code, but it is really -*- C++ -*-

//  ------------------------------------------------------------------
//  The Goldware Library
//  Copyright (C) 1990-1999 Odinn Sorensen
//  Copyright (C) 1999 Alexander S. Aganichev
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
//  Multitasking.
//  ------------------------------------------------------------------

#include <cstdlib>
#include <gmemall.h>
#include <gutlmtsk.h>


#ifdef __WIN32__
    #include <windows.h>
#endif

#ifdef __GNUC__
    #include <unistd.h>
#endif

//  ------------------------------------------------------------------
//  Global multitasker data

GMTsk gmtsk;


//  ------------------------------------------------------------------

GMTsk::GMTsk()
{

    detected = GMTSK_NONE;
    name = "";
#if defined(__UNIX__)
    detected = GMTSK_LINUX;
    name = "UNIX";
#else
    if(win32())
        return;
    if(windows())
        return;
    dosint28();
#endif
}


//  ------------------------------------------------------------------

int GMTsk::win32()
{

#if defined(__WIN32__)
    detected = GMTSK_W32;
    name = "Win32";
#endif
    return detected;
}


//  ------------------------------------------------------------------

int GMTsk::windows()
{

#if defined(__MSDOS__)
    i86 cpu;
    cpu.ax(0x352F);
    cpu.genint(0x21);
    if(cpu.es() != 0)
    {
        cpu.ax(0x1600);
        cpu.genint(0x2F);
        if(cpu.ax() & 0x007F)
        {
            detected = GMTSK_WINDOWS;
            name = "Windows";
        }
    }
#endif
    return detected;
}



//  ------------------------------------------------------------------

int GMTsk::dosint28()
{

#if defined(__MSDOS__)
    detected = GMTSK_DOS;
    name = "DOS";
#endif

    return detected;
}


//  ------------------------------------------------------------------

void GMTsk::timeslice()
{
#if defined(__MSDOS__)
    i86 cpu;
#endif

    switch(detected)
    {
#if defined(__UNIX__)
    case GMTSK_LINUX:
        usleep(5000);
        break;
#endif
#if defined(__WIN32__)
    case GMTSK_W32:
        Sleep(5);
        break;
    case GMTSK_DOS:
        Sleep(5);
        break;
#endif
#if defined(__MSDOS__)
        // Drop through if this is a DOS version
#if defined(__MSDOS__)
    case GMTSK_WINDOWS:
        cpu.ax(0x1680);
        cpu.genint(0x2F);
        break;
    case GMTSK_DOS:
        cpu.genint(0x28);
        break;
#endif
#endif
    }
}


//  ------------------------------------------------------------------
