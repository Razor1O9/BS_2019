#
# Makefile for EOS32
# OS course version
#

VERSION = 0.2

DIRS = kernel userland disk
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
		 tar -cvf os-eos-$(VERSION).tar os-eos-$(VERSION)/* ; \
		 gzip -f os-eos-$(VERSION).tar)
