#include<Windows.h>    
// first include Windows.h header file which is required    
#include<stdio.h>
#include "GL/glew.h"
#include<gl/GL.h>   // GL.h header file    
#include<gl/GLU.h> // GLU.h header file    
#include<gl/glut.h>  // glut.h header file from freeglut\include\GL folder    
#include<conio.h>
#include<math.h>
#include<string>
#include <vector>
#include <glm/glm.hpp>
#include <string>
#include <sstream>
#include <fstream>
#include <iostream>

#include <glm/gtc/matrix_transform.hpp>

int width = 0, height = 0;
int mode = 1;
GLuint Program, vProgram, hProgram;
GLint Attrib_vertex, Unif_color;
GLint Unif_v_color1, Unif_v_color2;
GLint Unif_h_color1, Unif_h_color2;
GLint Unif_v_lines, Unif_h_lines;
float v_color1[]{ 1.0f, 1.0f, 1.0f, 1.0f };
float v_color2[]{ 1.0f, 0.0f, 1.0f, 1.0f };
float h_color1[]{ 1.0f, 0.0f, 0.0f, 1.0f };
float h_color2[]{ 0.0f, 0.0f, 1.0f, 1.0f };
int ver_line = 50;
int hor_line = 50;
void checkOpenGLerror() {
	GLenum errCode;
	if ((errCode = glGetError()) != GL_NO_ERROR)
		std::cout << "OpenGl error! - " << gluErrorString(errCode);
}

std::string readfile(const char* path)
{
	std::string res = "";
	std::ifstream file(path);
	std::string line;
	getline(file, res, '\0');
	while (getline(file, line))
	{
		res += "\n " + line;
	}
	return res;
}

void initShader()
{
	std::string readed = readfile("vertex.shader");
	const char* vsSource = readed.c_str();

	std::string readed2 = readfile("fragment.shader");
	const char* fsSource = readed2.c_str();

	GLuint vShader, fShader;
	vShader = glCreateShader(GL_VERTEX_SHADER);
	glShaderSource(vShader, 1, &vsSource, NULL);
	glCompileShader(vShader);

	fShader = glCreateShader(GL_FRAGMENT_SHADER);
	glShaderSource(fShader, 1, &fsSource, NULL);
	glCompileShader(fShader);

	Program = glCreateProgram();
	glAttachShader(Program, vShader);
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
		std::cout << "could not bind attrib " << attr_name << std::endl;
		return;
	}

	const char* color_name = "color";
	Unif_color = glGetUniformLocation(Program, color_name);
	if (Unif_color == -1)
	{
		std::cout << "could not bind uniform " << color_name << std::endl;
		return;
	}

	checkOpenGLerror();
}

void initVertShader()
{
	std::string readed = readfile("vertex.shader");
	const char* vsSource = readed.c_str();

	std::string readed2 = readfile("fragment_vert.shader");
	const char* fsSource = readed2.c_str();

	GLuint vShader, fShader;
	vShader = glCreateShader(GL_VERTEX_SHADER);
	glShaderSource(vShader, 1, &vsSource, NULL);
	glCompileShader(vShader);

	fShader = glCreateShader(GL_FRAGMENT_SHADER);
	glShaderSource(fShader, 1, &fsSource, NULL);
	glCompileShader(fShader);

	vProgram = glCreateProgram();
	glAttachShader(vProgram, vShader);
	glAttachShader(vProgram, fShader);
	glLinkProgram(vProgram);

	int link_ok;
	glGetProgramiv(vProgram, GL_LINK_STATUS, &link_ok);
	if (!link_ok)
	{
		std::cout << "error attach shaders \n";
		return;
	}

	const char* attr_name = "coord";
	Attrib_vertex = glGetAttribLocation(vProgram, attr_name);
	if (Attrib_vertex == -1)
	{
		std::cout << "could not bind attrib " << attr_name << std::endl;
		return;
	}

	const char* color1_name = "v_color1";
	Unif_v_color1 = glGetUniformLocation(vProgram, color1_name);
	if (Unif_v_color1 == -1)
	{
		std::cout << "could not bind uniform " << color1_name << std::endl;
		return;
	}

	const char* color2_name = "v_color2";
	Unif_v_color2 = glGetUniformLocation(vProgram, color2_name);
	if (Unif_v_color2 == -1)
	{
		std::cout << "could not bind uniform " << color2_name << std::endl;
		return;
	}

	const char* lines_name = "lines";
	Unif_v_lines = glGetUniformLocation(vProgram, lines_name);
	if (Unif_v_lines == -1)
	{
		std::cout << "could not bind uniform " << lines_name << std::endl;
		return;
	}
	checkOpenGLerror();
}

