# -*- mode: Makefile -*-
# Time-stamp: "2013-05-17 18:42:59 sb"

#  file       Makefile
#  copyright  (c) Sebastian Blatt 2013

include Makefile.defs

.PHONY: all $(master_directory) $(src_directory) flags clean

all: $(master_directory) $(src_directory)

$(master_directory):
	$(MAKE) -C $(master_directory)

$(src_directory):
	$(MAKE) -C $(src_directory)

flags:
	@echo "* Information about project $(project):"
	@echo "  compiler settings"
	@echo "    compiler: $(CC)"
	@echo "    flags: $(cflags)"
	@echo "  linker settings"
	@echo "    flags: $(lflags)"
	@echo "  archive settings"
	@echo "    archiver: $(ar)"
	@echo "    options: $(aflags)"
	@echo "  install options"
	@echo "    prefix: $(prefix)"


clean :
	@echo "* clean up"
	$(MAKE) -C $(build_directory) clean
	$(MAKE) -C $(tmp_directory) clean
	@echo "_ complete: clean"
