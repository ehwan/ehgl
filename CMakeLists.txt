cmake_minimum_required(VERSION 3.5)

# clangd
set( CMAKE_EXPORT_COMPILE_COMMANDS ON )

project( ehgl CXX )
add_library( ehgl INTERFACE )
target_include_directories(
  ehgl
  INTERFACE
  .
)
set_target_properties(
  ehgl
  PROPERTIES
  CXX_STANDARD 17
)

project( mandelbrot )
add_executable( mandelbrot
  example/mandelbrot/mandelbrot.cpp
)
find_package( SFML COMPONENTS window graphics system REQUIRED )
find_package( OpenGL REQUIRED )
target_link_libraries( mandelbrot PUBLIC
  ehgl
  OpenGL::GL
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
find_package( SFML COMPONENTS window graphics system REQUIRED )
find_package( OpenGL REQUIRED )
target_link_libraries( cubes PUBLIC
  ehgl
  OpenGL::GL
  sfml-window
  sfml-graphics
  sfml-system
)
set_target_properties( cubes 
  PROPERTIES
  LANGUAGE CXX
  CXX_STANDARD 17
)