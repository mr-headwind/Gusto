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


/* Structure to group GST elements */

typedef struct _app_gst_objects
{
    GstElement *file_src, *v_decode, *fk_sink, *mf_sink;	
    GstElement *tee, *video_queue, *convert_queue, *encoder;
    GstElement *c_pipeline;
} app_gst_objs;


/* Structure to contain all our information, so we can pass it around */

typedef struct _AppData
{
    //GstElement *pipeline;           	/* Our one and only pipeline */
    app_gst_objs gst_objs;		/* GST objects for viewing & recording */
    GstState state;                 	/* Current state of the pipeline */
    char *video_fn;			/* Video file name */
    int interval_type;	    		/* Type of frame conversion (eg. each frame or every n frames / seconds) */
    int frame_interval;	    		/* Interval between frame conversion */
    int time_start;	    		/* Collect frames for a time interval */
    int time_duration;	    		/* Time period */
    gchar *output_dir;			/* Directory to hold output image files */
    char *image_type;	    		/* Image type (jpg, png, bmp) */
    char *img_prefix;			/* Prefix to use for image file names */
} AppData;
