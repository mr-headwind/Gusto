/*
**  Copyright (C) 202x Anthony Buckley
** 
**  This file is part of MYPROJECT.
** 
**  MYPROJECT is free software: you can redistribute it and/or modify
**  it under the terms of the GNU General Public License as published by
**  the Free Software Foundation, either version 3 of the License, or
**  (at your option) any later version.
**  
**  MYPROJECT is distributed in the hope that it will be useful,
**  but WITHOUT ANY WARRANTY; without even the implied warranty of
**  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
**  GNU General Public License for more details.
**  
**  You should have received a copy of the GNU General Public License
**  along with MYPROJECT.  If not, see <http://www.gnu.org/licenses/>.
*/



/*
** Description:
**  Screen appearance setup
**
** Author:	Anthony Buckley
**
** History
**	xx-xxx-202x	Initial code
**
*/


/* Defines */

#define SD_W 1600
#define SD_H 900
#define SD_SZ 3


/* Includes */

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <gtk/gtk.h>  
#include <gdk/gdk.h> 
#include <main.h>


/* Prototypes */

void set_css();
char * check_screen_res(int *);
void get_screen_res(GdkRectangle *);
void css_adjust_font_sz(char **);
void css_set_button_status(GtkWidget *, int, gchar *);
GdkRGBA * css_get_bg_colour(GtkWidget *);


/* Globals */

static const char *debug_hdr = "DEBUG-css.c ";

/*
** Colour guide
**              #800000 : Deep red or maroon
**              #400080 : Purple with shade of blue
**              #8a87c4 : Medium mauve blue
**              #fa8072 : Medium salmon
**              #e00b40 : Crimson
**              #b8860b : Dark golden yellow
**              #e6e6fa : Light bluey grey
**              #ebe7f8 : Light purple grey
**              #708090 : Gunmetal grey
**              #fbf6fe : Light mauve
**              #daf5d0 : Light green
**              #f8fcca : Light yellow
**              #cbd8fb : Light blue
**              #ffd6d6 : Light red
**              #000000 : Light black
**              #e6f3ff : Very light blue
**              #ffe6e6 : Very light red
**              #e6ffe6 : Very light green
*/

static char *css_data_fhd = 
	"@define-color DARK_BLUE rgba(0%,0%,50%,1.0); "
	"@define-color METAL_GREY rgba(55,83,103,1.0); "
	"button, entry, label { font-family: Sans; font-size: 14px; }"
	"label#data_1 { color: @DARK_BLUE; }"
	"label#data_2 { color: #800000; font-family: Sans; font-size: 11px; }"
	"label#data_3 { color: #400080; font-family: Sans; font-size: 10px; }"
	"label#title_1 { font-family: Sans; font-size: 18px; font-weight: bold; }"
	"label#title_2 { font-family: Serif; font-size: 18px; font-style: italic; color: #fa8072; }"
	"label#title_3 { font-family: Sans; font-size: 12px; color: @DARK_BLUE;}"
	"label#title_4 { font-family: Sans; font-size: 12px; font-weight: bold; }"
	"label#title_5 { font-family: Sans; font-size: 12px; color: #e00b40;}"
	"label#status { font-family: Sans; font-size: 12px; color: #b8860b; font-style: italic; }"
	"entry#ent_1 { color: @DARK_BLUE; }"
	"radiobutton#rad_1 { color: @DARK_BLUE; font-family: Sans; font-size: 12px; }"
	"radiobutton > label { color: @DARK_BLUE; font-family: Sans; font-size: 12px; }"
	"frame { background-color: #e6e6fa; border-radius: 8px}"
	"frame > label { color: #800000; font-weight: 500; }"
	"combobox * { color: @METAL_GREY; font-family: Sans; font-size: 12px; }"
	"progressbar#pbar_1 { color: @DARK_BLUE; font-family: Sans; font-size: 10px; }"
	"button#button_video { color: #708090; font-weight: bold; font-size: 14px; background-color: \
			       #e6ffe6; border-image: none; background-image: none;}"
	"button#button_conv { color: #708090; font-weight: bold; font-size: 14px; }"
	"button#button_reset { color: #708090; font-weight: bold; font-size: 14px; }"
	"button#button_close * { color: #800000; font-size: 14px; }"
	"notebook * { font-family: Sans; font-size: 11px; }"
	"textview { font-family: Sans; font-size: 14px; }"
	"textview text { font-family: Sans; font-size: 12px; }"
	"textview#txtview_1 text { font-family: Sans; font-size: 12px; background-color: #ebe7f8}"
	"button.link { font-family: Sans; font-size: 12px; color: @DARK_BLUE; }";


