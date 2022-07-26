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
** Description: Set up a gstreamer pipeline and convert frames to images as required
**
** Author:	Anthony Buckley
**
** History
**	24-Jun-2022	Initial code
**
*/



/* Defines */


/* Includes */
#include <stdlib.h>  
#include <string.h>  
#include <libgen.h>  
#include <gtk/gtk.h>  
#include <gdk/gdkkeysyms.h>  
#include <main.h>
#include <user_data.h>
#include <defs.h>


/* Prototypes */

void video_select(AppData *, MainUi *);
void output_dir_select(AppData *, MainUi *);
void set_convert_widgets(AppData *, MainUi *);
void video_convert(AppData *, MainUi *);
void get_user_data(AppData *, MainUi *);

extern void app_msg(char*, char *, GtkWidget *);
int choose_file_dialog(char *, int , gchar **, MainUi *);


/* Globals */

static const char *debug_hdr = "DEBUG-convert.c ";


/* Browse and select a video file to convert */

void video_select(AppData *app_data, MainUi *m_ui)
{  
    gint res;

    res = choose_file_dialog("Select Video", GTK_FILE_CHOOSER_ACTION_OPEN, &(app_data->video_fn), m_ui);

    if (res == GTK_RESPONSE_APPLY)
	gtk_entry_set_text (GTK_ENTRY (m_ui->fn), app_data->video_fn);

    return;
}


/* Browse and select a directory to hold output images */

void output_dir_select(AppData *app_data, MainUi *m_ui)
{  
    gint res;

    res = choose_file_dialog("Output Location", GTK_FILE_CHOOSER_ACTION_CREATE_FOLDER, &(app_data->output_dir), m_ui);

    if (res == GTK_RESPONSE_APPLY)
	gtk_entry_set_text (GTK_ENTRY (m_ui->out_dir), app_data->output_dir);

    return;
}


/* Set widgets sensitivity and visibility as required */

void set_convert_widgets(AppData *user_data, MainUi *m_ui)
{  
    int idx;

    idx = gtk_combo_box_get_active (GTK_COMBO_BOX (m_ui->frm_select_cbx));

    if (idx == 0)
    {
	gtk_entry_set_text(GTK_ENTRY (m_ui->frm_interval), "1");
	gtk_widget_set_visible (m_ui->int_hbox, FALSE);
	gtk_widget_set_visible (m_ui->time_hbox, FALSE);
    }
    else if (idx == 1)
    {
	gtk_widget_set_visible (m_ui->int_hbox, TRUE);
	gtk_widget_set_visible (m_ui->time_hbox, FALSE);
    }
    else if (idx == 2)
    {
	gtk_widget_set_visible (m_ui->time_hbox, TRUE);
	gtk_widget_set_visible (m_ui->int_hbox, FALSE);
    }

    return;
}


/* Collect necessary user data and set up a gstreamer pipeline to convert frames to images as required */

void video_convert(AppData *app_data, MainUi *m_ui)
{  
    /* Collect user data */
    get_user_data(app_data, m_ui);

    /* Conversion pipeline */

    return;
}


/* Collect necessary user data */

void get_user_data(AppData *app_data, MainUi *m_ui)
{  
    gchar *s;

printf("%s get_user_data 1\n", debug_hdr); fflush(stdout);
    if (app_data->video_fn[0] == '\0')
    	app_data->video_fn = (char *) gtk_entry_get_text(GTK_ENTRY (m_ui->fn));

    if (app_data->output_dir[0] == '\0')
    	app_data->output_dir = (char *) gtk_entry_get_text(GTK_ENTRY (m_ui->out_dir));

printf("%s get_user_data 2\n", debug_hdr); fflush(stdout);
    app_data->image_type = gtk_combo_box_text_get_active_text (GTK_COMBO_BOX_TEXT (m_ui->codec_select_cbx));

    app_data->interval_type = gtk_combo_box_get_active (GTK_COMBO_BOX(m_ui->frm_select_cbx));

printf("%s get_user_data 3\n", debug_hdr); fflush(stdout);
    switch(app_data->interval_type)
    {
    	case 0:				// Convert every frame
	    break;
	case 1:				// Convert a selection of frames
	    s = (char *) gtk_entry_get_text(GTK_ENTRY (m_ui->frm_interval));
	    app_data->frame_interval = atoi(s);
	    break;
	case 2:				// Convert frames for time period
	    s = (char *) gtk_entry_get_text(GTK_ENTRY (m_ui->video_start));
	    app_data->time_start = atoi(s);
	    s = (char *) gtk_entry_get_text(GTK_ENTRY (m_ui->duration));
	    app_data->time_duration = atoi(s);
	    break;
	default:
	    app_msg("MSG0004", "Error: Selection type", m_ui->window);
	    return;
	    break;
    }

printf("%s get_user_data 4\n", debug_hdr); fflush(stdout);
    return;
}
