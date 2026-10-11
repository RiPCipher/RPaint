## Core CMakeLists

add_library(core
    io/json.cpp

    io/file_io.h io/iimage.h io/json.h

    math/math.h math/matrix.h math/transform.h math/vector.h math/view.h)
target_include_directories(core PUBLIC ${CMAKE_SOURCE_DIR}/src)
