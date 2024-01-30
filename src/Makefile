TARGET = noumd
OBJS = main.o imports.o

INCDIR = ./include
LIBDIR = ./lib
LIBS = -lpspsystemctrl_user -lpspsystemctrl_kernel
CFLAGS = -Ofast -w

USE_PSPSDK_LIBS = 1
USE_PSPSDK_LIBC = 1
PRX_EXPORTS = exports.exp

PSPSDK = $(shell psp-config --pspsdk-path)
include $(PSPSDK)/lib/build_prx.mak