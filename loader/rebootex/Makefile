PSPSDK=$(shell psp-config --pspsdk-path)
INCDIR = $(PSPSDK)/include $(ARKROOT)/common/include $(ARKROOT)/kxploit/
TARGET = reboot.bin.gz

PYTHON=$(shell which python2)

all: $(TARGET)

CFLAGS = -Wall -Os -G0 -fno-pic $(addprefix -I, $(INCDIR))

ifdef DEBUG
CFLAGS += -DDEBUG=$(DEBUG)
endif

CC = psp-gcc
LD = psp-ld
STRIP = psp-strip
OBJCOPY = psp-objcopy
LINKFILE = linkfile.l

C_OBJS = main.o \
	pspbtcnf.o \
	psp_patches.o \
	psv_patches.o \
	$(ARKROOT)/common/functions.o \

LIBS = -L $(ARKROOT)/libs -lansic -lcolordebugger

main.elf: $(C_OBJS)

$(TARGET): main.elf
	$(Q)$(STRIP) -s $<
	$(Q)$(OBJCOPY) -O binary $< $(patsubst %.gz,%,$(TARGET))
	$(Q)$(PYTHON) $(ARKROOT)/contrib/PC/gz/gz.py $(patsubst %.gz,%,$(TARGET)) $@
	$(Q)bin2c $@ payload.h rebootbuffer
	@echo GET $@

clean:
	$(Q)rm -rf *~ *.s *.o *.elf reboot.bin reboot.bin.gz payload.h $(EXTRA_CLEAN)

include $(ARKROOT)/common/make/global.mak
include $(ARKROOT)/common/make/beauty_bin.mak
