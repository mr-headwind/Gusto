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
** Description: Functions to create the main user interface for the application.
**
** Author:	Anthony Buckley
**
** History
**	21-Jun-2022	Initial code
**
*/



/* Defines */
#define MAIN_UI


/* Includes */
#include <stdlib.h>  
#include <string.h>  
#include <libgen.h>  
#include <gtk/gtk.h>  
#include <gdk/gdkkeysyms.h>  
#include <main.h>
#include <user_data.h>
#include <defs.h>
#include <version.h>


/* Prototypes */

void main_ui(AppData *, MainUi *);
void create_main_view(MainUi *);
void set_heading_widgets(MainUi *);
void video_select_widgets(MainUi *);
void output_dir_widgets(MainUi *);
void image_prefix_widgets(MainUi *);
void video_convert_select_widgets(MainUi *);
void video_info_widgets(MainUi *);
void set_button_widgets(MainUi *);

void create_label(GtkWidget **, char *, char *, GtkWidget *);
void create_label2(GtkWidget **, char *, char *, GtkWidget *, int, int, int, int);
void create_entry(GtkWidget **, char *, GtkWidget *, int, int);
void create_radio(GtkWidget **, GtkWidget *, char *, char *, GtkWidget *, int, char *, char *);
void create_cbox(GtkWidget **, char *, const char *[], int, int, GtkWidget *, int, int);
	
GtkWidget * debug_cntr(GtkWidget *);


extern void app_msg(char*, char *, GtkWidget *);
extern void OnVideoBrowse(GtkWidget*, gpointer);
extern void OnDirBrowse(GtkWidget*, gpointer);
extern gboolean OnVideoIn(GtkWidget*, GdkEvent *, gpointer);
extern gboolean OnVideoSet(GtkWidget*, GdkEvent *, gpointer);
extern gboolean OnDirIn(GtkWidget*, GdkEvent *, gpointer);
extern gboolean OnDirSet(GtkWidget*, GdkEvent *, gpointer);
extern void OnFrameSet(GtkWidget*, gpointer);
extern void OnConvert(GtkWidget*, gpointer);
extern void OnQuit(GtkWidget*, gpointer);
extern void set_css();
extern char * home_dir();
extern GtkWidget * find_widget_by_name(GtkWidget *, char *);


/* Globals */

static const char *debug_hdr = "DEBUG-main_ui.c ";


/* Create the user interface and set the CallBacks */

void main_ui(AppData *app_data, MainUi *m_ui)
{  
    /* Set up the UI window */
    m_ui->window = gtk_window_new(GTK_WINDOW_TOPLEVEL);  
    g_object_set_data (G_OBJECT (m_ui->window), "app_data", app_data);
    g_object_set_data (G_OBJECT (m_ui->window), "ui", m_ui);
    gtk_window_set_title(GTK_WINDOW(m_ui->window), TITLE);
    gtk_window_set_position(GTK_WINDOW(m_ui->window), GTK_WIN_POS_CENTER);
    gtk_window_set_default_size(GTK_WINDOW(m_ui->window), 500, 300);
    gtk_container_set_border_width(GTK_CONTAINER(m_ui->window), 10);

    /* Overall view container */
    m_ui->main_vbox = gtk_box_new(GTK_ORIENTATION_VERTICAL, 1);
    gtk_widget_set_halign(GTK_WIDGET (m_ui->main_vbox), GTK_ALIGN_START);

    /* Application main area */
    create_main_view(m_ui);

    /* Information area */
    m_ui->status_info = gtk_label_new(NULL);
    gtk_widget_set_margin_top(GTK_WIDGET (m_ui->status_info), 5);
    gtk_label_set_text(GTK_LABEL (m_ui->status_info), " ");
    gtk_widget_set_halign(GTK_WIDGET (m_ui->status_info), GTK_ALIGN_START);

    /* Combine everything onto the window */
    gtk_box_pack_start (GTK_BOX (m_ui->main_vbox), m_ui->hdg_hbox, FALSE, FALSE, 0);
    gtk_box_pack_start (GTK_BOX (m_ui->main_vbox), m_ui->fn_grid, FALSE, FALSE, 0);
    gtk_box_pack_start (GTK_BOX (m_ui->main_vbox), m_ui->frm_grid, FALSE, FALSE, 0);
    gtk_box_pack_start (GTK_BOX (m_ui->main_vbox), m_ui->video_info_vbox, FALSE, FALSE, 0);
    gtk_box_pack_start (GTK_BOX (m_ui->main_vbox), m_ui->btn_hbox, FALSE, FALSE, 0);
    gtk_box_pack_start (GTK_BOX (m_ui->main_vbox), m_ui->status_info, TRUE, TRUE, 0);

    gtk_container_add(GTK_CONTAINER(m_ui->window), m_ui->main_vbox);  

    /* Exit when window closed */
    m_ui->close_hndlr_id = g_signal_connect(m_ui->window, "destroy", G_CALLBACK(OnQuit), m_ui->window);  

    /* Show window */
    set_css();
    gtk_widget_show_all(m_ui->window);
    gtk_widget_set_visible (m_ui->int_hbox, FALSE);
    gtk_widget_set_visible (m_ui->time_hbox, FALSE);
    gtk_widget_set_sensitive (m_ui->convert_btn, FALSE);

    return;
}


