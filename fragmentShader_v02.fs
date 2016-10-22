//comment

#ifdef GL_ES
  precision mediump float;
#endif

varying vec4 v_pos;

void main ()
{
  if (v_pos.x * v_pos.y > 0.0) {
    gl_FragColor = vec4(1.0, 1.0, 1.0, 1.0);
  } else {
    gl_FragColor = vec4(0.0, 0.0, 0.0, 1.0);
  }
}
