/*  ------------------------------------------------------------------
 *  The Goldware Library
 *  Copyright (C) 1999 Alexander S. Aganichev
 *  ------------------------------------------------------------------
 *  This library is free software; you can redistribute it and/or
 *  modify it under the terms of the GNU Library General Public
 *  License as published by the Free Software Foundation; either
 *  version 2 of the License, or (at your option) any later version.
 *
 *  This library is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 *  Library General Public License for more details.
 *
 *  You should have received a copy of the GNU Library General Public
 *  License along with this program; if not, write to the Free
 *  Software Foundation, Inc., 59 Temple Place, Suite 330, Boston,
 *  MA 02111-1307, USA
 *  ------------------------------------------------------------------
 *  $Id$
 *  ------------------------------------------------------------------
 *  Portable NLS functions for ctype.
 *  --------------------------------------------------------------- */

#ifndef __gctype_h
#define __gctype_h


/*  --------------------------------------------------------------- */

#include <gcmpall.h>


/*  --------------------------------------------------------------- */

#include <ctype.h>
#if defined(__WIN32__)
#ifdef __cplusplus
extern "C" {
extern char tl[256], tu[256];
__inline__ int g_tolower(int c)
{
    return tl[c];
}
__inline__ int g_toupper(int c)
{
    return tu[c];
}
}
#else
extern char tl[256], tu[256];
#define g_tolower(c) tl[(int)(uint8_t)(c)]
#define g_toupper(c) tu[(int)(uint8_t)(c)]
#endif
#else
#define g_tolower(c)  tolower(c)
#define g_toupper(c)  toupper(c)
#endif


/*  --------------------------------------------------------------- */

/* NLS chars detected by converting to lower or upper case and in case
 * they don't match they treated as characters
 */

#ifdef __cplusplus
extern "C" {
#endif

#if defined(__WIN32__)
int g_islower(int c);
int g_isupper(int c);
int g_isalpha(int c);
#else
#define g_islower(c) islower(c)
#define g_isupper(c) isupper(c)
#define g_isalpha(c) isalpha(c)
#endif

int isxalnum(int c);

#ifdef __cplusplus
}
#endif

/*  --------------------------------------------------------------- */

#endif

/*  --------------------------------------------------------------- */