/* Create the application interface widgets */

void create_main_view(MainUi *m_ui)
{  
    /* Add heading */
    set_heading_widgets(m_ui);

    /* Add Video file selection and entry */
    video_select_widgets(m_ui);

    /* Output directory */
    output_dir_widgets(m_ui);

    /* Output directory */
    image_prefix_widgets(m_ui);

    /* Add frame conversion criteria */
    video_convert_select_widgets(m_ui);

    /* Video meta data display */
    video_info_widgets(m_ui);

    /* Add control buttons */
    set_button_widgets(m_ui);

    return;
}


/* Set up the heading and version */

void set_heading_widgets(MainUi *m_ui)
{  
    char *s;

    m_ui->hdg_hbox = gtk_box_new(GTK_ORIENTATION_HORIZONTAL, 1);
    gtk_widget_set_valign(m_ui->hdg_hbox, GTK_ALIGN_CENTER);
    gtk_widget_set_halign(m_ui->hdg_hbox, GTK_ALIGN_CENTER);
    gtk_widget_set_margin_bottom(m_ui->hdg_hbox, 40);

    create_label(&(m_ui->hdg_lbl), "title_2", "Gusto video frame convert to image", m_ui->hdg_hbox);

    s = (char *) malloc(11 + strlen(VERSION));
    sprintf(s, "(Version: %s)", VERSION);
    create_label(&(m_ui->version_lbl), "title_3", s, m_ui->hdg_hbox);
    gtk_widget_set_valign(m_ui->version_lbl, GTK_ALIGN_END);
    free(s);

    return;
}


/* Video file selection */

void video_select_widgets(MainUi *m_ui)
{  
    m_ui->fn_grid = gtk_grid_new();
    gtk_widget_set_valign(m_ui->fn_grid, GTK_ALIGN_CENTER);
    gtk_widget_set_halign(m_ui->fn_grid, GTK_ALIGN_CENTER);
    gtk_widget_set_margin_bottom(m_ui->fn_grid, 10);

    create_label2(&(m_ui->fn_lbl), "title_4", "Video file", m_ui->fn_grid, 0, 0, 1, 1);
    gtk_widget_set_halign(m_ui->fn_lbl, GTK_ALIGN_END);

    create_entry(&(m_ui->fn), "ent_1", m_ui->fn_grid, 1, 0);
    gtk_entry_set_width_chars(GTK_ENTRY (m_ui->fn), 30);
    gtk_entry_set_text(GTK_ENTRY (m_ui->fn), home_dir(m_ui->window));
    gtk_widget_set_margin_left(m_ui->fn, 10);
    gtk_widget_set_margin_top (m_ui->fn, 5);
    g_signal_connect(G_OBJECT (m_ui->fn), "focus-in-event", G_CALLBACK(OnVideoIn), m_ui);  
    g_signal_connect(G_OBJECT (m_ui->fn), "focus-out-event", G_CALLBACK(OnVideoSet), m_ui);  

    m_ui->browse_video_btn = gtk_button_new_with_label("Browse...");
    gtk_widget_set_margin_left(m_ui->browse_video_btn, 10);
    gtk_grid_attach(GTK_GRID (m_ui->fn_grid), m_ui->browse_video_btn, 2, 0, 1, 1);
    g_signal_connect(m_ui->browse_video_btn, "clicked", G_CALLBACK(OnVideoBrowse), (gpointer) m_ui);

    return;
}


