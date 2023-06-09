#
# Copyright 2023 The Turbo Authors.
#
# Licensed under the Apache License, Version 2.0 (the "License");
# you may not use this file except in compliance with the License.
# You may obtain a copy of the License at
#
#      https://www.apache.org/licenses/LICENSE-2.0
#
# Unless required by applicable law or agreed to in writing, software
# distributed under the License is distributed on an "AS IS" BASIS,
# WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
# See the License for the specific language governing permissions and
# limitations under the License.
#

# https://github.com/google/oss-policies-info/blob/main/foundational-cxx-support-matrix.md
# As of 2022-09-06, CMake 3.10 is the minimum supported version.
cmake_minimum_required(VERSION 3.10)

# Compiler id for Apple Clang is now AppleClang.
if (POLICY CMP0025)
  cmake_policy(SET CMP0025 NEW)
endif (POLICY CMP0025)

# if command can use IN_LIST
if (POLICY CMP0057)
  cmake_policy(SET CMP0057 NEW)
endif (POLICY CMP0057)

# Project version variables are the empty string if version is unspecified
if (POLICY CMP0048)
  cmake_policy(SET CMP0048 NEW)
endif (POLICY CMP0048)

# option() honor variables
if (POLICY CMP0077)
  cmake_policy(SET CMP0077 NEW)
endif (POLICY CMP0077)

# Allow the user to specify the MSVC runtime
if (POLICY CMP0091)
  cmake_policy(SET CMP0091 NEW)
endif (POLICY CMP0091)

# try_compile() honors the CMAKE_CXX_STANDARD value
if (POLICY CMP0067)
  cmake_policy(SET CMP0067 NEW)
endif (POLICY CMP0067)

if (POLICY CMP0110)
  cmake_policy(SET CMP0110 NEW)
endif (POLICY CMP0110)

project(lambda LANGUAGES CXX)
include(CTest)

# Output directory is correct by default for most build setups. However, when
# building Turbo as a DLL, it is important to have the DLL in the same
# directory as the executable using it. Thus, we put all executables in a single
# /bin directory.
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)

# when lambda is included as subproject (i.e. using add_subdirectory(abseil-cpp))
# in the source tree of a project that uses it, install rules are disabled.
if(NOT CMAKE_SOURCE_DIR STREQUAL PROJECT_SOURCE_DIR)
  option(TURBO_ENABLE_INSTALL "Enable install rule" OFF)
else()
  option(TURBO_ENABLE_INSTALL "Enable install rule" ON)
endif()

set(CMAKE_CXX_STANDARD 14)

option(TURBO_PROPAGATE_CXX_STD
  "Use CMake C++ standard meta features (e.g. cxx_std_14) that propagate to targets that link to Turbo"
  OFF)  # TODO: Default to ON for CMake 3.8 and greater.
if(NOT TURBO_PROPAGATE_CXX_STD)
  message(WARNING "A future Turbo release will default TURBO_PROPAGATE_CXX_STD to ON for CMake 3.8 and up. We recommend enabling this option to ensure your project still builds correctly.")
endif()

option(TURBO_USE_SYSTEM_INCLUDES
  "Silence warnings in Turbo headers by marking them as SYSTEM includes"
  OFF)

list(APPEND CMAKE_MODULE_PATH
  ${CMAKE_CURRENT_LIST_DIR}/CMake
  ${CMAKE_CURRENT_LIST_DIR}/copts
)

list(APPEND CMAKE_PREFIX_PATH $ENV{CONDA_PREFIX})
include_directories(${PROJECT_BINARY_DIR}/output/include)

include(CMakePackageConfigHelpers)
include(GNUInstallDirs)
include(TurboHelpers)
include(Simd)

find_package(Turbo REQUIRED)

include_directories(${TURBO_INCLUDE_DIR})

# include current path
list(APPEND TURBO_COMMON_INCLUDE_DIRS ${CMAKE_CURRENT_SOURCE_DIR})

if("${CMAKE_CXX_COMPILER_ID}" MATCHES "Clang")
  set(TURBO_USING_CLANG ON)
