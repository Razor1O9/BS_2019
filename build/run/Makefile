#
# Makefile to build a bootable disk, and start
# the ECO32 simulator with a ROM monitor.
#

BUILD = ..

DSK_IMG = disk.img
DSK_SIZ = 650M
RUN_LOG = run.log

all:		$(DSK_IMG)

run-sim:	$(DSK_IMG)
		$(BUILD)/bin/sim -i -m 32 -c -s 2 -t 0 -t 1 \
		  -r $(BUILD)/monitor/simulator/monitor.bin \
		  -d $(DSK_IMG) -o $(RUN_LOG)

$(DSK_IMG):
		$(BUILD)/bin/mkdisk $(DSK_IMG) $(DSK_SIZ)
		$(BUILD)/bin/mkpart $(DSK_IMG) disk.part
		$(MAKE) -C fs-EOS32 all
		$(MAKE) -C fs-NetBSD all
		$(MAKE) -C fs-Linux all

clean:
		$(MAKE) -C fs-EOS32 clean
		$(MAKE) -C fs-NetBSD clean
		$(MAKE) -C fs-Linux clean
		rm -f *~ $(DSK_IMG) $(RUN_LOG)
