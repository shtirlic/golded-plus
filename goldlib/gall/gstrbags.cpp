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
//  String bag class.
//  ------------------------------------------------------------------

#include <gmemdbg.h>
#include <gstrall.h>
#include <gstrbags.h>


//  ------------------------------------------------------------------

const int BLOCKSIZE = 4096;


//  ------------------------------------------------------------------

GStrBag::GStrBag()
{

    items = 0;
    currno = 0;
}


//  ------------------------------------------------------------------

GStrBag::~GStrBag()
{

    Reset();
}


//  ------------------------------------------------------------------

void GStrBag::Reset()
{
    bag.clear();
}


//  ------------------------------------------------------------------

int GStrBag::Add(const char* string)
{
    std::string tmpStr(string);
    bag.push_back(tmpStr);
    return items++;
}


//  ------------------------------------------------------------------

int GStrBag::Add(const void* data, int length)
{
    std::string tmpStr((char*)data, length);
    bag.push_back(tmpStr);
    return items++;
}


//  ------------------------------------------------------------------

void GStrBag::Change(int index, const char* string)
{
    Change(index, string, strlen(string)+1);
}


//  ------------------------------------------------------------------

void GStrBag::Change(int index, const void* data, int length)
{
    std::string tmpStr((char*)data, length);
    bag[index] = tmpStr;    
}


//  ------------------------------------------------------------------

const char* GStrBag::operator[](int index)
{

    return Index(index);
}


//  ------------------------------------------------------------------

void GStrSet3::Put(const char* s1, const char* s2, const char* s3)
{

    int len1 = strlen(s1 ? s1 : "")+1;
    int len2 = strlen(s2 ? s2 : "")+1;
    int len3 = strlen(s3 ? s3 : "")+1;
    cfg.size = len1 + len2 + len3;
    set = (char *)throw_xmalloc(cfg.size);
    cfg.pos2 = len1;
    cfg.pos3 = len1 + len2;
    std::memcpy(set, s1 ? s1 : "", len1);
    std::memcpy(set+cfg.pos2, s2 ? s2 : "", len2);
    std::memcpy(set+cfg.pos3, s3 ? s3 : "", len3);
}


//  ------------------------------------------------------------------

void GStrSet3::Change(const char* s1, const char* s2, const char* s3)
{

    char* oldset = set;
    Put(s1, s2, s3);
    throw_xfree(oldset);
}


//  ------------------------------------------------------------------
