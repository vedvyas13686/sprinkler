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
#include <stdint.h>

#define SPRINKLER_SERVER_PORT 0x4912

// declarations
int get_calendar_events(time_t *duration);
int parse_options(int argc, char **argv);
int read_shared_secret(void);
void make_daemon(void (*f)());
void speck_key_expand_128x128(uint64_t *key);
void encrypt_speck_128x128(uint64_t *plain_text, uint64_t *cipher_text, uint64_t *key);
void decrypt_speck_128x128(uint64_t *cipher_text, uint64_t *plain_text, uint64_t *key);

extern int daemonize;
extern char *pythonpath;
extern char *request_message;


// inline functions

#endif
