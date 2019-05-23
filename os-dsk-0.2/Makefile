#
# Makefile for EOS32 disk tools
# OS course version
#

VERSION = 0.2

DIRS = dsktools
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
		 tar -cvf os-dsk-$(VERSION).tar os-dsk-$(VERSION)/* ; \
		 gzip -f os-dsk-$(VERSION).tar)
