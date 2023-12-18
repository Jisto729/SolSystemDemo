# Based on a template from the GPUengine GeGL example

cmake_minimum_required(VERSION 3.0)

set(PROJECT_NAME "SolSystemDemo")
set(APP_NAME "SolSystemDemo")
project(${PROJECT_NAME})

# Needed for automatic handling of Qt based classes (see Qt documentation).
set( CMAKE_AUTORCC ON )
set( CMAKE_AUTOUIC ON )
set( CMAKE_AUTOMOC ON )

SET(CMAKE_DEBUG_POSTFIX "d" CACHE STRING "add a postfix, usually d on windows")
SET(CMAKE_RELEASE_POSTFIX "" CACHE STRING "add a postfix, usually empty on windows")
SET(CMAKE_RELWITHDEBINFO_POSTFIX "rd" CACHE STRING "add a postfix, usually empty on windows")
SET(CMAKE_MINSIZEREL_POSTFIX "s" CACHE STRING "add a postfix, usually empty on windows")


SET(CMAKE_MODULE_PATH "${PROJECT_SOURCE_DIR}/CMakeModules;${CMAKE_MODULE_PATH}")


find_package(Qt5 COMPONENTS Widgets REQUIRED)
find_package(GPUEngine COMPONENTS geGL geUtil geCore)

set(DEFAULT_RESOURCES_PATH "${CMAKE_CURRENT_LIST_DIR}/resources")

find_file(vertexShader vert.glsl
        HINTS ${DEFAULT_RESOURCES_PATH}/shaders
)

find_file(fragmentShader frag.glsl
        HINTS ${DEFAULT_RESOURCES_PATH}/shaders
)

set(${APP_NAME}_RESOURCES "${DEFAULT_RESOURCES_PATH}" CACHE PATH "Relative or absolute path to Application resources.")

set(HEADER_PATH src)
set(SOURCE_PATH src)
get_filename_component(src_abs ${SOURCE_PATH} ABSOLUTE)

set(SOURCE_FILES
   src/main.cpp
   src/SolDemoWindow.h src/SolDemoWindow.cpp
   src/EventHandler.h src/EventHandler.cpp
   src/Scene.h src/Scene.cpp
   src/SceneObject.h src/SceneObject.cpp
   src/Renderer.h src/Renderer.cpp
   src/Camera.h src/Camera.cpp
   #TODO remove
   src/Sphere.h src/Sphere.cpp
)


add_executable(${APP_NAME} ${SOURCE_FILES})
#preferable linking
target_link_libraries(${APP_NAME} Qt5::Widgets geGL geUtil geCore)


target_include_directories(${APP_NAME} PUBLIC "src/")
add_compile_definitions(FRAGMENT_SHADER=\"${fragmentShader}\" VERTEX_SHADER=\"${vertexShader}\")

set_target_properties(${APP_NAME} PROPERTIES COMPILE_DEFINITIONS "APP_RESOURCES=\"${${APP_NAME}_RESOURCES}\"")
# set_property(TARGET ${APP_NAME} PROPERTY FOLDER "${examples_folder_name}")


# setting up the MSVC helper var
get_target_property(Qt5dllPath Qt5::Widgets IMPORTED_LOCATION_RELEASE)
get_filename_component(Qt5dllDir ${Qt5dllPath} DIRECTORY)
get_target_property(GPUEdllPath geGL IMPORTED_LOCATION_RELEASE)
set_target_properties(${APP_NAME} PROPERTIES VS_GLOBAL_DllPath "${OUTPUT_BINDIR};${Qt5dllDir}")