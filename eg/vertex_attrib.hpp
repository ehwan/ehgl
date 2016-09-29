#pragma once

#include "gl.hpp"

namespace eg { namespace core
{
  template < unsigned int >
  struct vertex_attrib_v_t;
}}

namespace eg
{

class VertexAttrib
{
  GLuint handler_;

public:
  constexpr GLuint get() const
  {
    return handler_;
  }
  constexpr operator GLuint() const
  {
    return get();
  }
  constexpr VertexAttrib( GLuint handler = 0 )
    : handler_( handler )
  {
  }

  void enableArray() const
  {
    glEnableVertexAttribArray( get() );
  }
  void disableArray() const
  {
    glDisableVertexAttribArray( get() );
  }
  void divisor( GLuint div = 0 ) const
  {
    glVertexAttribDivisor( get() , div );
  }
  void pointer( GLint size , GLenum type , GLboolean normalized = GL_FALSE ,
      GLsizei stride = 0 , size_t offset = 0 ) const
  {
    glVertexAttribPointer( get() , size , type , normalized , stride ,
        (void const*)offset );
  }

  template < unsigned int I , typename T >
  void v( T const* v ) const
  {
    core::vertex_attrib_v_t< I >::apply( get() , v );
  }

  void operator ()( GLfloat f1 ) const
  {
    glVertexAttrib1f( get() , f1 );
  }
  void operator ()( GLfloat f1 , GLfloat f2 ) const
  {
    glVertexAttrib2f( get() , f1 , f2 );
  }
  void operator ()( GLfloat f1 , GLfloat f2 , GLfloat f3 ) const
  {
    glVertexAttrib3f( get() , f1 , f2 , f3 );
  }
  void operator ()( GLfloat f1 , GLfloat f2 , GLfloat f3 , GLfloat f4 ) const
  {
    glVertexAttrib4f( get() , f1 , f2 , f3 , f4 );
  }

  void operator ()( GLint i1 ) const
  {
    glVertexAttribI1i( get() , i1 );
  }
  void operator ()( GLint i1 , GLint i2 ) const
  {
    glVertexAttribI2i( get() , i1 , i2 );
  }
  void operator ()( GLint i1 , GLint i2 , GLint i3 ) const
  {
    glVertexAttribI3i( get() , i1 , i2 , i3 );
  }
  void operator ()( GLint i1 , GLint i2 , GLint i3 , GLint i4 ) const
  {
    glVertexAttribI4i( get() , i1 , i2 , i3 , i4 );
  }

  void operator ()( GLuint i1 ) const
  {
    glVertexAttribI1ui( get() , i1 );
  }
  void operator ()( GLuint i1 , GLuint i2 ) const
  {
    glVertexAttribI2ui( get() , i1 , i2 );
  }
  void operator ()( GLuint i1 , GLuint i2 , GLuint i3 ) const
  {
    glVertexAttribI3ui( get() , i1 , i2 , i3 );
  }
  void operator ()( GLuint i1 , GLuint i2 , GLuint i3 , GLuint i4 ) const
  {
    glVertexAttribI4ui( get() , i1 , i2 , i3 , i4 );
  }
};

}

namespace eg { namespace core
{
  template <>
  struct vertex_attrib_v_t< 1 >
  {
    static void apply( GLuint index , GLfloat const* v )
    {
      glVertexAttrib1fv( index , v );
    }
    static void apply( GLuint index , GLint const* v )
    {
      glVertexAttribI1iv( index , v );
    }
    static void apply( GLuint index , GLuint const* v )
    {
      glVertexAttribI1uiv( index , v );
    }
  };
  template <>
  struct vertex_attrib_v_t< 2 >
  {
    static void apply( GLuint index , GLfloat const* v )
    {
      glVertexAttrib2fv( index , v );
    }
    static void apply( GLuint index , GLint const* v )
    {
      glVertexAttribI2iv( index , v );
    }
    static void apply( GLuint index , GLuint const* v )
    {
      glVertexAttribI2uiv( index , v );
    }
  };
  template <>
  struct vertex_attrib_v_t< 3 >
  {
    static void apply( GLuint index , GLfloat const* v )
    {
      glVertexAttrib3fv( index , v );
    }
    static void apply( GLuint index , GLint const* v )
    {
      glVertexAttribI3iv( index , v );
    }
    static void apply( GLuint index , GLuint const* v )
    {
      glVertexAttribI3uiv( index , v );
    }
  };
  template <>
  struct vertex_attrib_v_t< 4 >
  {
    static void apply( GLuint index , GLfloat const* v )
    {
      glVertexAttrib4fv( index , v );
    }
    static void apply( GLuint index , GLint const* v )
    {
      glVertexAttribI4iv( index , v );
    }
    static void apply( GLuint index , GLuint const* v )
    {
      glVertexAttribI4uiv( index , v );
    }
  };
}}
