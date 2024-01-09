// MANDELBROT Fractals Fragment shader

#define GL_SILENCE_DEPRECATION
// place your opengl header in <eg/gl.hpp>
#include <eg.hpp>
#include <vector>
#include <fstream>


const char* vertex_shader =
"#version 330 core\n"
"\n"
"void main(){\n"
  "const vec2 vertices[4] = vec2[](\n"
    "vec2(-1.0,-1.0),\n"
    "vec2(+1.0,-1.0),\n"
    "vec2(-1.0,+1.0),\n"
    "vec2(+1.0,+1.0)\n"
  ");\n"
  "gl_Position = vec4( vertices[gl_VertexID], 0.0, 1.0 );\n"
"}\n";
const char *fragment_shader =
"#version 330 core\n"
"out vec4 outcol;\n"
"\n"
"const float M_PI = 3.141592;\n"
"vec3 colormap( float x )\n"
"{\n"
  "vec3 factor = vec3(\n"
    "clamp( cos( (x-0.25)*M_PI/0.3 ), 0.0, 1.0 ),\n"
    "clamp( cos( (x-0.50)*M_PI/0.6 ), 0.0, 1.0 ),\n"
    "clamp( cos( (x-0.75)*M_PI/0.5 ), 0.0, 1.0 )\n"
  ");\n"
  "return factor;\n"
  "//return vec3(x);\n"
"}\n"
"\n"
"// z_{n+1} = (z_n)^2 + c\n"
"vec2 iterate( vec2 z, vec2 c )\n"
"{\n"
"vec2 z2 = vec2( z.x*z.x - z.y*z.y, 2.0*z.x*z.y );\n"
"return z2 + c;\n"
"}\n"
"float threshold_sq = 4.0;\n"
"const int MAXIT = 100;\n"
"// return iteration count\n"
"int mandelbrot_iterate( vec2 c )\n"
"{\n"
	"vec2 z = vec2(0.0);\n"
	"for( int it=0; it<MAXIT; ++it )\n"
	"{\n"
		"if( dot(z,z) > threshold_sq ){ return it; }\n"
		"z = iterate( z, c );\n"
	"}\n"
	"return MAXIT;\n"
"}\n"
"void main( void ) {\n"
	"vec2 scale_factor = vec2( 4.0 );\n"
	"vec2 position = (( gl_FragCoord.xy / vec2(800.0,800.0) )-vec2(0.5))*scale_factor;\n"
	"int i = mandelbrot_iterate( position );\n"
"\n"
	"outcol = vec4( colormap(float(i)/float(MAXIT)), 1.0);\n"
"\n"
"}\n";

eg::Program program;
eg::VertexArray varray;
eg::debug::window_context window;

// event callback function
void event( eg::debug::window_context &w )
{
  if( w.event().type == sf::Event::Closed )
  {
    w.close();
  }
}

// enterframe function; called every 1/60 sec
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
  window.create(800,800,"hello Mandelbrot" );
  window.set_enterframe( enterframe );
  window.set_event( event );
  std::cout << "OpenGL Version " << eg::context.major_version() << "." << eg::context.minor_version() << std::endl;
  std::cout << "Vendor : " << eg::context.vendor() << std::endl;
  std::cout << "Renderer : " << eg::context.renderer() << std::endl;
  std::cout << "Shader : " << eg::context.shading_version() << std::endl;

  // program creation & linkage
  {
    auto vertex = eg::make_shader( GL_VERTEX_SHADER );
    vertex.source( vertex_shader );
    vertex.compile();

    auto fragment = eg::make_shader( GL_FRAGMENT_SHADER );
    fragment.source( fragment_shader );
    fragment.compile();

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
