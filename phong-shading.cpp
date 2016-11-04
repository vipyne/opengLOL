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
#include <ctime>
#include <random>
#include "glint.h"

#define MAX_SPHERES 100

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
GLuint texture_id;
GLuint size_of_texture;
GLfloat theta = 0;
clock_t endTime;
GLfloat radiansPerSecond = 180.0 / M_PI * 2.0;
GLfloat data[MAX_SPHERES * 3 * 4] = {0.0};


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

void vpGetSphereInfo(float* data, int sphereIndex, float pos[3], float* velocity, float* radius, float* rgb)
{
  int i = sphereIndex * 3 * 4;

  pos[0] = data[i + 0];
  pos[1] = data[i + 1];
  pos[2] = data[i + 2];
  *radius = data[i + 3];

  velocity[0] = data[i + 4];
  velocity[1] = data[i + 5];
  velocity[2] = data[i + 6];

  rgb[0] = data[i + 8];
  rgb[1] = data[i + 9];
  rgb[2] = data[i + 10];
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

void vpSetSphereInfo(float* data, int sphereIndex, float* pos, float* velocity, float radius, float* rgb)
{
  int i = sphereIndex * 3 * 4;

  // pos
  data[i + 0] = pos[0]; // x
  data[i + 1] = pos[1]; // y
  data[i + 2] = pos[2]; // z
  data[i + 3] = radius; // radius
  // velocity
  data[i + 4] = velocity[0]; // v x
  data[i + 5] = velocity[1]; // v y
  data[i + 6] = velocity[2]; // v z
  data[i + 7] = 0.0; // free for now
  // color
  data[i + 8] = rgb[0]; // r
  data[i + 9] = rgb[1]; // g
  data[i + 10] = rgb[2]; // b
  data[i + 11] = 1.0; // free for now
}

void vpSetupSpheres()
{
  std::default_random_engine generator;
  std::normal_distribution<float> xJitter(0, 0.1);
  std::uniform_real_distribution<float> marginJitter(0.1, 0.5);
  std::uniform_real_distribution<float> colorJitter(0, 1);
  std::exponential_distribution<float> radiusJitter(0.1);
  float prevR = 2.0;
  float pos[3] = {0.0};
  float colors[3] = {0.0};
  float averageX = 0;
  float posY = -3.0;
  float velocity[] = {0.0, 50.0, 0.0};

  for (int i = 0; i < MAX_SPHERES; ++i) {
    float radius;
    pos[0] = xJitter(generator) + averageX;
    pos[1] = posY;
    pos[2] = -2.0;
    radius = radiusJitter(generator);
    colors[0] = colorJitter(generator);
    colors[1] = colorJitter(generator);
    colors[2] = colorJitter(generator);
    posY -= prevR + radius + marginJitter(generator);
    prevR = radius;
    vpSetSphereInfo(data, i, pos, velocity, radius, colors);
    std::cout<<"pos : " << pos[0] << " " << pos[1] << " " << pos[2] << std::endl;
    std::cout<<"radius : " << radius << std::endl;
  }
}

void vpLoadTexture()
{
  int num_of_floats = MAX_SPHERES * 3 * 4;

  vpSetupSpheres();

  glGenTextures(1, &texture_id);
  glBindTexture(GL_TEXTURE_1D, texture_id);
  glTexParameteri(GL_TEXTURE_1D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
  glTexParameteri(GL_TEXTURE_1D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
  glTexParameteri(GL_TEXTURE_1D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
  glTexParameteri(GL_TEXTURE_1D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);

  size_of_texture = num_of_floats / 4.0;

  glTexImage1D(GL_TEXTURE_1D, 0, GL_RGBA32F_ARB, size_of_texture, 0, GL_RGBA, GL_FLOAT, data);
}

void vpInitCanvas()
{
  glViewport(0, 0, 720, 480);
  glClearColor(0.3, 0.1, 0.5, 1.0); // clear color dark purple

  GLfloat vertices[12];
  getSquareVertices(vertices);

  vpLoadTexture();

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

void vpUpdateSpheres(GLfloat dt)
{
  for (int i = 0; i < MAX_SPHERES; ++i) {
    float newPos[3];
    float newVelocity[3];
    float newRadius;
    float newRGB[3];
    vpGetSphereInfo(data, i, newPos, newVelocity, &newRadius, newRGB);
    if (newPos[1] > 3.0)
      newPos[1] = -3.0;
    newPos[0] += newVelocity[0] * dt;
    newPos[1] += newVelocity[1] * dt;
    newPos[2] += newVelocity[2] * dt;
    // std::cout<<"pos : " << newPos[0] << " " << newPos[1] << " " << newPos[2] << " " <<"radius: " << newRadius << std::endl;
    vpSetSphereInfo(data, i, newPos, newVelocity, newRadius, newRGB);
  }
}

void vpTimer(int vp_time)
{
  clock_t startTime = clock();
  GLfloat dt = (float)(clock() - endTime)/CLOCKS_PER_SEC;
  theta += dt * radiansPerSecond;

  vpUpdateSpheres(dt);


  endTime = startTime;
  glutPostRedisplay();
  glutTimerFunc(100, vpTimer, 0);
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

  GLuint theta_u_location = glGetUniformLocation(shaderProgram, "theta");
  glUniform1f(theta_u_location, theta);

  GLuint size_texture_u_location = glGetUniformLocation(shaderProgram, "size_of_texture");
  glUniform1f(size_texture_u_location, size_of_texture);

  GLuint sphere_info_u_location = glGetUniformLocation(shaderProgram, "sphere_info");
  GLuint num_spheres_u_location = glGetUniformLocation(shaderProgram, "num_of_spheres");
  glUniform1f(num_spheres_u_location, MAX_SPHERES);

  glActiveTexture(GL_TEXTURE0);
  glBindTexture(GL_TEXTURE_1D, texture_id);
  glTexImage1D(GL_TEXTURE_1D, 0, GL_RGBA32F_ARB, size_of_texture, 0, GL_RGBA, GL_FLOAT, data);

  glUniform1i(sphere_info_u_location, 0);

  glBindVertexArrayAPPLE(vertex_array_object_id);
  glDrawArrays(GL_TRIANGLE_FAN, 0, 4);
  glBindVertexArrayAPPLE(0); // unbinding?
  glFlush();
}

int main (int argc, char *argv[])
{
  std::cout << "^^^^ starting program..." << std::endl;

  fragment_shader_source = loadFragmentShader("fragmentShader_v14.fs");

  // glut multisampling
  //

  // std::cout << "^^^^ fragment_shader_source" << fragment_shader_source << std::endl;
  glutInit(&argc, argv);
  glutInitDisplayMode(GLUT_RGBA);
  /////////// glutInitDisplayMode(GLUT_RGBA | GLUT_3_2_CORE_PROFILE);
  glutInitWindowSize(720, 480);
  glutInitWindowPosition(50, 50);
  glutCreateWindow("phong phong phong phong phong");
  glutDisplayFunc(vpDraw);
  vpInitCanvas();
  /////////// glEnable(GL_MULTISAMPLE_ARB);

  endTime = clock();
  glutTimerFunc(100, vpTimer, 0);
  glutMainLoop();

	return 0;
}