// These don't work
//"GtkLabel#title_deco1 { font: Comic Sans 15; font-weight: 500; color: #fa8072 }"
//"GtkLabel#title_deco2 { font-family: Comic Sans; font-size: 15px; font-style: italic; color: #fa8072 }"


/* Set up provider data and apply */

void set_css()
{
    int sd_flg;
    GError *err = NULL;
    char *css_data;

    css_data = check_screen_res(&sd_flg);

    GtkCssProvider *provider = gtk_css_provider_new();
    GdkDisplay *display = gdk_display_get_default();
    GdkScreen *screen = gdk_display_get_default_screen(display);

    gtk_style_context_add_provider_for_screen(screen,
    					      GTK_STYLE_PROVIDER(provider),
    					      GTK_STYLE_PROVIDER_PRIORITY_USER);

    gtk_css_provider_load_from_data(GTK_CSS_PROVIDER(provider),
				    (const gchar *) css_data,
				    -1,
				    &err);

    if (err != NULL)
    {
    	printf("%s set_css  ****css error  %s\n", debug_hdr, err->message); fflush(stdout);
    	g_clear_error (&err);
    }

    if (sd_flg == TRUE)
    	free(css_data);

    g_object_unref(provider);

    return;
}


/* Check the screen resolution and whether to adjust the font size */

char * check_screen_res(int *sd_flg)
{
    GdkRectangle workarea = {0};
    char *css_data_sd;

    get_screen_res(&workarea); 
    //printf ("%s get_screen_res W: %u x H:%u\n", debug_hdr, workarea.width, workarea.height);
	
    // Default font size suits Full HD resolution, but lesser res needs needs lesser font size to stop
    // MYPROJECT looking too large. If approaching FHD, keep the default.
    // SD_W and SD_H are not really standard def, but serve as a good cut-off point.
    if (workarea.width > SD_W || workarea.height > SD_H)
    {
    	*sd_flg = FALSE;
    	return css_data_fhd;
    }
    else
    {
	*sd_flg = TRUE;
	css_adjust_font_sz(&css_data_sd);
    	return css_data_sd;
    }
}


/* Get the screen resolution and apply the appropriate font */

void get_screen_res(GdkRectangle *workarea)
{
    gdouble res;
    GdkScreen *scr;

    /* 16.04
    */
    if ((scr = gdk_screen_get_default ()) == NULL)
    	return;
    
    gdk_screen_get_monitor_workarea (scr, 0, workarea);

    /* 18.04
    gdk_monitor_get_workarea (gdk_display_get_primary_monitor (gdk_display_get_default()),
			      workarea);
    */

    return;
}


/* Adjust the font size down */

