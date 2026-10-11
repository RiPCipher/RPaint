## Services CMakeLists

## Render Service
add_library(render
    render/render.cpp render/mesh.cpp render/shader.cpp render/texture.cpp render/material.cpp render/light.cpp render/_defaults.cpp

    render/render.h render/mesh.h render/shader.h render/texture.h render/material.h render/light.h render/_defaults.h

    ${CMAKE_SOURCE_DIR}/src/core/io/iimage.h)
target_include_directories(render PUBLIC ${CMAKE_SOURCE_DIR}/src)
target_link_libraries(render PUBLIC glad _debug core)
