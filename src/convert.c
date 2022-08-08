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
#include <gst/gst.h>
#include <gst/video/video-format.h>
#include <main.h>
#include <user_data.h>
#include <defs.h>


/* Prototypes */

void video_select(AppData *, MainUi *);
void output_dir_select(AppData *, MainUi *);
void set_convert_widgets(AppData *, MainUi *);
void video_convert(AppData *, MainUi *);
void get_user_data(AppData *, MainUi *);
int setup_gst_pipeline(AppData *, MainUi *);

extern void app_msg(char*, char *, GtkWidget *);
int choose_file_dialog(char *, int , gchar **, MainUi *);


/* Globals */

static const char *debug_hdr = "DEBUG-convert.c ";


/* Browse and select a video file to convert */

void video_select(AppData *app_data, MainUi *m_ui)
{  
    gint res;
    char *p;

    res = choose_file_dialog("Select Video", GTK_FILE_CHOOSER_ACTION_OPEN, &p, m_ui);

    if (res == GTK_RESPONSE_APPLY)
    {
	gtk_entry_set_text (GTK_ENTRY (m_ui->fn), p);
	free(p);
    }

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
    setup_gst_pipeline(app_data, m_ui);

    return;
}


/* Collect necessary user data */

void get_user_data(AppData *app_data, MainUi *m_ui)
{  
    gchar *s;

    app_data->video_fn = (char *) gtk_entry_get_text(GTK_ENTRY (m_ui->fn));

    if (*(app_data->video_fn) == '\0')
	app_msg("MSG0002", "Video file", m_ui->window);

    app_data->output_dir = (char *) gtk_entry_get_text(GTK_ENTRY (m_ui->out_dir));

    if (*(app_data->output_dir) == '\0')
	app_msg("MSG0002", "Output Location", m_ui->window);

    app_data->img_prefix = (char *) gtk_entry_get_text(GTK_ENTRY (m_ui->img_prefix));

    if (*(app_data->img_prefix) == '\0')
	app_msg("MSG0002", "Image Prefix", m_ui->window);

    app_data->image_type = gtk_combo_box_text_get_active_text (GTK_COMBO_BOX_TEXT (m_ui->codec_select_cbx));
    app_data->interval_type = gtk_combo_box_get_active (GTK_COMBO_BOX(m_ui->frm_select_cbx));

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

    return;
}


/* 
    Setup the gst pipeline and start conversion

    ** Conversion pipeline **

                                           /-> | Queue | Fakesink 
    | Filesrc | -> | Decodebin |-> | Tee |/
    |         |    |           |         |\
                                           \-> | Queue | | Image Encoder | Multifilesink location=xx%05d.(jpg, png, bmp)

*/

int setup_gst_pipeline(AppData *app_data, MainUi *m_ui)
{  
    /* GST setup */
    if (!set_elements(app_data, m_ui))
	return FALSE;

    /* Link all the viewing elements */
    if (link_pipeline(app_data, m_ui) == FALSE)
	return FALSE;

    /* Start conversion */
    if (start_pipeline(app_data, m_ui, TRUE) == FALSE)
	return FALSE;

    return TRUE;
}


/* Create pipeline and conversion elements */

int set_elements(AppData *app_data, MainUi *m_ui)
{
    memset(&(cam_data->gst_objs), 0, sizeof(app_gst_objects));

    if (! create_element(&(app_data->gst_objs.file_src), "filesrc", "v4l2", app_data, m_ui))
    	return FALSE;

    if (! create_element(&(app_data->gst_objs.v_convert), "videoconvert", "v_convert", app_data, m_ui))
    	return FALSE;

    //if (! create_element(&(cam_data->gst_objs.v_sink), "xvimagesink", "v_sink", cam_data, m_ui))
    if (! create_element(&(app_data->gst_objs.v_sink), "fakesink", "v_sink", app_data, m_ui))
    	return FALSE;

    if (app_data->gst_objs.v_sink == NULL)
	return FALSE;

    if (! create_element(&(app_data->gst_objs.q1), "queue", "block", app_data, m_ui))
    	return FALSE;

    /* Create the pipeline */
    app_data->pipeline = gst_pipeline_new ("video_convert");

    if (!app_data->pipeline)
    {
	app_msg("MSG0009", NULL, m_ui->window);
        return FALSE;
    }

    /* Set the device source, caps filter and other object properties */
    g_object_set (cam_data->gst_objs.v4l2_src, "device", cam_data->current_dev, NULL);
    g_object_set (cam_data->gst_objs.v_sink, "sync", FALSE, NULL);
    g_object_set (cam_data->gst_objs.v_filter, "caps", cam_data->gst_objs.v_caps, NULL);

    cam_data->gst_objs.blockpad = gst_element_get_static_pad (cam_data->gst_objs.q1, "src");

    /* Build the pipeline - add all the elements */
    gst_bin_add_many (GST_BIN (app_data->pipeline), 
    				app_data->gst_objs.v4l2_src, 
    				app_data->gst_objs.vid_rate, 
    				app_data->gst_objs.v_sink, 
    				app_data->gst_objs.v_convert, 
    				app_data->gst_objs.v_filter, 
    				app_data->gst_objs.q1, 
    				NULL);

    return TRUE;
}


/* Check the ref count of the element and set up if required */

int create_element(GstElement **element, char *factory_nm, char *nm, CamData *app_data, MainUi *m_ui)
{
    int rc;

    if (GST_IS_ELEMENT(*element) == TRUE)
    {
	if ((rc = GST_OBJECT_REFCOUNT_VALUE (*element)) > 0)
	{
	    //printf("%s Element %s (%s) has a ref count of %d\n", debug_hdr, nm, factory_nm, rc);   // debug
	    return TRUE;
	}
    }

    *element = gst_element_factory_make ((const gchar *) factory_nm, (const gchar *) nm);

    if (! *(element))
    {
	log_msg("CAM0020", NULL, "CAM0020", m_ui->window);
        return FALSE;
    }

    return TRUE;
}