else()
  set(TURBO_USING_CLANG OFF)
endif()

# find dependencies
## pthread
find_package(Threads REQUIRED)

include(CMakeDependentOption)

option(TURBO_BUILD_TESTING
  "If ON, Turbo will build all of Turbo's own tests." ON)

option(TURBO_BUILD_EXAMPLE
        "If ON, Turbo will build all of Turbo's example." ON)

option(TURBO_BUILD_TEST_HELPERS
  "If ON, Turbo will build libraries that you can use to write tests against Turbo code. This option requires that Turbo is configured to use GoogleTest."
  OFF)

option(TURBO_USE_EXTERNAL_GOOGLETEST
  "If ON, Turbo will assume that the targets for GoogleTest are already provided by the including project. This makes sense when Turbo is used with add_subdirectory." OFF)

cmake_dependent_option(TURBO_FIND_GOOGLETEST
  "If ON, Turbo will use find_package(GTest) rather than assuming that GoogleTest is already provided by the including project."
  ON
  "TURBO_USE_EXTERNAL_GOOGLETEST"
  OFF)


option(TURBO_USE_GOOGLETEST_HEAD
  "If ON, abseil will download HEAD from GoogleTest at config time." ON)

set(TURBO_GOOGLETEST_DOWNLOAD_URL "" CACHE STRING "If set, download GoogleTest from this URL")

set(TURBO_LOCAL_GOOGLETEST_DIR "/usr/src/googletest" CACHE PATH
  "If TURBO_USE_GOOGLETEST_HEAD is OFF and TURBO_GOOGLETEST_URL is not set, specifies the directory of a local GoogleTest checkout."
  )

if((BUILD_TESTING AND TURBO_BUILD_TESTING) OR TURBO_BUILD_TEST_HELPERS)
  if (TURBO_USE_EXTERNAL_GOOGLETEST)
    if (TURBO_FIND_GOOGLETEST)
      find_package(GTest REQUIRED)
    elseif(NOT TARGET GTest::gtest)
      if(TARGET gtest)
        # When Google Test is included directly rather than through find_package, the aliases are missing.
        add_library(GTest::gtest ALIAS gtest)
        add_library(GTest::gtest_main ALIAS gtest_main)
        add_library(GTest::gmock ALIAS gmock)
        add_library(GTest::gmock_main ALIAS gmock_main)
      else()
        message(FATAL_ERROR "TURBO_USE_EXTERNAL_GOOGLETEST is ON and TURBO_FIND_GOOGLETEST is OFF, which means that the top-level project must build the Google Test project. However, the target gtest was not found.")
      endif()
    endif()
  else()
    set(turbo_gtest_build_dir ${CMAKE_BINARY_DIR}/googletest-build)
    if(TURBO_USE_GOOGLETEST_HEAD AND TURBO_GOOGLETEST_DOWNLOAD_URL)
      message(FATAL_ERROR "Do not set both TURBO_USE_GOOGLETEST_HEAD and TURBO_GOOGLETEST_DOWNLOAD_URL")
    endif()
    if(TURBO_USE_GOOGLETEST_HEAD)
      set(turbo_gtest_download_url "https://gitee.com/gottingen/googletest/repository/archive/main.zip")
    elseif(TURBO_GOOGLETEST_DOWNLOAD_URL)
      set(turbo_gtest_download_url ${TURBO_GOOGLETEST_DOWNLOAD_URL})
    endif()
    if(turbo_gtest_download_url)
      set(turbo_gtest_src_dir ${CMAKE_BINARY_DIR}/googletest-src)
    else()
      set(turbo_gtest_src_dir ${TURBO_LOCAL_GOOGLETEST_DIR})
    endif()
    include(CMake/Googletest/DownloadGTest.cmake)
  endif()
endif()

set(TURBO_DYLINK)
if (APPLE)
  find_library(CoreFoundation CoreFoundation)
  list(APPEND TURBO_DYLINK ${CoreFoundation} pthread)
elseif (${CMAKE_SYSTEM_NAME} STREQUAL "Linux")
  list(APPEND TURBO_DYLINK rt dl pthread)
