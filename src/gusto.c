/*
**  Copyright (C) 2022 Anthony Buckley
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
** Application:	Gusto
**
** Author:	Anthony Buckley
**
** Description:
**  	 Application control for Gusto. Play video and capture individual frames.
**
** History
**	31-May-2022	Initial code
**
*/


/* Includes */

#include <stdlib.h>  
#include <string.h>  
#include <libgen.h>  
#include <gtk/gtk.h>  
#include <gst/gst.h>
#include <main.h>
#include <user_data.h>
#include <defs.h>


/* Defines */


/* Prototypes */

void initialise(AppData *, MainUi *);
void final();

extern void main_ui(AppData *, MainUi *);
extern void app_msg(char*, char *, GtkWidget *);
//extern void debug_session();


/* Globals */

static const char *debug_hdr = "DEBUG-Gusto.c ";
char d_separator;

/* Main program control */

int main(int argc, char *argv[])
{  
    AppData app_data;
    MainUi m_ui;

    /* Initial work */
    initialise(&app_data, &m_ui);

    /* Initialise Gtk */
    gtk_init(&argc, &argv);  
    gst_init (&argc, &argv);

    main_ui(&app_data, &m_ui);

    gtk_main();  

    final();

    exit(0);
}  


/* Initial work */

void initialise(AppData *app_data, MainUi *m_ui)
{
    /* Set variables */
    app_msg_extra[0] = '\0';
    memset(app_data, 0, sizeof (AppData));
    memset(m_ui, 0, sizeof (MainUi));
    app_data->video_fn = (char *) malloc(2);
    app_data->video_fn[0] = '\0';
    app_data->video_fn_last = (char *) malloc(2);
    app_data->video_fn_last[0] = '\0';

    app_msg("MSG9000", '\0', NULL);

    /* Set the directory separator */
    #ifdef __linux__
	d_separator = '/';
    #else
	d_separator = '\\';
    #endif

    return;
}


/* Final work */

void final()
{
    return;
}
