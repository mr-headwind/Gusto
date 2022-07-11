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

void video_convert(AppData *, MainUi *);
void video_select(AppData *, MainUi *);
void set_convert_widgets(AppData *, MainUi *);

extern void app_msg(char*, char *, GtkWidget *);


/* Globals */

static const char *debug_hdr = "DEBUG-convert.c ";


/* Set up a gstreamer pipeline and convert frames to images as required */

void video_convert(AppData *user_data, MainUi *m_ui)
{  

    return;
}


/* Browse and select a video file to convert */

void video_select(AppData *user_data, MainUi *m_ui)
{  

    return;
}


/* Set widgets sensitivity and visibility as required */

void set_convert_widgets(AppData *user_data, MainUi *m_ui)
{  
    int idx;

    idx = gtk_combo_box_get_active (GTK_COMBO_BOX (m_ui->frm_select_cbx));

    if (idx == 0)
    {
	gtk_widget_set_sensitive (m_ui->frm_interval_ent, FALSE);
	gtk_entry_set_text(GTK_ENTRY (m_ui->frm_interval_ent), "1");
    }
    else if (idx == 1)
    {
	gtk_widget_set_sensitive (m_ui->frm_interval_ent, TRUE);
    }
    else if (idx == 2)
    {
	gtk_widget_set_sensitive (m_ui->frm_interval_ent, TRUE);
    }

    return;
}