/* Output directory */

void output_dir_widgets(MainUi *m_ui)
{  
    char *dir, *out_dir;

    out_dir = (char *) malloc(strlen(home_dir(m_ui->window)) + 8);
    sprintf(out_dir, "%s/myimgs", home_dir(m_ui->window));

    create_label2(&(m_ui->out_dir_lbl), "title_4", "Output Location", m_ui->fn_grid, 0, 1, 1, 1);
    gtk_widget_set_halign(m_ui->out_dir_lbl, GTK_ALIGN_END);

    create_entry(&(m_ui->out_dir), "ent_1", m_ui->fn_grid, 1, 1);
    gtk_entry_set_width_chars(GTK_ENTRY (m_ui->out_dir), 30);
    gtk_entry_set_text(GTK_ENTRY (m_ui->out_dir), out_dir);
    gtk_widget_set_margin_left(m_ui->out_dir, 10);
    gtk_widget_set_margin_top (m_ui->out_dir, 5);
    g_signal_connect(G_OBJECT (m_ui->out_dir), "focus-in-event", G_CALLBACK(OnDirIn), m_ui);  
    g_signal_connect(G_OBJECT (m_ui->out_dir), "focus-out-event", G_CALLBACK(OnDirSet), m_ui);  
    free(out_dir);

    m_ui->browse_dir_btn = gtk_button_new_with_label("Browse...");
    gtk_widget_set_margin_left(m_ui->browse_dir_btn, 10);
    gtk_grid_attach(GTK_GRID (m_ui->fn_grid), m_ui->browse_dir_btn, 2, 1, 1, 1);
    g_signal_connect(m_ui->browse_dir_btn, "clicked", G_CALLBACK(OnDirBrowse), (gpointer) m_ui);

    return;
}


/* Image prefix widgets */

void image_prefix_widgets(MainUi *m_ui)
{  
    create_label2(&(m_ui->prefix_lbl), "title_4", "Output Prefix", m_ui->fn_grid, 0, 2, 1, 1);
    gtk_widget_set_halign(m_ui->prefix_lbl, GTK_ALIGN_END);

    create_entry(&(m_ui->img_prefix), "ent_1", m_ui->fn_grid, 1, 2);
    gtk_entry_set_width_chars(GTK_ENTRY (m_ui->img_prefix), 5);
    gtk_entry_set_text(GTK_ENTRY (m_ui->img_prefix), "Image-");
    gtk_widget_set_margin_left(m_ui->img_prefix, 10);
    gtk_widget_set_margin_right(m_ui->img_prefix, 100);
    gtk_widget_set_margin_top (m_ui->img_prefix, 5);

    return;
}


/* Frame conversion options */

