#pragma once

#include "gl.hpp"
#include "time.hpp"
#include <algorithm>

namespace eg
{

class Sync
{
  friend void swap( Sync& , Sync& );
  GLsync handler_;

  void release_if() const
  {
    if( handler_ )
    {
      glDeleteSync( handler_ );
    }
  }

public:
  EG_DEFINE_OBJECT_OPERATOR(Sync,GLsync,nullptr,handler_)

  template < typename T >
  constexpr static GLuint64 dur_to_nano( T x )
  {
    return static_cast< GLuint64 >(
      time::to_duration< std::chrono::nanoseconds , std::chrono::nanoseconds::period >( x ).count()
    );
  }

  template < typename T >
  bool wait_cpu( T nano_timeout ) const
  {
    return 
      glClientWaitSync( get() , GL_SYNC_FLUSH_COMMANDS_BIT , dur_to_nano( nano_timeout ) )
      != GL_TIMEOUT_EXPIRED;
  }
  template < typename T >
  bool wait_gpu( T nano_timeout ) const
  {
    return 
      glWaitSync( get() , GL_SYNC_FLUSH_COMMANDS_BIT , dur_to_nano( nano_timeout ) )
      != GL_TIMEOUT_EXPIRED;
  }
};
inline void swap( Sync& l , Sync& r )
{
  std::swap( l.handler_ , r.handler_ );
}

inline Sync make_sync()
{
  return { glFenceSync( GL_SYNC_GPU_COMMANDS_COMPLETE , 0 ) };
}


}
