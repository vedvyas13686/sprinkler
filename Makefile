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
CFLAGS = -Wall
DEBUG_CFLAGS = -Wall -g
INCLUDES = -I/usr/include/python2.7/
LFLAGS =
LIBS = -lpython2.7 
SRCS = sp.c gcal.c utils.c speck.c
OBJS = $(SRCS:.c=.o)
MAIN = sp

.PHONY: depend clean all debug
all: CFLAGS += -O3
all: $(MAIN)
debug: CFLAGS += -g
debug: $(MAIN)

$(MAIN):$(OBJS) 
	@echo Linking"   ": $(MAIN)
	@$(CC) $(CFLAGS) $(INCLUDES) -o $(MAIN) $(OBJS) $(LFLAGS) $(LIBS)

.c.o:
	@echo Building"  ":  $<
	@$(CC) $(CFLAGS) $(INCLUDES) -c $<  -o $@

clean:
	$(RM) *.o *.pyc *~ $(MAIN)

depend: $(SRCS)
	makedepend $(INCLUDES) $^
# DO NOT DELETE

sp.o: /usr/include/python2.7/Python.h /usr/include/python2.7/patchlevel.h
sp.o: /usr/include/python2.7/pyconfig.h /usr/include/python2.7/pymacconfig.h
sp.o: /usr/include/limits.h /usr/include/features.h /usr/include/stdio.h
sp.o: /usr/include/libio.h /usr/include/_G_config.h /usr/include/wchar.h
sp.o: /usr/include/string.h /usr/include/xlocale.h /usr/include/errno.h
sp.o: /usr/include/stdlib.h /usr/include/alloca.h /usr/include/unistd.h
sp.o: /usr/include/getopt.h /usr/include/assert.h
sp.o: /usr/include/python2.7/pyport.h /usr/include/inttypes.h
sp.o: /usr/include/stdint.h /usr/include/math.h /usr/include/time.h
sp.o: /usr/include/python2.7/pymath.h /usr/include/python2.7/pymem.h
sp.o: /usr/include/python2.7/object.h /usr/include/python2.7/objimpl.h
sp.o: /usr/include/python2.7/pydebug.h /usr/include/python2.7/unicodeobject.h
sp.o: /usr/include/ctype.h /usr/include/endian.h
sp.o: /usr/include/python2.7/intobject.h /usr/include/python2.7/boolobject.h
sp.o: /usr/include/python2.7/longobject.h
sp.o: /usr/include/python2.7/floatobject.h
sp.o: /usr/include/python2.7/complexobject.h
sp.o: /usr/include/python2.7/rangeobject.h
sp.o: /usr/include/python2.7/stringobject.h
sp.o: /usr/include/python2.7/memoryobject.h
sp.o: /usr/include/python2.7/bufferobject.h
sp.o: /usr/include/python2.7/bytesobject.h
sp.o: /usr/include/python2.7/bytearrayobject.h
sp.o: /usr/include/python2.7/tupleobject.h
sp.o: /usr/include/python2.7/listobject.h /usr/include/python2.7/dictobject.h
sp.o: /usr/include/python2.7/enumobject.h /usr/include/python2.7/setobject.h
sp.o: /usr/include/python2.7/methodobject.h
sp.o: /usr/include/python2.7/moduleobject.h
sp.o: /usr/include/python2.7/funcobject.h
sp.o: /usr/include/python2.7/classobject.h
sp.o: /usr/include/python2.7/fileobject.h /usr/include/python2.7/cobject.h
sp.o: /usr/include/python2.7/pycapsule.h /usr/include/python2.7/traceback.h
sp.o: /usr/include/python2.7/sliceobject.h
sp.o: /usr/include/python2.7/cellobject.h /usr/include/python2.7/iterobject.h
sp.o: /usr/include/python2.7/genobject.h /usr/include/python2.7/descrobject.h
sp.o: /usr/include/python2.7/warnings.h
sp.o: /usr/include/python2.7/weakrefobject.h /usr/include/python2.7/codecs.h
sp.o: /usr/include/python2.7/pyerrors.h /usr/include/python2.7/pystate.h
sp.o: /usr/include/python2.7/pyarena.h /usr/include/python2.7/modsupport.h
sp.o: /usr/include/python2.7/pythonrun.h /usr/include/python2.7/ceval.h
sp.o: /usr/include/python2.7/sysmodule.h /usr/include/python2.7/intrcheck.h
sp.o: /usr/include/python2.7/import.h /usr/include/python2.7/abstract.h
sp.o: /usr/include/python2.7/compile.h /usr/include/python2.7/code.h
sp.o: /usr/include/python2.7/eval.h /usr/include/python2.7/pyctype.h
sp.o: /usr/include/python2.7/pystrtod.h /usr/include/python2.7/pystrcmp.h
sp.o: /usr/include/python2.7/dtoa.h /usr/include/python2.7/pyfpe.h
sp.o: /usr/include/signal.h /usr/include/setjmp.h sp.h
gcal.o: /usr/include/python2.7/Python.h /usr/include/python2.7/patchlevel.h
gcal.o: /usr/include/python2.7/pyconfig.h
gcal.o: /usr/include/python2.7/pymacconfig.h /usr/include/limits.h
gcal.o: /usr/include/features.h /usr/include/stdio.h /usr/include/libio.h
gcal.o: /usr/include/_G_config.h /usr/include/wchar.h /usr/include/string.h
gcal.o: /usr/include/xlocale.h /usr/include/errno.h /usr/include/stdlib.h
gcal.o: /usr/include/alloca.h /usr/include/unistd.h /usr/include/getopt.h
gcal.o: /usr/include/assert.h /usr/include/python2.7/pyport.h
gcal.o: /usr/include/inttypes.h /usr/include/stdint.h /usr/include/math.h
gcal.o: /usr/include/time.h /usr/include/python2.7/pymath.h
gcal.o: /usr/include/python2.7/pymem.h /usr/include/python2.7/object.h
gcal.o: /usr/include/python2.7/objimpl.h /usr/include/python2.7/pydebug.h
gcal.o: /usr/include/python2.7/unicodeobject.h /usr/include/ctype.h
gcal.o: /usr/include/endian.h /usr/include/python2.7/intobject.h
gcal.o: /usr/include/python2.7/boolobject.h
gcal.o: /usr/include/python2.7/longobject.h
gcal.o: /usr/include/python2.7/floatobject.h
gcal.o: /usr/include/python2.7/complexobject.h
gcal.o: /usr/include/python2.7/rangeobject.h
gcal.o: /usr/include/python2.7/stringobject.h
gcal.o: /usr/include/python2.7/memoryobject.h
gcal.o: /usr/include/python2.7/bufferobject.h
gcal.o: /usr/include/python2.7/bytesobject.h
gcal.o: /usr/include/python2.7/bytearrayobject.h
gcal.o: /usr/include/python2.7/tupleobject.h
gcal.o: /usr/include/python2.7/listobject.h
gcal.o: /usr/include/python2.7/dictobject.h
gcal.o: /usr/include/python2.7/enumobject.h
gcal.o: /usr/include/python2.7/setobject.h
gcal.o: /usr/include/python2.7/methodobject.h
gcal.o: /usr/include/python2.7/moduleobject.h
gcal.o: /usr/include/python2.7/funcobject.h
gcal.o: /usr/include/python2.7/classobject.h
gcal.o: /usr/include/python2.7/fileobject.h /usr/include/python2.7/cobject.h
gcal.o: /usr/include/python2.7/pycapsule.h /usr/include/python2.7/traceback.h
gcal.o: /usr/include/python2.7/sliceobject.h
gcal.o: /usr/include/python2.7/cellobject.h
gcal.o: /usr/include/python2.7/iterobject.h
gcal.o: /usr/include/python2.7/genobject.h
gcal.o: /usr/include/python2.7/descrobject.h
gcal.o: /usr/include/python2.7/warnings.h
gcal.o: /usr/include/python2.7/weakrefobject.h
gcal.o: /usr/include/python2.7/codecs.h /usr/include/python2.7/pyerrors.h
gcal.o: /usr/include/python2.7/pystate.h /usr/include/python2.7/pyarena.h
gcal.o: /usr/include/python2.7/modsupport.h
gcal.o: /usr/include/python2.7/pythonrun.h /usr/include/python2.7/ceval.h
gcal.o: /usr/include/python2.7/sysmodule.h /usr/include/python2.7/intrcheck.h
gcal.o: /usr/include/python2.7/import.h /usr/include/python2.7/abstract.h
gcal.o: /usr/include/python2.7/compile.h /usr/include/python2.7/code.h
gcal.o: /usr/include/python2.7/eval.h /usr/include/python2.7/pyctype.h
gcal.o: /usr/include/python2.7/pystrtod.h /usr/include/python2.7/pystrcmp.h
gcal.o: /usr/include/python2.7/dtoa.h /usr/include/python2.7/pyfpe.h
gcal.o: /usr/include/signal.h /usr/include/setjmp.h
utils.o: /usr/include/python2.7/Python.h /usr/include/python2.7/patchlevel.h
utils.o: /usr/include/python2.7/pyconfig.h
utils.o: /usr/include/python2.7/pymacconfig.h /usr/include/limits.h
utils.o: /usr/include/features.h /usr/include/stdio.h /usr/include/libio.h
utils.o: /usr/include/_G_config.h /usr/include/wchar.h /usr/include/string.h
utils.o: /usr/include/xlocale.h /usr/include/errno.h /usr/include/stdlib.h
utils.o: /usr/include/alloca.h /usr/include/unistd.h /usr/include/getopt.h
utils.o: /usr/include/assert.h /usr/include/python2.7/pyport.h
utils.o: /usr/include/inttypes.h /usr/include/stdint.h /usr/include/math.h
utils.o: /usr/include/time.h /usr/include/python2.7/pymath.h
utils.o: /usr/include/python2.7/pymem.h /usr/include/python2.7/object.h
utils.o: /usr/include/python2.7/objimpl.h /usr/include/python2.7/pydebug.h
utils.o: /usr/include/python2.7/unicodeobject.h /usr/include/ctype.h
utils.o: /usr/include/endian.h /usr/include/python2.7/intobject.h
utils.o: /usr/include/python2.7/boolobject.h
utils.o: /usr/include/python2.7/longobject.h
utils.o: /usr/include/python2.7/floatobject.h
utils.o: /usr/include/python2.7/complexobject.h
utils.o: /usr/include/python2.7/rangeobject.h
utils.o: /usr/include/python2.7/stringobject.h
utils.o: /usr/include/python2.7/memoryobject.h
utils.o: /usr/include/python2.7/bufferobject.h
utils.o: /usr/include/python2.7/bytesobject.h
utils.o: /usr/include/python2.7/bytearrayobject.h
utils.o: /usr/include/python2.7/tupleobject.h
utils.o: /usr/include/python2.7/listobject.h
utils.o: /usr/include/python2.7/dictobject.h
utils.o: /usr/include/python2.7/enumobject.h
utils.o: /usr/include/python2.7/setobject.h
utils.o: /usr/include/python2.7/methodobject.h
utils.o: /usr/include/python2.7/moduleobject.h
utils.o: /usr/include/python2.7/funcobject.h
utils.o: /usr/include/python2.7/classobject.h
utils.o: /usr/include/python2.7/fileobject.h /usr/include/python2.7/cobject.h
utils.o: /usr/include/python2.7/pycapsule.h
utils.o: /usr/include/python2.7/traceback.h
utils.o: /usr/include/python2.7/sliceobject.h
utils.o: /usr/include/python2.7/cellobject.h
utils.o: /usr/include/python2.7/iterobject.h
utils.o: /usr/include/python2.7/genobject.h
utils.o: /usr/include/python2.7/descrobject.h
utils.o: /usr/include/python2.7/warnings.h
utils.o: /usr/include/python2.7/weakrefobject.h
utils.o: /usr/include/python2.7/codecs.h /usr/include/python2.7/pyerrors.h
utils.o: /usr/include/python2.7/pystate.h /usr/include/python2.7/pyarena.h
utils.o: /usr/include/python2.7/modsupport.h
utils.o: /usr/include/python2.7/pythonrun.h /usr/include/python2.7/ceval.h
utils.o: /usr/include/python2.7/sysmodule.h
utils.o: /usr/include/python2.7/intrcheck.h /usr/include/python2.7/import.h
utils.o: /usr/include/python2.7/abstract.h /usr/include/python2.7/compile.h
utils.o: /usr/include/python2.7/code.h /usr/include/python2.7/eval.h
utils.o: /usr/include/python2.7/pyctype.h /usr/include/python2.7/pystrtod.h
utils.o: /usr/include/python2.7/pystrcmp.h /usr/include/python2.7/dtoa.h
utils.o: /usr/include/python2.7/pyfpe.h /usr/include/signal.h
utils.o: /usr/include/setjmp.h sp.h
