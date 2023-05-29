#version 330 core

void main(){
  const vec2 vertices[4] = vec2[](
    vec2(-1.0,-1.0),
    vec2(+1.0,-1.0),
    vec2(-1.0,+1.0),
    vec2(+1.0,+1.0)
  );
  gl_Position = vec4( vertices[gl_VertexID], 0.0, 1.0 );
}
