## Shader \#2

The previous shader drew a triangle which was then colored by the fragment shader. Now, we need to draw a square to match the 
resolution of the screen and then assign 'world coordinates' to this screen so we can position it along with an imaginary camera and 
geometry. 

To start, let's simply define our quad geometry and draw it. The quad is a square having four vertices, centered at the origin.

If we use the same shader as before, we get the following output. Now it covers the who screen and not just our triangle.

![step \#2](https://github.com/vipyne/opengLOL/blob/master/screenshots/step_03.png "Step \#2 Results")

But let's also test whether our square is centered like we expect.

```
varying vec4 v_pos;
void main ()
{
  if (v_pos.x * v_pos.y > 0.0) {
    gl_FragColor = vec4(1.0, 1.0, 1.0, 1.0);
  } else {
    gl_FragColor = vec4(0.0, 0.0, 0.0, 1.0);
  }
}
```

The above shader draws each quadrant of our square in either black or white. If both the x and y coordinates of the quad are 
either both positive or both negative (e.g. x * y > 0), then we draw white. Otherwise, we draw black.

![step \#2](https://github.com/vipyne/opengLOL/blob/master/screenshots/step_04.png "Step \#2 Results")
