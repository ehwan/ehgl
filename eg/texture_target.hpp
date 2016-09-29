#pragma once

#include "gl.hpp"

namespace eg { namespace core
{
  template < GLenum >
  struct texture_binding_enum_t;

  template < typename Crtp >
  class TextureTargetBase
  {
    constexpr GLenum target() const
    {
      return static_cast< Crtp const& >( *this ).target();
    }

  protected:

    GLsizei width( GLint level=0 ) const
    {
      GLint ret;
      glGetTexLevelParameteriv( target() , level , GL_TEXTURE_WIDTH , &ret );
      return static_cast< GLsizei >( ret );
    }
    GLsizei height( GLint level=0 ) const
    {
      GLint ret;
      glGetTexLevelParameteriv( target() , level , GL_TEXTURE_HEIGHT , &ret );
      return static_cast< GLsizei >( ret );
    }
    GLsizei depth( GLint level=0 ) const
    {
      GLint ret;
      glGetTexLevelParameteriv( target() , level , GL_TEXTURE_DEPTH , &ret );
      return static_cast< GLsizei >( ret );
    }

  public:
    GLuint get() const
    {
      GLint ret;
      glGetIntegerv( texture_binding_enum_t< target() >::value , &ret );
      return static_cast< GLuint >( ret );
    }
    operator GLenum() const
    {
      return target();
    }

    void bind( GLuint texture ) const
    {
      glBindTexture( target() , texture );
    }
    void unbind() const
    {
      glBindTexture( target() , 0 );
    }

    GLint internalFormat( GLint level=0 ) const
    {
      GLint ret;
      glGetTexLevelParameteriv( target() , level , GL_TEXTURE_INTERNAL_FORMAT , &ret );
      return ret;
    }

    GLint levelParemeter( GLenum pname , GLint level=0 ) const
    {
      GLint ret;
      glGetTexLevelParameteriv( target() , pname , &ret );
      return ret;
    }
    void parameter( GLenum pname , GLint value ) const
    {
      glTexParameteri( target() , pname , value );
    }
    void parameter( GLenum pname , GLfloat value ) const
    {
      glTexParameterf( target() , pname , value );
    }
  };
}}

