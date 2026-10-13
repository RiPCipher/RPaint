cmake_minimum_required(VERSION 3.28)
project(RPAINT LANGUAGES CXX C)
set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)
include(FetchContent)

## Setup GLFW
FetchContent_Declare(
    glfw
    GIT_REPOSITORY https://github.com/glfw/glfw.git
    GIT_TAG 3.4
    GIT_SHALLOW ON
)

set(GLFW_BUILD_DOCS OFF CACHE BOOL "" FORCE)
set(GLFW_BUILD_TESTS OFF CACHE BOOL "" FORCE)
set(GLFW_BUILD_EXAMPLES OFF CACHE BOOL "" FORCE)

FetchContent_MakeAvailable(glfw)

## Setup Assimp
FetchContent_Declare(
    assimp
    GIT_REPOSITORY https://github.com/assimp/assimp.git
    GIT_TAG v6.0.2
    GIT_SHALLOW ON
)

set(BUILD_SHARED_LIBS OFF CACHE BOOL "" FORCE)
set(ASSIMP_BUILD_TESTS OFF CACHE BOOL "" FORCE)
set(ASSIMP_BUILD_ASSIMP_TOOLS OFF CACHE BOOL "" FORCE)
set(ASSIMP_BUILD_SAMPLES OFF CACHE BOOL "" FORCE)
set(ASSIMP_INSTALL OFF CACHE BOOL "" FORCE)
set(ASSIMP_NO_EXPORT ON CACHE BOOL "" FORCE)
set(ASSIMP_WARNINGS_AS_ERRORS OFF CACHE BOOL "" FORCE)
set(ASSIMP_BUILD_ALL_IMPORTERS_BY_DEFAULT OFF CACHE BOOL "" FORCE)
foreach(fmt OBJ FBX GLTF COLLADA PLY STL 3DS BLEND)
    set(ASSIMP_BUILD_${fmt}_IMPORTER ON CACHE BOOL "" FORCE)
endforeach()

FetchContent_MakeAvailable(assimp)

## Setup OpenGL
add_library(glad STATIC third_party/glad/src/glad.c)
target_include_directories(glad PUBLIC third_party/glad/include)

## Add STB
add_library(stb STATIC src/extensions/stb/ext_stb.cpp src/extensions/stb/stb_image_loader.cpp)
target_include_directories(stb PUBLIC src/extensions/stb ${CMAKE_SOURCE_DIR}/src)

## Add Assimp mesh loader
add_library(assimp_loader STATIC src/extensions/assimp/assimp_loader.cpp)
target_include_directories(assimp_loader PUBLIC src/extensions/assimp ${CMAKE_SOURCE_DIR}/src)
target_compile_features(assimp_loader PUBLIC cxx_std_20)
target_link_libraries(assimp_loader PRIVATE assimp::assimp)


## Build
add_executable(rpaint src/main/main.cpp src/main/demo_scene.cpp src/core/app/app.cpp)
target_compile_features(rpaint PUBLIC cxx_std_20)
add_subdirectory(src/debug)
add_subdirectory(src/core)
add_subdirectory(src/services)

target_compile_definitions(rpaint PRIVATE GLFW_INCLUDE_NONE)
target_link_libraries(rpaint PRIVATE glfw glad stb assimp_loader _debug core render paint)

## Copy Assets Directory
add_custom_target(copy_assets ALL
    COMMAND ${CMAKE_COMMAND} -E copy_directory_if_different
        ${CMAKE_SOURCE_DIR}/assets
        $<TARGET_FILE_DIR:rpaint>/assets/
    COMMENT "Copying assets directory to output"
)
add_dependencies(rpaint copy_assets)