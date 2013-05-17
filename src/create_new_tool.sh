#!/bin/bash
# -*- mode: Sh -*-
# Time-stamp: "2013-05-17 17:17:38 sb"

#  file       create_new_tool.sh
#  copyright  (c) Sebastian Blatt 2013

project=$1
vc_project_file=${project}/${project}.vcxproj
project_source=${project/${project}.cc

echo "This script creates a new subdirectory and corresponding"
echo ".cc and Visual Studio .vcxproj files."

mkdir "${project}"

cp lsvisa/lsvisa.vcxproj "${vc_project_file}" && \
    sed -E -i "s/lsvisa/${project}/g" "${vc_project_file}" && \
    cp lsvisa/lsvisa.cc "${project_source}" && \
    sed -E -i "s/lsvisa/${project}/g" "${project_source}"
