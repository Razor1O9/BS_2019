BUILD = ../build

all:		ex0-1.bin
		mkdir -p $(BUILD)/run
		cp ex0-1.bin $(BUILD)/run

ex0-1.bin:	start.o main.o
		$(BUILD)/bin/ld -h -rc 0xC0000000 \
		  -o ex0-1.bin start.o main.o

start.o:	start.s
		$(BUILD)/bin/as -o start.o start.s

main.o:		main.s
		$(BUILD)/bin/as -o main.o main.s

main.s:		main.c
		$(BUILD)/bin/lcc -S -o main.s main.c

clean:
		rm -f *~ main.s main.o start.o ex0-1.bin

dist:		clean
		(cd .. ; \
		 tar -cvf ex0-1.tar ex0-1/* ; \
		 gzip -f ex0-1.tar)
