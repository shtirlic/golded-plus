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
//  Sound class.
//  ------------------------------------------------------------------

#include <cstdio>
#include <cstdlib>
#include <gutlos.h>


#include <gstrall.h>
#include <gasmamis.h>
#include <gsndall.h>
#include <gfilutil.h>

//  ------------------------------------------------------------------

#ifdef DEBUG
    int debug = false;
#endif


//  ------------------------------------------------------------------

gsnd::gsnd()
{
    api_open = false;
    file_open = false;
}


//  ------------------------------------------------------------------

gsnd::~gsnd()
{

    if(api_open)
        close_api();

    free_buffer();
}


//  ------------------------------------------------------------------

#

//  ------------------------------------------------------------------

void gsnd::free_buffer()
{

}


//  ------------------------------------------------------------------

int gsnd::open_api()
{
#if defined(GUTLOS_FUNCS)

    api_open = true;
    return 0;
#else

    api_open = false;
    return -1;

#endif
}


//  ------------------------------------------------------------------

int gsnd::close_api()
{

#if defined(GUTLOS_FUNCS)

    if(api_open)
    {

        if(file_open)
            close();

        api_open = false;
    }

    return 0;

#else

    return -1;

#endif
}


//  ------------------------------------------------------------------

int gsnd::open(const char* file)
{
#if defined(GUTLOS_FUNCS)

    if(file_open)
        close();

    char buf[GMAXPATH+sizeof("open  alias noise wait")];
    sprintf(buf, "open %s alias noise wait", file);
    if(g_send_mci_string(buf, NULL))
    {
        file_open = true;
        return 0;
    }
    return -1;

#else

    NW(file);
    return -1;

#endif
}


//  ------------------------------------------------------------------

int gsnd::close()
{

#if defined(GUTLOS_FUNCS)

    if(file_open)
    {
        g_send_mci_string("close noise wait", NULL);
        file_open = false;
        return 0;
    }
    return 0xFFFF;

#else

    return 0xFFFF;

#endif
}


//  ------------------------------------------------------------------

int gsnd::play(uint sample_rate)
{
#if defined(GUTLOS_FUNCS)

    NW(sample_rate);

    if(file_open)
    {
        g_send_mci_string("seek noise to start", NULL);
        g_send_mci_string("play noise", NULL);
        return 0;
    }
    return 1;

#else

    NW(sample_rate);
    return 1;

#endif
}


//  ------------------------------------------------------------------

int gsnd::stop()
{
#if defined(GUTLOS_FUNCS)

    if(file_open)
    {
        g_send_mci_string("stop noise wait", NULL);
        return 0;
    }
    return 1;

#else

    return 1;

#endif
}


//  ------------------------------------------------------------------

int gsnd::pause()
{
#if defined(GUTLOS_FUNCS)

    if(file_open)
    {
        g_send_mci_string("pause noise", NULL);
        return 0;
    }
    return 1;

#else

    return 1;

#endif
}


//  ------------------------------------------------------------------

int gsnd::resume()
{
#if defined(GUTLOS_FUNCS)

    if(file_open)
    {
        g_send_mci_string("resume noise", NULL);
        return 0;
    }
    return 1;

#else

    return 1;

#endif
}


//  ------------------------------------------------------------------

int gsnd::break_loop(int method)
{
#if defined(GUTLOS_FUNCS)

    NW(method);
    return 0xFFFF;

#else

    NW(method);
    return 0xFFFF;

#endif
}



//  ------------------------------------------------------------------

void gsnd::speaker(int onoff)
{
#if defined(GUTLOS_FUNCS)

    // Not implemented
    NW(onoff);

#else

    NW(onoff);

#endif
}


//  ------------------------------------------------------------------

uint gsnd::get_sample_rate()
{
#if defined(GUTLOS_FUNCS)

    return 0;

#else

    return 0;

#endif
}


//  ------------------------------------------------------------------

int gsnd::is_playing()
{
#if defined(GUTLOS_FUNCS)

    char return_buffer[BUFFERSIZE];

    if (file_open)
    {
        if(g_send_mci_string("status noise mode wait", return_buffer))
            return strieql(return_buffer, "playing") or strieql(return_buffer, "seeking");
    }

    return 0;

#else

    return 0;

#endif
}


//  ------------------------------------------------------------------

