// compile arguments :
// -lsfml-window -lsfml-graphics -lsfml-system -framework OpenGL

#define GL_SILENCE_DEPRECATION
#include "eg.hpp"
#include "eg/debug_window.hpp"
#include "eg/debug.hpp"

eg::debug::debug_context debug;

void enterframe( eg::debug::window_context &w )
{
  glClearColor( 0, 0, 0, 0 );
  glClear( GL_COLOR_BUFFER_BIT );
  debug.begin( GL_TRIANGLES );
  debug.vertex2f( 0.0f, 0.0f );
  debug.set_color( 1.0, 0.0, 0.0 );
  debug.vertex2f( 1.0f, 0.0f );
  debug.set_color( 0.0f, 1.0f, 0.0 );
  debug.vertex2f( 1.0f, 1.0f );
  debug.set_color( 0.0f, 0.0f, 1.0f );
  debug.end();
  w.swap_buffer();
}
void event( eg::debug::window_context &w )
{
  if( w.event().type == sf::Event::Closed )
  {
    w.close();
  }
}
int main()
{
  eg::debug::window_context window(800,800,"hello window");
  window.set_enterframe( enterframe );
  window.set_event( event );
  debug.init();
  std::cout << "OpenGL Version " << eg::context.major_version() << "." << eg::context.minor_version() << std::endl;
  std::cout << "Vendor : " << eg::context.vendor() << std::endl;
  std::cout << "Renderer : " << eg::context.renderer() << std::endl;
  std::cout << "Shader : " << eg::context.shading_version() << std::endl;
  window.run();
  return 0;
}
