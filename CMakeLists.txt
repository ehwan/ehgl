cmake_minimum_required(VERSION 3.0)
set( CMAKE_EXPORT_COMPILE_COMMANDS ON )
set( LANGUAGE CXX )

add_compile_options( -O2 -std=c++17 )

# for Eigen and SFML
include_directories( /opt/homebrew/include )

project( ehgl )
add_library( ehgl STATIC eg/gl.cpp )

project( mandelbrot )
add_executable( mandelbrot
  example/mandelbrot/mandelbrot.cpp
)
target_link_libraries( mandelbrot PUBLIC
  ehgl
  sfml-window
  sfml-graphics
  sfml-system
)

if( APPLE )
  target_link_libraries( mandelbrot PUBLIC
    "-framework OpenGL"
  )
else()
  target_link_libraries( mandelbrot PUBLIC
    GL
  )
endif()