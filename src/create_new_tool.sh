#!/bin/bash
# -*- mode: Sh -*-
# Time-stamp: "2013-05-17 13:48:48 sb"

#  file       create_new_tool.sh
#  copyright  (c) Sebastian Blatt 2013

project=$1
vc_project_file=${project}/${project}.vcxproj

mkdir "${project}"

cp lsvisa/lsvisa.vcxproj "${vc_project_file}" && \
    sed -E -i "s/lsvisa/${project}/g" "${vc_project_file}"
