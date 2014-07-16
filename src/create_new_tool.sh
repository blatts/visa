#!/bin/bash
# -*- mode: Sh -*-
# Time-stamp: "2014-07-16 10:13:09 sb"

#  file       create_new_tool.sh
#  copyright  (c) Sebastian Blatt 2013, 2014

project=$1
vc_project_file=${project}/${project}.vcxproj
project_source=${project}/${project}.cc
#sed="/sw/bin/sed -E -i"
sed="/usr/local/bin/gsed -E -i"

echo "This script creates a new subdirectory and corresponding"
echo ".cc and Visual Studio .vcxproj files."

mkdir "${project}"

cp lsvisa/lsvisa.vcxproj "${vc_project_file}" && \
    ${sed} "s/lsvisa/${project}/g" "${vc_project_file}" && \
    cp lsvisa/lsvisa.cc "${project_source}" && \
    ${sed} "s/lsvisa/${project}/g" "${project_source}"
