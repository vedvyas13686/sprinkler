// -------------------------------------------------------------------------
// This software is furnished "as is", without technical
// support, and with no warranty, express or implied, as to its usefulness for
// any purpose.
//
// sp.c 
//    main file for the sprinker controller
//
// Author: Vedvyas Shanbhogue
// -------------------------------------------------------------------------

#ifdef IDENT_C
static const char* const <filename>_c_Id = "$Id$";
#endif

#include <Python.h>
#include "sp.h"
int daemonize = 0;
char *pythonpath;

// -- fileScope -------------------------------------------------------------
// main_loop()
//     This is where the magic happens
// Returns:
//     None
//     
// 
// -------------------------------------------------------------------------*/
void
main_loop(
    void)
{
    time_t duration;
    // Read events from the google calendar
    if ( !get_calendar_events(&duration) ) {
        if ( duration > 0 ) {
            printf("Start sprinkler for %ld\n", duration);
        }
    }
    while(1);
}
int 
main(
    int argc, char **argv)
{

    // Parse command line options
    if ( parse_options(argc, argv) ) {
        return 0;
    }

    // Daemonize if required
    make_daemon(main_loop);

    return 0;
}