void video_convert_select_widgets(MainUi *m_ui)
{  
    const char *frame_selection_arr[] = { "Every frame", "Selected frames", "Duration (mins)", "Duration (secs)" };
    const int frm_max = 4;
    const char *codec_selection_arr[] = { "JPG", "PNG", "BMP" };
    const int codec_max = 3;

    /*Set container */
    m_ui->frm_grid = gtk_grid_new();
    gtk_widget_set_valign(m_ui->frm_grid, GTK_ALIGN_CENTER);
    gtk_widget_set_halign(m_ui->frm_grid, GTK_ALIGN_START);

    /* Select which frames are to be converted */
    create_cbox(&(m_ui->frm_select_cbx), "frm_sel", frame_selection_arr, frm_max, 0, m_ui->frm_grid, 0, 0);
    g_signal_connect(m_ui->frm_select_cbx, "changed", G_CALLBACK(OnFrameSet), (gpointer) m_ui);

    /* Selected frames */
    m_ui->int_hbox = gtk_box_new(GTK_ORIENTATION_HORIZONTAL, 1);
    gtk_widget_set_valign(m_ui->int_hbox, GTK_ALIGN_CENTER);
    gtk_widget_set_halign(m_ui->int_hbox, GTK_ALIGN_CENTER);

    create_label(&(m_ui->frm_interval_lbl), "title_4", "Every nth", m_ui->int_hbox);
    gtk_widget_set_margin_left(m_ui->frm_interval_lbl, 10);

    m_ui->frm_interval = gtk_entry_new();
    gtk_widget_set_name(m_ui->frm_interval, "ent_1");
    gtk_entry_set_width_chars(GTK_ENTRY (m_ui->frm_interval), 3);
    gtk_widget_set_margin_left(m_ui->frm_interval, 10);
    gtk_box_pack_start (GTK_BOX (m_ui->int_hbox), m_ui->frm_interval, FALSE, FALSE, 0);

    gtk_grid_attach(GTK_GRID (m_ui->frm_grid), m_ui->int_hbox, 2, 0, 1, 1);

    /* Select each frame */
    gtk_entry_set_text(GTK_ENTRY (m_ui->frm_interval), "1");

    /* Select frames for a part of the video */
    m_ui->time_hbox = gtk_box_new(GTK_ORIENTATION_HORIZONTAL, 1);
    gtk_widget_set_valign(m_ui->time_hbox, GTK_ALIGN_CENTER);
    gtk_widget_set_halign(m_ui->time_hbox, GTK_ALIGN_CENTER);

    create_label(&(m_ui->video_start_lbl), "title_4", "Start at", m_ui->time_hbox);
    gtk_widget_set_margin_left(m_ui->video_start_lbl, 10);

    m_ui->video_start = gtk_entry_new();
    gtk_widget_set_name(m_ui->video_start, "ent_1");
    gtk_entry_set_width_chars(GTK_ENTRY (m_ui->video_start), 3);
    gtk_widget_set_margin_left(m_ui->video_start, 10);
    gtk_box_pack_start (GTK_BOX (m_ui->time_hbox), m_ui->video_start, FALSE, FALSE, 0);

    create_label(&(m_ui->duration_lbl), "title_4", "Duration", m_ui->time_hbox);
    gtk_widget_set_margin_left(m_ui->duration_lbl, 10);

    m_ui->duration = gtk_entry_new();
    gtk_widget_set_name(m_ui->duration, "ent_1");
    gtk_entry_set_width_chars(GTK_ENTRY (m_ui->duration), 3);
    gtk_widget_set_margin_left(m_ui->duration, 10);
    gtk_widget_set_tooltip_text (m_ui->duration, "Enter '0' for remainder of video.");
    gtk_box_pack_start (GTK_BOX (m_ui->time_hbox), m_ui->duration, FALSE, FALSE, 0);

    gtk_grid_attach(GTK_GRID (m_ui->frm_grid), m_ui->time_hbox, 2, 0, 1, 1);

    /* Select the type of output image format */
    create_label2(&(m_ui->codec_lbl), "title_4", "Codec", m_ui->frm_grid, 3, 0, 1, 1);
    gtk_widget_set_margin_left(m_ui->codec_lbl, 10);

    create_cbox(&(m_ui->codec_select_cbx), "codec_sel", codec_selection_arr, codec_max, 0, m_ui->frm_grid, 4, 0);
    gtk_widget_set_margin_left(m_ui->codec_select_cbx, 10);

    return;
}


/* Video information widgets */

