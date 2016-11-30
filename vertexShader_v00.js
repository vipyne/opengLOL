var vertexProgram = `
attribute vec3 pos;
varying vec4 v_pos;

void main ()
{
  gl_Position = vec4(pos, 1.0);
  v_pos = vec4(pos.x * 1.5, pos.y, pos.z, 1.0);
}
`;