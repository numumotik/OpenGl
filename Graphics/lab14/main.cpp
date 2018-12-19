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

#include <SOIL.h>
#include <glm/gtc/matrix_transform.hpp>

unsigned char* image;
GLuint texture1;
int width = 0, height = 0;
//1 - без текстур по модели Фонга через вершинный шейдер
//2 - без текстур по модели Фонга через фрагментный шейдер
//3 - с текстурами по модели Фонга через вершинный шейдер
//4 - с текстурами по модели Фонга через фрагментный шейдер
int mode = 1;
GLuint Program1;
GLint Unif_tex_1, Unif_tex_2, Unif_coef;
GLuint VBO, VAO, IBO;
std::vector<GLushort> indices;

float light_angle = 0, light_pos = 5, light_rad = 10;
glm::vec3 light{ 0, 5, 0 };

void makeTextureImage()
{
	texture1 = SOIL_load_OGL_texture
	(
		"penguin.jpg",
		SOIL_LOAD_AUTO,
		SOIL_CREATE_NEW_ID,
		SOIL_FLAG_MIPMAPS | SOIL_FLAG_INVERT_Y | SOIL_FLAG_NTSC_SAFE_RGB | SOIL_FLAG_COMPRESS_TO_DXT
	);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
}

void checkOpenGLerror() {
	GLenum errCode;
	if ((errCode = glGetError()) != GL_NO_ERROR)
		std::cout << "OpenGl error! - " << errCode << gluErrorString(errCode);
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

//без текстур по модели Фонга через вершинный шейдер - интерполяция Гуро
void initShader1()
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

	Program1 = glCreateProgram();
	glAttachShader(Program1, vShader);
	glAttachShader(Program1, fShader);
	glLinkProgram(Program1);

	int link_ok;
	glGetProgramiv(Program1, GL_LINK_STATUS, &link_ok);
	if (!link_ok)
	{
		std::cout << "error attach shaders \n";
		return;
	}

	checkOpenGLerror();
}

// без текстур по модели Фонга через фрагментный шейдер - интерполяция Фонга
void initShader2()
{
	//todo
}

//с текстурами по модели Фонга через вершинный шейдер - интерполяция Гуро
void initShader3()
{
	//todo
}

//с текстурами по модели Фонга через вершинный шейдер - интерполяция Гуро
void initShader4()
{
	//todo	
}

void freeShader()
{
	glUseProgram(0);
	glDeleteProgram(Program1);
	//glDeleteProgram(Program2);
	//glDeleteProgram(Program3);
	//glDeleteProgram(Program4);
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

void initBuffers()
{
	GLfloat vertices[] = {
		// Позиции          // normals             // Текстурные координаты
		 0.5f,  0.5f, 0.0f,   1.0f, 0.0f, 0.0f,   1.0f, 1.0f,   // Верхний правый
		 0.5f, -0.5f, 0.0f,   0.0f, 1.0f, 0.0f,   1.0f, 0.0f,   // Нижний правый
		-0.5f, -0.5f, 0.0f,   0.0f, 0.0f, 1.0f,   0.0f, 0.0f,   // Нижний левый
		-0.5f,  0.5f, 0.0f,   1.0f, 1.0f, 0.0f,   0.0f, 1.0f    // Верхний левый
	};


	indices.push_back(0);
	indices.push_back(1);
	indices.push_back(2);
	indices.push_back(3);


	glGenBuffers(1, &VBO);
	glGenVertexArrays(1, &VAO);
	glGenBuffers(1, &IBO);

	glBindVertexArray(VAO);

	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, IBO);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices.size() * sizeof(GLushort), &indices[0], GL_STATIC_DRAW);

	//pos
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(GLfloat), (GLvoid*)0);
	glEnableVertexAttribArray(0);
	//normals
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(GLfloat), (GLvoid*)(3 * sizeof(GLfloat)));
	glEnableVertexAttribArray(1);
	//texcoords
	glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(GLfloat), (GLvoid*)(6 * sizeof(GLfloat)));
	glEnableVertexAttribArray(2);

	glBindVertexArray(0);
}

void freeBuffers()
{
	glDeleteBuffers(1, &VBO);
	glDeleteBuffers(1, &VAO);
	glDeleteBuffers(1, &IBO);


	glDisableVertexAttribArray(0);
	glDisableVertexAttribArray(1);
	glDisableVertexAttribArray(2);
}


const double pi = 3.14159265358979323846;
void recountLightPos()
{
	double x = light_rad * glm::cos(light_angle / 180 * pi);
	double z = light_rad * glm::sin(light_angle / 180 * pi);
	light.x = x;
	light.y = light_pos;
	light.z = z;
}

void display(void)
{
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glLoadIdentity();
	glEnable(GL_TEXTURE_2D);

	if (mode == 1)
	{
		glUseProgram(Program1);
		glBindTexture(GL_TEXTURE_2D, texture1);
	}
	//todo 2,3,4
	//...


	glBindVertexArray(VAO);
	glDrawElements(GL_QUADS, indices.size(), GL_UNSIGNED_SHORT, 0);
	glBindVertexArray(0);

	glUseProgram(0);
	checkOpenGLerror();
	glDisable(GL_TEXTURE_2D);
	glutSwapBuffers();
}

//Организовать управление источником света (перемещение, вращение источника вокруг центра сцены) с помощью шейдеров
void special(int key, int x, int y)
{
	switch (key)
	{
	case GLUT_KEY_F1: mode = 1;
		break;
	case GLUT_KEY_F2:
		mode = 2;
		break;
	case GLUT_KEY_F3:
		mode = 3;
		break;
	case GLUT_KEY_F4:
		mode = 4;
		break;
	case GLUT_KEY_UP: light_pos += 0.5; break;
	case GLUT_KEY_DOWN: light_pos -= 0.5; break;
	case GLUT_KEY_RIGHT: light_angle -= 3; break;
	case GLUT_KEY_LEFT: light_angle += 3; break;
	case GLUT_KEY_PAGE_UP: light_rad -= 0.5; break;
	case GLUT_KEY_PAGE_DOWN: light_rad += 0.5; break;
	}
	recountLightPos();
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
	initBuffers();
	makeTextureImage();
	initShader1();
	init();
	recountLightPos();

	glutDisplayFunc(display);
	glutReshapeFunc(reshape);
	glutSpecialFunc(special);

	glutMainLoop();

	freeShader();
	freeBuffers();
}