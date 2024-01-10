#define GL_SILENCE_DEPRECATION
#include <eg.hpp>
#include <vector>
#include <iostream>
#include <fstream>

eg::Program program;
eg::VertexArray varray;
eg::Buffer vbuffer, nbuffer;
eg::Buffer pbuffer;
eg::debug::window_context window;
eg::EyeAngle eye;
eg::Perspective projection;

const int CUBE_COUNT = 5;

const char* vertex_shader_source =
"#version 330 core\n"
"\n"
"layout( location=0 ) in vec3 invec;\n"
"layout( location=1 ) in vec3 innormal;\n"
"layout( location=2 ) in vec3 aPos;\n"
"\n"
"uniform mat4 u_eye;\n"
"uniform mat4 u_projection;\n"
"\n"
"smooth out vec3 normal;\n"
"out vec3 color;\n"
"out vec3 pos;\n"
"\n"
"void main(){\n"
  "normal = innormal;\n"
  "vec3 pp = invec*0.13 + aPos;\n"
  "pos = pp;\n"
  "gl_Position = u_projection*u_eye*vec4(pp,1.0);\n"
"\n"
  "color =  vec3(0.6,0.8,1.0);\n"
"}\n";

const char *fragment_shader_source =
"#version 330 core\n"
"\n"
"uniform vec3 light0;\n"
"uniform vec3 lightvec;\n"
"\n"
"smooth in vec3 normal;\n"
"in vec3 color;\n"
"in vec3 pos;\n"
"\n"
"out vec4 outcol;\n"
"\n"
"void main( void ) {\n"
  "vec3 light0 = vec3(0,10,0);\n"
  "vec3 lightin = pos - light0;\n"
  "vec3 lightout = reflect( lightin, normalize(normal) );\n"
  "float diff = -dot(lightin,normal);\n"
  "float spec = clamp(dot(light0-pos,lightout),0,1);\n"
  "if( diff < 0 ){ spec = 0; }\n"
  "diff = clamp( diff,0,1 );\n"
  "vec3 col0 = color;\n"
  "outcol = vec4( col0*(0.4 + diff*0.8) + vec3(1)*spec*0.3, 1 );\n"
"}\n";



void move()
{
  {
    float spd = 1.0f/60.0f;
    float anglespd = 1.0f/60.0f;
    if( sf::Keyboard::isKeyPressed( sf::Keyboard::S ) )
    {
      if( sf::Mouse::isButtonPressed( sf::Mouse::Left ) )
      {
        auto angle = eye.angle();
        angle.x() -= spd;
        eye.angle(angle);
      }else
      {
        eye.move(2,spd);
      }
    }else if( sf::Keyboard::isKeyPressed( sf::Keyboard::W ) )
    {
      if( sf::Mouse::isButtonPressed( sf::Mouse::Left ) )
      {
        auto angle = eye.angle();
        angle.x() += spd;
        eye.angle(angle);
      }else
      {
        eye.move(2,-spd);
      }
    }else if( sf::Keyboard::isKeyPressed( sf::Keyboard::A ) )
    {
      if( sf::Mouse::isButtonPressed( sf::Mouse::Left ) )
      {
        auto angle = eye.angle();
        angle.y() += spd;
        eye.angle(angle);
      }else
      {
        eye.move(0,-spd);
      }
    }else if( sf::Keyboard::isKeyPressed( sf::Keyboard::D ) )
    {
      if( sf::Mouse::isButtonPressed( sf::Mouse::Left ) )
      {
        auto angle = eye.angle();
        angle.y() -= spd;
        eye.angle(angle);
      }else
      {
        eye.move(0,+spd);
      }
    }
  }
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
  move();

  glDisable( GL_CULL_FACE );
  glClear( GL_COLOR_BUFFER_BIT|GL_DEPTH_BUFFER_BIT );
  program.bind();
  program.uniformLocation( "u_eye" ).matrix<4>( &eye()(0,0) );
  program.uniformLocation( "u_projection" ).matrix<4>( &projection()(0,0) );
  program.uniformLocation( "light0" ).v<3>( &eye.position()(0) );
  Eigen::Vector3f lightvec = -eye.axis(2);
  program.uniformLocation( "lightvec" ).v<3>( &lightvec(0) );
  varray.bind();
  eg::VertexAttrib(0).enableArray();
  eg::VertexAttrib(1).enableArray();
  eg::VertexAttrib(2).enableArray();
  eg::VertexAttrib(2).divisor(1);

  glDrawArraysInstanced( GL_TRIANGLES, 0, 36, CUBE_COUNT );
  //glDrawArrays( GL_TRIANGLES, 0, 24 );
  w.swap_buffer();
}

