## Shader \#4

Now we extend our previous fragment shader to compute the points of intersection, not just whether we intersect or not. 

After computing the perpendicular distance to the ray, we know whether the ray intercepts or not. Then we can use trigonometry 
to find the points on the sphere where we intersect. The hypotenuse has length radius.

We have two possibilities: if the ray starts inside the sphere, there will be 
one intersection. If the ray starts outside, there will be two. 

```
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

  if (len_dir_perpend > radius) {
    gl_FragColor = vec4(0.0, 0.0, 0.0, 1.0);
  } else {
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

    gl_FragColor = diffuse_k * vec4(0.90, 0.690, 0.690, 1.0) + ambient_color;
  }
}
```

![Shader 4](https://github.com/vipyne/opengLOL/blob/master/screenshots/step_06.png "Results of Shader \#4")

And the aspect ratio is now fixed because we cheated in the vertex shader

```
v_pos = vec4(pos.x * 1.5, pos.y, pos.z, 1.0);
```

Alternatively, we could make the quad we're drawing have vertices with positions having the correct ratio. 
We will probably fix this at some point.

