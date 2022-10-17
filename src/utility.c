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
** Description:
**  Error and Message Reference functions
**  Logging functions
**  Window management
**  General usage functions
**
** Author:	Anthony Buckley
**
** History
**	19-Jun-2022	Initial code
**
*/


/* Defines */

#define ERR_FILE


/* Includes */

#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <string.h>
#include <ctype.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <errno.h>
#include <time.h>
#include <gtk/gtk.h>
#include <defs.h>
#include <main.h>


/* Prototypes */

void app_msg(char*, char *, GtkWidget*);
char * app_msg_text(char*, char *);
void info_dialog(GtkWidget *, char *, char *);
gint query_dialog(GtkWidget *, char *, char *);
int choose_file_dialog(char *, int, gchar **, MainUi *);
void get_msg(char*, char*, char*);
void string_trim(char*);
void register_window(GtkWidget *);
void deregister_window(GtkWidget *);
void free_window_reg();
void close_open_ui();
int close_ui(char *);
int is_ui_reg(char *, int);
char * home_dir();
void strlower(char *, char *);
void dttm_stamp(char *, size_t);
int check_dir(char *);
int make_dir(char *);
FILE * open_file(char *, char *);
int read_file(FILE *, char *, int);
int val_str2numb(char *, int *, char *, GtkWidget *);
int check_errno();
void print_bits(size_t const, void const * const);
GtkWidget * find_parent(GtkWidget *);
GtkWidget * find_widget_by_name(GtkWidget *, char *);
GtkWidget * find_widget_by_parent(GtkWidget *, char *);
GList * ctrl_widget_list(GtkWidget *, GtkWidget *);
void delete_menu_items(GtkWidget *, char *);

extern void cur_date_str(char *, int, char *);



/* Globals */

static const char *app_messages[][2] = 
{ 
    { "MSG0001", "Error: %s has an invalid value. "},
    { "MSG0002", "Error: Please enter a value for %s. "},
    { "MSG0003", "%s does not exist. "},
    { "MSG0004", "Error: No %s found. "},
    { "MSG0005", "Warning: No %s found. "},
    { "MSG0006", "File error: %s. "},
    { "MSG0007", "Failed to create image: %s "},
    { "MSG0008", "File %s does not exist or cannot be read. "},
    { "MSG0009", "Error: %s has an invalid value. "},
    { "MSG9000", "Session started. "},
    { "MSG9001", "Session ends. "},
    { "MSG9003", "Failed to start application. "},
    { "MSG9004", "Failed to read $HOME variable. "},
    { "MSG9005", "Debug: %s. "},
    { "MSG9006", "Failed to get parent container widget. %s "},
    { "MSG9007", "Failed to find widget. %s "},
    { "MSG9008", "Failed to create directory: %s "},
    { "MSG9009", "Not all GST elements could be created. "},
    { "MSG9010", "GST Pipeline elements could not be linked. "},
    { "MSG9011", "Unable to set the pipeline to the %s state. "},
    { "MSG9012", "Pipeline Message: %s received. "},
    { "MSG9013", "Error video creating discoverer. "},
    { "MSG9014", "Failed to video discoverer for: %s "},
    { "MSG9015", "Discoverer info result error: %s "},
    { "MSG9016", "'Video' file - %s - cannot be played "},
    { "MSG9999", "Error - Unknown error message given. "}			// NB - MUST be last
};

static const int Msg_Count = 26;
static char *Home;
static const char *debug_hdr = "DEBUG-utility.c ";
static GList *open_ui_list_head = NULL;
static GList *open_ui_list = NULL;


/* Process application messages and error conditions */

void app_msg(char *msg_id, char *opt_str, GtkWidget *window)
{
    char msg[512];

    /* Lookup the message */
    get_msg(msg, msg_id, opt_str);
    strcat(msg, " \n");

    /* Print the message */
    printf("%s: %s\n", TITLE, msg); fflush(stdout);

    if (app_msg_extra[0] != '\0')
	printf("%s\n", app_msg_extra); fflush(stdout);

    /* Display the message */
    if (window)
	info_dialog(window, msg, app_msg_extra);

    /* Reset global message extra details */
    app_msg_extra[0] = '\0';

    return;
}


/* Process application messages and error conditions and return text */

