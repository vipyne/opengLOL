//comment

#ifdef GL_ES
  precision mediump float;
#endif

void main ()
{
  vec2 w_xy; // "real" world

  vec2 screen = vec2(720.0, 480.0);

  w_xy.x = gl_FragCoord.x / screen.x;
  w_xy.y = gl_FragCoord.y / screen.y;

  gl_FragColor = vec4(w_xy.x, w_xy.y, 0.0, 1.0);
}
