#pragma once

#include "gl.hpp"
#include <string>
#include <memory>
#include <initializer_list>
#include <algorithm>
#include <fcntl.h>
#include <unistd.h>

namespace eg
{

class Shader
{
  friend void swap( Shader& , Shader& );

  GLuint handler_;

  void release_if() const
  {
    if( handler_ )
    {
      glDeleteShader( handler_ );
    }
  }

public:
  EG_DEFINE_OBJECT_OPERATOR(Shader,GLuint,0,handler_)

  void source( GLsizei count , GLchar const* const* strings , GLint const* lengths = nullptr ) const
  {
    glShaderSource( get() , count , strings , lengths );
  }
  void source( GLchar const* string , GLint const length ) const
  {
    glShaderSource( get() , 1 , &string , &length );
  }
  void source( GLchar const* strings ) const
  {
    glShaderSource( get() , 1 , &strings , nullptr );
  }
  void source( std::initializer_list< GLchar const* > strings , std::initializer_list< GLint > lengths ) const
  {
    source( strings.size() , &(*strings.begin()) , &(*lengths.begin()) );
  }
  void source( std::initializer_list< GLchar const* > strings ) const
  {
    source( strings.size() , &(*strings.begin()) , nullptr );
  }

  bool compile() const
  {
    glCompileShader( get() );
    GLint ret;
    glGetShaderiv( get() , GL_COMPILE_STATUS , &ret );
    return ret != GL_FALSE;
  }
  std::string errorMessage() const
  {
    GLint length;
    glGetShaderiv( get() , GL_INFO_LOG_LENGTH , &length );

    if( length > 0 )
    {
      std::unique_ptr< char[] > buf( new char[ length ] );
      GLsizei read = 0;
      glGetShaderInfoLog( get() , length , &read , buf.get() );
      if( read > 0 )
      {
        return std::string( buf.get() , read );
      }
    }
    return std::string();
  }
};
inline void swap( Shader& l , Shader& r )
{
  std::swap( l.handler_ , r.handler_ );
}

inline Shader make_vertex_shader()
{
  return { glCreateShader( GL_VERTEX_SHADER ) };
}
inline Shader make_fragment_shader()
{
  return { glCreateShader( GL_FRAGMENT_SHADER ) };
}
inline Shader make_geometry_shader()
{
  return { glCreateShader( GL_GEOMETRY_SHADER ) };
}

#if EG_GL_VERSION>=400 || defined(EG_ARB_TESSELLATION_SHADER)
// tessellation shader
inline Shader make_tess_control_shader()
{
  return { glCreateShader( GL_TESS_CONTROL_SHADER ) };
}
inline Shader make_tess_eval_shader()
{
  return { glCreateShader( GL_TESS_EVALUATION_SHADER ) };
}
#endif
#if EG_GL_VERSION>=430 || (defined(EG_ARB_COMPUTE_SHADER)&&EG_GL_VERSION>=420)
inline Shader make_compute_shader()
{
  return { glCreateShader( GL_COMPUTE_SHADER ) };
}
#endif

inline std::string read_file( const char* file )
{
  int fd = ::open( file , O_RDONLY );
  if( fd != -1 )
  {
    auto len = ::lseek( fd , 0 , SEEK_END );
    if( len > 0 && (::lseek( fd , 0 , SEEK_SET )!=-1) )
    {
      std::unique_ptr< char[] > buf( new char[ len ] );
      auto read = ::read( fd , buf.get() , len );
      if( read > 0 )
      {
        return std::string( buf.get() , read );
      }
    }
  }
  return std::string();
}

}
