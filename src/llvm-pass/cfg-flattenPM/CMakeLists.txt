cmake_minimum_required(VERSION 3.13.4)
project(mino-obfuscator)

if (NOT CMAKE_BUILD_TYPE)
set(CMAKE_BUILD_TYPE Debug CACHE STRING "Build type (default Debug):" FORCE)
endif()

if("$ENV{LLVM_VERSION}" STREQUAL "4.0")
list(APPEND CMAKE_PREFIX_PATH "/usr/lib/llvm-4.0/lib/cmake/llvm/")
elseif("$ENV{LLVM_VERSION}" STREQUAL "5.0")
list(APPEND CMAKE_PREFIX_PATH "/usr/lib/llvm-5.0/lib/cmake/llvm/")
elseif("$ENV{LLVM_VERSION}" STREQUAL "6.0")
list(APPEND CMAKE_PREFIX_PATH "/usr/lib/llvm-6.0/lib/cmake/llvm/")
elseif("$ENV{LLVM_VERSION}" STREQUAL "7")
list(APPEND CMAKE_PREFIX_PATH "/usr/lib/llvm-7/lib/cmake/llvm/")
elseif("$ENV{LLVM_VERSION}" STREQUAL "8")
list(APPEND CMAKE_PREFIX_PATH "/usr/lib/llvm-8/lib/cmake/llvm/")
elseif("$ENV{LLVM_VERSION}" STREQUAL "9")
list(APPEND CMAKE_PREFIX_PATH "/usr/lib/llvm-9/lib/cmake/llvm/")
elseif("$ENV{LLVM_VERSION}" STREQUAL "10")
list(APPEND CMAKE_PREFIX_PATH "/usr/lib/llvm-10/lib/cmake/llvm/")
elseif("$ENV{LLVM_VERSION}" STREQUAL "11")
list(APPEND CMAKE_PREFIX_PATH "/usr/lib/llvm-11/lib/cmake/llvm/")
elseif("$ENV{LLVM_VERSION}" STREQUAL "12")
list(APPEND CMAKE_PREFIX_PATH "/usr/lib/llvm-12/lib/cmake/llvm/")
elseif("$ENV{LLVM_VERSION}" STREQUAL "13")
list(APPEND CMAKE_PREFIX_PATH "/usr/lib/llvm-13/lib/cmake/llvm/")
elseif("$ENV{LLVM_VERSION}" STREQUAL "14")
list(APPEND CMAKE_PREFIX_PATH "/usr/lib/llvm-14/lib/cmake/llvm/")
elseif("$ENV{LLVM_VERSION}" STREQUAL "15")
list(APPEND CMAKE_PREFIX_PATH "/usr/lib/llvm-15/lib/cmake/llvm/")
elseif("$ENV{LLVM_VERSION}" STREQUAL "16")
list(APPEND CMAKE_PREFIX_PATH "/usr/lib/llvm-16/lib/cmake/llvm/")
elseif("$ENV{LLVM_VERSION}" STREQUAL "17")
list(APPEND CMAKE_PREFIX_PATH "/usr/lib/llvm-17/lib/cmake/llvm/")
elseif("$ENV{LLVM_VERSION}" STREQUAL "18")
list(APPEND CMAKE_PREFIX_PATH "/usr/lib/llvm-18/lib/cmake/llvm/")
endif()

find_package(LLVM REQUIRED CONFIG)

message(STATUS "Found LLVM ${LLVM_PACKAGE_VERSION}")

message("LLVM STATUS:
    Definition ${LLVM_DEFINITIONS}
    Includes ${LLVM_INCLUDE_DIRS}
    Libraries ${LLVM_LIBRARY_DIRS}")

include_directories(SYSTEM ${LLVM_INCLUDE_DIRS})
link_directories(${LLVM_LIBRARY_DIRS})
add_definitions(${LLVM_DEFINITIONS})

set(CMAKE_CXX_STANDARD 14 CACHE STRING "")

# LLVM is normally built without RTTI. Be consistent with that.
if(NOT LLVM_ENABLE_RTTI)
  set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -fno-rtti")
endif()

#===============================================================================
# 3. ADD THE TARGET
#===============================================================================
add_library(Obfuscation SHARED PassPluginLibrary.cpp CFGFlattenPass.cpp FakeBBInsertionPass.cpp JunkCodeInsertionPass.cpp)

# Allow undefined symbols in shared objects on Darwin (this is the default
# behaviour on Linux)
target_link_libraries(Obfuscation
  "$<$<PLATFORM_ID:Darwin>:-undefined dynamic_lookup>")
