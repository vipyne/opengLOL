//comment

#ifdef GL_ES
  precision mediump float;
#endif

varying vec4 v_pos;

void main ()
{
  vec3 camera_pos = vec3(0.0, 0.0, 2.0);
  vec3 sphere_center = vec3(0.0, 0.0, -2.0);
  float radius = 1.0;

  vec3 dir = v_pos.xyz - camera_pos;

  vec3 normalized_dir = normalize(dir);
  vec3 sphere_dir = sphere_center - camera_pos;
  float projection = dot(sphere_dir, normalized_dir);

  vec3 dir_perpendicular = sphere_dir - (normalized_dir * projection);

  float len_dir_perpend = length(dir_perpendicular);

  float  edge = smoothstep(radius, radius - 0.02, len_dir_perpend);
  // float  edge = smoothstep(radius, radius - 0.02, len_dir_perpend) - smoothstep(radius - 0.02, radius, len_dir_perpend) ;
  // float edge = smoothstep(len_dir_perpend - 0.02, len_dir_perpend, radius) - smoothstep(len_dir_perpend, len_dir_perpend - 0.02, radius);

  // if (len_dir_perpend > radius) {
  //   gl_FragColor = edge * vec4(0.0, 0.0, 0.0, 1.0);
  // } else {
    vec4 ambient_color = vec4(0.750, 0.750, 0.750, 1.0);

    float k = sqrt(radius * radius - len_dir_perpend * len_dir_perpend);
    float point1_dir = projection - k;
    vec3 point1 = normalized_dir * point1_dir;
    vec3 normalized_point1 = point1 - sphere_center;

    // light
    vec3 light_pos = vec3(0.0, 6.0, -2.0);
    vec3 light_dir = normalize(light_pos - point1);
    vec3 camera_light_dir = normalize(camera_pos - point1);
    float diffuse_k = dot(light_dir, normalized_point1);

    // gl_FragColor = diffuse_k * vec4(0.90, 0.690, 0.690, 1.0) + ambient_color;
    gl_FragColor = diffuse_k * edge * vec4(0.90, 0.690, 0.690, 1.0) + ambient_color;
  // }

}