endif ()

find_package(Protobuf REQUIRED)

find_library(PROTOC_LIB NAMES protoc)
if (NOT PROTOC_LIB)
  message(FATAL_ERROR "Fail to find protoc lib")
endif ()

include(CompileProto)
set(PROTO_FILES
        lambda/proto/document.proto
        lambda/proto/ann_query.proto
        lambda/proto/query.proto
        )

file(MAKE_DIRECTORY ${PROJECT_BINARY_DIR}/output/include/lambda)
set(PROTOC_FLAGS ${PROTOC_FLAGS} -I${PROTOBUF_INCLUDE_DIR} -I${PROJECT_SOURCE_DIR})
compile_proto(PROTO_HDRS PROTO_SRCS ${PROJECT_BINARY_DIR}
        ${PROJECT_BINARY_DIR}/output/include
        ${PROJECT_SOURCE_DIR}
        "${PROTO_FILES}")

turbo_cc_library(
        NAME
        proto
        SRCS
        ${PROTO_SRCS}
        COPTS
       # ${TURBO_DEFAULT_COPTS}
        DEPS
        ${TURBO_DYLINK}
        PUBLIC
)

add_subdirectory(lambda)

if(TURBO_BUILD_EXAMPLE)
  add_subdirectory(examples)
endif()

if(TURBO_ENABLE_INSTALL)
  # lambda:lts-remove-begin(system installation is supported for LTS releases)
  # We don't support system-wide installation
  list(APPEND SYSTEM_INSTALL_DIRS "/usr/local" "/usr" "/opt/" "/opt/local" "c:/Program Files/${PROJECT_NAME}")
  if(NOT DEFINED CMAKE_INSTALL_PREFIX OR CMAKE_INSTALL_PREFIX IN_LIST SYSTEM_INSTALL_DIRS)
    message(WARNING "\
  The default and system-level install directories are unsupported except in LTS \
  releases of Turbo.  Please set CMAKE_INSTALL_PREFIX to install Turbo in your \
  source or build tree directly.\
    ")
  endif()
  # lambda:lts-remove-end

  # install as a subdirectory only
  install(EXPORT ${PROJECT_NAME}Targets
    NAMESPACE lambda::
    DESTINATION "${CMAKE_INSTALL_LIBDIR}/cmake/${PROJECT_NAME}"
  )

  configure_package_config_file(
    CMake/turboConfig.cmake.in
    "${PROJECT_BINARY_DIR}/${PROJECT_NAME}Config.cmake"
    INSTALL_DESTINATION "${CMAKE_INSTALL_LIBDIR}/cmake/${PROJECT_NAME}"
  )
  install(FILES "${PROJECT_BINARY_DIR}/${PROJECT_NAME}Config.cmake"
    DESTINATION "${CMAKE_INSTALL_LIBDIR}/cmake/${PROJECT_NAME}"
  )

  # Turbo only has a version in LTS releases.  This mechanism is accomplished
  # Turbo's internal Copybara (https://github.com/google/copybara) workflows and
  # isn't visible in the CMake buildsystem itself.
  if(turbo_VERSION)
    write_basic_package_version_file(
      "${PROJECT_BINARY_DIR}/${PROJECT_NAME}ConfigVersion.cmake"
      COMPATIBILITY ExactVersion
    )

    install(FILES "${PROJECT_BINARY_DIR}/${PROJECT_NAME}ConfigVersion.cmake"
      DESTINATION "${CMAKE_INSTALL_LIBDIR}/cmake/${PROJECT_NAME}"
    )
  endif()  # turbo_VERSION

  install(DIRECTORY lambda
    DESTINATION ${CMAKE_INSTALL_INCLUDEDIR}
    FILES_MATCHING
      PATTERN "*.inc"
      PATTERN "*.h"
      PATTERN "copts" EXCLUDE
      PATTERN "test/testdata" EXCLUDE
    )

  install(DIRECTORY dict DESTINATION share/lambda)

endif()  # TURBO_ENABLE_INSTALL