void video_info_widgets(MainUi *m_ui)
{  
    m_ui->video_info_vbox = gtk_box_new(GTK_ORIENTATION_VERTICAL, 1);
    gtk_widget_set_margin_top(m_ui->video_info_vbox, 15);

    create_label(&(m_ui->video_info_lbl), "title_4", "Video Information", m_ui->video_info_vbox);

    m_ui->video_frm = gtk_frame_new(NULL);
    gtk_box_pack_start (GTK_BOX (m_ui->video_info_vbox), m_ui->video_frm, FALSE, FALSE, 0);

    m_ui->txt_view = gtk_text_view_new();
    gtk_widget_set_name (m_ui->txt_view, "txtview_1");
    gtk_text_view_set_wrap_mode (GTK_TEXT_VIEW (m_ui->txt_view), GTK_WRAP_WORD);
    gtk_text_view_set_editable (GTK_TEXT_VIEW (m_ui->txt_view), FALSE);
    gtk_container_add (GTK_CONTAINER (m_ui->video_frm), m_ui->txt_view);

    m_ui->txt_buffer = gtk_text_view_get_buffer (GTK_TEXT_VIEW (m_ui->txt_view));
    gtk_text_buffer_set_text (m_ui->txt_buffer, "\n\n\n", -1);

    return;
}


/* Set up the control buttons */

void set_button_widgets(MainUi *m_ui)
{  
    m_ui->btn_hbox = gtk_box_new(GTK_ORIENTATION_HORIZONTAL, 1);
    gtk_widget_set_valign(m_ui->btn_hbox, GTK_ALIGN_CENTER);
    gtk_widget_set_halign(m_ui->btn_hbox, GTK_ALIGN_CENTER);
    gtk_widget_set_margin_top(m_ui->btn_hbox, 30);

    m_ui->convert_btn = gtk_button_new_with_label("Convert");
    gtk_widget_set_name(m_ui->convert_btn, "button_1");
    g_signal_connect(m_ui->convert_btn, "clicked", G_CALLBACK(OnConvert), m_ui);
    gtk_box_pack_start (GTK_BOX (m_ui->btn_hbox), m_ui->convert_btn, FALSE, FALSE, 0);
    gtk_widget_set_sensitive (m_ui->convert_btn, FALSE);

    m_ui->close_btn = gtk_button_new_with_label("Close");
    gtk_widget_set_name(m_ui->close_btn, "button_2");
    g_signal_connect_swapped(m_ui->close_btn, "clicked", G_CALLBACK(OnQuit), m_ui->window);
    gtk_box_pack_start (GTK_BOX (m_ui->btn_hbox), m_ui->close_btn, FALSE, FALSE, 0);
    gtk_widget_set_margin_left(m_ui->close_btn, 30);

    return;
}


/* Create standard label */

void create_label(GtkWidget **lbl, char *nm, char *txt, GtkWidget *cntr) 
{  
    *lbl = gtk_label_new(txt);  
    gtk_widget_set_name(*lbl, nm);

    gtk_widget_set_valign(*lbl, GTK_ALIGN_CENTER);
    gtk_widget_set_halign(*lbl, GTK_ALIGN_START);
    gtk_widget_set_margin_top (*lbl, 5);
    gtk_widget_set_margin_start (*lbl, 10);
    gtk_box_pack_start (GTK_BOX (cntr), *lbl, FALSE, FALSE, 0);

    return;
}  


/* Create standard label */

void create_label2(GtkWidget **lbl, char *nm, char *txt, GtkWidget *cntr, 
		  int col, int row, int c_spn, int r_spn)
{  
    *lbl = gtk_label_new(txt);  
    gtk_widget_set_name(*lbl, nm);

    gtk_widget_set_halign(*lbl, GTK_ALIGN_START);
    gtk_widget_set_valign(*lbl, GTK_ALIGN_CENTER);
    gtk_widget_set_margin_top (*lbl, 5);
    gtk_grid_attach(GTK_GRID (cntr), *lbl, col, row, c_spn, r_spn);

    return;
}


