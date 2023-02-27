// compile arguments :
// -lsfml-window -lsfml-graphics -lsfml-system -framework OpenGL

#define GL_SILENCE_DEPRECATION
#include "../eg.hpp"
#include <vector>
#include <fstream>

eg::Program program;
eg::VertexArray varray;
eg::debug::window_context window;

std::vector<char> loadfile( char const* name )
{
  std::ifstream stream(name);
  stream.seekg( 0, stream.end );
  size_t len = stream.tellg();
  std::vector<char> buf(len+1);
  buf.back() = (char)0;
  stream.seekg( 0, stream.beg );
  stream.read( buf.data(), len );
  return buf;
}

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

  {
    eg::Shader vertex = eg::make_vertex_shader();
    auto buf = loadfile( "vert.glsl" );
    vertex.source( buf.data() );
    if( vertex.compile() == false )
    {
      std::cout << "VertexShader : \n" << vertex.errorMessage() << std::endl;
      return 0;
    }
    eg::Shader fragment = eg::make_fragment_shader();
    buf = loadfile( "frag.glsl" );
    fragment.source( buf.data() );
    if( fragment.compile() == false )
    {
      std::cout << "Fragment : \n" << fragment.errorMessage() << std::endl;
      return 0;
    }
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
