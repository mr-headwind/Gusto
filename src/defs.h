/*
**  Copyright (C) 2016 Anthony Buckley
** 
**  This file is part of Gusto.
** 
**  Gusto is free software: you can redistribute it and/or modify
**  it under the terms of the GNU General Public License as published by
**  the Free Software Foundation, either version 3 of the License, or
**  (at your option) any later version.
**  
**  Gusto is distributed in the hope that it will be useful,
**  but WITHOUT ANY WARRANTY; without even the implied warranty of
**  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
**  GNU General Public License for more details.
**  
**  You should have received a copy of the GNU General Public License
**  along with Gusto.  If not, see <http://www.gnu.org/licenses/>.
*/



/*
** Description:	Local standard constant defines
**
** Author:	Anthony Buckley
**
** History
**	21-Jun-2022	Initial
**
*/


/* Includes */


/* General */
#ifndef TRUE
#define TRUE 1
#endif

#ifndef FALSE
#define FALSE 0
#endif

#ifndef _FILE_OFFSET_BITS
#define _FILE_OFFSET_BITS 64
#endif

#if defined (GDK_WINDOWING_X11)
#include <gdk/gdkx.h>
#elif defined (GDK_WINDOWING_WIN32)
#include <gdk/gdkwin32.h>
#elif defined (GDK_WINDOWING_QUARTZ)
#include <gdk/gdkquartz.h>
#endif

#ifndef AC_COLOURS
#define AC_COLOURS
#ifdef MAIN_UI
const GdkRGBA BLUE_GRAY = {.9, .9, 1.0, 1};
const GdkRGBA LIGHT_BLUE = {.5, .5, 1.0, 1.0};
const GdkRGBA MID_BLUE = {.5, .5, 1.0, 1.0};
const GdkRGBA DARK_BLUE = {0, 0, 0.5, 1.0};
const GdkRGBA RED1 = {.8, .1, .1, 1.0};
const GdkRGBA NIGHT = {0.95, 0.05, 0.05, 0.3};
//const GdkRGBA NIGHT = {0.57, 0.24, 0.24, 0.5};
#else
extern const GdkRGBA BLUE_GRAY;
extern const GdkRGBA LIGHT_BLUE;
extern const GdkRGBA MID_BLUE;
extern const GdkRGBA DARK_BLUE;
extern const GdkRGBA RED1;
extern const GdkRGBA NIGHT;
#endif
#endif


/* Application Name et al */
#ifndef TITLE
#define TITLE "Gusto"
#define PACKAGE_DATA_DIR "/usr/share"			// Release only
//#define PACKAGE_DATA_DIR "/home/tony/.local/share"	// Dev only
#define APP_URI "http://Gusto.sourceforge.net"
#endif


/* Camera */
#ifndef USR_HDR
#include <user_data.h>
#endif


/* Utility globals */
#ifndef ERR_INCLUDED
#define ERR_INCLUDED
#ifdef ERR_FILE
char app_msg_extra[1000];
#else
extern char app_msg_extra[1000];
#endif
#endif
