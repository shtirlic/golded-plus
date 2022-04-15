//  This may look like C code, but it is really -*- C++ -*-

//  ------------------------------------------------------------------
//  The Goldware Library
//  Copyright (C) 1990-1999 Odinn Sorensen
//  Copyright (C) 1999-2000 Alexander S. Aganichev
//  Copyright (C) 2000 Jacobo Tarrio
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
//  ------------------------------------------------------------------
//  GCUI: Golded+ Character-oriented User Interface.
//  Device-independent video functions. Initialise class GVid.
//  ------------------------------------------------------------------

#include <cassert>
#include <cstdio>
#include <cstdlib>
#include <cstdarg>
#include <gmemall.h>
#include <gstrall.h>
#include <gvidall.h>

#if defined(__WIN32__)
    #include <windows.h>
#endif

#if !defined(__USE_NCURSES__) && defined(__UNIX__)
    #include <sys/ioctl.h>
    #include <termios.h>
    #include <unistd.h>
    #include <errno.h>
#endif

//  ------------------------------------------------------------------
//  Global video data

GVid *gvid;

#if defined(__USE_NCURSES__)

    // add statics here

#elif defined(__UNIX__)

    int gvid_stdout = -1;
    const char* gvid_acs_enable;
    const char* gvid_acs_disable;

    void _vputx(int row, int col, int atr, char chr, uint len);
    void gvid_printf(const char* fmt, ...) __attribute__ ((format (printf, 1, 2)));

#elif defined(__WIN32__)

    HANDLE gvid_hout = INVALID_HANDLE_VALUE;

#endif


//  ------------------------------------------------------------------
//  Video Class constructor

GVid::GVid()
{
    dmaptr = dmadir = NULL;
    bufchr = NULL;
    bufwrd = NULL;
    bufansi = NULL;
    init();
}


//  ------------------------------------------------------------------
//  Video Class destructor

GVid::~GVid()
{

#if defined(__USE_NCURSES__)

    attrset(A_NORMAL);
    if(0 == (--curses_initialized))
        endwin();

#elif defined(__UNIX__)

    // "\033<"        Enter ANSI mode
    // "\033[?5l"     Normal screen
    // "\033[0m"      Normal character attributes

    gvid_printf("\033<\033[?5l\033[0m");

#endif
    free(bufwrd);
    free(bufchr);
    free(bufansi);
}


//  ------------------------------------------------------------------

void GVid::init()
{

#if defined(__USE_NCURSES__)
    // Both display and keyboard will be initialized at once
    if(0 == (curses_initialized++))
    {
        initscr();
        raw();
        noecho();
        nonl();
        intrflush(stdscr, FALSE);
        keypad(stdscr, TRUE);
    }
#endif

    // Detect video adapter
    detectadapter();

    // Detect original video information
    detectinfo(&orig);
    std::memcpy(&curr, &orig, sizeof(GVidInfo));

#if defined(__WIN32__)
    device = GVID_W32;
#endif

#if defined(__USE_NCURSES__)
    dmaptr = dmadir = NULL;
#elif defined(__WIN32__)
    dmaptr = dmadir = NULL;
#elif defined(__UNIX__)
    dmaptr = (gdma)calloc((orig.screen.rows+1)*orig.screen.columns, sizeof(word));
    dmadir = NULL;
#else
    dmaptr = dmadir = (gdma)MK_FP(videoseg, 0);
#endif

    bufchr = NULL;
    bufwrd = NULL;
    bufansi = NULL;

    resetcurr();
}

//  ------------------------------------------------------------------
//  Video adapter detect

int GVid::detectadapter()
{

    // Initialize to a valid value
    adapter = GV_NONE;

#if defined(__USE_NCURSES__)

    start_color();

    setcolorpairs();

    adapter = V_VGA;

#elif defined(__WIN32__)

    gvid_hout = CreateFile("CONOUT$", GENERIC_READ | GENERIC_WRITE,
                           FILE_SHARE_WRITE | FILE_SHARE_READ, NULL,
                           OPEN_EXISTING,
                           FILE_FLAG_NO_BUFFERING|FILE_FLAG_WRITE_THROUGH, NULL);

    adapter = V_VGA;

#elif defined(__linux__)

    for(int n=0; n<8; n++)
        __box_table[n] = __box_table[8];

    gvid_acs_enable  = "\016";
    gvid_acs_disable  = "\017";

    gvid_stdout = fileno(stdout);

    adapter = V_VGA;

#elif defined(__UNIX__) // code below should not be normally used...

    bool gvid_xterm = false;
    const char* term = getenv("TERM");

    if(term and strneql(term, "xterm", 5))
    {
        gvid_xterm = true;
        for(int n=0; n<8; n++)
            __box_table[n] = __box_table[8];
    }

    gvid_acs_enable  = gvid_xterm ? "\033)0\033(B\016" : "\033[11m";
    gvid_acs_disable = gvid_xterm ? "\033(B\033)B\017" : "\033[10m";

    gvid_stdout = fileno(stdout);

    adapter = V_VGA;

#endif

    return adapter;
}

