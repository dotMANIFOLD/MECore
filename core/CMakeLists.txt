cmake_minimum_required(VERSION 3.30)
cmake_policy(SET CMP0048 NEW)

project(MECore VERSION 1.0.0 DESCRIPTION "MANIFOLDEngine Core")
set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_STANDARD_REQUIRED TRUE)
set(CMAKE_POSITION_INDEPENDENT_CODE TRUE)

# DISABLE TROUBLESOME OPTIONS
set(WITH_VM OFF CACHE INTERNAL "" FORCE)

find_package(Threads REQUIRED)
find_package(Vulkan REQUIRED)

# VENDORED DEPENDENCIES
add_subdirectory(vendor/SDL3 EXCLUDE_FROM_ALL)
add_subdirectory(vendor/hashlink EXCLUDE_FROM_ALL)
add_subdirectory(vendor/taskflow EXCLUDE_FROM_ALL)
add_subdirectory(vendor/ozz-animation EXCLUDE_FROM_ALL)
add_subdirectory(vendor/JoltPhysics/Build EXCLUDE_FROM_ALL)
add_subdirectory(vendor/nvrhi EXCLUDE_FROM_ALL)
add_subdirectory(vendor/spdlog EXCLUDE_FROM_ALL)
add_subdirectory(vendor/stduuid EXCLUDE_FROM_ALL)
add_subdirectory(vendor/cereal EXCLUDE_FROM_ALL)

# HAXE VM
file(GLOB HAXE_EMBED_SOURCES
        vendor/hashlink/src/code.c
        vendor/hashlink/src/module.c
        vendor/hashlink/src/jit.c)
add_library(MECoreVM STATIC)
target_sources(MECoreVM PRIVATE ${HAXE_EMBED_SOURCES})
target_include_directories(MECoreVM
    PRIVATE ${CMAKE_CURRENT_BINARY_DIR}/vendor/hashlink/src
    PUBLIC $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/vendor/hashlink/src>)

# ENGINE LIBRARY
file(GLOB_RECURSE SOURCES src/*.cpp src/*.h)
add_library(MECore STATIC)
target_sources(MECore PRIVATE ${SOURCES})
target_include_directories(MECore
    PRIVATE ${CMAKE_CURRENT_BINARY_DIR}/include
    PUBLIC $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
           $<INSTALL_INTERFACE:${CMAKE_INSTALL_INCLUDEDIR}>)

target_link_libraries(MECore PRIVATE Threads::Threads Vulkan::Vulkan SDL3::SDL3 libhl MECoreVM Taskflow Jolt nvrhi ozz_base ozz_animation spdlog::spdlog stduuid cereal::cereal)

add_library(ME::Core ALIAS MECore)
