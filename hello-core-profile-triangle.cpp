
//#include <glbinding/gl/gl.h>
#include <cstdio>
#include <OpenGL/gl.h>
#include <iostream>
#include <GLUT/glut.h>
#include "glint.h"

//using namespace gl;

////////////////////////////////////////////////////////////////////////////////

// Vertex Shader
//
const char *vertex_shader_source = "\n" \
"attribute vec3 pos;        \n" \
"void main ()          \n" \
"{                          \n" \
"   gl_Position = gl_ModelViewProjectionMatrix * vec4(pos, 1.0);    \n" \
"}                    \n" \
"\n";

////////////////////////////////////////////////////////////////////////////////


////////////////////////////////////////////////////////////////////////////////

// Fragment Shader
//
const char *fragment_shader_source = "\n" \
"void main ()                               \n" \
"{                                          \n" \
"   gl_FragColor = vec4(0.3, 0.55, 0.7, 1.0); \n" \
"}                                          \n" \
"\n";

////////////////////////////////////////////////////////////////////////////////
GLuint vector_buffer_object_id; // initializing GLuint id
GLuint shaderProgram;
GLuint vertex_array_object_id;

// shadersPointerToVertices = gl.getAttribLocation(webglolProgram, 'pos');


void vipInitCanvas()
{
  glViewport(0, 0, 1000, 1000);
  glClearColor(0.5, 0.2, 0.8, 1.0);

  GLfloat vertices[] = { -0.5f, -0.5f, -0.5f,
                          0.5f, -0.5f, -0.5f,
                          0.0f, 0.5f, -0.5f };   // array of vertices

  glGenBuffers(1, &vector_buffer_object_id); //passing in ad`dress of id so original value can be changed

  // bind vertex buffer
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

  glShaderSource(fragmentShader, 1, &fragment_shader_source, NULL);

  glCompileShader(fragmentShader);

  // catch fragment shader compilation error
  GLint f_success;
  //CLchar infoLog[512];
  glGetShaderiv(fragmentShader, GL_COMPILE_STATUS, &f_success);

  if (!f_success)
  {
    glGetShaderInfoLog(fragmentShader, 512, NULL, infoLog);
    std::cout << "oops. - fragment shader did not compile\n" << infoLog << std::endl;
  }

  // S H A D E R   P R O G R A M
  shaderProgram = glCreateProgram();

  glAttachShader(shaderProgram, vertexShader);
  glAttachShader(shaderProgram, fragmentShader);
  glLinkProgram(shaderProgram);

  // catch program linking error
  GLint p_success;
  //CLchar infoLog[512];
  glGetProgramiv(shaderProgram, GL_LINK_STATUS, &p_success);

  if (!f_success)
  {
    glGetProgramInfoLog(shaderProgram, 512, NULL, infoLog);
    std::cout << "oops. - shader program did not link\n" << infoLog << std::endl;
  }

  glUseProgram(shaderProgram);

  // // like freeing memory?
  // glDeleteShader(vertexShader);
  // glDeleteShader(fragmentShader);


  // // again that location = 0, in webGL this be `gl.getAttribLocation`
  // glEnableVertexAttribArray(pos_attr_location); // 0


  glGenVertexArraysAPPLE(1, &vertex_array_object_id);

  glBindVertexArrayAPPLE(vertex_array_object_id);
}



void vipDraw ()
{
  std::cout << "vipDraw" << std::endl;
  glClear(GL_COLOR_BUFFER_BIT);

  glMatrixMode(GL_PROJECTION);
  glLoadIdentity(); // doing matrix math
  gluOrtho2D(-1.0, 1.0, -1.0, 1.0);

  glMatrixMode(GL_MODELVIEW);
  glLoadIdentity(); // doing matrix math
  glUseProgram(shaderProgram);


  GLuint pos_attr_location = glGetAttribLocation(shaderProgram, "pos");

  // first arg refers to (location = 0)
  // values per vertex
  // not sure when we would ever not use GL_FLOAT here
  // normalize data to between 0-1 - false because our point data already is normalized
  // stride - space between consecutive vertex attribute sets
  // offset of where to start reading from array buffer - needs to be of GLvoid* type
  glVertexAttribPointer(pos_attr_location, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(GLfloat), (GLvoid*)0);
  glEnableVertexAttribArray(pos_attr_location);

  glBindVertexArrayAPPLE(vertex_array_object_id);
  glDrawArrays(GL_TRIANGLES, 0, 3);
  glBindVertexArrayAPPLE(0); // unbinding?
  glFlush();
}

int main (int argc, char** argv)
{
  std::cout << "hi from new version of openGL\n" << std::endl;

  glutInit(&argc, argv);
  glutInitDisplayMode(GLUT_RGBA);
  glutInitWindowSize(1000, 1000);
  glutInitWindowPosition(50, 50);
  glutCreateWindow("hi");
  glutDisplayFunc(vipDraw);
  vipInitCanvas();
  glutMainLoop();

  // glbinding::Binding::initialize();


	return 0;
}
