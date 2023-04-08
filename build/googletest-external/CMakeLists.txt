cmake_minimum_required(VERSION 3.10)

project(googletest-external NONE)

include(ExternalProject)
ExternalProject_Add(googletest
  URL               "https://gitee.com/gottingen/googletest/repository/archive/main.zip"  # May be empty
  SOURCE_DIR        "/Users/liyinbin/github/gottingen/lambda/build/googletest-src"
  BINARY_DIR        "/Users/liyinbin/github/gottingen/lambda/build/googletest-build"
  CONFIGURE_COMMAND ""
  BUILD_COMMAND     ""
  INSTALL_COMMAND   ""
  TEST_COMMAND      ""
)
