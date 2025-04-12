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

void OnVideo(GtkWidget*, gpointer);
void OnConvert(GtkWidget*, gpointer);
void OnVideoBrowse(GtkWidget*, gpointer);
void OnDirBrowse(GtkWidget*, gpointer);
void OnFrameSet(GtkWidget *, gpointer);
void OnReset(GtkWidget*, gpointer);
void OnQuit(GtkWidget*, gpointer);


extern void video_select(MainUi *);
extern void output_dir_select(AppData *, MainUi *);
extern int check_make_dir(char *, GtkWidget *);
extern void set_convert_widgets(AppData *, MainUi *);
extern void video_info(AppData *, MainUi *);
extern void video_convert(AppData *, MainUi *);
extern int get_video_data(AppData *, MainUi *);
extern void reset_form(AppData *, MainUi *);
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


/* Callback - Retrieve the video details */

void OnVideo(GtkWidget *btn, gpointer user_data)
{  
    MainUi *m_ui;
    AppData *app_data;

    /* Get data */
    m_ui = (MainUi *) user_data;
    app_data = (AppData *) g_object_get_data (G_OBJECT (m_ui->window), "app_data");

    /* Video Information */
    video_info(app_data, m_ui);

    return;
}  


/* Callback - Convert the video to images */

void OnConvert(GtkWidget *btn, gpointer user_data)
{  
    MainUi *m_ui;
    AppData *app_data;

    /* Get data */
    m_ui = (MainUi *) user_data;
    app_data = (AppData *) g_object_get_data (G_OBJECT (m_ui->window), "app_data");

    /* Conversion */
    video_convert(app_data, m_ui);

    return;
}  


/* Callback - Video browse and selection */

void OnVideoBrowse(GtkWidget *btn, gpointer user_data)
{  
    MainUi *m_ui;

    /* Get data */
    m_ui = (MainUi *) user_data;

    /* Select video */
    video_select(m_ui);

    return;
}  


/* Callback - Output directory selection */

void OnDirBrowse(GtkWidget *btn, gpointer user_data)
{  
    MainUi *m_ui;
    AppData *app_data;

    /* Get data */
    m_ui = (MainUi *) user_data;
    app_data = (AppData *) g_object_get_data (G_OBJECT (m_ui->window), "app_data");

    /* Output directory */
    output_dir_select(app_data, m_ui);

    return;
}  


/* Callback - Select the type of frame conversion */

void OnFrameSet(GtkWidget *cbx, gpointer user_data)
{  
    MainUi *m_ui;
    AppData *app_data;

    /* Get data */
    m_ui = (MainUi *) user_data;
    app_data = (AppData *) g_object_get_data (G_OBJECT (m_ui->window), "app_data");

    /* Set widgets sensitivity and visibility as required */
    set_convert_widgets(app_data, m_ui);

    return;
}  


/* Callback - Reset */

void OnReset(GtkWidget *window, gpointer user_data)
{  
    MainUi *m_ui;
    AppData *app_data;

    /* Get data */
    m_ui = (MainUi *) user_data;
    app_data = (AppData *) g_object_get_data (G_OBJECT (m_ui->window), "app_data");

    /* Reset all fields to default */
    reset_form(app_data, m_ui);

    return;
}  


/* Callback - Quit */

void OnQuit(GtkWidget *window, gpointer user_data)
{  
    MainUi *m_ui;
    AppData *app_data;

    /* Get data */
    app_data = (AppData *) g_object_get_data (G_OBJECT (window), "app_data");
    m_ui = (MainUi *) g_object_get_data (G_OBJECT (window), "ui");

    /* Close any open windows */
    close_open_ui();
    free_window_reg();

    /* Free file names */
    free(app_data->video_fn);
    free(app_data->video_fn_last);

    if (m_ui->convbtn_bg_color != NULL)
	gdk_rgba_free(m_ui->convbtn_bg_color);

    /* Main quit */
    gtk_main_quit();

    return;
}  


/* CALLBACK other functions */

