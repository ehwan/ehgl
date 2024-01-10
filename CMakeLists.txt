cmake_minimum_required(VERSION 3.5)

# clangd
set( CMAKE_EXPORT_COMPILE_COMMANDS ON )

# for Eigen and SFML
include_directories( /opt/homebrew/include )

project( ehgl CXX )
add_library( ehgl INTERFACE )
set_target_properties(
  ehgl
  PROPERTIES
  CXX_STANDARD 17
)
target_include_directories(
  ehgl
  INTERFACE
  .
)

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
set_target_properties( mandelbrot
  PROPERTIES
  LANGUAGE CXX
  CXX_STANDARD 17
)

project( cubes )
add_executable( cubes  
  example/3d/cubes.cpp
)
target_link_libraries( cubes PUBLIC
  ehgl
  sfml-window
  sfml-graphics
  sfml-system
)
set_target_properties( cubes 
  PROPERTIES
  LANGUAGE CXX
  CXX_STANDARD 17
)

if( APPLE )
  target_link_libraries( mandelbrot PUBLIC
    "-framework OpenGL"
  )
  target_link_libraries( cubes PUBLIC
    "-framework OpenGL"
  )
else()
  target_link_libraries( cubes PUBLIC
    GL
  )
endif()