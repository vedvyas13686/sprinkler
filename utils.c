// -------------------------------------------------------------------------
// This software is furnished "as is", without technical
// support, and with no warranty, express or implied, as to its usefulness for
// any purpose.
//
// utils.c 
//    This file holds utility functions
//
// Author: Vedvyas Shanbhogue
// ------------------------------------------------------------------------*/

#ifdef IDENT_C
static const char* const <filename>_c_Id = "$Id$";
#endif

#include <Python.h>
#include "sp.h"

// -- global -----------------------------------------------------------------
// parse_options()
//
// Returns:
//     0 - success 
//     1 - error
// 
// -------------------------------------------------------------------------*/
int
parse_options(
    int argc, char **argv)
{
    char c;
    while ( (c = getopt(argc, argv, "dhp:") ) != -1 ) {
        switch ( c ) {
            case 'd':
                daemonize = 1;
                break;
          
            case 'p':
                pythonpath = strdup(optarg);
                break;

            case '?':
                if ( optopt == 'p' ) {
                    fprintf (stderr, "Option -%c requires an argument.\n", optopt);
                } else if ( isprint (optopt) ) {
                    fprintf (stderr, "Unknown option `-%c'.\n", optopt);
                } else {
                    fprintf (stderr, "Unknown option character `\\x%x'.\n", optopt);
                }
                return 1;
             
            case 'h':
                printf("sp [OPTIONS]\n");
                printf("Sprinker controller program\n");
                printf("   -d, daemonize the program\n");
                printf("   -p, path of the python libraries\n");
                printf("Report bugs to vedvyas13686@gmail.com\n");

            default:
                return 1;
        }
    }
    return 0;
}
// -- global -----------------------------------------------------------------
// make_daemon()
//    If program was asked to be daemonized then makes it a daemon
//
// Returns:
//     None
// 
// -------------------------------------------------------------------------*/
void
make_daemon(
    void (*work_fn)(void))
{
    // Our process ID and Session ID
    pid_t pid, sid;
    
    if ( daemonize == 0 ) {
        work_fn();
    }

    // Fork off the parent process
    pid = fork();
    if ( pid < 0 ) {
        exit(EXIT_FAILURE);
    }
    // If we got a good PID, then
    // we can exit the parent process.
    if ( pid > 0 ) {
        exit(EXIT_SUCCESS);
    }

    // Change the file mode mask
    umask(0);
                
    // Create a new SID for the child process
    sid = setsid();
    if ( sid < 0 ) {
        // Log the failure
        exit(EXIT_FAILURE);
    }

    // Change the current working directory
    if ( chdir("/") < 0 ) {
        exit(EXIT_FAILURE);
    }
        
    // Close out the standard file descriptors
    //close(STDIN_FILENO);
    //close(STDOUT_FILENO);
    //close(STDERR_FILENO);

    if ( setenv("PYTHONPATH", pythonpath, 1) ) {
        exit(EXIT_FAILURE);
    }

    // Invoke the main worker loop
    work_fn();

    exit(EXIT_SUCCESS);
}
