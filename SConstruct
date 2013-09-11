#!/usr/bin/env python
# -*- mode: Python; coding: latin-1 -*-
# Time-stamp: "2013-09-11 12:41:04 sb"

#  file       SConstruct
#  copyright  (c) Sebastian Blatt 2013

# environment variables:
#   LIBPATH, LIBS, ASFLAGS, LINKFLAGS, CPPFLAGS, CPPPATH, CCFLAGS

import os.path

use_clang = True

programs = [
    'afg3102c',
    'agilent33410A',
    'keithley3390',
    'lsvisa',
    'sr760',
    'tds2000'
    ]

build_directory = 'build/scons/'

include_directories = [
    '/sw/include',
    '/Library/Frameworks/VISA.framework/Versions/A/Headers',
    os.path.realpath('lib')
    ]

library_directories = [
    '/sw/lib',
    os.path.realpath(build_directory + 'master')
    ]

frameworks = [
    'VISA'
    ]

warnings = [
    '',
    'all'
    ]

env = Environment()

# switch to clang++
if use_clang:
  cc = 'clang'
  cxx = 'clang++'
  env.Replace(CC = cc, CXX = cxx)

# VISA library is 32 bit only, need -m32
cxxflags  = '-g -O3 -m32'
linkflags  = '-m32'


cxxflags += " " + " ".join(map(lambda w: '-W%s' % w, warnings))
linkflags += " " + " ".join(map(lambda f: '-framework %s' % f, frameworks))


env.Append(LINKFLAGS = linkflags)
env.Append(LIBPATH = library_directories)
env.Append(CPPPATH = include_directories)
env.Append(CXXFLAGS = cxxflags)

env.SConscript('lib/SConscript',
               variant_dir = build_directory + 'master',
               duplicate = 0,
               exports = 'env')

for p in programs:
   env.SConscript('src/%s/SConscript' % p,
                   variant_dir = build_directory + p,
                   duplicate = 0,
                   exports = 'env')

# SConstruct ends here
