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

GLuint Program;
GLint Attrib_vertex, Unif_color, Unif_matrix, Unif_proj;

float angle_x = 90.0f, angle_y = 90.0f, angle_z = 90.0f;
int axis = 0;

float scale_x = 1, scale_y = 1, scale_z = 1;
//0 - scale, 1 - rotation
int mode = 0;
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
		res += "\n " +line;
	}
	return res;
}

void initShader()
{
	/*const GLchar* vsSource = "attribute vec3 coord;\n" 
		"uniform mat4 matrix;\n" 
		"uniform mat4 projection;\n"
		" void main() {\n" 
		" gl_Position = projection * matrix * vec4(coord, 1.0);\n" 
		"}\n"; */
	std::string readed = readfile("vertex.shader");
	const char* vsSource = readed.c_str();

	/*const GLchar* fsSource = "uniform vec4 color;"
		"void main() {\n" 
		" gl_FragColor = color;\n" 
		"}\n";*/
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

	const char* matrix_name = "matrix";
	Unif_matrix = glGetUniformLocation(Program, matrix_name);
	if (Unif_matrix == -1)
	{
		std::cout << "could not bind uniform " << matrix_name << std::endl;
		return;
	}

	const char* proj_name = "projection";
	Unif_proj = glGetUniformLocation(Program, proj_name);
	if (Unif_proj == -1)
	{
		std::cout << "could not bind uniform " << proj_name << std::endl;
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
	glViewport(0, 0, w, h);
}

glm::mat4 rotate_matrix()
{
	float sin = glm::sin(angle_x*3.14 / 180);
	float cos = glm::sin(angle_x*3.14 / 180);
	//OX
	glm::mat4 m1 = { 1.0f,   0.0f,     0.0f,   0.0f,
					0.0f,  cos,   -sin,  0.0f,
					0.0f,  sin,  cos,  0.0f,
					 0.0f,   0.0f,     0.0f,   1.0f };

	//OY

	sin = glm::sin(angle_y*3.14 / 180);
	cos = glm::sin(angle_y*3.14 / 180);

	glm::mat4 m2 = { cos, 0.0f, -sin, 0.0f,
		0.0f, 1.0f, 0.0f, 0.0f,
		sin, 0.0f, cos, 0.0f,
		0.0f, 0.0f, 0.0f, 1.0f };
	//OZ

	sin = glm::sin(angle_z*3.14 / 180);
	cos = glm::sin(angle_z*3.14 / 180);
	glm::mat4 m3 = { cos, sin, 0.0f, 0.0f,
		-sin, cos, 0.0f, 0.0f,
		0.0f, 0.0f, 1.0f, 0.0f,
		0.0f, 0.0f, 0.0f, 1.0f };
	
	glm::mat4 res = glm::matrixCompMult(m2, m1);
	return m1;
}
void display(void)
{
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glLoadIdentity();
	glUseProgram(Program);
	static float red[4] = { 1.0f, 0.0f, 0.0f, 1.0f };
	glUniform4fv(Unif_color, 1, red);
	
	//glm::mat4 Matrix_projection = glm::perspective(	60.0f, (float)width/(float)height, 1.0f, 100.0f);
	/*glm::mat4 Matrix_projection = { 1.0f, 0.0f, 0.0f, 0.0f,
											  0.0f, 1.0f, 0.0f, 0.0f,
											  0.0f, 0.0f, 0.0f, -1 / (float)1000,
											  0.0f, 0.0f, 0.0f, 1.0f };
*/

	glm::mat4 Projection = glm::perspective(glm::radians(45.0f), (float)width / (float)height, 0.1f, 100.0f);
	glm::mat4 View = glm::lookAt(
		glm::vec3(0, 5, 10),
		glm::vec3(0, 0, 0),
		glm::vec3(0, 1, 0)
	);
	glm::mat4 MVP = Projection * View;
	glUniformMatrix4fv(Unif_proj, 1, GL_FALSE, &MVP[0][0]);

	glm::mat4 S = { scale_x, 0.0f,0.0f, 0.0f,
					0.0f , scale_y, 0.0f, 0.0f ,
					0.0f ,0.0f , scale_z, 0.0f ,
						0.0f ,0.0f ,0.0f, 1.0f };
	float a = angle_x * 3.14f / 180.0f;
	glm::mat4 R = rotate_matrix();

	glm::mat4 Matrix = glm::matrixCompMult(S, R);
	
	glUniformMatrix4fv(Unif_matrix, 1, GL_FALSE, &Matrix[0][0]);

	/*glBegin(GL_QUADS);
	glColor3f(1.0, 0.0, 0.0); glVertex2f(-0.5f, -0.5f);
	glColor3f(0.0, 1.0, 0.0); glVertex2f(-0.5f, 0.5f);
	glColor3f(0.0, 0.0, 1.0); glVertex2f(0.5f, 0.5f);
	glColor3f(1.0, 1.0, 1.0); glVertex2f(0.5f, -0.5f);
	glEnd();*/
	
	//glutSolidCube(1);
	//	glutSolidTeapot(1);
	glutSolidCube(1);
	glFlush();

	glUseProgram(0);

	checkOpenGLerror();

	glutSwapBuffers();
}

void special(int key, int x, int y)
{
	switch (key)
	{
	case GLUT_KEY_F1: mode = 0;
		break;
	case GLUT_KEY_F2: mode = 1;
		break;
	}
}

void keyboard(unsigned char key, int x, int y)
{
	if (mode == 0)
	{
		switch (key)
		{
		case '1': scale_x += 0.1;
			break;
		case '2': scale_x -= 0.1;
			break;
		case '3': scale_y += 0.1;
			break;
		case '4': scale_y -= 0.1;
			break;
		case '5': scale_z += 0.1;
			break;
		case '6': scale_z -= 0.1;
			break;
		}
	}
	else

		switch (key)
		{
		case '1': angle_x += 5;
			break;
		case '2': angle_x -= 5;
			break;
		case '3': angle_y += 5;
			break;
		case '4': angle_y -= 5;
			break;
		case '5': angle_z += 5;
			break;
		case '6': angle_z -= 5;
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

	if (!GLEW_VERSION_2_0) 
	{ 
		std::cout << "No support for OpenGL 2.0 found\n";
	return 1; 
	}

	initShader();

	init();
	glutDisplayFunc(display);
	glutReshapeFunc(reshape);
	glutKeyboardFunc(keyboard);
	glutSpecialFunc(special);
	glutMainLoop();

	freeShader();
}