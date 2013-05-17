# -*- mode: Makefile-Bsdmake -*-
# Time-stamp: "2013-05-17 19:01:17 sb"

#  file       Makefile
#  copyright  (c) Sebastian Blatt 2013

include ../../Makefile.defs

program_name := agilent33410A

ext        := cc
sources := $(wildcard *.$(ext))

builddir   := ../../$(build_directory)/$(cxx)
program    := $(builddir)/$(program_name)
libobjects := $(patsubst %, $(builddir)/%, $(libsources:.$(ext)=.o))
# deps       := $(patsubst %, $(builddir)/%, $(libsources:.$(ext)=.d))

all: $(builddir) $(program)

$(program): $(objects)
	@echo "*   link    : $(program)"
	@$(CC) -o $@ $^ $(lflags)
	@echo "_ complete: $(program)"

# -include $(deps)

$(builddir) :
	@echo "*   mkdir $@"
	@-mkdir $@

# $(builddir)/%.d : %.$(ext)
# 	@echo "*   depend  : $< -> $@"
# 	@set -e; $(CC) -MM $(cflags) $< \
# 		| $(sed) -e 's/\($*\)\.o[ :]*/\1.o $@ : /g' > $@; \
# 		[ -s $@ ] || rm -f $@

$(builddir)/%.o : %.$(ext)
	@echo "*   compile : $< -> $@"
	@$(CC) -c $(cflags) $< -o $@
