# opengLOL

##Phong Shader

compile:
```
$ make phong
```

run:
```
$ ./phong.exe
```

![new alt](https://github.com/vipyne/opengLOL/blob/master/screenshots/readmedia/1000bubbles.gif)

##Effervescent bubbles were built up in steps

<img src="https://github.com/vipyne/opengLOL/blob/master/screenshots/step_01.png" alt="Drawing" height="64"/>
<a href="https://github.com/vipyne/opengLOL/blob/master/docs/Shader0.md">Shader #0</a> In which, we create a baby shader to test our development setup

<p><img src="https://github.com/vipyne/opengLOL/blob/master/screenshots/step_02.png" alt="Drawing" height="64"/>
<a href="https://github.com/vipyne/opengLOL/blob/master/docs/Shader1.md">Shader #1</a> In which, we test out gl_FragCoord

<p><img src="https://github.com/vipyne/opengLOL/blob/master/screenshots/step_03.png" alt="Drawing" height="64"/>
<a href="https://github.com/vipyne/opengLOL/blob/master/docs/Shader2.md">Shader #2</a> In which, we remember that we don't need gl_FragCoord and can just use the interpolated values from a drawn square.

<p><img src="https://github.com/vipyne/opengLOL/blob/master/screenshots/step_05.png" alt="Drawing" height="64"/>
<a href="https://github.com/vipyne/opengLOL/blob/master/docs/Shader2.md">Shader #3</a> In which, we expand our fictional world to include a 
sphere and a camera. If a ray emanating from the cemera through a pixel intersects the sphere, we draw that pixel in white.

<p><img src="https://github.com/vipyne/opengLOL/blob/master/screenshots/step_06.png" alt="Drawing" height="64"/>
<a href="https://github.com/vipyne/opengLOL/blob/master/docs/Shader2.md">Shader #4</a> In which, we compute the points at which a ray intersects the sphere, and then use that point to compute diffuse shading.