void initHorShader()
{
	std::string readed = readfile("vertex.shader");
	const char* vsSource = readed.c_str();

	std::string readed2 = readfile("fragment_hor.shader");
	const char* fsSource = readed2.c_str();

	GLuint vShader, fShader;
	vShader = glCreateShader(GL_VERTEX_SHADER);
	glShaderSource(vShader, 1, &vsSource, NULL);
	glCompileShader(vShader);

	fShader = glCreateShader(GL_FRAGMENT_SHADER);
	glShaderSource(fShader, 1, &fsSource, NULL);
	glCompileShader(fShader);

	hProgram = glCreateProgram();
	glAttachShader(hProgram, vShader);
	glAttachShader(hProgram, fShader);
	glLinkProgram(hProgram);

	int link_ok;
	glGetProgramiv(hProgram, GL_LINK_STATUS, &link_ok);
	if (!link_ok)
	{
		std::cout << "error attach shaders \n";
		return;
	}

	const char* attr_name = "coord";
	Attrib_vertex = glGetAttribLocation(hProgram, attr_name);
	if (Attrib_vertex == -1)
	{
		std::cout << "could not bind attrib " << attr_name << std::endl;
		return;
	}

	const char* color1_name = "h_color1";
	Unif_h_color1 = glGetUniformLocation(hProgram, color1_name);
	if (Unif_h_color1 == -1)
	{
		std::cout << "could not bind uniform " << color1_name << std::endl;
		return;
	}

	const char* color2_name = "h_color2";
	Unif_h_color2 = glGetUniformLocation(hProgram, color2_name);
	if (Unif_h_color2 == -1)
	{
		std::cout << "could not bind uniform " << color2_name << std::endl;
		return;
	}

	const char* lines_name = "lines";
	Unif_h_lines = glGetUniformLocation(hProgram, lines_name);
	if (Unif_h_lines == -1)
	{
		std::cout << "could not bind uniform " << lines_name << std::endl;
		return;
	}
	checkOpenGLerror();
}

void freeShader()
{
	glUseProgram(0);
	glDeleteProgram(Program);
	glDeleteProgram(vProgram);
	glDeleteProgram(hProgram);
}

void init(void)
{
	glClearColor(0.0, 0.0, 0.0, 0.0);

	glEnable(GL_DEPTH_TEST);
}

void reshape(int w, int h)
{
	width = w; height = h;
	glViewport(0, 0, w, h);
}

void display(void)
{
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glLoadIdentity();
	
	if (mode == 1)
	{
		glUseProgram(Program);
		static float red[4] = { 1.0f, 0.0f, 0.0f, 1.0f };
		glUniform4fv(Unif_color, 1, red);
	}
	else if (mode == 2)
	{
		glUseProgram(vProgram);
		glUniform4fv(Unif_v_color1, 1, v_color1);
		glUniform4fv(Unif_v_color2, 1, v_color2);
		glUniform1i(Unif_v_lines, ver_line);
	}
   else if (mode == 3)
	{
		glUseProgram(hProgram);
		glUniform4fv(Unif_h_color1, 1, h_color1);
		glUniform4fv(Unif_h_color2, 1, h_color2);
		glUniform1i(Unif_h_lines, hor_line);
	}

	glBegin(GL_QUADS);
	glColor3f(1.0, 0.0, 0.0); glVertex2f(-0.5f, -0.5f);
	glColor3f(0.0, 1.0, 0.0); glVertex2f(-0.5f, 0.5f);
	glColor3f(0.0, 0.0, 1.0); glVertex2f(0.5f, 0.5f);
	glColor3f(1.0, 1.0, 1.0); glVertex2f(0.5f, -0.5f);  
	glEnd();

	//glutSolidCube(1);
	//glutSolidTeapot(1);
	//glutSolidCube(1);
	glFlush();

	glUseProgram(0);

	checkOpenGLerror();

	glutSwapBuffers();
}

void keyboard(unsigned char key, int x, int y)
{
	switch (key)
	{
	case '1': mode = 1;
		break;
	case '2':
		mode = 2;
		break;
	case '3':
		mode = 3;
		break;
	case '=':
		if (mode == 2)
			++ver_line;
		else ++hor_line;
		break;
	case '-':
		if (mode == 2)
			--ver_line;
		else --hor_line;
		break;
	}
	glutPostRedisplay();
}

int main(int argc, char **argv)
{
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_DEPTH | GLUT_DOUBLE | GLUT_RGB);
	glutInitWindowSize(800, 800);
	glutInitWindowPosition(10, 10);
	glutCreateWindow("Lab 12");

	GLenum glew_status = glewInit();
	if (GLEW_OK != glew_status) {
		std::cout << "Error: " << glewGetErrorString(glew_status) << "\n";   return 1;
	}

	if (!GLEW_VERSION_2_0)
	{
		std::cout << "No support for OpenGL 2.0 found\n";
		return 1;
	}

	initShader();
	initVertShader();
	initHorShader();
	init();
	glutDisplayFunc(display);
	glutReshapeFunc(reshape);
	glutKeyboardFunc(keyboard);
	glutMainLoop();

	freeShader();
}