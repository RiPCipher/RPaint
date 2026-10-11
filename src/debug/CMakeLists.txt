## Debug CMakeLists

add_library(_debug debug.cpp)
target_compile_features(_debug PUBLIC cxx_std_20)
target_include_directories(_debug PUBLIC ${CMAKE_SOURCE_DIR}/src)
target_link_libraries(_debug PUBLIC glfw glad)