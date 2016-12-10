var fragmentProgram = `
//comment

#ifdef GL_ES
  precision mediump float;
#endif

varying vec4 v_pos;

void main ()
{
  vec3 camera_pos     = vec3(0.0, 0.0, 2.0);
  vec3 sphere_center  = vec3(0.0, 0.0, -2.0);
  float sphere_radius = 1.0;

  vec3 camera_dir = v_pos.xyz - camera_pos;
  vec3 unit_camera_dir = normalize(camera_dir);

  vec3 sphere_dir = sphere_center - camera_pos;
  float projection_scalar = dot(sphere_dir, unit_camera_dir);

  vec3 perpendicular_to_unit_camera_dir = sphere_dir - (unit_camera_dir * projection_scalar);

  float len_uc_dir_perpend = length(perpendicular_to_unit_camera_dir);

  float edge_of_circle = smoothstep(sphere_radius, sphere_radius - 0.02, len_uc_dir_perpend);

  vec4 ambient_color = vec4(0.1750, 0.750, 0.750, 1.0);

  float len_k = sqrt(sphere_radius * sphere_radius - len_uc_dir_perpend * len_uc_dir_perpend);
  float point1_dir = projection_scalar - len_k;
  vec3 point1 = unit_camera_dir * point1_dir;
  vec3 normalized_point1 = point1 - sphere_center;

  // light
  vec3 light_pos = vec3(-1.50, 4.0, -1.0);
  vec3 light_dir = normalize(light_pos - point1);
  vec3 camera_light_dir = normalize(camera_pos - point1);
  float diffuse_k = dot(light_dir, normalized_point1);

  gl_FragColor = diffuse_k * edge_of_circle * vec4(0.90, 0.690, 0.690, 1.0) + ambient_color;
}
`;
