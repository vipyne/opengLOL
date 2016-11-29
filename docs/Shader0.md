# Ray tracing in the fragment shader: step by step

## Shader \#0

To start, color every pixel a solid color

```
void main ()
{
   gl_FragColor = vec4(0.3, 0.55, 0.7, 1.0);
}
```

Now, we know our development environment is properly set up. 

![step_00](https://github.com/vipyne/opengLOL/blob/master/screenshots/step_01.png "Shader \#0 output")

## Shader \#1

We will use gl_FragCoord, a built-in variable passed to the fragment shader, to get the screen coordinate of each pixel we are drawing. The following shader let's us test the range 

```
void main ()
{
  vec2 w_xy; // "real" world

  vec2 screen = vec2(720.0, 480.0);

  w_xy.x = gl_FragCoord.x / screen.x;
  w_xy.y = gl_FragCoord.y / screen.y;

  gl_FragColor = vec4(w_xy.x, w_xy.y, 0.0, 1.0);
}
```

We've hard-coded the screen size (720,480) in the shader to match our created window. _w\_xy_ has (x,y) coordinates which range from (0,0) to (1,1). Thus, the bottom left corner is black (0,0,0,1), the top left corner is green (0,1,0,1), the top right corner is (1,1,0,1), and the bottom right corner is red (1,0,0,1). All other pixels are interpolated between these colors.

![step_01](https://github.com/vipyne/opengLOL/blob/master/screenshots/step_02.png "Shader \#1 output")

