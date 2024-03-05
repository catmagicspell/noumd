TARGET = noumd
OBJS = main.o imports.o

INCDIR = ./inc
LIBDIR = ./lib

USE_PSPSDK_LIBC = 1
USE_PSPSDK_LIBS = 1
CFLAGS = -Ofast -w
LIBS = -lpspsystemctrl_user -lpspsystemctrl_kernel
PRX_EXPORTS = exports.exp

PSPSDK = $(shell psp-config --pspsdk-path)
include $(PSPSDK)/lib/build_prx.mak