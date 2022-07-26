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
** Description:	User entered data
**
** Author:	Anthony Buckley
**
** History
**	21-Jun-2022	Initial
**
*/


/* Defines */

#ifndef USR_HDR
#define USR_HDR
#endif

/* Includes */

#include <linux/videodev2.h>
#include <gst/gst.h>
#include <gst/video/video.h>


/* Enums */


/* Structure to contain all our information, so we can pass it around */

typedef struct _AppData
{
    //GstElement *pipeline;           	/* Our one and only pipeline */
    //app_gst_objects gst_objs;		/* GST objects for viewing & recording */
    GstState state;                 	/* Current state of the pipeline */
    char *video_fn;			/* Video file name */
    int interval_type;	    		/* Type of frame conversion (eg. each frame or every n frames / seconds) */
    int frame_interval;	    		/* Interval between frame conversion */
    int time_start;	    		/* Collect frames for a time interval */
    int time_duration;	    		/* Time period */
    gchar *output_dir;			/* Directory to hold output image files */
    char *output_prefix;		/* Prefix to use for image file names */
    char *image_type;	    		/* Image type (jpg, png, bmp) */
} AppData;


/* Structure to group GST elements */

/*
typedef struct _app_gst_objects
{
    GstElement *v4l2_src, *vid_rate, *v_filter, *v_convert, *v_sink;	// View only
    GstElement *tee, *video_queue, *capt_queue, *muxer, *file_sink;	// Fixed capture
    GstElement *c_convert;						// Fixed capture
    GstElement *encoder; 						// Encoder capture
    GstElement *c_filter;						// Caps capture
    GstElement *q1; 							// Reticule (insertion) related
    GstPad *tee_capt_pad, *tee_video_pad;
    GstCaps *v_caps, *c_caps;						
    GstElement *cairo_overlay, *cairo_convert;				// Cairo elements for reticule
    GstPad *blockpad;							// Reticule only
    gulong probe_id;							// Reticule only
    CairoOverlayState *overlay_state;					// Reticule only
} app_gst_objects;
*/