char * app_msg_text(char *msg_id, char *opt_str)
{
    char msg[512];
    char *msg_txt;
    int len;

    /* Lookup the message */
    get_msg(msg, msg_id, opt_str);
    strcat(msg, " \n");

    /* Print the message */
    printf("%s: %s", TITLE, msg); fflush(stdout);

    if (app_msg_extra[0] != '\0')
	printf("%s\n", app_msg_extra); fflush(stdout);

    /* Set the text */
    len = strlen(msg) + strlen(app_msg_extra) + 2;
    msg_txt = (char *) malloc(len);
    sprintf(msg_txt, "%s%s\n", msg, app_msg_extra);

    /* Reset global message extra details */
    app_msg_extra[0] = '\0';

    return msg_txt;
}


/* General prupose information dialog */

void info_dialog(GtkWidget *window, char *msg, char *opt)
{
    GtkWidget *dialog;

    dialog = gtk_message_dialog_new (GTK_WINDOW (window),
				     GTK_DIALOG_MODAL,
				     GTK_MESSAGE_ERROR,
				     GTK_BUTTONS_CLOSE,
				     msg,
				     opt);

    gtk_dialog_run (GTK_DIALOG (dialog));
    gtk_widget_destroy (dialog);

    return;
}


/* General prupose query dialog */

gint query_dialog(GtkWidget *window, char *msg, char *opt)
{
    GtkWidget *dialog;
    gint res;

    GtkDialogFlags flags = GTK_DIALOG_MODAL | GTK_DIALOG_DESTROY_WITH_PARENT;

    dialog = gtk_message_dialog_new (GTK_WINDOW (window),
				     flags,
				     GTK_MESSAGE_QUESTION,
				     GTK_BUTTONS_YES_NO,
				     msg,
				     opt);

    res = gtk_dialog_run (GTK_DIALOG (dialog));
    gtk_widget_destroy (dialog);

    return res;
}


/* Callback - Select a file or directory */

int choose_file_dialog(char *heading, int chooser_action, gchar **nm, MainUi *m_ui)
{  
    GtkWidget *dialog;
    gint res;

    /* Selection */
    dialog = gtk_file_chooser_dialog_new (heading,
					  GTK_WINDOW (m_ui->window),
					  chooser_action,
					  "_Cancel", GTK_RESPONSE_CANCEL,
					  "_Apply", GTK_RESPONSE_APPLY,
					  NULL);

    res = gtk_dialog_run (GTK_DIALOG (dialog));

    if (res == GTK_RESPONSE_APPLY)
    {
	GtkFileChooser *chooser = GTK_FILE_CHOOSER (dialog);
	*nm = gtk_file_chooser_get_filename (chooser);
    }

    gtk_widget_destroy (dialog);

    return res;
}


/* Return the Home directory path */

char * home_dir(GtkWidget *window)
{
    if (Home)
    {
	return Home;
    }
    else
    {
    	if ((Home = getenv("HOME")) == NULL)
	{
	    app_msg("MSG9004", '\0', window);
	    return NULL;
	}
    }
}


/* Message lookup and optional string argument substitution */

void get_msg(char *s, char *msg_id, char *opt_str)
{
    int i;
    char *p, *p2;

    /* Find message */
    for(i = 0; i < Msg_Count; i++)
    {
    	if ((strcmp(msg_id, app_messages[i][0])) == 0)
	    break;
    }

    if (i >= Msg_Count)
    	i--;

    /* Check substitution. If none, show message as is with any '%s' blanked out. */
    p = (char *) app_messages[i][1];
    p2 = strstr(p, "%s");

    if ((! opt_str) || (strlen(opt_str) == 0) || (p2 == NULL))
    {
	sprintf(s, "(%s) %s", app_messages[i][0], app_messages[i][1]);

	if (p2 != NULL)
	{
	    p2 = strstr(s, "%s");
	    *p2++ = ' ';
	    *p2 = ' ';
	}

    	return;
    }

    /* Add substitution string */
    *s = '\0';
    sprintf(s, "(%s) ", app_messages[i][0]);

    for(s = (s + strlen(app_messages[i][0]) + 3); p < p2; p++)
    	*s++ = *p;

    *s = '\0';

    strcat(s, opt_str);
    strcat(s, p2 + 2);

    return;
}


/* Remove leading and trailing spaces from a string */

void string_trim(char *s)
{
    int i;
    char *p;

    /* Trailing */
    for(i = strlen(s) - 1; i >= 0; i--)
    {
	if (isspace(s[i]))
	    s[i] = '\0';
	else
	    break;
    }

    /* Empty - all spaces */
    if (*s == '\0')
    	return;

    /* Leading */
    p = s;

    while(isspace(*p))
    {
    	p++;
    }

    while(*p != '\0')
    {
    	*s++ = *p++;
    }

    *s = '\0';

    return;
}


/* Regiser the window as open */

