# -*- mode: Makefile -*-
# Time-stamp: "2013-05-17 18:48:18 sb"

#  file       Makefile
#  copyright  (c) Sebastian Blatt 2013

include ../Makefile.defs

.PHONY : clean

cleanwhat := $(wildcard $(cxx)/*)

clean :
	@echo "  * clean up temporary directory"
	@-rm -rf $(cleanwhat)
	@-rmdir $(cxx)
	@echo "  _ complete: clean temporary directory"
