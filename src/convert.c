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
#include <glib.h>
#include <main.h>
#include <user_data.h>
#include <defs.h>


/* Prototypes */

void video_select(AppData *, MainUi *);
void output_dir_select(AppData *, MainUi *);
void set_convert_widgets(AppData *, MainUi *);
int video_convert(AppData *, MainUi *);
void get_user_data(AppData *, MainUi *);
int setup_gst_pipeline(AppData *, MainUi *);
int set_elements(AppData *, MainUi *);
int link_pipeline(AppData *, MainUi *);
int start_pipeline(AppData *, MainUi *, int);
int set_pipeline_state(AppData *, GstState, GtkWidget *);
int create_element(GstElement **, char *, const char *, AppData *, MainUi *);
GstBusSyncReply bus_sync_handler (GstBus*, GstMessage*, gpointer);
gboolean bus_message_watch (GstBus *, GstMessage *, gpointer);

extern void app_msg(char*, char *, GtkWidget *);
extern int choose_file_dialog(char *, int , gchar **, MainUi *);


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

int video_convert(AppData *app_data, MainUi *m_ui)
{  
    /* Collect user data */
    get_user_data(app_data, m_ui);

    /* Conversion pipeline */
    if (setup_gst_pipeline(app_data, m_ui) == FALSE)
    	return FALSE;

    /* Link all the elements */
    if (link_pipeline(app_data, m_ui) == FALSE)
	return FALSE;

    /* Start pipeline */
    if (start_pipeline(app_data, m_ui, TRUE) == FALSE)
	return FALSE;

    return TRUE;
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
    const char *codec_selection_arr[] = { "JPG", "PNG", "BMP" };
    const char *encoder_arr[] = { "jpegenc", "pngenc", "pnmenc" };
    const int codec_max = 3;
    int i;

    /* Initial */
    memset(&(app_data->gst_objs), 0, sizeof(app_gst_objs));

    /* Determine which image encoder factory to use (bmp will be special later conversion) */
    i = 0;

    for(i = 0; i < codec_max; i++)
    {
    	if (strcmp(app_data->image_type, codec_selection_arr[i]) == 0)
	    break;
    }

    if (i >= codec_max)
    {
	app_msg("MSG0001", "Image Type", m_ui->window);
    	return FALSE;
    }

    /* Create factories */
    if (! create_element(&(app_data->gst_objs.file_src), "filesrc", "video", app_data, m_ui))
    	return FALSE;

    if (! create_element(&(app_data->gst_objs.v_decode), "decodebin", "v_decode", app_data, m_ui))
    	return FALSE;

    if (! create_element(&(app_data->gst_objs.tee), "tee", "split", NULL, m_ui))
    	return FALSE;

    if (! create_element(&(app_data->gst_objs.video_queue), "queue", "v_queue", app_data, m_ui))
    	return FALSE;

    if (! create_element(&(app_data->gst_objs.convert_queue), "queue", "c_queue", app_data, m_ui))
    	return FALSE;

    if (! create_element(&(app_data->gst_objs.fk_sink), "fakesink", "fk_sink", app_data, m_ui))
    	return FALSE;

    if (! create_element(&(app_data->gst_objs.encoder), "encoder", encoder_arr[i], app_data, m_ui))
    	return FALSE;

    if (! create_element(&(app_data->gst_objs.mf_sink), "multifilesink", "file_sink", NULL, m_ui))
    	return FALSE;

    /* Create the pipeline */
    app_data->c_pipeline = gst_pipeline_new ("video_convert");

    if (!app_data->c_pipeline)
    {
	app_msg("MSG0009", NULL, m_ui->window);
        return FALSE;
    }

    /* Build the pipeline - add all the elements */
    gst_bin_add_many (GST_BIN (app_data->c_pipeline), 
    				app_data->gst_objs.file_src, 
    				app_data->gst_objs.v_decode, 
    				app_data->gst_objs.tee, 
    				app_data->gst_objs.video_queue, 
    				app_data->gst_objs.convert_queue, 
    				app_data->gst_objs.fk_sink, 
    				app_data->gst_objs.encoder, 
    				app_data->gst_objs.mf_sink, 
    				NULL);

    return TRUE;
}


