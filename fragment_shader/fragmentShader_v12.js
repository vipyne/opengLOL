var fragmentProgram = `
// relection
#ifdef GL_ES
  precision mediump float;
#endif

varying vec4 v_pos;
uniform float theta;

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

void deform0(in vec3 p, in float theta, out float d, out float maxDisplacement)
{
   float offset = (1.0 + cos(theta)) * 3.14 * 10.0;
   d = sin(p.x * p.y * 50.0 + offset) * 0.2;
   maxDisplacement = 0.2 * 2.0;
}

void deform3(in vec3 p, in float theta, out float d, out float maxDisplacement)
{
   float offset = (1.0 + cos(theta)) * 3.14 * 4.0;
   d = sin(p.x * p.x * p.y * 50.0 + offset) * 0.2;
   maxDisplacement = 0.2 * 2.0;
}

void deform2(in vec3 p, in float theta, out float d, out float maxDisplacement)
{
   float offset = (1.0 + cos(theta)) * 3.14 * 4.0;
   d = sin(p.x * p.x * p.y * p.y * 50.0 + offset) * 0.2;
   maxDisplacement = 0.2 * 2.0;
}

void deform1(in vec3 p, in float theta, out float d, out float maxDisplacement)
{
   float offset = (1.0 + cos(theta)) * 3.14 * 4.0;
   d = sin((sin(3.14*0.25) + p.x) * (cos(3.14*0.25)+p.y) * 20.0 * (1.5+cos(theta)) + offset) * 0.2;
   maxDisplacement = 0.2 * 2.0;
}

void surfaceNormal(in float distance, in vec3 p, out vec3 n)
{
}

void sphereIntersection(in vec3 ray_start, in vec3 ray_dir, in vec3 sphere_center, in float radius, in float theta, 
        out float t, out float surfaceDistance, out float maxDisplacement)
{
  t = -1.0;
  for (float d = 0.5; d < 4.0; d += 0.01) {
    vec3 p = ray_start + d * ray_dir;
    vec3 n = p - sphere_center;
    float distance = length(n);
    n = n / distance;
    vec3 surfacep = sphere_center + radius * n;
    float offset;
    deform2(surfacep, theta, offset, maxDisplacement);
    surfaceDistance = radius + offset;
    if (distance < surfaceDistance) {
        t = d;
        return;
    }
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

void main ()
{
  vec3 camera_pos = vec3(0.0, 0.0, 0.0);
  vec3 sphere_center = vec3(0.0, 0.0, -2.0);
  float radius = 1.0;
  float maxDisplacement = 0.2;

  vec3 view_dir = v_pos.xyz - camera_pos;
  vec3 normalized_view_dir = normalize(view_dir);

  // rotate camera by 45
  float angle1 = theta;
  float angle2 = -3.14/8.0;
  // first 3 vars = first column
  mat3 m = mat3(
    cos(angle1), 0, sin(angle1), // first column 
    0, 1, 0, // second column
    -sin(angle1), 0, cos(angle1)  // third column
  );  
  mat3 m2 = mat3(
    1, 0, 0, // first column 
    0, cos(angle2), sin(angle2), // second column
    0, -sin(angle2), cos(angle2)  // third column
  );  
  normalized_view_dir = m2 * m * normalized_view_dir;
  camera_pos = m2 * m * (camera_pos - sphere_center) + sphere_center;

  float t;
  float distanceFromCenter;
  sphereIntersection(camera_pos, normalized_view_dir, sphere_center, radius, theta, t, distanceFromCenter, maxDisplacement);

  if (t < 0.0) {
    vec4 color;
    computeColor(camera_pos, normalized_view_dir, color);
    gl_FragColor = color;

  } else {
    vec4 ambient_color = vec4(0.1750, 0.1750, 0.1750, 1.0);

    vec3 point1 = normalized_view_dir * t + camera_pos;
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

    /*
    // relection
    vec3 relection_ray = normalized_view_dir - 2.0 * (dot(normalized_view_dir, sphere_normal_p1)) * sphere_normal_p1;
    vec4 relection;
    computeColor(point1, relection_ray, relection);

    // refraction
    float refraction_coef = 0.155;
    float Epsilon = 0.001;
    vec3 point2 = point1 + Epsilon * normalized_view_dir;
    vec3 normal_at_point2 = normalize(point1 - sphere_center);

    vec3 point2_dir;
    refractionDirection(refraction_coef, normalized_view_dir, normal_at_point2, point2_dir); // first refraction

    float exit_t;
    sphereIntersection(point2, normalize(point2_dir), sphere_center, radius, theta, exit_t); // first sphere intersection

    vec3 point3 = point2 + exit_t * point2_dir;
    vec3 normalized_point2_dir = normalize(point2_dir);
    vec3 normal_at_point3 = normalize(point3 - sphere_center);

    vec3 point3_dir;
    refractionDirection( (1.0 / refraction_coef), normalized_point2_dir, normal_at_point3, point3_dir); // second refraction

    vec4 refraction_color;
    computeColor(point3, normalize(point3_dir), refraction_color); // second sphere intersection
*/
    float frac = (distanceFromCenter - radius)/maxDisplacement;
    vec4 color = frac * vec4(1.0, 1.0, 1.0, 1.0) + (1.0-frac)*vec4(0.25, 0.6, 0.7, 1.0);
    gl_FragColor = color; //diffuse_k * vec4(0.36, 0.40, 0.650, 1.0) +specular_color + refraction_color;
  }

}
`;
