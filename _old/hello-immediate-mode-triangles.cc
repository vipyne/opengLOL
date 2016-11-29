#include <cstdio>
#include <iostream>
#include <OpenGL/Gl.h>
#include <GLUT/glut.h>
#include "glint.h"

int viewportX = 400,
	viewportY = 400;

void vipInitCanvas (void)
{
	glViewport(0, 0, viewportX, viewportY);
	glClearColor(1.0, 1.0, 1.0, 0.3);
	glColor3f(0.0f, 0.6f, 0.3f);
	glPointSize(2.0);
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	gluOrtho2D(0.0, 1000.0, 0.0, 1000.0);
}

void vipDrawDot (int x, int y)
{
	glBegin(GL_POINTS);
		glVertex2i(x, y);
	glEnd();
}

int random (int number)
{
	return rand() % number;
}

void vipDraw (void) 
{
	GLintPoint arrayOfPoints[3] = {{100,  100 },
								   {3000, 300 },
								   {2000, 3000}};
	int index = random(3);
	glClear(GL_COLOR_BUFFER_BIT);

	GLintPoint point = arrayOfPoints[index];
	vipDrawDot(point.x, point.y);
	
	for (int i = 0; i < 10000; ++i) 
	{
		index = random(3);
		point.x = (point.x + arrayOfPoints[index].x) / 2;
		point.y = (point.y + arrayOfPoints[index].y) / 2;
		vipDrawDot(point.x, point.y);
	}
	glFlush();
}

int main (int argc, char** argv)
{
	std::cout << "hi" << std::endl;

	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_RGB);
	glutInitWindowSize(viewportX, viewportY);
	glutInitWindowPosition(50, 50);
	glutCreateWindow("well hello again window");
	glutDisplayFunc(vipDraw);
	vipInitCanvas();
	glutMainLoop();
	
	return 0;
}