void register_window(GtkWidget *window)
{
    open_ui_list = g_list_append (open_ui_list_head, window);

    if (open_ui_list_head == NULL)
    	open_ui_list_head = open_ui_list;

    return;
}


/* De-register the window as closed */

void deregister_window(GtkWidget *window)
{
    open_ui_list_head = g_list_remove (open_ui_list_head, window);

    return;
}


/* Check if a window title is registered (open) and present it to the user if reguired */

int is_ui_reg(char *s, int present)
{
    GtkWidget *window;
    const gchar *title;

    open_ui_list = g_list_first(open_ui_list_head);

    while(open_ui_list != NULL)
    {
	window = GTK_WIDGET (open_ui_list->data);
	title = gtk_window_get_title(GTK_WINDOW (window));

	if (strcmp(s, title) == 0)
	{
	    if (present == TRUE)
	    {
	    	gtk_window_present (GTK_WINDOW (window));
	    }

	    return TRUE;
	}

	open_ui_list = g_list_next(open_ui_list);
    }

    return FALSE;
}


/* Close any open windows */

void close_open_ui()
{
    GtkWidget *window;

    open_ui_list = g_list_first(open_ui_list_head);

    while(open_ui_list != NULL)
    {
	window = GTK_WIDGET (open_ui_list->data);
	gtk_window_close(GTK_WINDOW (window));
	open_ui_list = g_list_next(open_ui_list);
    }

    return;
}


/* Close a window */

int close_ui(char *s)
{
    GtkWidget *window;
    const gchar *title;

    open_ui_list = g_list_first(open_ui_list_head);

    while(open_ui_list != NULL)
    {
	window = GTK_WIDGET (open_ui_list->data);
	title = gtk_window_get_title(GTK_WINDOW (window));

	if (strcmp(s, title) == 0)
	{
	    gtk_window_close(GTK_WINDOW (window));
	    return TRUE;
	}

	open_ui_list = g_list_next(open_ui_list);
    }

    return FALSE;
}


/* Free the window register */

void free_window_reg()
{
    g_list_free (open_ui_list_head);

    return;
}


/* Convert a string to lowercase */

void strlower(char *s1, char *s2)
{
    for(; *s1 != '\0'; s1++, s2++)
    	*s2 = tolower(*s1);

    *s2 = *s1;

    return;
}


/* Return a date and time stamp */

void dttm_stamp(char *s, size_t max)
{
    size_t sz;
    struct tm *tm;
    time_t current_time;

    *s = '\0';
    current_time = time(NULL);
    tm = localtime(&current_time);
    sz = strftime(s, max, "%d%m%Y_%H%M%S", tm);

    return;
}


/* Check directory exists */

int check_dir(char *s)
{
    struct stat fileStat;
    int err;

    if ((err = stat(s, &fileStat)) < 0)
	return FALSE;

    if ((fileStat.st_mode & S_IFMT) == S_IFDIR)
	return TRUE;
    else
	return FALSE;
}


/* Create a directory */

int make_dir(char *s)
{
    int err;

    if ((err = mkdir(s, 0700)) != 0)
    {
	app_msg("MSG99008", s, NULL);
	return FALSE;
    }

    return TRUE;
}


/* Open a file */

FILE * open_file(char *fn, char *op)
{
    FILE *fd;

    if ((fd = fopen(fn, op)) == (FILE *) NULL)
    {
    	check_errno();
	return (FILE *) NULL;
    }

    return fd;
}


/* Read a file */

int read_file(FILE *fd, char *buf, int sz_len)
{
    int i, max;
    char c;

    i = 0;
    max = sz_len - 1;
    buf[0] = '\0';
    
    while((c = fgetc(fd)) != EOF)
    {
    	buf[i++] = c;

    	if (i >= max)
	    break;
    }

    buf[i] = '\0';

    if (c == EOF)
    {
	fclose(fd);
	return (int) c;
    }
    else
    {
	return i;
    }
}


/* Convert a string to a number and validate */

int val_str2numb(char *s, int *numb, char *subst, GtkWidget *window)
{
    int i;
    char *end;

    if (strlen(s) > 0)
    {
	errno = 0;
	i = strtol(s, &end, 10);

	if (errno != 0)
	{
	    app_msg("MSG0009", subst, window);
	    return FALSE;
	}
	else if (*end)
	{
	    app_msg("MSG0009", subst, window);
	    return FALSE;
	}
    }
    else
    {
    	i = 0;
    }

    *numb = i;

    return TRUE;
}


/* Check and print error message */

int check_errno(char *s)
{
    int err;

    if (errno != 0)
    {
	printf("%s %s - error: (%d) %s\n", debug_hdr, s, errno, strerror(errno));
	return errno;
    }

    return 0;
}


