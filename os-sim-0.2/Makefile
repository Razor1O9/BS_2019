#
# Makefile for ECO32 simulator
# OS course version
#

VERSION = 0.2

DIRS = doc sim
BUILD = `pwd`/../build

.PHONY:		all clean dist

all:
		for i in $(DIRS) ; do \
		  $(MAKE) -C $$i install ; \
		done

clean:
		for i in $(DIRS) ; do \
		  $(MAKE) -C $$i clean ; \
		done
		rm -f *~

dist:		clean
		(cd .. ; \
		 tar -cvf os-sim-$(VERSION).tar os-sim-$(VERSION)/* ; \
		 gzip -f os-sim-$(VERSION).tar)
