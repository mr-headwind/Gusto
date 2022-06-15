/*
**  Copyright (C) 202x Anthony Buckley
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
** Application:	Gusto
**
** Author:	Anthony Buckley
**
** Description:
**  	 Application control for Gusto. Play video and capture individual frames.
**
** History
**	31-May-2022	Initial code
**
*/


/* Includes */

#include <stdlib.h>  
#include <string.h>  
#include <libgen.h>  
#include <gtk/gtk.h>  
#include <main.h>
#include <user_data.h>
#include <defs.h>


/* Defines */


/* Prototypes */

void initialise(UserData *, MainUi *);
void final();

extern void main_ui(UserData *, MainUi *);
extern int check_app_dir();
extern int reset_log();
extern void close_log();
extern void log_msg(char*, char*, char*, GtkWidget*);
//extern void debug_session();


/* Globals */

static const char *debug_hdr = "DEBUG-Gusto.c ";


/* Main program control */

int main(int argc, char *argv[])
{  
    UserData user_data;
    MainUi m_ui;

    /* Initial work */
    initialise(&user_data, &m_ui);

    /* Initialise Gtk */
    gtk_init(&argc, &argv);  

    main_ui(&user_data, &m_ui);

    gtk_main();  

    final();

    exit(0);
}  


/* Initial work */

void initialise(UserData *user_data, MainUi *m_ui)
{
    char *p;

    /* Set variables */
    app_msg_extra[0] = '\0';
    memset(user_data, 0, sizeof (UserData));
    memset(m_ui, 0, sizeof (MainUi));

    /* Set application directory */
    if (! check_app_dir())
    	exit(-1);

    /* Reset log file and log start */
    if (! reset_log())
    	exit(-1);

    log_msg("SYS9001", NULL, NULL, NULL);

    return;
}


/* Final work */

void final()
{
    /* Close log file */
    log_msg("SYS9002", NULL, NULL, NULL);
    close_log();

    return;
}
