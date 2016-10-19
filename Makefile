CC=g++

tri: hello-core-profile-triangle.cpp
	g++ hello-core-profile-triangle.cpp -o triangle.exe -std=c++11 -framework OpenGL -framework GLUT -Wno-deprecated

warn:
	g++ hello-core-profile-triangle.cpp -o triangle.exe -std=c++11 -framework OpenGL -framework GLUT

immediate: hello-immediate-mode-triangles.cc
	g++ hello-immediate-mode-triangles.cc -o old-triangle.exe -framework OpenGL -framework GLUT

run: 
	./triangle.exe

asdf:
	g++ hello-core-profile-triangle.cpp -o triangle.exe -std=c++11 -framework OpenGL -framework GLUT -Wno-deprecated && ./triangle.exe
