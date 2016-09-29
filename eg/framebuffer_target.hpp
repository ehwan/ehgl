#pragma once

#include "gl.hpp"
#include <initializer_list>

namespace eg { namespace core
{
  template < GLenum >
  struct framebuffer_binding_enum_t;

  template < typename Crtp >
  class FramebufferTargetBase
  {
    constexpr GLenum target() const
    {
      return static_cast< Crtp const& >( *this ).target();
    }

  protected:
    void readBuffer( GLenum src ) const
    {
      glReadBuffer( src );
    }
    void drawBuffers( GLsizei count , GLenum const* bufs ) const
    {
      glDrawBuffers( count , bufs );
    }
    void drawBuffers( std::initializer_list< GLenum > bufs ) const
    {
      drawBuffers( bufs.size() , &(*bufs.begin()) );
    }

    /// one of left right back front none
    void drawBuffer( GLenum buf = GL_NONE ) const
    {
      glDrawBuffer( buf );
    }
  public:
    
    GLuint get() const
    {
      GLint ret;
      glGetIntegerv( framebuffer_binding_enum_t< target() >::value , &ret );
      return static_cast< GLuint >( ret );
    }
    operator GLenum() const
    {
      return target();
    }

    void bind( GLuint fb ) const
    {
      glBindFramebuffer( target() , fb );
    }
    void unbind() const
    {
      glBindFramebuffer( target() , 0 );
    }

    bool check_status() const
    {
      return glCheckFramebufferStatus( target() ) == GL_FRAMEBUFFER_COMPLETE;
    }

    bool attachTexture1d( GLenum attachment , GLenum textarget , GLuint texture , GLint level=0 ) const
    {
      glFramebufferTexture1D( target() , attachment , textarget , texture , level );
      return check_status();
    }
    bool attachTexture2d( GLenum attachment , GLenum textarget , GLuint texture , GLint level=0 ) const
    {
      glFramebufferTexture2D( target() , attachment , textarget , texture , level );
      return check_status();
    }
    bool attachTexture3d( GLenum attachment , GLenum textarget , GLuint texture , GLint level=0 ) const
    {
      glFramebufferTexture3D( target() , attachment , textarget , texture , level );
      return check_status();
    }
    bool attachRenderbuffer( GLenum attachment , GLuint renderbuffer ) const
    {
      glFramebufferRenderbuffer( target() , attachment , GL_RENDERBUFFER , renderbuffer );
      return check_status();
    }

  };
}}

namespace eg
{

template < GLenum Target >
class FramebufferTarget;

template <>
class FramebufferTarget< GL_FRAMEBUFFER >
  : public core::FramebufferTargetBase< FramebufferTarget< GL_FRAMEBUFFER > >
{
  using parent = core::FramebufferTargetBase< FramebufferTarget< GL_FRAMEBUFFER > >;
public:
  using parent::drawBuffers;
  using parent::drawBuffer;
  using parent::readBuffer;
  constexpr GLenum target() const
  {
    return GL_FRAMEBUFFER;
  }
};
template <>
class FramebufferTarget< GL_READ_FRAMEBUFFER >
  : public core::FramebufferTargetBase< FramebufferTarget< GL_READ_FRAMEBUFFER > >
{
  using parent = core::FramebufferTargetBase< FramebufferTarget< GL_READ_FRAMEBUFFER > >;
public:
  using parent::readBuffer;
  constexpr GLenum target() const
  {
    return GL_READ_FRAMEBUFFER;
  }
};
template <>
class FramebufferTarget< GL_DRAW_FRAMEBUFFER >
  : public core::FramebufferTargetBase< FramebufferTarget< GL_DRAW_FRAMEBUFFER > >
{
  using parent = core::FramebufferTargetBase< FramebufferTarget< GL_DRAW_FRAMEBUFFER > >;
public:
  using parent::drawBuffers;
  using parent::drawBuffer;
  constexpr GLenum target() const
  {
    return GL_DRAW_FRAMEBUFFER;
  }
};

constexpr FramebufferTarget< GL_FRAMEBUFFER > framebuffer{};
constexpr FramebufferTarget< GL_READ_FRAMEBUFFER > read_framebuffer{};
constexpr FramebufferTarget< GL_DRAW_FRAMEBUFFER > draw_framebuffer{};

}

namespace eg { namespace core
{
  template <>
  struct framebuffer_binding_enum_t< GL_FRAMEBUFFER >
  {
    constexpr static GLenum value = GL_FRAMEBUFFER_BINDING;
  };
  template <>
  struct framebuffer_binding_enum_t< GL_READ_FRAMEBUFFER >
  {
    constexpr static GLenum value = GL_READ_FRAMEBUFFER_BINDING;
  };
  template <>
  struct framebuffer_binding_enum_t< GL_DRAW_FRAMEBUFFER >
  {
    constexpr static GLenum value = GL_DRAW_FRAMEBUFFER_BINDING;
  };
}}
