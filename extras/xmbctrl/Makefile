TARGET = xmbctrl
OBJS = main.o utils.o stub.o exports.o

CFLAGS = -O2 -Os -G0 -Wall -fshort-wchar -fno-pic -mno-check-zero-division -std=c99
CXXFLAGS = $(CFLAGS) -fno-exceptions -fno-rtti
ASFLAGS = $(CFLAGS)

BUILD_PRX = 1
PRX_EXPORTS = exports.exp

INCDIR = include/
LIBDIR = lib/

all:
	psp-packer xmbctrl.prx

USE_PSPSDK_LIBS = 1
USE_PSPSDK_LIBC = 1

LIBS = -lpspsystemctrl_user -lpspkubridge -lpspvshctrl -lpspsystemctrl_user -lpspkubridge -L./lib

PSPSDK = $(shell psp-config --pspsdk-path)
include $(PSPSDK)/lib/build_prx.mak
