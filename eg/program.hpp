#pragma once

#include "gl.hpp"
#include <algorithm>
#include <string>
#include <memory>
#include <initializer_list>

namespace eg
{
 
class Uniform;
class VertexAttrib;

class Program
{
  friend void swap( Program& , Program& );
  GLuint handler_;

  void release_if() const
  {
    if( handler_ )
    {
      glDeleteProgram( handler_ );
    }
  }
public:
  EG_DEFINE_OBJECT_OPERATOR(Program,GLuint,0,handler_)


  void bind() const
  {
    glUseProgram( get() );
  }
  void unbind() const
  {
    glUseProgram( 0 );
  }
  void attachShader( GLuint shader ) const
  {
    glAttachShader( get() , shader );
  }
  void detachShader( GLuint shader ) const
  {
    glDetachShader( get() , shader );
  }
  void detachShaders() const
  {
    int count;
    glGetProgramiv( get() , GL_ATTACHED_SHADERS , &count );
    if( count > 0 )
    {
      std::unique_ptr< GLuint[] > shaders( new GLuint[ count ] );
      GLsizei read;
      glGetAttachedShaders( get() , count , &read , shaders.get() );
      for( GLsizei i=0; i<read; ++i )
      {
        detachShader( shaders[ i ] );
      }
    }
  }

  void bindAttribLocation( GLuint index , const char* name ) const
  {
    glBindAttribLocation( get() , index , name );
  }
  void bindFragDataLocation( GLuint index , const char* name ) const
  {
    glBindFragDataLocation( get() , index , name );
  }
  void transformFeedbackVaryings( GLsizei count , GLchar const* const* names ,
      GLenum mode = GL_INTERLEAVED_ATTRIBS ) const
  {
    glTransformFeedbackVaryings( get() , count , names , mode );
  }
  void transformFeedbackVaryings( std::initializer_list< GLchar const* > names ,
      GLenum mode = GL_INTERLEAVED_ATTRIBS ) const
  {
    transformFeedbackVaryings( names.size() , &(*names.begin()) , mode );
  }


  /// post-link
  void bindUniformBlock( GLuint bindindex , const char* name ) const
  {
    glUniformBlockBinding( get() ,
        glGetUniformBlockIndex( get() , name ) , bindindex );
  }

  bool link() const
  {
    glLinkProgram( get() );
    GLint linked;
    glGetProgramiv( get() , GL_LINK_STATUS , &linked );
    return linked != GL_FALSE;
  }
  bool link_detach() const
  {
    const bool ret = link();
    detachShaders();
    return ret;
  }
  std::string errorMessage() const
  {
    GLint length;
    glGetProgramiv( get() , GL_INFO_LOG_LENGTH , &length );
    if( length > 0 )
    {
      GLsizei read;
      std::unique_ptr< char[] > buf( new char[ length ] );
      glGetProgramInfoLog( get() , length , &read , buf.get() );
      if( read > 0 )
      {
        return std::string( buf.get() , read );
      }
    }
    return std::string();
  }

  VertexAttrib vertexAttrib( const char* name ) const;
  Uniform uniformLocation( const char* name ) const;
};
inline void swap( Program& l , Program& r )
{
  std::swap( l.handler_ , r.handler_ );
}

inline Program make_program()
{
  return { glCreateProgram() };
}

}