namespace eg
{

template < GLenum Target >
class TextureTarget
  : public core::TextureTargetBase< TextureTarget< Target > >
{
public:
  constexpr GLenum target() const
  {
    return Target;
  }
};

template <>
class TextureTarget< GL_TEXTURE_1D >
  : public core::TextureTargetBase< TextureTarget< GL_TEXTURE_1D > >
{
  using parent = core::TextureTargetBase< TextureTarget< GL_TEXTURE_1D > >;

public:
  using parent::width;

  constexpr GLenum target() const
  {
    return GL_TEXTURE_1D;
  }

  void data( GLint level , GLint internalFormat , 
      GLsizei width , 
      GLenum format , GLenum type , GLvoid const* ptr ) const
  {
    glTexImage1D( target() , level , internalFormat , width , 0 , format , type , ptr );
  }
  void dataReadbuffer( GLint level , GLenum internalFormat ,
      GLint readX , GLint readY , GLsizei width ) const
  {
    glCopyTexImage1D( target() , level , internalFormat , readX , readY , width , 0 );
  }
  void subData( GLint level ,
      GLint xoffset , GLsizei width ,
      GLenum format , GLenum type , GLvoid const* ptr ) const
  {
    glTexSubImage1D( target() , level , xoffset , width , format , type , ptr );
  }
  void copyReadbuffer( GLint level ,
      GLint xoffset , GLint readX , GLint readY ,
      GLsizei width ) const
  {
    glCopyTexSubImage1D( target() , level , xoffset , readX , readY , width );
  }
};
template <>
class TextureTarget< GL_TEXTURE_2D >
  : public core::TextureTargetBase< TextureTarget< GL_TEXTURE_2D > >
{
  using parent = core::TextureTargetBase< TextureTarget< GL_TEXTURE_2D > >;

public:
  using parent::width;
  using parent::height;
  constexpr GLenum target() const
  {
    return GL_TEXTURE_2D;
  }

  void data( GLint level , GLint internalFormat , 
      GLsizei width , GLsizei height , 
      GLenum format , GLenum type , GLvoid const* ptr ) const
  {
    glTexImage2D( target() , level , internalFormat , width , height , 0 , format , type , ptr );
  }
  void dataReadbuffer( GLint level , GLenum internalFormat ,
      GLint readX , GLint readY , GLsizei width , GLsizei height ) const
  {
    glCopyTexImage2D( target() , level , internalFormat , readX , readY , width , height , 0 );
  }
  void subData( GLint level ,
      GLint xoffset , GLint yoffset , GLsizei width , GLsizei height ,
      GLenum format , GLenum type , GLvoid const* ptr ) const
  {
    glTexSubImage2D( target() , level , xoffset , yoffset , width , height , format , type , ptr );
  }
  void copyReadbuffer( GLint level ,
      GLint xoffset , GLint yoffset , GLint readX , GLint readY ,
      GLsizei width , GLsizei height ) const
  {
    glCopyTexSubImage2D( target() , level , xoffset , yoffset , readX , readY , width , height );
  }
};
template <>
class TextureTarget< GL_TEXTURE_3D >
  : public core::TextureTargetBase< TextureTarget< GL_TEXTURE_3D > >
{
  using parent = core::TextureTargetBase< TextureTarget< GL_TEXTURE_3D > >;

public:
  using parent::width;
  using parent::height;
  using parent::depth;
  constexpr GLenum target() const
  {
    return GL_TEXTURE_3D;
  }

  void data( GLint level , GLint internalFormat , 
      GLsizei width , GLsizei height , GLsizei depth ,
      GLenum format , GLenum type , GLvoid const* ptr ) const
  {
    glTexImage3D( target() , level , internalFormat , width , height , depth , 0 , format , type , ptr );
  }
  void subData( GLint level ,
      GLint xoffset , GLint yoffset , GLint zoffset , GLsizei width , GLsizei height , GLsizei depth ,
      GLenum format , GLenum type , GLvoid const* ptr ) const
  {
    glTexSubImage3D( target() , level , xoffset , yoffset , zoffset , width , height , depth , format , type , ptr );
  }
  void copyReadbuffer( GLint level ,
      GLint xoffset , GLint yoffset , GLint zoffset , GLint readX , GLint readY ,
      GLsizei width , GLsizei height ) const
  {
    glCopyTexSubImage3D( target() , level , xoffset , yoffset , zoffset , readX , readY , width , height );
  }
};
template <>
class TextureTarget< GL_TEXTURE_RECTANGLE >
  : public core::TextureTargetBase< TextureTarget< GL_TEXTURE_RECTANGLE > >
{
  using parent = core::TextureTargetBase< TextureTarget< GL_TEXTURE_RECTANGLE > >;

public:
  using parent::width;
  using parent::height;
  constexpr GLenum target() const
  {
    return GL_TEXTURE_RECTANGLE;
  }

  void data( GLint internalFormat , 
      GLsizei width , GLsizei height , 
      GLenum format , GLenum type , GLvoid const* ptr ) const
  {
    glTexImage2D( target() , 0 , internalFormat , width , height , 0 , format , type , ptr );
  }
};
template <>
class TextureTarget< GL_TEXTURE_BUFFER >
  : public core::TextureTargetBase< TextureTarget< GL_TEXTURE_BUFFER > >
{
  using parent = core::TextureTargetBase< TextureTarget< GL_TEXTURE_BUFFER > >;

public:
  using parent::width;
  constexpr GLenum target() const
  {
    return GL_TEXTURE_BUFFER;
  }

  void bindBuffer( GLenum internalFormat , GLuint buffer ) const
  {
    glTexBuffer( target() , internalFormat , buffer );
  }
};

constexpr TextureTarget< GL_TEXTURE_1D > texture1d{};
constexpr TextureTarget< GL_TEXTURE_2D > texture2d{};
constexpr TextureTarget< GL_TEXTURE_3D > texture3d{};
constexpr TextureTarget< GL_TEXTURE_BUFFER > texture_buffer{};
constexpr TextureTarget< GL_TEXTURE_RECTANGLE > texture_rectangle{};

}

namespace eg { namespace core
{
#define EG_TEXTURE_BINDING_ENUM(a,b) \
  template <> \
  struct texture_binding_enum_t< a > { \
    constexpr static GLenum value = b; \
  }

  EG_TEXTURE_BINDING_ENUM( GL_TEXTURE_1D , GL_TEXTURE_BINDING_1D );
  EG_TEXTURE_BINDING_ENUM( GL_TEXTURE_2D , GL_TEXTURE_BINDING_2D );
  EG_TEXTURE_BINDING_ENUM( GL_TEXTURE_3D , GL_TEXTURE_BINDING_3D );
  EG_TEXTURE_BINDING_ENUM( GL_TEXTURE_RECTANGLE , GL_TEXTURE_BINDING_RECTANGLE );
  EG_TEXTURE_BINDING_ENUM( GL_TEXTURE_BUFFER , GL_TEXTURE_BINDING_BUFFER );

#undef EG_TEXTURE_BINDING_ENUM
  
}}
