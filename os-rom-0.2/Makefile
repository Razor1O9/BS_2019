#
# Makefile for ECO32 ROM monitor
# OS course version
#

VERSION = 0.2

DIRS = monitor
BUILD = `pwd`/../build

.PHONY:		all clean dist

all:
		for i in $(DIRS) ; do \
		  $(MAKE) -C $$i install ; \
		done
		mkdir -p $(BUILD)/run
		cp Makefile.run $(BUILD)/run/Makefile

clean:
		for i in $(DIRS) ; do \
		  $(MAKE) -C $$i clean ; \
		done
		rm -f *~

dist:		clean
		(cd .. ; \
		 tar -cvf os-rom-$(VERSION).tar os-rom-$(VERSION)/* ; \
		 gzip -f os-rom-$(VERSION).tar)
