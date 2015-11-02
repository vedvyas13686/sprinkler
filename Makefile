#* -------------------------------------------------------------------------
#* This software is furnished "as is", without technical
#* support, and with no warranty, express or implied, as to its usefulness for
#* any purpose.
#*
#* Makefile
#*
#* Author: Vedvyas Shanbhogue
#* -------------------------------------------------------------------------
CC = gcc
CFLAGS = -Wall `mysql_config --cflags`
DEBUG_CFLAGS = -Wall -g `mysql_config --cflags`
INCLUDES = -I/usr/include/python2.7/ -I/usr/include/mysql/
LFLAGS =
LIBS = -lpython2.7 -lfcgi `mysql_config --libs`
SRCSSP = sp.c gcal.c utils.c speck.c
OBJSSP = $(SRCSSP:.c=.o)
MAIN = sp
SRCSFC = sprinkler-fcgi.c
OBJSFC = $(SRCSFC:.c=.o)
FCGI = spfcgi

.PHONY: depend clean all debug
all: CFLAGS += -O3
all: $(MAIN) $(FCGI)
debug: CFLAGS += -g
debug: $(MAIN)

$(MAIN):$(OBJSSP) 
	@echo Linking"   ": $(MAIN)
	@$(CC) $(CFLAGS) $(INCLUDES) -o $(MAIN) $(OBJSSP) $(LFLAGS) $(LIBS)

$(FCGI):$(OBJSFC) 
	@echo Linking"   ": $(FCGI)
	@$(CC) $(CFLAGS) $(INCLUDES) -o $(FCGI) $(OBJSFC) $(LFLAGS) $(LIBS)

.c.o:
	@echo Building"  ":  $<
	@$(CC) $(CFLAGS) $(INCLUDES) -c $<  -o $@

clean:
	$(RM) *.o *.pyc *~ $(MAIN)

