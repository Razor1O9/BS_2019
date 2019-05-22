#
# Makefile for running the ECO32 ROM monitor
#

BUILD = ..
SIM = $(BUILD)/bin/sim
ROM = $(BUILD)/monitor/simulator/monitor.bin

all:
		@echo "Please use one of the following targets:"
		@echo "    make run"
		@echo "    make stop"

run:
		$(SIM) -c -s 2 -t 0 -t 1 -r $(ROM)

stop:
		$(SIM) -i -c -s 2 -t 0 -t 1 -r $(ROM)
