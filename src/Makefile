# -*- mode: Makefile-Bsdmake -*-
# Time-stamp: "2013-05-17 18:55:24 sb"

#  file       Makefile
#  copyright  (c) Sebastian Blatt 2013

include ../Makefile.defs

projects = agilent33410A keithley3390 lsvisa sr760 tds2000

.PHONY: all $(projects)

all: $(projects)

$(projects):
	$(MAKE) -C $@
