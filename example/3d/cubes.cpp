#define GL_SILENCE_DEPRECATION
// place your opengl header in <eg/gl.hpp>
#include <eg.hpp>
#include <iostream>

eg::Program program;
eg::VertexArray varray;
eg::Buffer vbuffer, nbuffer;
eg::Buffer pbuffer;
eg::debug::window_context window;
eg::EyeAngle eye;
eg::Perspective projection;

const int CUBE_COUNT = 5;

// Vertex shader source code
const char* vertex_shader_source = R"(
#version 330 core

layout( location=0 ) in vec3 invec;
layout( location=1 ) in vec3 innormal;
layout( location=2 ) in vec3 aPos;

uniform mat4 u_eye;
uniform mat4 u_projection;

smooth out vec3 normal;
out vec3 color;
out vec3 pos;

void main(){
  normal = innormal;
  vec3 pp = invec*0.13 + aPos;
  pos = pp;
  gl_Position = u_projection*u_eye*vec4(pp,1.0);

  color =  vec3(0.6,0.8,1.0);
})";

// Fragment shader source c
const char* fragment_shader_source = R"(
#version 330 core

uniform vec3 light0;
uniform vec3 lightvec;

smooth in vec3 normal;
in vec3 color;
in vec3 pos;

out vec4 outcol;

void main( void ) {
  vec3 light0 = vec3(0,10,0);
  vec3 lightin = pos - light0;
  vec3 lightout = reflect( lightin, normalize(normal) );
  float diff = -dot(lightin,normal);
  float spec = clamp(dot(light0-pos,lightout),0,1);
  if( diff < 0 ){ spec = 0; }
  diff = clamp( diff,0,1 );
  vec3 col0 = color;
  outcol = vec4( col0*(0.4 + diff*0.8) + vec3(1)*spec*0.3, 1 );
}
)";

// Function to move the camera based on keyboard and mouse input
void move()
{
  {
    float spd = 1.0f / 60.0f;
    float anglespd = 1.0f / 60.0f;
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::S))
    {
      if (sf::Mouse::isButtonPressed(sf::Mouse::Left))
      {
        auto angle = eye.angle();
        angle.x() -= spd;
        eye.angle(angle);
      }
      else
      {
        eye.move(2, spd);
      }
    }
    else if (sf::Keyboard::isKeyPressed(sf::Keyboard::W))
    {
      if (sf::Mouse::isButtonPressed(sf::Mouse::Left))
      {
        auto angle = eye.angle();
        angle.x() += spd;
        eye.angle(angle);
      }
      else
      {
        eye.move(2, -spd);
      }
    }
    else if (sf::Keyboard::isKeyPressed(sf::Keyboard::A))
    {
      if (sf::Mouse::isButtonPressed(sf::Mouse::Left))
      {
        auto angle = eye.angle();
        angle.y() += spd;
        eye.angle(angle);
      }
      else
      {
        eye.move(0, -spd);
      }
    }
    else if (sf::Keyboard::isKeyPressed(sf::Keyboard::D))
    {
      if (sf::Mouse::isButtonPressed(sf::Mouse::Left))
      {
        auto angle = eye.angle();
        angle.y() -= spd;
        eye.angle(angle);
      }
      else
      {
        eye.move(0, +spd);
      }
    }
  }
}

// Function to handle window events
void event(eg::debug::window_context& w)
{
  if (w.event().type == sf::Event::Closed)
  {
    w.close();
  }
}

// Function called every frame, responsible for camera movement and rendering
void enterframe(eg::debug::window_context& w)
{
  move();

  glDisable(GL_CULL_FACE);
  glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
  program.bind();
  program.uniformLocation("u_eye").matrix<4>(&eye()(0, 0));
  program.uniformLocation("u_projection").matrix<4>(&projection()(0, 0));
  program.uniformLocation("light0").v<3>(&eye.position()(0));
  Eigen::Vector3f lightvec = -eye.axis(2);
  program.uniformLocation("lightvec").v<3>(&lightvec(0));
  varray.bind();
  eg::VertexAttrib(0).enableArray();
  eg::VertexAttrib(1).enableArray();
  eg::VertexAttrib(2).enableArray();
  eg::VertexAttrib(2).divisor(1);

  glDrawArraysInstanced(GL_TRIANGLES, 0, 36, CUBE_COUNT);
  // glDrawArrays( GL_TRIANGLES, 0, 24 );
  w.swap_buffer();
}

