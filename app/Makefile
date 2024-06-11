TARGET = noumd
SRCDIR = ./source
INCDIR = ./include
LIBDIR = ./library

CFILES := $(wildcard $(SRCDIR)/*.c)
SFILES := $(wildcard $(SRCDIR)/*.S)
OBJS := $(CFILES:.c=.o) $(SFILES:.S=.o)

CFLAGS = -Ofast -w
LIBS = -lpspsystemctrl_kernel
USE_PSPSDK_LIBC = 1
USE_PSPSDK_LIBS = 1

PSPSDK = $(shell psp-config --pspsdk-path)
include $(PSPSDK)/lib/build_prx.mak