/* Create standard entry */

void create_entry(GtkWidget **ent, char *nm, GtkWidget *cntr, int col, int row) 
{  
    *ent = gtk_entry_new();  
    gtk_widget_set_name(*ent, nm);

    gtk_widget_set_valign(GTK_WIDGET (*ent), GTK_ALIGN_CENTER);
    gtk_grid_attach(GTK_GRID (cntr), *ent, col, row, 1, 1);

    return;
}


/* Create standard radio */

void create_radio(GtkWidget **rad, GtkWidget *grp, char *txt, char *nm, GtkWidget *cntr, 
		  int active, char *obj_nm, char *obj_data_str)
{  
    if (grp == NULL)
	*rad = gtk_radio_button_new_with_label (NULL, txt);
    else
	*rad = gtk_radio_button_new_with_label_from_widget (GTK_RADIO_BUTTON (grp), txt);

    gtk_widget_set_name(*rad, "rad_1");
    gtk_widget_set_halign(*rad, GTK_ALIGN_START);
    gtk_widget_set_valign(*rad, GTK_ALIGN_CENTER);
    gtk_widget_set_margin_top (*rad, 0);
    gtk_widget_set_margin_start (*rad, 0);
    gtk_box_pack_start (GTK_BOX (cntr), *rad, FALSE, FALSE, 0);

    if (active == TRUE)
	gtk_toggle_button_set_active (GTK_TOGGLE_BUTTON (*rad), TRUE);

    if (obj_nm != NULL && obj_data_str != NULL)
	g_object_set_data_full (G_OBJECT (*rad), obj_nm, g_strdup (obj_data_str), (GDestroyNotify) g_free);

    return;
}


/* Create standard combobox */

void create_cbox(GtkWidget **cbox, char *nm, const char *arr[], int max, int active, 
		 GtkWidget *cntr, int col, int row)
{  
    int i;
    char s[max];

    *cbox = gtk_combo_box_text_new();  
    gtk_widget_set_name(*cbox, nm);

    for(i = 0; i < max; i++)
    {
    	sprintf(s, "%d", i);
    	gtk_combo_box_text_append (GTK_COMBO_BOX_TEXT(*cbox), s, arr[i]);
    }

    gtk_combo_box_set_active (GTK_COMBO_BOX(*cbox), active);

    gtk_widget_set_halign(*cbox, GTK_ALIGN_START);
    gtk_widget_set_valign(*cbox, GTK_ALIGN_CENTER);
    gtk_widget_set_margin_top (*cbox, 0);
    gtk_grid_attach(GTK_GRID (cntr), *cbox, col, row, 1, 1);

    return;
}


/* Debug widget container */

GtkWidget * debug_cntr(GtkWidget *cntr)
{
    const gchar *widget_name;
    GtkWidget *widget;
    GtkWidgetPath *w_path;

    if (! GTK_IS_CONTAINER(cntr))
    {
	app_msg("MSG9006", "btn children", NULL);
    	return NULL;
    }

    widget_name = gtk_widget_get_name (cntr);
    printf("%s widget structure for %s\n", debug_hdr, widget_name);

    GList *child_widgets = gtk_container_get_children(GTK_CONTAINER (cntr));
    //printf("%s \tno of children %d\n", debug_hdr, g_list_length(child_widgets));

    child_widgets = g_list_first(child_widgets);

    while (child_widgets != NULL)
    {
	widget = child_widgets->data;
	widget_name = gtk_widget_get_name (widget);
	printf("%s \tname %s\n", debug_hdr, widget_name);

	w_path = gtk_container_get_path_for_child (GTK_CONTAINER (cntr), widget);
	printf("%s \tpath %s\n", debug_hdr, gtk_widget_path_to_string (w_path));

	if (GTK_IS_CONTAINER(widget))
	    debug_cntr(widget);

	if (GTK_IS_LABEL (widget))
	    break;

	child_widgets = g_list_next(child_widgets);
    }

    g_list_free (child_widgets);

    return widget;
}
