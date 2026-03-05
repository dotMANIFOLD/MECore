cmake_minimum_required(VERSION 3.30)
cmake_policy(SET CMP0048 NEW)

project(MECore VERSION 1.0.0 DESCRIPTION "MANIFOLDEngine Core")
set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_STANDARD_REQUIRED TRUE)
set(CMAKE_POSITION_INDEPENDENT_CODE TRUE)

find_package(Threads REQUIRED)
find_package(Vulkan REQUIRED)
find_package(Boost REQUIRED)

# VENDORED DEPENDENCIES
add_subdirectory(vendor/SDL3 EXCLUDE_FROM_ALL)
add_subdirectory(vendor/ozz-animation EXCLUDE_FROM_ALL)
add_subdirectory(vendor/JoltPhysics/Build EXCLUDE_FROM_ALL)
add_subdirectory(vendor/nvrhi EXCLUDE_FROM_ALL)
add_subdirectory(vendor/DiligentEngine EXCLUDE_FROM_ALL)
add_subdirectory(vendor/spdlog EXCLUDE_FROM_ALL)
add_subdirectory(vendor/angelscript/sdk/angelscript/projects/cmake EXCLUDE_FROM_ALL)
add_subdirectory(vendor/flecs EXCLUDE_FROM_ALL)
add_subdirectory(vendor/json EXCLUDE_FROM_ALL)

# ANGELSCRIPT ADDONS
file(GLOB_RECURSE AS_ADDON_SOURCES vendor/angelscript/sdk/add_on/*.cpp vendor/angelscript/sdk/add_on/*.h)
add_library(asAddons STATIC ${AS_ADDON_SOURCES})
target_include_directories(asAddons PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/vendor/angelscript/sdk/add_on)
target_link_libraries(asAddons PUBLIC angelscript)

# ENGINE LIBRARY
file(GLOB_RECURSE SOURCES src/*.cpp src/*.h)
add_library(MECore STATIC ${SOURCES})
target_include_directories(MECore PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/include)
target_include_directories(MECore PRIVATE ${Boost_INCLUDE_DIRS})

target_link_libraries(MECore PUBLIC
        Threads::Threads
        Vulkan::Vulkan
        SDL3::SDL3
        Diligent-Common
        Diligent-GraphicsTools
        Diligent-GraphicsEngineVk-static
        flecs::flecs_static
        Jolt
        spdlog::spdlog
        angelscript
        asAddons
        nlohmann_json::nlohmann_json
)

add_library(ME::Core ALIAS MECore)
