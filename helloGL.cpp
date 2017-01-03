#include <iostream>
#include <math.h>
#include <ctime>
#if ( (defined(__MACH__)) && (defined(__APPLE__)) )   
#include <stdlib.h>
#include <OpenGL/gl.h>
#include <GLUT/glut.h>
#include <OpenGL/glext.h>
#else
#include <stdlib.h>
#include <GL/glew.h>
#include <GL/gl.h>
#include <GL/glut.h>
#include <GL/glext.h>
#endif

using namespace std;

const char *VERTEX_SHADER_SOURCE =                "\n" \
"attribute vec3 pos;                               \n" \
"varying vec4 v_pos;                               \n" \
"void main ()                                      \n" \
"{                                                 \n" \
"   gl_Position = vec4(pos, 1.0);                  \n" \
"   v_pos = vec4(pos.x * 1.5, pos.y, pos.z, 1.0);  \n" \
"}                                                 \n" \
"\n";

const char * FRAGMENT_SHADER_SOURCE =    "\n" \
"void main ()                             \n" \
"{                                        \n" \
"   gl_FragColor = vec4(0.3, 0.55, 0.7, 1.0); \n" \
"}                                        \n" \
"\n";

GLuint theShaderProgram;
GLuint theQuadId;
clock_t theLastTime;

void DefineUnitQuad ()
{
   GLfloat vertices[12];
   vertices[0] = -0.5f;
   vertices[1] = -0.5f;
   vertices[2] =  0.0f;
 
   vertices[3] = -0.5f;
   vertices[4] =  0.5f;
   vertices[5] =  0.0f;
 
   vertices[6] = 0.5f;
   vertices[7] = 0.5f;
   vertices[8] = 0.0f;
 
   vertices[9] =  0.5f;
   vertices[10] = -0.5f;
   vertices[11] = 0.0f;

   glGenBuffers(1, &theQuadId); 
   glBindBuffer(GL_ARRAY_BUFFER, theQuadId);
   glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);
}

void InitCanvas()
{
  glViewport(0, 0, 720, 480);
  glClearColor(0.1, 0.1, 0.1, 1.0); 

  DefineUnitQuad();

  GLuint vertexShader = glCreateShader(GL_VERTEX_SHADER);
  glShaderSource(vertexShader, 1, &VERTEX_SHADER_SOURCE, NULL);
  glCompileShader(vertexShader);

  // catch vertex shader compilation error
  GLint success;
  GLchar infoLog[512];
  glGetShaderiv(vertexShader, GL_COMPILE_STATUS, &success);
  if (!success)
  {
    glGetShaderInfoLog(vertexShader, 512, NULL, infoLog);
    std::cout << "Vertex shader did not compile\n" << infoLog << std::endl;
  }

  GLuint fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
  glShaderSource(fragmentShader, 1, &FRAGMENT_SHADER_SOURCE, NULL);
  glCompileShader(fragmentShader);

  // catch fragment shader compilation error
  glGetShaderiv(fragmentShader, GL_COMPILE_STATUS, &success);
  if (!success)
  {
    glGetShaderInfoLog(fragmentShader, 512, NULL, infoLog);
    std::cout << "Fragment shader did not compile\n" << infoLog << std::endl;
  }

  theShaderProgram = glCreateProgram();
  glAttachShader(theShaderProgram, vertexShader);
  glAttachShader(theShaderProgram, fragmentShader);
  glLinkProgram(theShaderProgram);

  // catch program linking error
  glGetProgramiv(theShaderProgram, GL_LINK_STATUS, &success);
  if (!success)
  {
    glGetProgramInfoLog(theShaderProgram, 512, NULL, infoLog);
    std::cout << "Shader program did not link\n" << infoLog << std::endl;
  }

  glDeleteShader(vertexShader);
  glDeleteShader(fragmentShader);
}

void Timer(int vp_time)
{
  clock_t startTime = clock();
  GLfloat dt = (float)(clock() - theLastTime)/CLOCKS_PER_SEC;

  theLastTime = startTime;
  glutPostRedisplay();
  glutTimerFunc(100, Timer, 0);
}

void Draw ()
{
  glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

  glMatrixMode(GL_PROJECTION);
  glLoadIdentity(); // doing matrix math
  glOrtho(-1.0, 1.0, -1.0, 1.0, -10, 10);

  glMatrixMode(GL_MODELVIEW);
  glLoadIdentity(); // doing matrix math
  glUseProgram(theShaderProgram);

  GLuint posAttrLoc = glGetAttribLocation(theShaderProgram, "pos");
  glVertexAttribPointer(posAttrLoc, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(GLfloat), (GLvoid*)0);
  glEnableVertexAttribArray(posAttrLoc);

  glBindVertexArray(theQuadId);
  glDrawArrays(GL_TRIANGLE_FAN, 0, 4);
  glFlush();
}

int main (int argc, char *argv[])
{
   glutInit(&argc, argv);
   glutInitDisplayMode(GLUT_RGBA);
   glutInitWindowSize(500, 500);
   glutInitWindowPosition(50, 50);
   glutCreateWindow("helloGL");

   glewInit();
   if (glewIsSupported("GL_VERSION_2_0"))
   {
      cout << "Ready for OpenGL 2.0\n";
   }
   else 
   {
      cout << "OpenGL 2.0 not supported\n";
      exit(1);
   }

   InitCanvas();
   glutTimerFunc(100, Timer, 0);
   glutDisplayFunc(Draw);
   glutMainLoop();

   return 0;
}
