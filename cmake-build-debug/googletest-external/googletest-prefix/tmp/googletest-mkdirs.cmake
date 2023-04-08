# Distributed under the OSI-approved BSD 3-Clause License.  See accompanying
# file Copyright.txt or https://cmake.org/licensing for details.

cmake_minimum_required(VERSION 3.5)

file(MAKE_DIRECTORY
  "/Users/liyinbin/github/gottingen/lambda/cmake-build-debug/googletest-src"
  "/Users/liyinbin/github/gottingen/lambda/cmake-build-debug/googletest-build"
  "/Users/liyinbin/github/gottingen/lambda/cmake-build-debug/googletest-external/googletest-prefix"
  "/Users/liyinbin/github/gottingen/lambda/cmake-build-debug/googletest-external/googletest-prefix/tmp"
  "/Users/liyinbin/github/gottingen/lambda/cmake-build-debug/googletest-external/googletest-prefix/src/googletest-stamp"
  "/Users/liyinbin/github/gottingen/lambda/cmake-build-debug/googletest-external/googletest-prefix/src"
  "/Users/liyinbin/github/gottingen/lambda/cmake-build-debug/googletest-external/googletest-prefix/src/googletest-stamp"
)

set(configSubDirs )
foreach(subDir IN LISTS configSubDirs)
    file(MAKE_DIRECTORY "/Users/liyinbin/github/gottingen/lambda/cmake-build-debug/googletest-external/googletest-prefix/src/googletest-stamp/${subDir}")
endforeach()
if(cfgdir)
  file(MAKE_DIRECTORY "/Users/liyinbin/github/gottingen/lambda/cmake-build-debug/googletest-external/googletest-prefix/src/googletest-stamp${cfgdir}") # cfgdir has leading slash
endif()
