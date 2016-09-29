#pragma once

#include "gl.hpp"
#include <algorithm>

namespace eg {

class VertexArray
{
  friend void swap( VertexArray& , VertexArray& );
  GLuint handler_;

  void release_if() const
  {
    if( handler_ )
    {
      glDeleteVertexArrays( 1 , &handler_ );
    }
  }

public:
  EG_DEFINE_OBJECT_OPERATOR(VertexArray,GLuint,0,handler_)

  void bind() const
  {
    glBindVertexArray( get() );
  }
  void unbind() const
  {
    glBindVertexArray( 0 );
  }
};
inline void swap( VertexArray& l , VertexArray& r )
{
  std::swap( l.handler_ , r.handler_ );
}

inline VertexArray make_vertex_array()
{
  GLuint ret;
  glGenVertexArrays( 1 , &ret );
  return { ret };
}

}
