CC=g++

tri: hello-core-profile-triangle.cpp
	g++ hello-core-profile-triangle.cpp -o triangle.exe -std=c++11 -framework OpenGL -framework GLUT -Wno-deprecated

warn:
	g++ hello-core-profile-triangle.cpp -o triangle.exe -std=c++11 -framework OpenGL -framework GLUT

immediate: hello-immediate-mode-triangles.cc
	g++ hello-immediate-mode-triangles.cc -o old-triangle.exe -framework OpenGL -framework GLUT

run:
	./triangle.exe

derp:
	$(tri)
	$(run)

triandrun:
	g++ hello-core-profile-triangle.cpp -o triangle.exe -std=c++11 -framework OpenGL -framework GLUT -Wno-deprecated && ./triangle.exe

phong:
	g++ phong-shading.cpp -o phong.exe -std=c++11 -framework OpenGL -framework GLUT -Wno-deprecated `pkg-config --libs glfw3 glew`

spin:
	g++ transpose-try-angle.cpp -o spin.exe -std=c++11 -framework OpenGL -framework GLUT -Wno-deprecated -framework CoreGraphics

phong-cygwin:
	g++ phong-shading.cpp -o phong.exe -g -std=c++11 -Wno-deprecated -DCYGWIN -lGLEW -lglut -lGL -lGLU -L/usr/X11R6/lib -lX11
	g++ phong-shading-sphere.cpp -o sphere.exe -g -std=c++11 -Wno-deprecated -DCYGWIN -lGLEW -lglut -lGL -lGLU -L/usr/X11R6/lib -lX11

