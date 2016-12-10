//comment
var fragmentProgram = `
#ifdef GL_ES
  precision mediump float;
#endif

varying vec4 v_pos;

void main ()
{
  vec3 c_pos = vec3(0.0, 0.0, 2.0);
  vec3 sphere_center = vec3(0.0, 0.0, -2.0);
  float radius = 1.0;

  vec3 dir = v_pos.xyz - c_pos;

  vec3 n_dir = normalize(dir);
  vec3 sphere_dir = sphere_center - c_pos;

  float projection = dot(sphere_dir, n_dir);

  vec3 d_perpendicular = sphere_dir - (n_dir * projection);

  if (length(d_perpendicular) > radius) {
    gl_FragColor = vec4(0.0, 0.0, 0.0, 1.0);
  } else {
    gl_FragColor = vec4(1.0, 1.0, 1.0, 1.0);
  }

}
`;
