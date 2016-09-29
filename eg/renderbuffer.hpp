#pragma once

#include "gl.hpp"
#include <algorithm>

namespace eg
{

class Renderbuffer
{
  friend void swap( Renderbuffer& , Renderbuffer& );
  GLuint handler_;

  void release_if() const
  {
    if( handler_ )
    {
      glDeleteTextures( 1 , &handler_ );
    }
  }

public:
  EG_DEFINE_OBJECT_OPERATOR(Renderbuffer,GLuint,0,handler_)
};
inline void swap( Renderbuffer& l , Renderbuffer& r )
{
  std::swap( l.handler_ , r.handler_ );
}

inline Renderbuffer make_renderbuffer()
{
  GLuint ret;
  glGenRenderbuffers( 1 , &ret );
  return { ret };
}

}
