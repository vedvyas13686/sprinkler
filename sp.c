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

#include "sp.h"
int daemonize = 0;
char *pythonpath, *request_message;

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
    int listen_fd, connection_fd, msg_len;
    struct sockaddr_in addr, client_addr;
    socklen_t client_addr_len = sizeof(client_addr);
    char message[strlen(request_message)];

    // Create socket to listen for connections from the sprinkler
    if ( (listen_fd = socket(AF_INET, SOCK_STREAM, 0)) < 0 ) {
        exit(EXIT_FAILURE);
    }

    // Create server side address
    bzero(&addr, sizeof(addr));
    addr.sin_family = AF_INET;
    addr.sin_addr.s_addr=htonl(INADDR_ANY);
    addr.sin_port=htons(SPRINKLER_SERVER_PORT);

    // Bind address to socket and start listening for connections
    if ( bind(listen_fd, (struct sockaddr *)&addr, sizeof(addr)) < 0 ) {
        exit(EXIT_FAILURE);
    } 

    // listen for connections from the sprinkler. One connection expected
    if ( listen(listen_fd, 1) ) {
        exit(EXIT_FAILURE);
    }

    // Wait for sprinker connections forever
    while (1) {
        bzero(message, strlen(request_message));
        if ( (connection_fd = accept(listen_fd, (struct sockaddr *)&client_addr, &client_addr_len)) < 0 ) {
            close(connection_fd);
            continue;
        }
        if ( (msg_len = read(connection_fd, message, strlen(request_message))) < 0 ) {
            close(connection_fd);
            continue;
        }
        if ( ( msg_len == strlen(request_message) ) && 
             ( strncmp(message, request_message, strlen(request_message)) == 0 ) ) {
            time_t duration;
            // Read events from the google calendar
            if ( !get_calendar_events(&duration) ) {
                if ( duration > 0 ) {
                    printf("Start sprinkler for %ld\n", duration);
                }
                write(connection_fd, &duration, sizeof(duration));
            }
        }
        close(connection_fd);
    }
}
int 
main(
    int argc, char **argv)
{

    // Parse command line options
    if ( parse_options(argc, argv) ) {
        exit(EXIT_FAILURE);
    }
    if ( read_shared_secret() ) {
        exit(EXIT_FAILURE);
    }

    // Daemonize if required
    make_daemon(main_loop);

    return 0;
}
