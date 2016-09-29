#pragma once

#include "gl.hpp"

namespace eg
{
  template < GLenum Target >
  class RenderbufferTarget;

  template <>
  class RenderbufferTarget< GL_RENDERBUFFER >
  {
  public:
    constexpr GLenum target() const
    {
      return GL_RENDERBUFFER;
    }
    operator GLenum() const
    {
      return target();
    }
    GLuint get() const
    {
      GLint ret;
      glGetIntegerv( GL_RENDERBUFFER_BINDING , &ret );
      return static_cast< GLuint >( ret );
    }
    GLint parameter( GLenum pname ) const
    {
      GLint ret;
      glGetRenderbufferParameteriv( target() , pname , &ret );
      return ret;
    }
    GLsizei width() const
    {
      GLint ret;
      glGetRenderbufferParameteriv( target() , GL_RENDERBUFFER_WIDTH , &ret );
      return static_cast< GLsizei >( ret );
    }
    GLsizei height() const
    {
      GLint ret;
      glGetRenderbufferParameteriv( target() , GL_RENDERBUFFER_HEIGHT , &ret );
      return static_cast< GLsizei >( ret );
    }
    GLenum internalFormat() const
    {
      GLint ret;
      glGetRenderbufferParameteriv( target() , GL_RENDERBUFFER_INTERNAL_FORMAT , &ret );
      return static_cast< GLenum >( ret );
    }

    void bind( GLuint renderbuffer ) const
    {
      glBindRenderbuffer( target() , renderbuffer );
    }
    void unbind() const
    {
      glBindRenderbuffer( target() , 0 );
    }

    void data( GLenum internalFormat , GLsizei width , GLsizei height ) const
    {
      glRenderbufferStorage( target() , internalFormat , width , height );
    }
  };

  constexpr RenderbufferTarget< GL_RENDERBUFFER > renderbuffer{};
}