int main( int argc, char **argv )
{
  window.create(1000,1000,"hello OpenGL" );
  window.set_enterframe( enterframe );
  window.set_event( event );
  std::cout << "OpenGL Version " << eg::context.major_version() << "." << eg::context.minor_version() << std::endl;
  std::cout << "Vendor : " << eg::context.vendor() << std::endl;
  std::cout << "Renderer : " << eg::context.renderer() << std::endl;
  std::cout << "Shader : " << eg::context.shading_version() << std::endl;

  {
    auto vertex = eg::make_shader( GL_VERTEX_SHADER );
    vertex.source( vertex_shader_source );
    vertex.compile();

    auto fragment = eg::make_shader( GL_FRAGMENT_SHADER );
    fragment.source( fragment_shader_source );
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
  program.bind();
  varray.bind();
  vbuffer = eg::make_buffer();
  nbuffer = eg::make_buffer();
  pbuffer = eg::make_buffer();

  // cube vertices
  const GLfloat datas[] =
  {
    -1, -1, 1,
    +1, -1, 1,
    -1, +1, 1,
    -1, +1, 1,
    +1, -1, 1,
    +1, +1, 1,

    -1, -1, -1,
    +1, -1, -1,
    -1, +1, -1,
    -1, +1, -1,
    +1, -1, -1,
    +1, +1, -1,

    -1, -1, -1,
    -1, -1, +1,
    -1, +1, -1,
    -1, +1, -1,
    -1, -1, +1,
    -1, +1, +1,

    1, -1, +1,
    1, -1, -1,
    1, +1, +1,
    1, +1, +1,
    1, -1, -1,
    1, +1, -1,

    -1, 1, +1,
    +1, 1, +1,
    -1, 1, -1,
    -1, 1, -1,
    +1, 1, +1,
    +1, 1, -1,

    -1, -1, +1,
    +1, -1, +1,
    -1, -1, -1,
    -1, -1, -1,
    +1, -1, +1,
    +1, -1, -1,
  };

  // cube normals
  const GLfloat normals[] =
  {
    0,0,1,
    0,0,1,
    0,0,1,
    0,0,1,
    0,0,1,
    0,0,1,

    0,0,-1,
    0,0,-1,
    0,0,-1,
    0,0,-1,
    0,0,-1,
    0,0,-1,

    -1,0,0,
    -1,0,0,
    -1,0,0,
    -1,0,0,
    -1,0,0,
    -1,0,0,

    1,0,0,
    1,0,0,
    1,0,0,
    1,0,0,
    1,0,0,
    1,0,0,

    0,1,0,
    0,1,0,
    0,1,0,
    0,1,0,
    0,1,0,
    0,1,0,

    0,-1,0,
    0,-1,0,
    0,-1,0,
    0,-1,0,
    0,-1,0,
    0,-1,0,
  };
  eg::array_buffer.bind( vbuffer );
  eg::array_buffer.data(GL_STATIC_DRAW,datas);

  eg::array_buffer.bind( nbuffer );
  eg::array_buffer.data(GL_STATIC_DRAW,normals);

  const GLfloat cube_pos[CUBE_COUNT*3] =
  {
    0.0, 0.0, 0.0,
    0.2, 0.0, 0.0,
    0.4, 0.0, 0.0,
    0.2, 0.5, 0.0,
    0.3, 0.1, 0.2
  };

  eg::array_buffer.bind( pbuffer );
  eg::array_buffer.data( GL_DYNAMIC_DRAW, sizeof(float)*3*CUBE_COUNT, cube_pos );

  auto va = eg::VertexAttrib(0);
  eg::array_buffer.bind( vbuffer );
  va.enableArray();
  va.pointer( 3, GL_FLOAT );

  auto na = eg::VertexAttrib(1);
  eg::array_buffer.bind( nbuffer );
  na.enableArray();
  na.pointer( 3, GL_FLOAT );

  auto pa = eg::VertexAttrib(2);
  eg::array_buffer.bind( pbuffer );
  pa.enableArray();
  pa.divisor( 1 );
  pa.pointer( 3, GL_FLOAT );

  // set local basis
  eye.set( {1,0,0}, {0,1,0}, {0,0,1} );

  // set camera origin
  eye.position( {0.2,1.0,2.0} );

  // set angle from each axis
  eye.angle( {-0.5,-0.4,0} );
  //projection.frustum( {-0.3,-0.3}, {1.3,1.3}, 1, 5 );

  // perspective matrix
  projection.perspective( 3.141592f/2.0f, 1.0f, 0.1, 5.0f );

  glEnable( GL_DEPTH_TEST );

  window.run();

  return 0;
}
