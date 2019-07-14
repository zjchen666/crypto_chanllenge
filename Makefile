TARGET       := base64
CC           = g++
LD           = g++
AR           = ar
RM           = rm
ARFLAGS      = rcs
CPPFLAGS     = -g -Wall -Wl,-Map=$(TARGET).map

OBJCOPYFLAGS = -j .text -O ihex
OBJCOPY      = objcopy

default: base64
.PHONY:  all clean

all : $(TARGET) 

base64 : base64.cpp base64.elf
	echo [CC] $@ $(CPPFLAGS)
	$(CC) $(CPPFLAGS) -o $@ $<

base64.dump: base64.elf
	$(OBJDUMP) $^ -S > $@

base64.elf : base64.o
	echo [LD] $@
	$(LD) $(LDFLAGS) -o $@ $^

base64.a : base64.o
	echo [AR] $@
	$(AR) $(ARFLAGS) $@ $^

lib : base64.a

clean:
	$(RM) -f $(TARGET) $(TARGET).elf $(TARGET).o $(TARGET).dump $(TARGET).map