void GVid::setcolorpairs(bool enabletransparent)
{
#if defined(__USE_NCURSES__)
    /* init colors */
    short mycolors[] = { COLOR_BLACK, COLOR_BLUE, COLOR_GREEN, COLOR_CYAN,
                         COLOR_RED, COLOR_MAGENTA, COLOR_YELLOW, COLOR_WHITE
                       };

    if(enabletransparent)
        use_default_colors();
    for(int i = 1; i < 64 and i < COLOR_PAIRS; i++)
    {
        short bg=mycolors[(i>>3)&0x07];
        init_pair(i, mycolors[(~i)&0x07], ((bg==COLOR_BLACK) && enabletransparent)?-1:bg);
    }
#endif
}

//  ------------------------------------------------------------------
//  Video info detect

void GVid::detectinfo(GVidInfo* _info)
{
    // Reset all original values
    memset(_info, 0, sizeof(GVidInfo));

#if defined(__USE_NCURSES__)

    _info->screen.mode = 0;
    _info->screen.rows = LINES;
    _info->screen.columns = COLS;
    getyx(stdscr, _info->cursor.row, _info->cursor.column);
    _info->color.textattr = 7;
    _info->cursor.start = 11;
    _info->cursor.end = 12;
    _info->cursor.attr = 7;
    _info->color.intensity = 1;
    _info->color.overscan = 0;
#elif defined(__WIN32__)

    // Get video mode and number of rows and columns
    CONSOLE_SCREEN_BUFFER_INFO csbi;
    int result = GetConsoleScreenBufferInfo(gvid_hout, &csbi);
    assert(result != 0);

    _info->screen.mode = 0;
    _info->screen.rows = csbi.dwSize.Y;
    _info->screen.columns = csbi.dwSize.X;

    // Get cursor position and character attribute under the cursor
    _info->cursor.row = csbi.dwCursorPosition.Y;
    _info->cursor.column = csbi.dwCursorPosition.X;
    _info->color.textattr = csbi.wAttributes;

    if(_info->screen.rows > 100)
    {
        _info->screen.rows = csbi.srWindow.Bottom - csbi.srWindow.Top + 1;
        _info->screen.columns = csbi.srWindow.Right - csbi.srWindow.Left + 1;
        _info->cursor.row = csbi.dwCursorPosition.Y - csbi.srWindow.Top + 1;
        _info->cursor.column = csbi.dwCursorPosition.X - csbi.srWindow.Left + 1;
    }

    // Get cursor form
    CONSOLE_CURSOR_INFO cci;
    GetConsoleCursorInfo(gvid_hout, &cci);

    _info->cursor.start = (int) 1;
    _info->cursor.end = cci.bVisible ? (int) 15 : 0;
    _info->cursor.attr = (word)(cci.bVisible ? 7 : 0);

    // Get intensity state
    _info->color.intensity = 1;

    // Get overscan color
    _info->color.overscan = 0;

#elif defined(__UNIX__)

    int r = 0, c = 0;

    if(r <= 0)
    {
        char *s = getenv("LINES");
        if(s)
        {
            //printf("getenv(\"LINES\") = %s\n", s);
            r = atoi(s);
        }
    }

    if(c <= 0)
    {
        char *s = getenv("COLUMNS");
        if(s)
        {
            //printf("getenv(\"COLUMNS\") = %s\n", s);
            c = atoi(s);
        }
    }

    //printf("init1: c=%i, r=%i\n", c, r);

#if defined(TIOCGSIZE)
    if(r <= 0 or c <= 0)
    {
        struct ttysize sz;

        do
        {
            if((ioctl(1,TIOCGSIZE,&sz) == 0) or (ioctl(0, TIOCGSIZE, &sz) == 0) or (ioctl(2, TIOCGSIZE, &sz) == 0))
            {
                c = (int)sz.ts_cols;
                r = (int)sz.ts_lines;
                break;
            }
        }
        while(errno == EINTR);
    }
    //printf("init2: c=%i, r=%i\n", c, r);
#elif defined(TIOCGWINSZ)
    if(r <= 0 or c <= 0)
    {
        struct winsize wind_struct;

        do
        {
            if((ioctl(1,TIOCGWINSZ,&wind_struct) == 0) or (ioctl(0, TIOCGWINSZ, &wind_struct) == 0) or (ioctl(2, TIOCGWINSZ, &wind_struct) == 0))
            {
                c = (int)wind_struct.ws_col;
                r = (int)wind_struct.ws_row;
                break;
            }
        }
        while(errno == EINTR);
    }
    //printf("init3: c=%i, r=%i\n", c, r);
#endif


    if((r <= 0) or (r > 200))
        r = 24;
    if((c <= 0) or (c > 250))
        c = 80;

    //printf("init4: c=%i, r=%i\n", c, r);

    //*dmadir = 0;

    _info->screen.mode = 0;
    _info->screen.rows = r;
    _info->screen.columns = c;
    _info->cursor.row = 0;
    _info->cursor.column = 0;
    _info->color.textattr = 7;
    _info->cursor.start = 11;
    _info->cursor.end = 12;
    _info->cursor.attr = 7;
    _info->color.intensity = 1;
    _info->color.overscan = 0;

#endif

    getpalette(_info->color.palette);
}


