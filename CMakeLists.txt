cmake_minimum_required(VERSION 3.1)
project(ConsoleCraftEditor)

set(CMAKE_CXX_STANDARD 20)

set(CMAKE_BUILD_TYPE Debug)
# IMGUI directory
set(IMGUI_DIR ${CMAKE_CURRENT_SOURCE_DIR}/Vendor/imgui)
# set(RAPIDJSON_DIR ${CMAKE_CURRENT_SOURCE_DIR}/Vendor/rapidjson)

# Engine source
file(GLOB_RECURSE ENGINE_SOURCES Engine/Source/*.cpp)
file(GLOB_RECURSE ENGINE_HEADERS Engine/Source/*.h)

# Add include directory for Engine headers
include_directories(Engine/Source)

# Create the ConsoleCraftEngine library
add_library(ConsoleCraftEngine SHARED ${ENGINE_SOURCES} ${ENGINE_HEADERS})

# Include directories
include_directories(${IMGUI_DIR})
include_directories(${IMGUI_DIR}/backends)
# include_directories(${RAPIDJSON_DIR}/include)


# SDL2 package
find_package(SDL2 REQUIRED)
include_directories(${SDL2_INCLUDE_DIRS})


message("Source directories: ${PROJECT_SOURCES}")

#Source files
set(SOURCES 
    Source/main.cpp 
    Source/window.cpp
    Source/Core/InEditorRenderer.cpp
    Source/Core/ProjectManager.cpp
    Source/Core/FileHandling/FileHandler.cpp
    Source/Core/FileHandling/SceneDataSerializer.cpp


    #imgui
    ${IMGUI_DIR}/imgui.cpp 
    ${IMGUI_DIR}/imgui_demo.cpp 
    ${IMGUI_DIR}/imgui_draw.cpp 
    ${IMGUI_DIR}/imgui_tables.cpp 
    ${IMGUI_DIR}/imgui_widgets.cpp 
    ${IMGUI_DIR}/backends/imgui_impl_sdl2.cpp 
    ${IMGUI_DIR}/backends/imgui_impl_opengl3.cpp
)

# Executable
add_executable(ConsoleCraftEditor ${SOURCES})

# Link libraries
target_link_libraries(ConsoleCraftEditor ConsoleCraftEngine ${SDL2_LIBRARIES} GL ncurses)