void css_adjust_font_sz(char **css)
{
    int i, j, fn_len, new_fn_len;
    char *p, *p_new, *p_fhd;
    char num[15];

    /* Copy to a new css string and extract and adjust the font size */
    *css = (char *) malloc(strlen(css_data_fhd) + 1);
    p_new = *css;
    p_fhd = css_data_fhd;

    while ((p = strstr(p_fhd, "px")) != NULL)
    {
    	/* Determine the number of font bytes */
    	for(fn_len = 1; *(p - fn_len) != ' '; fn_len++);
    	
    	fn_len--;

    	/* Determine the font value */
    	i = 0;

	while(i < fn_len)
	{
	    num[i] = *(p - fn_len + i);
	    i++;
	}

	num[i] = '\0';
	//printf("%s fn_len is: %d  font sz: %s\n", debug_hdr, fn_len, num); fflush(stdout);

    	/* Copy up to font */
    	memcpy(p_new, p_fhd, p - p_fhd - fn_len);
    	p_new = p_new + (p - p_fhd - fn_len);

	/* Adjust to new font size and convert back to string */
	i = atoi(num) - SD_SZ;
	sprintf(num, "%d", i);
	//printf("%s new num is: %s\n", debug_hdr, num); fflush(stdout);

	/* Add to new string */
	for(i = 0; num[i] != '\0'; i++)
	{
	    *p_new = num[i];
	    p_new++;
	}

	*p_new = 'p';
	*(p_new + 1) = 'x';
	p_new += 2;

	/* Advance to next */
	p_fhd = p + 2;
    }

    /* Copy any residual bytes */
    while(*p_fhd != '\0')
    {
    	*p_new = *p_fhd;
    	p_new++;
    	p_fhd++;
    }

    *p_new = '\0';
    //printf("%s new css is: %s\n", debug_hdr, *css); fflush(stdout);

    return;
}


/* Set up css for button status */

void css_set_button_status(GtkWidget *btn, int btn_status, gchar *rgba)
{ 
    char bg_colour[8], css_text[160];
    const gchar *nm;

    // Get the name
    nm = gtk_widget_get_name(GTK_WIDGET (btn));

    /* Set the appropriate colour and set the provider string */
    switch (btn_status)
    {
    	case 1:
	    strcpy(bg_colour, "#e6ffe6");	// Ready to be selected (green)
	    sprintf(css_text, " button#%s { color: #708090; font-weight: bold; font-size: 14px; "
			      "background-color: %s; border-image: none;  background-image: none; }", nm, bg_colour);
	    break;
    	case 2:
	    strcpy(bg_colour, "#e6f3ff");	// Finished (blue)
	    sprintf(css_text, " button#%s { color: #708090; font-weight: bold; font-size: 14px; "
			      "background-color: %s; border-image: none;  background-image: none; }", nm, bg_colour);
	    break;
    	case 3:
	    if (rgba != NULL)
	    {	
		sprintf(css_text, " button#%s { color: #708090; font-weight: bold; font-size: 14px; "
				  "background-color: %s; border-image: none;  background-image: none; }", nm, rgba);
		break;
	    }
	default:
	    sprintf(css_text, " button#%s { color: #708090; font-weight: bold; font-size: 14px; }", nm);
	    break;
    };

    // Need an object to store css information: the CSS Provider
    GtkCssProvider * cssProvider = gtk_css_provider_new();

    // Load CSS into the object ("-1" says, that the css string is \0-terminated)
    gtk_css_provider_load_from_data(cssProvider, css_text, -1, NULL); 

    // The "Style context" manages CSS providers (as there can be more of them)            
    GtkStyleContext * context = gtk_widget_get_style_context(btn);   

    // Add our CSS provider (that contains the CSS) to that "style manager".
    gtk_style_context_add_provider(context, GTK_STYLE_PROVIDER(cssProvider),GTK_STYLE_PROVIDER_PRIORITY_USER);

    // Possibly not required
    g_object_unref (cssProvider); 

    return;
}


/* Save the background colour for a widget */

GdkRGBA * css_get_bg_colour(GtkWidget *w)
{ 
    GtkStyleContext* style_context;
    GdkRGBA *background_color = NULL;

    style_context = gtk_widget_get_style_context (w);
    gtk_style_context_get (style_context,
			   gtk_style_context_get_state (style_context),
			   GTK_STYLE_PROPERTY_BACKGROUND_COLOR, &background_color,
			   NULL);

    return background_color;
}