/* Build (link) all the pipeline elements */

int link_pipeline(AppData *app_data, MainUi *m_ui)
{
    app_gst_objs *gst_objs;

    /* Convenience pointer */
    gst_objs = &(app_data->gst_objs);

    /* Link */
    if (gst_element_link_many (gst_objs->file_src, 
			       gst_objs->v_decode, 
			       gst_objs->tee,
			       gst_objs->video_queue,
			       gst_objs->convert_queue,
			       gst_objs->fk_sink,
			       gst_objs->encoder,
			       gst_objs->mf_sink,
			       NULL) != TRUE)
    {
	app_msg("MSG9010", NULL, m_ui->window);
	return FALSE;
    }

    return TRUE;
}


/* Pipeline watch and start */

int start_pipeline(AppData *app_data, MainUi *m_ui, int init)
{
    GstBus *bus;
    guint source_id;
    char s[100];

    if (init == TRUE)
    {
	/* Set up sync handler for setting the xid once the pipeline is started */
	bus = gst_pipeline_get_bus (GST_PIPELINE (app_data->c_pipeline));
	gst_bus_set_sync_handler (bus, (GstBusSyncHandler) bus_sync_handler, NULL, NULL);
    }

    if (set_pipeline_state(app_data, GST_STATE_PLAYING, m_ui->window) == FALSE)
        return FALSE;

    if (init == TRUE)
    {
	/* Add a bus watch for messages */
	source_id = gst_bus_add_watch (bus, (GstBusFunc) bus_message_watch, m_ui);
	gst_object_unref (bus);
    }

    /* Information status line */
    sprintf(s, "Converting video to %s images ...", app_data->image_type);
    gtk_label_set_text (GTK_LABEL (m_ui->status_info), s);

    return TRUE;
}


/* Set pileline state */

int set_pipeline_state(AppData *app_data, GstState state, GtkWidget *window)
{
    GstStateChangeReturn ret, ret2;
    char s[10];
    GstState chg_state;

    if (! GST_IS_ELEMENT(app_data->c_pipeline))
    	return -1;

    ret = gst_element_set_state (app_data->c_pipeline, state);

    switch(ret)
    {
	case GST_STATE_CHANGE_SUCCESS:
	case GST_STATE_CHANGE_NO_PREROLL:
	    ret2 = gst_element_get_state (app_data->c_pipeline, &chg_state, NULL, GST_CLOCK_TIME_NONE);

	    if (chg_state != state)
	    {
		app_msg("MSG9011", "Playing", window);
		return FALSE;
	    }

	    break;

	case GST_STATE_CHANGE_ASYNC:
	    break;

	case GST_STATE_CHANGE_FAILURE:
	    switch (state)
	    {
		case GST_STATE_NULL:
		    strcpy(s, "NULL");
		    break;

		case GST_STATE_READY:
		    strcpy(s, "READY");
		    break;

		case GST_STATE_PAUSED:
		    strcpy(s, "PAUSED");
		    break;

		case GST_STATE_PLAYING:
		    strcpy(s, "PLAYING");
		    break;

		default:
		    strcpy(s, "Unknown");
	    }

	    app_msg("MSG9011", s, window);
	    return FALSE;

	default:
	    app_msg("MSG9011", s, window);
	    return FALSE;
    }

    app_data->state = state;

    return TRUE;
}


/* Check the ref count of the element and set up if required */

int create_element(GstElement **element, char *factory_nm, const char *nm, AppData *app_data, MainUi *m_ui)
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
	app_msg("MSG0009", NULL, m_ui->window);
        return FALSE;
    }

    return TRUE;
}
