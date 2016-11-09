//comment

#ifdef GL_ES
  precision mediump float;
#endif

varying vec4 v_pos;

void planeIntersection(in vec3 ray_start, in vec3 ray_dir, in vec3 box_normal, in vec3 box_p1,
  out float plane_intersect)
  {
    float term_1 = dot(ray_start, box_normal);
    float term_2 = dot(box_p1, box_normal);
    float term_3 = dot(ray_dir, box_normal);

    float t = (term_2 - term_1) / term_3;

    plane_intersect = t;
  }

void boxIntersection(in vec3 ray_start, in vec3 ray_dir, out vec3 intersection_point, out float min_t)
{
  vec3 box_normals[5];
  vec3 box_points[5];
  float t;

  box_normals[0] = vec3(0.0, -1.0, 0.0); // top
  box_normals[1] = vec3(0.0, 0.0, -1.0); // back
  box_normals[2] = vec3(1.0, 0.0, 0.0); // left
  box_normals[3] = vec3(-1.0, 0.0, 0.0); // right
  box_normals[4] = vec3(0.0, 1.0, 0.0); // bottom

  box_points[0] = vec3(0.0, 3.0, 0.0); // top
  box_points[1] = vec3(0.0, 0.0, -4.0); // back
  box_points[2] = vec3(3.0, 0.0, 0.0); // left
  box_points[3] = vec3(-3.0, 0.0, 0.0); // right
  box_points[4] = vec3(0.0, -3.0, 0.0); // bottom

  min_t = 100.0;

  for (int i = 0; i < 5; ++i) {
    planeIntersection(ray_start, ray_dir, box_normals[i], box_points[i], t);
    if (t > 0.0)
      min_t = min(min_t, t);
  }

  intersection_point = ray_start + min_t * ray_dir;
}

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
    vec3 intersection_point;
    float time;
    boxIntersection(camera_pos, normalized_view_dir, intersection_point, time);

    float fraction_of_screen = intersection_point.y + 2.0 / 4.0;

    vec4 top_color = vec4(0.25, 0.6, 0.7, 1.0);
    vec4 bottom_color = vec4(0.6, 0.3, 0.6, 1.0);

    gl_FragColor = top_color * fraction_of_screen + bottom_color * (1.0 - fraction_of_screen);
  } else {
    vec4 ambient_color = vec4(0.1750, 0.1750, 0.1750, 1.0);

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
    vec4 specular_color = pow(specular_k, 70.0) * vec4(0.0, 1.0, 0.0, 1.0);

    gl_FragColor = diffuse_k * vec4(0.36, 0.40, 0.650, 1.0) + ambient_color + specular_color;
  }

}
