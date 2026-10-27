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

# VENDORED DEPENDENCIES
add_subdirectory(vendor/SDL3 EXCLUDE_FROM_ALL)
add_subdirectory(vendor/ozz-animation EXCLUDE_FROM_ALL)
add_subdirectory(vendor/JoltPhysics/Build EXCLUDE_FROM_ALL)
add_subdirectory(vendor/DiligentEngine EXCLUDE_FROM_ALL)
add_subdirectory(vendor/spdlog EXCLUDE_FROM_ALL)
add_subdirectory(vendor/angelscript/sdk/angelscript/projects/cmake EXCLUDE_FROM_ALL)
add_subdirectory(vendor/flecs EXCLUDE_FROM_ALL)
add_subdirectory(vendor/json EXCLUDE_FROM_ALL)
add_subdirectory(vendor/daslang)

# ANGELSCRIPT ADDONS
file(GLOB_RECURSE AS_ADDON_SOURCES vendor/angelscript/sdk/add_on/*.cpp vendor/angelscript/sdk/add_on/*.h)
add_library(asAddons STATIC ${AS_ADDON_SOURCES})
target_include_directories(asAddons PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/vendor/angelscript/sdk/add_on)
target_link_libraries(asAddons PUBLIC angelscript)

# ENGINE LIBRARY
file(GLOB_RECURSE SOURCES src/*.cpp src/*.h)
add_library(MECore STATIC ${SOURCES})
target_include_directories(MECore PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/include ${CMAKE_CURRENT_SOURCE_DIR}/vendor/daslang/include)
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
        libDaScriptDyn
)

if(ME_USE_IMGUI)
    set(DAS_IMGUI_DISABLED ON CACHE BOOL "Disable DAS imgui implementation" FORCE)

    set (DAS_IMGUI_DIR ${CMAKE_CURRENT_SOURCE_DIR}/vendor/daslang/modules/dasImgui)
    set(DAS_IMGUI_BIND_SRC
            ${DAS_IMGUI_DIR}/src/dasIMGUI.cpp
            ${DAS_IMGUI_DIR}/src/dasIMGUI.main.cpp
            ${DAS_IMGUI_DIR}/src/dasIMGUI.func_1.cpp
            ${DAS_IMGUI_DIR}/src/dasIMGUI.func_2.cpp
            ${DAS_IMGUI_DIR}/src/dasIMGUI.func_3.cpp
            ${DAS_IMGUI_DIR}/src/dasIMGUI.func_4.cpp
            ${DAS_IMGUI_DIR}/src/dasIMGUI.func_5.cpp
            ${DAS_IMGUI_DIR}/src/dasIMGUI.func_6.cpp
            ${DAS_IMGUI_DIR}/src/dasIMGUI.func_7.cpp
            ${DAS_IMGUI_DIR}/src/dasIMGUI.func_8.cpp
            ${DAS_IMGUI_DIR}/src/dasIMGUI.func_9.cpp
            ${DAS_IMGUI_DIR}/src/dasIMGUI.func_10.cpp
            ${DAS_IMGUI_DIR}/src/dasIMGUI.func_11.cpp
            ${DAS_IMGUI_DIR}/src/dasIMGUI.func_12.cpp
            ${DAS_IMGUI_DIR}/src/dasIMGUI.func_13.cpp
            ${DAS_IMGUI_DIR}/src/dasIMGUI.func_14.cpp
            ${DAS_IMGUI_DIR}/src/dasIMGUI.func_15.cpp
            ${DAS_IMGUI_DIR}/src/dasIMGUI.func_16.cpp
            ${DAS_IMGUI_DIR}/src/dasIMGUI.func_17.cpp
            ${DAS_IMGUI_DIR}/src/dasIMGUI.func_18.cpp
            ${DAS_IMGUI_DIR}/src/dasIMGUI.func_19.cpp
            ${DAS_IMGUI_DIR}/src/dasIMGUI.func_20.cpp
            ${DAS_IMGUI_DIR}/src/dasIMGUI.func_21.cpp
            ${DAS_IMGUI_DIR}/src/dasIMGUI.func_22.cpp
            ${DAS_IMGUI_DIR}/src/dasIMGUI.func_23.cpp
            ${DAS_IMGUI_DIR}/src/dasIMGUI.func_24.cpp
            ${DAS_IMGUI_DIR}/src/dasIMGUI.func_25.cpp
            ${DAS_IMGUI_DIR}/src/dasIMGUI.func_26.cpp
            ${DAS_IMGUI_DIR}/src/dasIMGUI.func_27.cpp
            ${DAS_IMGUI_DIR}/src/dasIMGUI.func_28.cpp
            ${DAS_IMGUI_DIR}/src/dasIMGUI.func_29.cpp
            ${DAS_IMGUI_DIR}/src/dasIMGUI.func_30.cpp
            ${DAS_IMGUI_DIR}/src/dasIMGUI.func_31.cpp
            ${DAS_IMGUI_DIR}/src/dasIMGUI.func_32.cpp
            ${DAS_IMGUI_DIR}/src/dasIMGUI.func_33.cpp
            ${DAS_IMGUI_DIR}/src/dasIMGUI.func_34.cpp
    )

    # IMGUI
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

    # ADD_DAS_STATIC_MODULE_LIB(dasImgui ${DAS_IMGUI_BIND_SRC})
    # ADD_MODULE_CPP(dasImgui)
    # target_link_libraries(dasModuleImgui PRIVATE imgui)
    # add_library(imgui STATIC ${IMGUI_SOURCES} ${DAS_IMGUI_BIND_SRC})

    # add_library(dasImgui SHARED ${DAS_IMGUI_BIND_SRC})
    # target_include_directories(dasImgui PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/vendor/daslang/include)
    # target_link_libraries(dasImgui PRIVATE libDaScriptDyn imgui)
    # set_target_properties(dasImgui PROPERTIES
    #     SUFFIX ".shared_module"
    #     PREFIX "")

    target_link_libraries(MECore PUBLIC imgui)
endif()

add_library(ME::Core ALIAS MECore)
