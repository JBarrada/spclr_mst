CFLAGS=-std=gnu99 -ffreestanding -fbuiltin -Wall -Wextra -g -fdiagnostics-color
LIBS=-Iinclude -Iui/include -Isf/include -nostdlib

AS=/home/swdev/cross-compile/i686-elf-as
CC=/home/swdev/cross-compile/usr/bin/i686-elf-gcc

PROJDIR=machine ui sf

SRCFILES:= $(shell find $(PROJDIRS) -type f -name "*.c")
OBJFILES:= $(patsubst %.c,%.o,$(SRCFILES)) interrupt_handler.o boot.o machine/memcpy.o machine/memset.o

all: clean fancy.iso
	
%.o: %.c
	$(CC) -c $(CFLAGS) $(LIBS) $< -o $@
	
%.o: %.s
	$(AS) -c $< -o $@

%.o: %.S
	$(CC) -c $< -o $@
	
fancy.bin: $(OBJFILES)
	$(LD) -T linker.ld -o $@ $(OBJFILES) $(LIBS)
	
	
fancy.iso: grub.cfg fancy.bin
	mkdir -p isodir
	mkdir -p isodir/boot
	cp fancy.bin isodir/boot/fancy.bin
	mkdir -p isodir/boot/grub
	cp grub.cfg isodir/boot/grub/grub.cfg
	grub-mkrescue -o $@ isodir
	rm -R isodir
	
clean:
	rm -f *.o *.iso fancy.bin $(OBJFILES)
