var fragmentProgram = `
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

  vec3 view_dir = v_pos.xyz - camera_pos;

  vec3 normalized_view_dir = normalize(view_dir);
  vec3 sphere_dir = sphere_center - camera_pos;            // intersection test
  float projection = dot(sphere_dir, normalized_view_dir); // intersection test

  vec3 dir_perpendicular = sphere_dir - (normalized_view_dir * projection); // intersection test

  float len_dir_perpend = length(dir_perpendicular);       // intersection test

  if (len_dir_perpend > radius) {
    gl_FragColor = vec4(0.0, 0.0, 0.0, 1.0);
  } else {
    vec4 ambient_color = vec4(0.1750, 0.1750, 0.1750, 1.0);
    // vec4 ambient_color = vec4(0.750, 0.750, 0.750, 1.0);

    float intersection_dist = sqrt(radius * radius - len_dir_perpend * len_dir_perpend);
    float point1_len = projection - intersection_dist;

    vec3 point1 = normalized_view_dir * point1_len + camera_pos;
    vec3 sphere_normal_p1 = normalize(point1 - sphere_center); // sphere normal

    // light
    vec3 light_pos = vec3(2.0, 2.0, 2.0);
    vec3 light_dir = light_pos - point1;
    vec3 normalized_light_dir = normalize(light_dir);
    vec3 camera_light_dir = normalize(camera_pos - point1);
    float diffuse_k = clamp( dot(normalized_light_dir, sphere_normal_p1), 0.0, 1.0 );

    // specular
    float scalar = 2.0 * ( dot(light_dir, sphere_normal_p1) );
    vec3 normal_extended = scalar * sphere_normal_p1;
    vec3 half_angle = -light_dir + normal_extended;

    vec3 normalized_half_angle = half_angle / length(half_angle); // implentation detail

    float specular_k = clamp( dot( normalized_half_angle, -normalized_view_dir ), 0.0, 1.0 );

    // 1 - 200  larger - smaller
    vec4 specular_color = pow(specular_k, 70.0) * vec4(1.0, 1.0, 1.0, 1.0);

    // gl_FragColor = diffuse_k * vec4(0.90, 0.690, 0.690, 1.0);
    gl_FragColor = diffuse_k * vec4(1.00, 0.30, 0.30, 1.0) + ambient_color + specular_color;
  }

}
`;
