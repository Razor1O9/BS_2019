#
# Makefile for ECO32 binary utilities and C compiler
# OS course version
#

VERSION = 0.2

DIRS = binutils
BUILD = `pwd`/../build

.PHONY:		all compiler builddir clean dist

all:		compiler
		for i in $(DIRS) ; do \
		  $(MAKE) -C $$i install ; \
		done

compiler:	builddir
		$(MAKE) -C lcc BUILDDIR=$(BUILD)/bin \
		  HOSTFILE=etc/eco32-linux.c lcc
		$(MAKE) -C lcc BUILDDIR=$(BUILD)/bin all
		rm -f $(BUILD)/bin/*.c
		rm -f $(BUILD)/bin/*.o
		rm -f $(BUILD)/bin/*.a
		rm -f $(BUILD)/bin/bprint

builddir:
		mkdir -p $(BUILD)/bin

clean:
		rm -f lcc/lburg/gram.c
		for i in $(DIRS) ; do \
		  $(MAKE) -C $$i clean ; \
		done
		rm -f *~

dist:		clean
		(cd .. ; \
		 tar -cvf os-lcc-$(VERSION).tar os-lcc-$(VERSION)/* ; \
		 gzip -f os-lcc-$(VERSION).tar)
