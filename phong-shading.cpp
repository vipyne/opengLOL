////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
// phong shading in Fragment Shader
//
// shoutout to partner in pairing Lynn, github.com/alinen
// author: vanessa pyne --- github.com/vipyne
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////

#include <cstdio>
#include <iostream>
#include <fstream>
#include <math.h>
#include <OpenGL/gl.h>
#include <GLUT/glut.h>
#include "glint.h"

using namespace std;

////////////////////////////////////////////////////////////////////////////////

// Vertex Shader
//
const char *vertex_shader_source =                "\n" \
"attribute vec3 pos;                               \n" \
"varying vec4 v_pos;                               \n" \
"void main ()                                      \n" \
"{                                                 \n" \
"   gl_Position = vec4(pos, 1.0);                  \n" \
"   v_pos = vec4(pos.x * 1.5, pos.y, pos.z, 1.0); \n" \
"}                                                 \n" \
"\n";

////////////////////////////////////////////////////////////////////////////////

// global variables
GLuint shaderProgram;
GLuint vector_buffer_object_id; // initializing GLuint id
string fragment_shader_source;
GLuint vertex_array_object_id;

string loadFragmentShader (const char* filename)
{
  string line = "";
  string fragline = "";
  ifstream fragmentShader_ptr (filename);
  while (getline(fragmentShader_ptr, fragline))
  {
    line += fragline;
    line += '\n';
  }
  return line;
}

void getSquareVertices (GLfloat* vertices)
{
  // GLfloat camera_positon = {0, 0, 2};
  GLfloat near_plane = 2;
  GLfloat theta = 30;
  GLfloat screen[2] = {720, 480};

  GLfloat world_height;
  GLfloat world_width;

  world_height = 2 * tan(theta);
  world_width = (world_height * screen[0]) / screen[1];

  GLfloat p1x = -(world_width/2);
  GLfloat p1y = world_height/2;
  GLfloat p1z = 0;

  GLfloat p2x = world_width/2;
  GLfloat p2y = world_height/2;
  GLfloat p2z = 0;

  GLfloat p3x = world_width/2;
  GLfloat p3y = -(world_height/2);
  GLfloat p3z = 0;

  GLfloat p4x = -(world_width/2);
  GLfloat p4y = -(world_height/2);
  GLfloat p4z = 0;

  vertices[0] = p1x;
  vertices[1] = p1y;
  vertices[2] = p1z;

  vertices[3] = p2x;
  vertices[4] = p2y;
  vertices[5] = p2z;

  vertices[6] = p3x;
  vertices[7] = p3y;
  vertices[8] = p3z;

  vertices[9] = p4x;
  vertices[10] = p4y;
  vertices[11] = p4z;
}

void vpInitCanvas()
{
  glViewport(0, 0, 720, 480);
  glClearColor(0.3, 0.1, 0.5, 1.0); // clear color dark purple

  GLfloat vertices[12];
  getSquareVertices(vertices);

  // Buffer Arrays - generate, bind, send data
  glGenBuffers(1, &vector_buffer_object_id); //passing in address of id so original value can be changed
  glBindBuffer(GL_ARRAY_BUFFER, vector_buffer_object_id);
  glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

  // V E R T E X   S H A D E R
  GLuint vertexShader;
  vertexShader = glCreateShader(GL_VERTEX_SHADER);
  glShaderSource(vertexShader, 1, &vertex_shader_source, NULL);
  glCompileShader(vertexShader);

  // catch vertex shader compilation error
  GLint success;
  GLchar infoLog[512];
  glGetShaderiv(vertexShader, GL_COMPILE_STATUS, &success);
  if (!success)
  {
    glGetShaderInfoLog(vertexShader, 512, NULL, infoLog);
    std::cout << "oops. - vertex shader did not compile\n" << infoLog << std::endl;
  }

  // F R A G M E N T   S H A D E R
  GLuint fragmentShader;
  fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);

  const char* fragShader_str = fragment_shader_source.c_str();
  glShaderSource(fragmentShader, 1, &fragShader_str, NULL);

  glCompileShader(fragmentShader);

  // catch fragment shader compilation error
  glGetShaderiv(fragmentShader, GL_COMPILE_STATUS, &success);
  if (!success)
  {
    glGetShaderInfoLog(fragmentShader, 512, NULL, infoLog);
    std::cout << "oops. - fragment shader did not compile\n" << infoLog << std::endl;
  }

  // S H A D E R   P R O G R A M
  // declared in global scope
	shaderProgram = glCreateProgram();
  glAttachShader(shaderProgram, vertexShader);
  glAttachShader(shaderProgram, fragmentShader);
  glLinkProgram(shaderProgram);

  // catch program linking error
  glGetProgramiv(shaderProgram, GL_LINK_STATUS, &success);
  if (!success)
  {
    glGetProgramInfoLog(shaderProgram, 512, NULL, infoLog);
    std::cout << "oops. - shader program did not link\n" << infoLog << std::endl;
  }

  glUseProgram(shaderProgram);

  // like freeing memory?
  glDeleteShader(vertexShader);
  glDeleteShader(fragmentShader);

  glGenVertexArraysAPPLE(1, &vertex_array_object_id);
  glBindVertexArrayAPPLE(vertex_array_object_id);
}

void vpDraw ()
{
  glClear(GL_COLOR_BUFFER_BIT);

  glMatrixMode(GL_PROJECTION);
  glLoadIdentity(); // doing matrix math
  gluOrtho2D(-1.0, 1.0, -1.0, 1.0);

  glMatrixMode(GL_MODELVIEW);
  glLoadIdentity(); // doing matrix math
  glUseProgram(shaderProgram);

  GLuint pos_attr_location = glGetAttribLocation(shaderProgram, "pos");

  glVertexAttribPointer(pos_attr_location, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(GLfloat), (GLvoid*)0);
  glEnableVertexAttribArray(pos_attr_location);

  glBindVertexArrayAPPLE(vertex_array_object_id);
  glDrawArrays(GL_TRIANGLE_FAN, 0, 4);
  glBindVertexArrayAPPLE(0); // unbinding?
  glFlush();
}

int main (int argc, char *argv[])
{
  std::cout << "^^^^ starting program..." << std::endl;

  fragment_shader_source = loadFragmentShader("fragmentShader_v05.fs");

  // std::cout << "^^^^ fragment_shader_source" << fragment_shader_source << std::endl;

  glutInit(&argc, argv);
  glutInitDisplayMode(GLUT_RGBA);
  glutInitWindowSize(720, 480);
  glutInitWindowPosition(50, 50);
  glutCreateWindow("phong phong phong phong phong");
  glutDisplayFunc(vpDraw);
  vpInitCanvas();
  glutMainLoop();

	return 0;
}
