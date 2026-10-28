cmake_minimum_required(VERSION 3.30)
cmake_policy(SET CMP0048 NEW)

project(MECore VERSION 1.0.0 DESCRIPTION "MANIFOLDEngine Core")
set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_STANDARD_REQUIRED TRUE)
set(CMAKE_POSITION_INDEPENDENT_CODE TRUE)

option(ME_USE_IMGUI "Compile ImGui support" ON)

# SYSTEM PACKAGES
# maybe we should provide boost as well?
find_package(Threads REQUIRED)
find_package(Vulkan REQUIRED)
find_package(Boost REQUIRED)

# IMGUI
if(ME_USE_IMGUI)
    set(IMGUI_SOURCES
            ${CMAKE_CURRENT_SOURCE_DIR}/vendor/imgui/imconfig.h
            ${CMAKE_CURRENT_SOURCE_DIR}/vendor/imgui/imgui_draw.cpp
            ${CMAKE_CURRENT_SOURCE_DIR}/vendor/imgui/imgui_tables.cpp
            ${CMAKE_CURRENT_SOURCE_DIR}/vendor/imgui/imgui_widgets.cpp
            ${CMAKE_CURRENT_SOURCE_DIR}/vendor/imgui/imgui_internal.h
            ${CMAKE_CURRENT_SOURCE_DIR}/vendor/imgui/imgui.cpp
            ${CMAKE_CURRENT_SOURCE_DIR}/vendor/imgui/imgui.h
            ${CMAKE_CURRENT_SOURCE_DIR}/vendor/imgui/imstb_rectpack.h
            ${CMAKE_CURRENT_SOURCE_DIR}/vendor/imgui/imstb_textedit.h
            ${CMAKE_CURRENT_SOURCE_DIR}/vendor/imgui/imstb_truetype.h
            ${CMAKE_CURRENT_SOURCE_DIR}/vendor/imgui/imgui_demo.cpp
            ${CMAKE_CURRENT_SOURCE_DIR}/vendor/imgui/misc/cpp/imgui_stdlib.cpp
            ${CMAKE_CURRENT_SOURCE_DIR}/vendor/imgui/misc/cpp/imgui_stdlib.h
            ${CMAKE_CURRENT_SOURCE_DIR}/vendor/imgui/backends/imgui_impl_sdl3.cpp
            ${CMAKE_CURRENT_SOURCE_DIR}/vendor/imgui/backends/imgui_impl_sdl3.h)
    add_library(imgui STATIC ${IMGUI_SOURCES})
    target_include_directories(imgui PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/vendor/imgui)
endif()

# VENDORED DEPENDENCIES
add_subdirectory(vendor/SDL3 EXCLUDE_FROM_ALL)
add_subdirectory(vendor/ozz-animation EXCLUDE_FROM_ALL)
add_subdirectory(vendor/JoltPhysics/Build EXCLUDE_FROM_ALL)
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
        Diligent-GraphicsAccessories
        Diligent-GraphicsEngineVk-static
        flecs::flecs_static
        Jolt
        spdlog::spdlog
        angelscript
        asAddons
        nlohmann_json::nlohmann_json
)

if(TARGET imgui)
    target_compile_definitions(MECore PUBLIC ME_IMGUI)
    target_link_libraries(MECore PUBLIC imgui)
endif()

add_library(ME::Core ALIAS MECore)
