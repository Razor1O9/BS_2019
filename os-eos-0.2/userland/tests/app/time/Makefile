#
# Makefile for the 'time' command
#

BUILD = ../../../../../build

all:		time

install:	time
		mkdir -p $(BUILD)/tests/app
		cp time $(BUILD)/tests/app

hello:		time.c
		$(BUILD)/bin/lcc -A -o time time.c

clean:
		rm -f *~ time
