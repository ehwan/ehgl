#pragma once

#include <SDL2/SDL.h>
#include <unistd.h>
#include <utility>

namespace eg { namespace sdl
{
  class Context;
  class Window;
  class Application;

class Application
{
  bool inited_;

public:
  Application()
  {
    inited_ = false;
  }
  explicit Application( Uint32 flags )
  {
    if( SDL_Init( flags ) == 0 )
    {
      inited_ = true;
      SDL_GL_SetAttribute( SDL_GL_CONTEXT_PROFILE_MASK , SDL_GL_CONTEXT_PROFILE_CORE );
      SDL_GL_SetSwapInterval( 1 );
    }else
    {
      inited_ = false;
      //error
    }
  }
  ~Application()
  {
    if( inited_ )
    {
      SDL_Quit();
    }
  }
  Application( Application const& ) = delete;
  Application& operator=( Application const& ) = delete;
  Application( Application &&rhs )
  {
    inited_ = rhs.inited_;
    rhs.inited_ = false;
  }
  Application& operator=( Application &&rhs )
  {
    if( rhs.inited_ == false && inited_ )
    {
      SDL_Quit();
    }
    inited_ = rhs.inited_;
    rhs.inited_ = false;
    return *this;
  }


#define EG_SDL_GL_ATTRIBUTE_GET_SET( name , value , pred ) \
  void name( int value ) const { \
    SDL_GL_SetAttribute( pred , value ); \
  } \
  int name() const { \
    int ret; \
    SDL_GL_GetAttribute( pred , &ret ); \
    return ret; \
  }

  EG_SDL_GL_ATTRIBUTE_GET_SET( GLMajor , major , SDL_GL_CONTEXT_MAJOR_VERSION )
  EG_SDL_GL_ATTRIBUTE_GET_SET( GLMinor , minor , SDL_GL_CONTEXT_MINOR_VERSION )
  void GLVersion( int major , int minor ) const
  {
    GLMajor( major );
    GLMinor( minor );
  }
  std::pair< int , int > GLVersion() const
  {
    return std::make_pair( GLMajor() , GLMinor() );
  }

  EG_SDL_GL_ATTRIBUTE_GET_SET( doubleBuffer , enabled , SDL_GL_DOUBLEBUFFER )
  EG_SDL_GL_ATTRIBUTE_GET_SET( depthSize , bits , SDL_GL_DEPTH_SIZE )
  EG_SDL_GL_ATTRIBUTE_GET_SET( redSize , bits , SDL_GL_RED_SIZE )
  EG_SDL_GL_ATTRIBUTE_GET_SET( greenSize , bits , SDL_GL_GREEN_SIZE )
  EG_SDL_GL_ATTRIBUTE_GET_SET( blueSize , bits , SDL_GL_BLUE_SIZE )
  EG_SDL_GL_ATTRIBUTE_GET_SET( alphaSize , bits , SDL_GL_ALPHA_SIZE )
  void colorSize( int r , int g , int b ) const
  {
    redSize( r );
    greenSize( g );
    blueSize( b );
  }
  void colorSize( int r , int g , int b , int a ) const
  {
    redSize( r );
    greenSize( g );
    blueSize( b );
    alphaSize( a );
  }
  void setDebug() const
  {
    SDL_GL_SetAttribute( SDL_GL_CONTEXT_FLAGS , SDL_GL_CONTEXT_DEBUG_FLAG );
  }

#undef EG_SDL_GL_ATTRIBUTE_GET_SET
};
class Window
{
  friend class Context;
  SDL_Window *window_;

  void release_if() const
  {
    if( window_ )
    {
      SDL_DestroyWindow( window_ );
    }
  }

public:
  Window()
  {
    window_ = nullptr;
  }
  Window( const char* title , int w , int h )
  {
    Uint32 flags = 0;
    flags = SDL_WINDOW_OPENGL | SDL_WINDOW_SHOWN;
    window_ = SDL_CreateWindow( title , SDL_WINDOWPOS_CENTERED , SDL_WINDOWPOS_CENTERED , w , h , flags );
  }
  ~Window()
  {
    release_if();
  }

  Window( Window const& ) = delete;
  Window& operator=( Window const& ) = delete;

  Window( Window &&rhs )
    : window_( rhs.window_ )
  {
    rhs.window_ = nullptr;
  }
  Window& operator=( Window &&rhs )
  {
    release_if();
    window_ = rhs.window_;
    rhs.window_ = nullptr;
    return *this;
  }

  Context createContext() const;
  void swapBuffer() const
  {
    SDL_GL_SwapWindow( window_ );
  }

  std::pair< int , int > getSize() const
  {
    int x , y;
    SDL_GetWindowSize( window_ , &x , &y );
    return std::make_pair( x , y );
  }
  void fullscreen( bool enabled=true ) const
  {
    SDL_SetWindowFullscreen( window_ , enabled ? SDL_WINDOW_FULLSCREEN : 0 );
  }
};

class Context
{
  SDL_GLContext context_;

  void release_if() const
  {
    if( context_ )
    {
      SDL_GL_DeleteContext( context_ );
    }
  }
public:
  Context()
    : context_( nullptr )
  {
  }
  Context( SDL_GLContext context )
    : context_( context )
  {
  }
  ~Context()
  {
    release_if();
  }

  Context( Context const& ) = delete;
  Context& operator=( Context const& ) = delete;
  Context( Context &&rhs )
  {
    context_ = rhs.context_;
    rhs.context_ = nullptr;
  }
  Context& operator=( Context &&rhs )
  {
    release_if();
    context_ = rhs.context_;
    rhs.context_ = nullptr;
    return *this;
  }

  void makeCurrent( Window const& window ) const
  {
    SDL_GL_MakeCurrent( window.window_ , context_ );
  }

};

inline Context Window::createContext() const
{
  return { SDL_GL_CreateContext( window_ ) };
}
inline void debug_callback( GLenum source , GLenum type ,
    GLuint id , GLenum severity , 
    GLsizei length , const GLchar *message ,
    const void *userParam )
{
  ::write( STDOUT_FILENO , message , length );
  const char nl = '\n';
  ::write( 1 , &nl , 1 );
}

}}
