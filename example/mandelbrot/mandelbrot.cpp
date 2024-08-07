// MANDELBROT Fractals Fragment shader

#define GL_SILENCE_DEPRECATION
// place your opengl header in <eg/gl.hpp>
#include <eg.hpp>

const char* vertex_shader = R"(
#version 330 core

void main() {
  const vec2 vertices[4] = vec2[](
    vec2(-1.0,-1.0),
    vec2(+1.0,-1.0),
    vec2(-1.0,+1.0),
    vec2(+1.0,+1.0)
  );
  gl_Position = vec4( vertices[gl_VertexID], 0.0, 1.0 );
}
)";

const char* fragment_shader = R"(
#version 330 core

out vec4 outcol;

const float M_PI = 3.141592;

vec3 colormap( float x )
{
  vec3 factor = vec3(
    clamp( cos( (x-0.25)*M_PI/0.3 ), 0.0, 1.0 ),
    clamp( cos( (x-0.50)*M_PI/0.6 ), 0.0, 1.0 ),
    clamp( cos( (x-0.75)*M_PI/0.5 ), 0.0, 1.0 )
  );
  return factor;
}

// z_{n+1} = (z_n)^2 + c
vec2 iterate( vec2 z, vec2 c )
{
  vec2 z2 = vec2( z.x*z.x - z.y*z.y, 2.0*z.x*z.y );
  return z2 + c;
}

float threshold_sq = 4.0;
const int MAXIT = 100;

// return iteration count
int mandelbrot_iterate( vec2 c )
{
  vec2 z = vec2(0.0);
  for( int it=0; it<MAXIT; ++it )
  {
    if( dot(z,z) > threshold_sq ){ return it; }
    z = iterate( z, c );
  }
  return MAXIT;
}
void main( void ) {
  vec2 scale_factor = vec2( 4.0 );
  vec2 position = (
    ( gl_FragCoord.xy / vec2(800.0,800.0) ) - vec2(0.5) 
  )*scale_factor;
  int i = mandelbrot_iterate( position );

  outcol = vec4( colormap(float(i)/float(MAXIT)), 1.0);
}
)";

eg::Program program;
eg::VertexArray varray;
eg::debug::window_context window;

// event callback function
void event(eg::debug::window_context& w)
{
  if (w.event().type == sf::Event::Closed)
  {
    w.close();
  }
}

// enterframe function; called every 1/60 sec
void enterframe(eg::debug::window_context& w)
{
  glDisable(GL_CULL_FACE);
  glClear(GL_COLOR_BUFFER_BIT);
  program.bind();
  varray.bind();
  glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);
  w.swap_buffer();
}

int main()
{
  window.create(800, 800, "hello Mandelbrot");
  window.set_enterframe(enterframe);
  window.set_event(event);
  std::cout << "OpenGL Version " << eg::context.major_version() << "."
            << eg::context.minor_version() << std::endl;
  std::cout << "Vendor : " << eg::context.vendor() << std::endl;
  std::cout << "Renderer : " << eg::context.renderer() << std::endl;
  std::cout << "Shader : " << eg::context.shading_version() << std::endl;

  // program creation & linkage
  {
    auto vertex = eg::make_shader(GL_VERTEX_SHADER);
    vertex.source(vertex_shader);
    vertex.compile();

    auto fragment = eg::make_shader(GL_FRAGMENT_SHADER);
    fragment.source(fragment_shader);
    fragment.compile();

    program = eg::make_program();
    program.attachShader(vertex);
    program.attachShader(fragment);
    program.bindFragDataLocation(0, "outcol");
    if (program.link_detach() == false)
    {
      std::cout << "program link failed : \n"
                << program.errorMessage() << std::endl;
      return 0;
    }
  }
  varray = eg::make_vertex_array();
  window.run();

  return 0;
}
