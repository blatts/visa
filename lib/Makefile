# -*- mode: Makefile -*-
# Time-stamp: "2013-05-17 18:51:08 sb"

#  file       Makefile
#  copyright  (c) Sebastian Blatt 2013

include ../Makefile.defs

libname := master

ext        := cc
libsources := $(wildcard *.$(ext))

builddir   := ../$(build_directory)/$(cxx)
lib        := $(builddir)/$(libname).a
libobjects := $(patsubst %, $(builddir)/%, $(libsources:.$(ext)=.o))
# deps       := $(patsubst %, $(builddir)/%, $(libsources:.$(ext)=.d))

all: $(builddir) $(lib)

$(lib): $(libobjects)
	@echo "*   link    : $(lib)"
	@$(ar) $(aflags) $@ $^
	@echo "_ complete: $(lib)"

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
