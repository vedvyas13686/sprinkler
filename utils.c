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
    char c, print_help = 0;
    pythonpath = NULL;
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
                print_help = 1;
                break;
             
            case 'h':
                print_help = 1;
                break;

            default:
                print_help = 1;
        }
    }
    if ( pythonpath != NULL ) {
        // Get file status
        struct stat sb;
        if ( stat(pythonpath, &sb) < 0 ) {
            print_help = 1;
        }
        // Check if it is a directory
        if ( (sb.st_mode & S_IFMT) != S_IFDIR ) {
            print_help = 1;
        }
    } else {
        print_help = 1;
    }
    if ( print_help ) {
        printf("sp [OPTIONS]\n");
        printf("Sprinker controller program\n");
        printf("   -d, daemonize the program\n");
        printf("   -p, (mandatory) path of the python libraries and config files\n");
        printf("Report bugs to vedvyas13686@gmail.com\n");
    }
    return print_help;
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
    close(STDIN_FILENO);
    close(STDOUT_FILENO);
    close(STDERR_FILENO);

    if ( setenv("PYTHONPATH", pythonpath, 1) ) {
        exit(EXIT_FAILURE);
    }

    // Invoke the main worker loop
    work_fn();

    exit(EXIT_SUCCESS);
}
// -- global -----------------------------------------------------------------
// read_shared_secret()
//    read the shared secret of the attached sprinkler
//
// Returns:
//     0, success
//     1, failed
// -------------------------------------------------------------------------*/
int
read_shared_secret(
	void)
{
    FILE *fp;
    char *file_name;
    int sz;

    // Build the file name to read
    if ( (file_name = malloc(strlen(pythonpath)+strlen("/shared_secret.txt"))) == NULL ) {
        return 1;
    }
    strcpy(file_name, pythonpath);
    strcat(file_name, "/shared_secret.txt");
    // Open the file
    if ( (fp = fopen(file_name, "r")) == NULL ) {
        free(file_name); 
        return 1;
    }
    free(file_name); 
    // Find the size of the file
    if ( fseek(fp, 0L, SEEK_END) < 0 ) {
        fclose(fp);
        return 1;
    }
    // Determine the number of bytes in the file
    if ( (sz = ftell(fp)) < 0 ) {
        fclose(fp);
        return 1;
    }
    // Allocate memory for the request message string
    if ( (request_message = malloc(sz)) == NULL ) {
        fclose(fp);
        return 1;
    }
    // Rewind and read the string from file into the buffer
    rewind(fp);
    if ( fread(request_message, 1, sz, fp) != sz ) {
        fclose(fp);
        return 1;
    }
    fclose(fp);
    return 0;
}
