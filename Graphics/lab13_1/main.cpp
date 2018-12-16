#include<Windows.h>    
// first include Windows.h header file which is required    
#include<stdio.h>
#include "GL/glew.h"
#include<gl/GL.h>   // GL.h header file    
#include<gl/GLU.h> // GLU.h header file    
#include<gl/glut.h>  // glut.h header file from freeglut\include\GL folder    
#include<conio.h>
#include<math.h>
#include<string.h>
#include <vector>
#include <glm/glm.hpp>
#include <string>
#include <sstream>
#include <fstream>
#include <iostream>

int width = 0, height = 0;

GLuint Program;
GLint Attrib_vertex, Unif_color;
double rotate_z = 0;

void checkOpenGLerror() {
	GLenum errCode;
	if ((errCode = glGetError()) != GL_NO_ERROR)
		std::cout << "OpenGl error! - " << gluErrorString(errCode);
}

void initShader()
{
	const char* vsSource = 
		"attribute vec2 coord;\n"
		"mat2 rot(in float a) {return mat2(cos(a), sin(a), -sin(a), cos(a));}\n"
		"void main() {\n"
		"vec2 pos = rot(3.14*0.25)*coord;\n"
		"gl_Position = vec4(pos, 0.0, 1.0);\n"
		"}\n";
	const char* fsSource = "uniform vec4 color;\nvoid main() {\ngl_FragColor = color;\n}\n";
	GLuint vShader, fShader;	vShader = glCreateShader(GL_VERTEX_SHADER);	glShaderSource(vShader, 1, &vsSource, NULL);
	glCompileShader(vShader);

	fShader = glCreateShader(GL_FRAGMENT_SHADER);	glShaderSource(fShader, 1, &fsSource, NULL);
	glCompileShader(fShader);

	Program = glCreateProgram();	glAttachShader(Program, vShader);
	glAttachShader(Program, fShader);
	glLinkProgram(Program);

	int link_ok;
	glGetProgramiv(Program, GL_LINK_STATUS, &link_ok);
	if (!link_ok)
	{
		std::cout << "error attach shaders \n";
		return;
	}

	const char* attr_name = "coord";
	Attrib_vertex = glGetAttribLocation(Program, attr_name);
	if (Attrib_vertex == -1)
	{
		std::cout << "could not bind uniform " << attr_name << std::endl;
		return;
	}

	const char* unif_name = "color";
	Unif_color = glGetUniformLocation(Program, unif_name);
	if (Unif_color == -1)
	{
		std::cout << "could not bind uniform " << unif_name << std::endl;
		return;
	}
	checkOpenGLerror();
}

void freeShader()
{
	glUseProgram(0);
	glDeleteProgram(Program);
}

void init(void)
{
	glClearColor(0.0, 0.0, 0.0, 0.0);
	
	glEnable(GL_DEPTH_TEST);
}



void reshape(int w, int h)
{
	width = w; height = h;
	glViewport(0, 0, (GLsizei)w, (GLsizei)h);
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	gluPerspective(60.0, w /h, 1.0, 100.0);
	gluLookAt(0, 5, 5, 0.0, 0.0, 0.0, 0.0, 1.0, 0.0);
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
}

void display(void)
{
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	//glColor3f(1, 1, 1);
	//glutSolidCube(1);
	glLoadIdentity();
	glRotatef(rotate_z, 0, 0, 1);
	glUseProgram(Program);
	static float red[4] = { 1.0f, 0.0f, 0.0f, 1.0f };
	glUniform4fv(Unif_color, 1, red);

	/*glBegin(GL_QUADS);
	glColor3f(1.0, 0.0, 0.0); glVertex2f(-0.5f, -0.5f);
	glColor3f(0.0, 1.0, 0.0); glVertex2f(-0.5f, 0.5f);
	glColor3f(0.0, 0.0, 1.0); glVertex2f(0.5f, 0.5f);
	glColor3f(1.0, 1.0, 1.0); glVertex2f(0.5f, -0.5f);
	glEnd();
	*/
	glutSolidCube(1);
	glFlush();

	glUseProgram(0);

	checkOpenGLerror();

	glutSwapBuffers();
}

void keyboard(unsigned char key, int x, int y)
{
	switch (key)
	{
	case '1': rotate_z += 5;
		break;
	case '2': rotate_z -= 5;
		break;
	}
	glutPostRedisplay();
}

int main(int argc, char **argv)
{
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_DEPTH|GLUT_DOUBLE | GLUT_RGB);
	glutInitWindowSize(800, 800);
	glutInitWindowPosition(10, 10);
	glutCreateWindow("Lab 12");
	
	GLenum glew_status = glewInit();
	if (GLEW_OK != glew_status) {
		std::cout << "Error: " << glewGetErrorString(glew_status) << "\n";   return 1;
	}

	if (!GLEW_VERSION_2_0) { std::cout << "No support for OpenGL 2.0 found\n";   return 1; }

	initShader();

	init();
	glutDisplayFunc(display);
	glutReshapeFunc(reshape);
	glutKeyboardFunc(keyboard);
	glutMainLoop();

	freeShader();
}