//  ------------------------------------------------------------------
//  Reset current video info

void GVid::resetcurr()
{

    currow = curr.cursor.row;
    curcol = curr.cursor.column;

    numrows = curr.screen.rows;
    numcols = curr.screen.columns;

    free(bufchr);
    free(bufwrd);
    free(bufansi);

    bufchr = (vchar*)calloc(sizeof(vchar), numcols+1);
    bufwrd = (vatch*)calloc(sizeof(vatch), numcols+1);
    bufansi = (vchar*)calloc(sizeof(vchar), (11*numcols)+1);

    setdevice(device);
}


//  ------------------------------------------------------------------
//  Sets video output device

void GVid::setdevice(int _device)
{

    device = _device;
}


//  ------------------------------------------------------------------
//  Sets video mode

void GVid::setmode(int _mode)
{
    detectinfo(&curr);
    resetcurr();
}


//  ------------------------------------------------------------------
//  Sets screen rows

void GVid::setrows(int _rows)
{

    int origrows = curr.screen.rows;

#if defined(__USE_NCURSES__)

    NW(_rows);

#elif defined(__WIN32__) || defined(__UNIX__)

    NW(_rows);

#endif

    if(origrows < _rows)
        vfill(origrows, 0, _rows, 80, ' ', LGREY_|_BLACK);

    detectinfo(&curr);
    resetcurr();
}


//  ------------------------------------------------------------------
//  Set the screen border (overscan) color

void GVid::setoverscan(vattr _overscan)
{
#if defined(__USE_NCURSES__)

    NW (_overscan);


#elif defined(__WIN32__) || defined(__UNIX__)

    NW(_overscan);

#endif
}


//  ------------------------------------------------------------------
//  Set intensity/blinking state

void GVid::setintensity(int _intensity)
{
#if defined(__USE_NCURSES__)

    NW(_intensity);

#elif defined(__WIN32__) || defined(__UNIX__)

    NW(_intensity);

#endif
}


//  ------------------------------------------------------------------

void GVid::getpalette(int* _palette)
{
#if defined(__USE_NCURSES__)

    NW(_palette);

#elif defined(__WIN32__) || defined(__UNIX__)

    NW(_palette);

#endif
}


//  ------------------------------------------------------------------

void GVid::setpalette(int* _palette)
{
#if defined(__USE_NCURSES__)

    NW(_palette);

#elif defined(__WIN32__) || defined(__UNIX__)

    NW(_palette);

#endif
}


//  ------------------------------------------------------------------

void GVid::restore_cursor()
{

    vcurset(orig.cursor.start, orig.cursor.end);
    vcurshow();
    vposset(orig.cursor.row-1, 0);
}


//  ------------------------------------------------------------------

void GVid::resize_screen(int columns, int rows)
{

    numcols = curr.screen.columns = columns;
    numrows = curr.screen.rows    = rows;

    bufchr = (vchar*)realloc(bufchr, numcols+1);
    bufwrd = (vatch*)realloc(bufwrd, (numcols+1)*sizeof(vatch));
    bufansi = (vchar*)realloc(bufansi, 1+(11*numcols));

#if defined(__UNIX__) && !defined(__USE_NCURSES__)
    dmaptr = (gdma)realloc(dmaptr, (rows+1)*columns*sizeof(word));
#endif
}


//  ------------------------------------------------------------------
