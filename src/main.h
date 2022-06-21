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
** Description:	Main program include file
**
** Author:	Anthony Buckley
**
** History
**	31-Junp-2022	Initial
**
*/




/* Defines */

#ifndef MAIN_HDR
#define MAIN_HDR
#endif

/* Structure to contain main interface items for easy access */

typedef struct _main_ui
{
    /* Main view widgets */
    GtkWidget *window;
    GtkWidget *status_info;  

    /* Control Panel widgets */
    GtkWidget *main_vbox;
    GtkWidget *hdg_lbl, *version_lbl, *hdg_hbox;
    GtkWidget *fn_lbl, *fn_ent, *lookup_btn, *fn_hbox;
    GtkWidget *frm_lbl, *frm_select_cbx, *frm_count_lbl, *frm_count_ent, *frm_hbox;
    GtkWidget *img_lbl, *ing_select_cbx, *img_hbox;
    GtkWidget *ok_btn, *close_btn, *btn_hbox;

    /* Callback Handlers */
    int close_hndlr_id;

    /* Main window base size */
    int main_width;
    int main_height;

    int duration;
    int no_of_frames;
} MainUi;
