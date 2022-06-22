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
** Description:	Module for (Main) Callback functions
**
** Author:	Anthony Buckley
**
** History
**	22-Jun-2022	Initial code
*/


/* Includes */

#include <stdlib.h>  
#include <string.h>  
#include <libgen.h>  
#include <gtk/gtk.h>  
#include <main.h>
#include <defs.h>  


/* Defines */

#define round(x) ((x)>=0?(long)((x)+0.5):(long)((x)-0.5))


/* Prototypes */

void OnConvert(GtkWidget*, gpointer *user_data);
void OnQuit(GtkWidget*, gpointer *user_data);


extern void video_convert(MainUi *);
extern void free_window_reg();
extern void close_open_ui();
extern int is_ui_reg(char *, int);
extern void app_msg(char*, char*, GtkWidget*);
extern GtkWidget* view_file_main(char  *);
extern int close_ui(char *);
extern gint query_dialog(GtkWidget *, char *, char *);


/* Globals */

static const char *debug_hdr = "DEBUG-callbacks.c ";


/* Callbacks */


/* Callback - Convert the video to images */

void OnConvert(GtkWidget *btn, gpointer *user_data)
{  
    MainUi *m_ui;

    /* Check if already open */
    if (is_ui_reg(MAIN_UI, TRUE))
    	return;

    /* Get data */
    m_ui = (MainUi *) user_data;

    /* Open */
    video_convert(m_ui->window);

    return;
}  


/* Callback - Quit */

void OnQuit(GtkWidget *window, gpointer *user_data)
{  
    /* Close any open windows */
    close_open_ui();
    free_window_reg();

    /* Main quit */
    gtk_main_quit();

    return;
}  


/* CALLBACK other functions */

