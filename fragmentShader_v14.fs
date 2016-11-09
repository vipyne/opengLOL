// reflection
#ifdef GL_ES
  precision mediump float;
#endif

varying vec4 v_pos;
uniform float theta;
uniform sampler1D sphere_info;
uniform float num_of_spheres;
uniform float size_of_texture;

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

  box_points[0] = vec3(0.0, 206.0, 0.0); // top
  box_points[1] = vec3(0.0, 0.0, -4.0); // back
  box_points[2] = vec3(3.0, 0.0, 0.0); // left
  box_points[3] = vec3(-3.0, 0.0, 0.0); // right
  box_points[4] = vec3(0.0, -206.0, 0.0); // bottom

  min_t = 100.0;

  for (int i = 0; i < 5; ++i) {
    planeIntersection(ray_start, ray_dir, box_normals[i], box_points[i], t);
    if (t > 0.0)
      min_t = min(min_t, t);
  }

  intersection_point = ray_start + min_t * ray_dir;
}

void computeColor(in vec3 ray_start, in vec3 ray_dir, out vec4 color)
{
  vec3 intersection_point;
  float time;
  boxIntersection(ray_start, ray_dir, intersection_point, time);

  float fraction_of_screen = intersection_point.y + 2.0 / 4.0;

  vec4 top_color = vec4(0.25, 0.6, 0.7, 1.0);
  vec4 bottom_color = vec4(0.6, 0.3, 0.6, 1.0);

  color = top_color * fraction_of_screen + bottom_color * (1.0 - fraction_of_screen);
}

void sphereIntersection(in vec3 ray_start, in vec3 ray_dir, in vec3 sphere_center, in float radius, out float t)
{
  vec3 sphere_dir = sphere_center - ray_start;            // intersection test
  float sphere_len = length(sphere_center - ray_start);            // intersection test
  float projection = dot(sphere_dir, ray_dir); // intersection test
  vec3 dir_perpendicular = sphere_dir - (ray_dir * projection); // intersection test
  float len_dir_perpend = length(dir_perpendicular);       // intersection test

  if (len_dir_perpend > radius) {
    t = -1.0;
    return;
  }

  float intersection_dist = sqrt(radius * radius - len_dir_perpend * len_dir_perpend);
  if (sphere_len > radius) {
    float point1_len = projection - intersection_dist;
    t = point1_len;
  } else {
    t = projection + intersection_dist;
  }
}

void refractionDirection(in float refraction_coef, in vec3 input_dir, in vec3 normal, out vec3 refraction_dir)
{
  float dot_prod = dot(input_dir, normal);
  float dot_prod_sq = dot_prod * dot_prod;

  float cos_theta = 1.0 - (refraction_coef * refraction_coef) * (1.0 - dot_prod_sq);

  if (cos_theta > 0.0) {
    float cos_theta_2 = sqrt( cos_theta );

    float scalar = refraction_coef * dot_prod - cos_theta_2;

    refraction_dir = normalize(refraction_coef * input_dir + scalar * normal);
  } else {
    refraction_dir = input_dir;
  }
}

void animatePath(in float theta, out vec3 sphere_center)
{
  float R = 5.0,
        r = 3.0,
        d = 5.0;
  float x = (R - r) * cos(theta) + d * cos(theta * (R - r) / r);
  float y = (R - r) * sin(theta) - d * sin(theta * (R - r) / r);
  sphere_center.xy = vec2(x, y) * 0.1;
  sphere_center.z = -2.0;
}

void main ()
{
  vec3 camera_pos = vec3(0.0, 0.0, 2.0);

  vec3 view_dir = v_pos.xyz - camera_pos;
  vec3 normalized_view_dir = normalize(view_dir);

  float t;
  vec4 hit_sphere_pos_rad = vec4(0.0, 0.0, 0.0, 0.0);
  vec4 hit_sphere_rgb = vec4(0.0, 1.0, 0.0, 0.0);

  for (float i = 0.0; i < num_of_spheres; i += 1.0) {
    float startIndex = i * 3.0;

    float tex_coord_1 = (startIndex + 0.0)/size_of_texture + 1.0/(2.0 * size_of_texture);
    float tex_coord_2 = (startIndex + 1.0)/size_of_texture + 1.0/(2.0 * size_of_texture);
    float tex_coord_3 = (startIndex + 2.0)/size_of_texture + 1.0/(2.0 * size_of_texture);

    vec4 pos_rad = texture1D(sphere_info, tex_coord_1);
    vec4 vel = texture1D(sphere_info, tex_coord_2);
    vec4 rgb = texture1D(sphere_info, tex_coord_3);

    sphereIntersection(camera_pos, normalized_view_dir, pos_rad.xyz, pos_rad.w, t);

    if (t >= 0.0) {
      hit_sphere_pos_rad = pos_rad;
      hit_sphere_rgb = rgb;
      break;
    }
  }

  if (t < 0.0) {
    vec4 color;
    computeColor(camera_pos, normalized_view_dir, color);

    gl_FragColor = color;
  } else {
    vec4 ambient_color = vec4(0.1750, 0.1750, 0.1750, 1.0);

    vec3 point1 = normalized_view_dir * t + camera_pos;
    vec3 sphere_normal_p1 = normalize(point1 - hit_sphere_pos_rad.xyz); // sphere normal

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

    // reflection
    vec3 reflection_ray = normalized_view_dir - 2.0 * (dot(normalized_view_dir, sphere_normal_p1)) * sphere_normal_p1;
    vec4 reflection;
    computeColor(point1, reflection_ray, reflection);

    // refraction
    float refraction_coef = 0.155;
    float Epsilon = 0.001;
    vec3 point2 = point1 + Epsilon * normalized_view_dir;
    vec3 normal_at_point2 = normalize(point1 - hit_sphere_pos_rad.xyz);

    vec3 point2_dir;
    refractionDirection(refraction_coef, normalized_view_dir, normal_at_point2, point2_dir); // first refraction

    float exit_t;
    sphereIntersection(point2, normalize(point2_dir), hit_sphere_pos_rad.xyz, hit_sphere_pos_rad.w, exit_t); // first sphere intersection

    vec3 point3 = point2 + exit_t * point2_dir;
    vec3 normalized_point2_dir = normalize(point2_dir);
    vec3 normal_at_point3 = normalize(point3 - hit_sphere_pos_rad.xyz);

    vec3 point3_dir;
    refractionDirection( (1.0 / refraction_coef), normalized_point2_dir, normal_at_point3, point3_dir); // second refraction

    vec4 refraction_color;
    computeColor(point3, normalize(point3_dir), refraction_color); // second sphere intersection

    gl_FragColor = diffuse_k * hit_sphere_rgb + specular_color + refraction_color;
    // gl_FragColor = diffuse_k * vec4(0.36, 0.40, 0.650, 1.0) + ambient_color + specular_color + reflection + refraction_color;
  }

}
