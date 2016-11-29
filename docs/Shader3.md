## Shader \#3

In the previous step, we defined the view plane as being centered at (0,0,0). Now we will define the camera so it's located at (0,0,2) and 
a single sphere so it's located at (0,0,-2). In other words, Y points UP and Z points towards us, the viewers.

To start, let's test our scene with a simple intersection test. Here, we only care whether we hit the sphere or not, not the 
position at which we intersect the sphere.

Each pixel has a coordinate on the viewplane that corresponds to a point in the world. The GPU automatically interpolates this 
position for us based on the four corners of the square we're drawing, e.g. by passing through the global position of the square's vertex 
positions in the vertex shader like so

```
attribute vec3 pos;       
varying vec4 v_pos;  // v_pos will be passed through and interpolated for us, thanks GPU!
void main ()                 
{                                 
   gl_Position = vec4(pos, 1.0);       
   v_pos = vec4(pos.x, pos.y, pos.z, 1.0);
}                                            
```

Using the pixel's world positions, we can construct a ray emanating from the camera through each pixel center. If this ray 
intersects our sphere, we will draw a white pixel. Otherwise, we will draw a black pixel.

How do we determine whether a ray intersects a sphere or not?  We find the shortest distance between the ray and the sphere center. 
If this distance is less than the radius, we intersect. Otherwise, we don't. 

So now, how do we find the shortest distance? Two steps:
1) Project the view ray onto the vector from the camera to the sphere center
2) Subtract the projected vector from view ray. This will give us a perpendicular vector whose length is our distance to the ray.

TODO: Picture

```
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
```

![step_03](https://github.com/vipyne/opengLOL/blob/master/screenshots/step_05.png)