/* Show binary representation of value (useful debug) */

void print_bits(size_t const size, void const * const ptr)
{
    unsigned char *b = (unsigned char*) ptr;
    unsigned char byte;
    int i, j;

    for(i = size - 1; i >= 0; i--)
    {
        for(j = 7; j >= 0; j--)
        {
            byte = b[i] & (1<<j);
            byte >>= j;
            printf("%u", byte);
        }
    }

    printf("\n");
    fflush(stdout);
}


/* Return the parent of a widget */

GtkWidget * find_parent(GtkWidget *init_widget)
{
    GtkWidget *parent_contnr;

    parent_contnr = gtk_widget_get_parent(init_widget);

    if (! GTK_IS_CONTAINER(parent_contnr))
    {
	app_msg("MSG9006", "From initial widget", NULL);
    	return NULL;
    }

    return parent_contnr;
}


/* Search for a child widget using the widget name */

GtkWidget * find_widget_by_name(GtkWidget *parent_contnr, char *nm)
{
    GtkWidget *widget;
    const gchar *widget_name;

    if (! GTK_IS_CONTAINER(parent_contnr))
    {
	app_msg("MSG9006", "By name", NULL);
    	return NULL;
    }

    GList *child_widgets = gtk_container_get_children(GTK_CONTAINER (parent_contnr));

    child_widgets = g_list_first(child_widgets);

    while (child_widgets != NULL)
    {
	widget = child_widgets->data;
	widget_name = gtk_widget_get_name (widget);

	if (widget_name != NULL)
	{
	    if (strcmp(widget_name, nm) == 0)
	    {
		g_list_free (child_widgets);
		return widget;
	    }
	}

	child_widgets = g_list_next(child_widgets);
    }

    g_list_free (child_widgets);

    return NULL;
}


/* Search for a widget using the parent of an initiating widget */

GtkWidget * find_widget_by_parent(GtkWidget *init_widget, char *nm)
{
    GtkWidget *widget;
    GtkWidget *parent_contnr;
    const gchar *widget_name;

    parent_contnr = gtk_widget_get_parent(init_widget);

    if (! GTK_IS_CONTAINER(parent_contnr))
    {
	app_msg("MSG9006", "By parent", NULL);
    	return NULL;
    }

    GList *child_widgets = gtk_container_get_children(GTK_CONTAINER (parent_contnr));

    child_widgets = g_list_first(child_widgets);

    while (child_widgets != NULL)
    {
	widget = child_widgets->data;
	widget_name = gtk_widget_get_name (widget);

	if (strcmp(widget_name, nm) == 0)
	{
	    g_list_free (child_widgets);
	    return widget;
	}

	child_widgets = g_list_next(child_widgets);
    }

    g_list_free (child_widgets);

    return NULL;
}


/* Find all the control widgets in a container */

GList * ctrl_widget_list(GtkWidget *contr, GtkWidget *window)
{
    GtkWidget *widget;
    GList *ctl_list = NULL;
    GList *tmp_list = NULL;

    if (! GTK_IS_CONTAINER(contr))
    {
	app_msg("MSG9006", "Get Next Control", window);
	return NULL;
    }

    GList *child_widgets = gtk_container_get_children(GTK_CONTAINER (contr));
    child_widgets = g_list_first(child_widgets);

    while (child_widgets != NULL)
    {
	widget = child_widgets->data;

	if ((GTK_IS_RANGE (widget)) || (GTK_IS_COMBO_BOX_TEXT (widget)) || (GTK_IS_RADIO_BUTTON (widget)))
	{
	    ctl_list = g_list_prepend(ctl_list, widget);
	}
	else if (GTK_IS_CONTAINER (widget))
	{
	    tmp_list = ctrl_widget_list(widget, window);
	    ctl_list = g_list_concat(ctl_list, tmp_list);
	}

	child_widgets = g_list_next(child_widgets);
    }

    return ctl_list;
}


/* Delete items in a menu by name */

void delete_menu_items(GtkWidget *menu, char *nm)
{
    int len;
    GtkWidget *widget;
    const gchar *s;

    len = strlen(nm);

    GList *child_widgets = gtk_container_get_children(GTK_CONTAINER (menu));
    child_widgets = g_list_first(child_widgets);

    while (child_widgets != NULL)
    {
	widget = child_widgets->data;
	s = gtk_widget_get_name(widget);

	if (strncmp(s, nm, len) == 0)
	    gtk_widget_destroy(widget);

	child_widgets = g_list_next(child_widgets);
    }

    g_list_free(child_widgets);

    return;
}
