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

set(LIBTEXT_ALL_SRCS
        "core/ann_index.cc"
        "hnsw/hnswlib_index.cc"
)

file(GLOB_RECURSE HNSW_H "hnsw/*.h")
turbo_cc_library(
        NAME
        hnsw
        HRDS
        ${HNSW_H}
        COPTS
        ${TURBO_DEFAULT_COPTS}
        ${TURBO_RANDOM_RANDEN_COPTS}
        ${TURBO_SIMD_FLAGS}
        DEPS
        ${TURBO_DYLINK}
)

if (TURBO_BUILD_TESTING)
 message(STATUS "testing enable")
endif (TURBO_BUILD_TESTING)
