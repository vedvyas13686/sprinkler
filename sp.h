// ---------------------------------------------------------------------------
// This software is furnished "as is", without technical
// support, and with no warranty, express or implied, as to its usefulness for
// any purpose.
//
// sp.h
//     Sprinker controller header file
//
// Author: Vedvyas Shanbhogue
// ---------------------------------------------------------------------------
#ifndef _sp_h
#define _sp_h

#ifdef IDENT_H
static const char* const <filename>_H_Id = "$Id$";
#endif

// include directives
#include <Python.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h> 
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>

#define SPRINKLER_SERVER_PORT 0x4912

// declarations
int get_calendar_events(time_t *duration);
int parse_options(int argc, char **argv);
void make_daemon(void (*f)());
extern int daemonize;
extern char *pythonpath;

// inline functions

#endif
