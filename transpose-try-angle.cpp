////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
// vanessa attempts to transpose a triangle
// running exe produces a window with a triangle that turns with mouse movement
//
// compile : $ make tri
// usage   : $ make run
// example : $ make run
//           ==> a window with a triangle in it opens. srsly.
//
// author: vanessa pyne --- github.com/vipyne
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////

#include <cstdio>
#include <iostream>
#include <OpenGL/gl.h>
#include <GLUT/glut.h>
#include <CoreGraphics/CGEvent.h>
#include <math.h>
#include "glint.h"

#define PI 3.14159265

////////////////////////////////////////////////////////////////////////////////

// Vertex Shader
//
const char *vertex_shader_source =                              "\n" \
"attribute vec3 pos;                                             \n" \
"uniform mat4 m_matrixModelView;                                 \n" \
"void main ()                                                    \n" \
"{																												       \n" \
"   gl_Position = m_matrixModelView * vec4(pos, 1.0);            \n" \
"}                                                               \n" \
"\n";

// Fragment Shader
//
const char *fragment_shader_source =         "\n" \
"void main ()                                 \n" \
"{                                            \n" \
"   gl_FragColor = vec4(0.3, 0.55, 0.7, 1.0); \n" \
"}                                            \n" \
"\n";

////////////////////////////////////////////////////////////////////////////////

// global variables
GLuint shaderProgram;
GLuint vector_buffer_object_id; // initializing GLuint id
GLuint vertex_array_object_id;
float* mousePos_ptr;
GLfloat m_modelViewMatrix[16] = { 1.0, 0.0, 0.0, 0.0,
                                  0.0, 1.0, 0.0, 0.0,
                                  0.0, 0.0, 1.0, 0.0,
                                  0.0, 0.0, 0.0, 1.0 };
GLfloat* m_modelViewMatrix_ptr = m_modelViewMatrix;

GLuint pos_attr_location;
GLuint m_matrixModelView_model;


void getMousePosCallback(void) {
  CGEventRef event = CGEventCreate(NULL);
	CGPoint cursor = CGEventGetLocation(event);

  GLfloat x = ((GLfloat) cursor.x) / 1000.0;

  m_modelViewMatrix[0] = x;
}

void vpInitShaders()
{
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
}

void vpInitCanvas()
{
  glViewport(0, 0, 1000, 1000);
  glClearColor(0.5, 0.2, 0.8, 1.0);

  // GLfloat vertices[] = { -0.5f, -0.5f, -0.5f,
  //                         0.5f, -0.5f, -0.5f,
  //                         0.0f,  0.5f, -0.5f };   // array of vertices

  GLfloat vertices[] = { -0.5f, -0.5f, -0.5f,
                          0.5f, -0.5f, -0.5f,
                          0.5f,  0.5f, -0.5f,
                         -0.5f,  0.5f, -0.5f };   //

	// Buffer Arrays - generate, bind, send data
  glGenBuffers(1, &vector_buffer_object_id); //passing in address of id so original value can be changed
  glBindBuffer(GL_ARRAY_BUFFER, vector_buffer_object_id);
  glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_DYNAMIC_DRAW);

  glGenVertexArraysAPPLE(1, &vertex_array_object_id);
  glBindVertexArrayAPPLE(vertex_array_object_id);

  pos_attr_location = glGetAttribLocation(shaderProgram, "pos");
  m_matrixModelView_model = glGetUniformLocation(shaderProgram, "m_matrixModelView");
}

void vpDraw ()
{
  glClear(GL_COLOR_BUFFER_BIT);

  getMousePosCallback();

  glBindVertexArrayAPPLE(vertex_array_object_id);

  // 1 location of attrib we are sending instructions for
  // 2 values per vertex
  // 3 not sure when we would ever not use GL_FLOAT here
  // 4 normalize data to between 0-1 - false because our point data already is normalized
  // 5 stride - space between consecutive vertex attribute sets
  // 6 offset of where to start reading from array buffer - needs to be of GLvoid* type
  glVertexAttribPointer(pos_attr_location, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(GLfloat), (GLvoid*)0);
  glEnableVertexAttribArray(pos_attr_location);

  glUniformMatrix4fv(m_matrixModelView_model, 1, GL_FALSE, m_modelViewMatrix_ptr);

  glDrawArrays(GL_TRIANGLE_FAN, 0, 4);
  glBindVertexArrayAPPLE(0); // unbinding?
  glFlush();

  glutPostRedisplay();
}

int main (int argc, char *argv[])
{
  std::cout << "^^^^ starting program..." << std::endl;

  glutInit(&argc, argv);
  glutInitDisplayMode(GLUT_RGBA);
  glutInitWindowSize(1000, 1000);
  glutInitWindowPosition(0, 0);
  glutCreateWindow("you spin me right round, baby, right round");
  glutDisplayFunc(vpDraw);

	vpInitShaders();
	vpInitCanvas();
  glutMainLoop();

	return 0;
}
