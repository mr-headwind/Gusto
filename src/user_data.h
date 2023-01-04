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
#include <gst/pbutils/pbutils.h>


/* Enums */


/* Structure to group GST elements */

typedef struct _app_gst_objects
{
    GstElement *file_src, *v_decode, *encoder, *mf_sink;
    GstElement *v_rate, *v_convert, *px_buf;
} app_gst_objs;


/* Structure to contain all our information, so we can pass it around */

typedef struct _AppData
{
    GstElement *c_pipeline;           	/* Our one and only pipeline */
    app_gst_objs gst_objs;		/* GST objects for viewing & recording */
    GstState state;                 	/* Current state of the pipeline */
    GstState init_state;                /* Start state of the pipeline, dependent on Interval */
    char *video_fn;			/* Video file name */
    char *video_fn_tmp;			/* Temporary file name storage */
    int interval_type;	    		/* Type of frame conversion (eg. each frame or every n frames, time) */
    int frame_interval;	    		/* Interval (no. of frames) between conversions */
    gint64 time_start;	    		/* Collect frames for a time interval */
    gint64 time_duration;	    	/* Time period */
    gchar *output_dir;			/* Directory to hold output image files */
    gchar *filenm_tmpl;			/* Image filename template */
    char *image_type;	    		/* Image type (jpg, png, pnm, bmp) */
    char *img_prefix;			/* Prefix to use for image file names */

    guint fr_denom;			/* Frame rate demoninator */
    guint fr_num;			/* Frame rate numerator */
    gboolean seekable;			/* Is video seekable */
    GstClockTime video_duration;	/* Video length in nanoseconds */
    char *fmt_duration;			/* String duration */
    GstDiscoverer *discoverer;

    GMainLoop *loop;
} AppData;
