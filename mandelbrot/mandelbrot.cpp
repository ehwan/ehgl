// compile arguments :
// -lsfml-window -lsfml-graphics -lsfml-system -framework OpenGL

#define GL_SILENCE_DEPRECATION
#include "../eg.hpp"
#include <vector>
#include <fstream>

eg::Program program;
eg::VertexArray varray;
eg::debug::window_context window;

void event( eg::debug::window_context &w )
{
  if( w.event().type == sf::Event::Closed )
  {
    w.close();
  }
}
void enterframe( eg::debug::window_context &w )
{
  glDisable( GL_CULL_FACE );
  glClear( GL_COLOR_BUFFER_BIT );
  program.bind();
  varray.bind();
  glDrawArrays( GL_TRIANGLE_STRIP, 0, 4 );
  w.swap_buffer();
}

int main()
{
  window.create(800,800,"hello OpenGL" );
  window.set_enterframe( enterframe );
  window.set_event( event );
  std::cout << "OpenGL Version " << eg::context.major_version() << "." << eg::context.minor_version() << std::endl;
  std::cout << "Vendor : " << eg::context.vendor() << std::endl;
  std::cout << "Renderer : " << eg::context.renderer() << std::endl;
  std::cout << "Shader : " << eg::context.shading_version() << std::endl;

  // program creation & linkage
  {
    auto vertex = eg::load_shader( "vert.glsl", GL_VERTEX_SHADER );
    auto fragment = eg::load_shader( "frag.glsl", GL_FRAGMENT_SHADER );
    program = eg::make_program();
    program.attachShader( vertex );
    program.attachShader( fragment );
    program.bindFragDataLocation( 0, "outcol" );
    if( program.link_detach() == false )
    {
      std::cout << "program link failed : \n" << program.errorMessage() << std::endl;
      return 0;
    }
  }
  varray = eg::make_vertex_array();
  window.run();

  return 0;
}