int main(int argc, char** argv)
{
  window.create(1000, 1000, "hello OpenGL");
  window.set_enterframe(enterframe);
  window.set_event(event);
  std::cout << "OpenGL Version " << eg::context.major_version() << "."
            << eg::context.minor_version() << std::endl;
  std::cout << "Vendor : " << eg::context.vendor() << std::endl;
  std::cout << "Renderer : " << eg::context.renderer() << std::endl;
  std::cout << "Shader : " << eg::context.shading_version() << std::endl;

  //
  // Initialize shaders and variables
  //
  {
    auto vertex = eg::make_shader(GL_VERTEX_SHADER);
    vertex.source(vertex_shader_source);
    vertex.compile();

    auto fragment = eg::make_shader(GL_FRAGMENT_SHADER);
    fragment.source(fragment_shader_source);
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

  //
  // Initialize Vertex Array and Buffers
  //
  varray = eg::make_vertex_array();
  program.bind();
  varray.bind();
  vbuffer = eg::make_buffer();
  nbuffer = eg::make_buffer();
  pbuffer = eg::make_buffer();

  // cube vertices
  const GLfloat datas[] = {
    -1, -1, 1,  +1, -1, 1,  -1, +1, 1,  -1, +1, 1,  +1, -1, 1,  +1, +1, 1,

    -1, -1, -1, +1, -1, -1, -1, +1, -1, -1, +1, -1, +1, -1, -1, +1, +1, -1,

    -1, -1, -1, -1, -1, +1, -1, +1, -1, -1, +1, -1, -1, -1, +1, -1, +1, +1,

    1,  -1, +1, 1,  -1, -1, 1,  +1, +1, 1,  +1, +1, 1,  -1, -1, 1,  +1, -1,

    -1, 1,  +1, +1, 1,  +1, -1, 1,  -1, -1, 1,  -1, +1, 1,  +1, +1, 1,  -1,

    -1, -1, +1, +1, -1, +1, -1, -1, -1, -1, -1, -1, +1, -1, +1, +1, -1, -1,
  };

  // cube normals
  const GLfloat normals[] = {
    0,  0,  1,  0,  0,  1,  0,  0,  1,  0,  0,  1,  0,  0,  1,  0,  0,  1,

    0,  0,  -1, 0,  0,  -1, 0,  0,  -1, 0,  0,  -1, 0,  0,  -1, 0,  0,  -1,

    -1, 0,  0,  -1, 0,  0,  -1, 0,  0,  -1, 0,  0,  -1, 0,  0,  -1, 0,  0,

    1,  0,  0,  1,  0,  0,  1,  0,  0,  1,  0,  0,  1,  0,  0,  1,  0,  0,

    0,  1,  0,  0,  1,  0,  0,  1,  0,  0,  1,  0,  0,  1,  0,  0,  1,  0,

    0,  -1, 0,  0,  -1, 0,  0,  -1, 0,  0,  -1, 0,  0,  -1, 0,  0,  -1, 0,
  };
  eg::array_buffer.bind(vbuffer);
  eg::array_buffer.data(GL_STATIC_DRAW, datas);

  eg::array_buffer.bind(nbuffer);
  eg::array_buffer.data(GL_STATIC_DRAW, normals);

  const GLfloat cube_pos[CUBE_COUNT * 3]
      = { 0.0, 0.0, 0.0, 0.2, 0.0, 0.0, 0.4, 0.0,
          0.0, 0.2, 0.5, 0.0, 0.3, 0.1, 0.2 };

  eg::array_buffer.bind(pbuffer);
  eg::array_buffer.data(GL_DYNAMIC_DRAW, sizeof(float) * 3 * CUBE_COUNT,
                        cube_pos);

  auto va = eg::VertexAttrib(0);
  eg::array_buffer.bind(vbuffer);
  va.enableArray();
  va.pointer(3, GL_FLOAT);

  auto na = eg::VertexAttrib(1);
  eg::array_buffer.bind(nbuffer);
  na.enableArray();
  na.pointer(3, GL_FLOAT);

  auto pa = eg::VertexAttrib(2);
  eg::array_buffer.bind(pbuffer);
  pa.enableArray();
  pa.divisor(1);
  pa.pointer(3, GL_FLOAT);

  //
  // Set up camera and projection matrix
  //

  // set local basis
  eye.set({ 1, 0, 0 }, { 0, 1, 0 }, { 0, 0, 1 });

  // set camera origin
  eye.position({ 0.2, 1.0, 2.0 });

  // set angle from each axis
  eye.angle({ -0.5, -0.4, 0 });
  // projection.frustum( {-0.3,-0.3}, {1.3,1.3}, 1, 5 );

  // perspective matrix
  projection.perspective(3.141592f / 2.0f, 1.0f, 0.1, 5.0f);

  glEnable(GL_DEPTH_TEST);

  window.run();

  return 0;
}
