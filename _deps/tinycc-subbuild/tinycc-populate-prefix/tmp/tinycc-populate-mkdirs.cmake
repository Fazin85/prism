# Distributed under the OSI-approved BSD 3-Clause License.  See accompanying
# file Copyright.txt or https://cmake.org/licensing for details.

cmake_minimum_required(VERSION 3.5)

file(MAKE_DIRECTORY
  "/home/fazin85/prism/_deps/tinycc-src"
  "/home/fazin85/prism/_deps/tinycc-build"
  "/home/fazin85/prism/_deps/tinycc-subbuild/tinycc-populate-prefix"
  "/home/fazin85/prism/_deps/tinycc-subbuild/tinycc-populate-prefix/tmp"
  "/home/fazin85/prism/_deps/tinycc-subbuild/tinycc-populate-prefix/src/tinycc-populate-stamp"
  "/home/fazin85/prism/_deps/tinycc-subbuild/tinycc-populate-prefix/src"
  "/home/fazin85/prism/_deps/tinycc-subbuild/tinycc-populate-prefix/src/tinycc-populate-stamp"
)

set(configSubDirs )
foreach(subDir IN LISTS configSubDirs)
    file(MAKE_DIRECTORY "/home/fazin85/prism/_deps/tinycc-subbuild/tinycc-populate-prefix/src/tinycc-populate-stamp/${subDir}")
endforeach()
if(cfgdir)
  file(MAKE_DIRECTORY "/home/fazin85/prism/_deps/tinycc-subbuild/tinycc-populate-prefix/src/tinycc-populate-stamp${cfgdir}") # cfgdir has leading slash
endif()
