# opengLOL

### Requirements
`glew` and `glfw3`, both availabe via [homebrew](https://brew.sh/):
```
$ brew install glew && brew install glfw3
```

## Phong Shader

compile:
```
$ make phong
```

run:
```
$ ./phong.exe
```

![new alt](https://github.com/vipyne/opengLOL/blob/master/screenshots/readmedia/1000bubbles.gif)

## Effervescent bubbles were built up in steps

<img src="https://github.com/vipyne/opengLOL/blob/master/screenshots/step_01.png" alt="Drawing" height="64"/>
<a href="https://github.com/vipyne/opengLOL/blob/master/docs/Shader0.md">Shader #0</a> In which, we create a baby shader to test our development setup

<p><img src="https://github.com/vipyne/opengLOL/blob/master/screenshots/step_02.png" alt="Drawing" height="64"/>
<a href="https://github.com/vipyne/opengLOL/blob/master/docs/Shader1.md">Shader #1</a> In which, we test out gl_FragCoord

<p><img src="https://github.com/vipyne/opengLOL/blob/master/screenshots/step_03.png" alt="Drawing" height="64"/>
<a href="https://github.com/vipyne/opengLOL/blob/master/docs/Shader2.md">Shader #2</a> In which, we remember that we don't need gl_FragCoord and can just use the interpolated values from a drawn square.

<p><img src="https://github.com/vipyne/opengLOL/blob/master/screenshots/step_05.png" alt="Drawing" height="64"/>
<a href="https://github.com/vipyne/opengLOL/blob/master/docs/Shader3.md">Shader #3</a> In which, we expand our fictional world to include a
sphere and a camera. If a ray emanating from the cemera through a pixel intersects the sphere, we draw that pixel in white.

<p><img src="https://github.com/vipyne/opengLOL/blob/master/screenshots/step_06.png" alt="Drawing" height="64"/>
<a href="https://github.com/vipyne/opengLOL/blob/master/docs/Shader4.md">Shader #4</a> In which, we compute the points at which a ray intersects the sphere, and then use that point to compute diffuse shading.

<a href='http://www.recurse.com' title='Made with love at the Recurse Center'><img src='https://cloud.githubusercontent.com/assets/2883345/11322973/9e557144-910b-11e5-959a-8fdaaa4a88c5.png' height='14px'/></a>
