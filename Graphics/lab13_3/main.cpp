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
GLuint texture1, texture2;
int width = 0, height = 0;
int mode = 1;
GLuint Program1, Program2, Program3;
GLint Unif_tex_1, Unif_tex_2;
GLuint VBO, VAO;

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
	texture2 = SOIL_load_OGL_texture
	(
		"rainbow.jpg",
		SOIL_LOAD_AUTO,
		SOIL_CREATE_NEW_ID,
		SOIL_FLAG_MIPMAPS | SOIL_FLAG_INVERT_Y | SOIL_FLAG_NTSC_SAFE_RGB | SOIL_FLAG_COMPRESS_TO_DXT
	);
}
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

void initShader2()
{
	std::string readed = readfile("vertex.shader");
	const char* vsSource = readed.c_str();

	std::string readed2 = readfile("fragment2.shader");
	const char* fsSource = readed2.c_str();

	GLuint vShader, fShader;
	vShader = glCreateShader(GL_VERTEX_SHADER);
	glShaderSource(vShader, 1, &vsSource, NULL);
	glCompileShader(vShader);

	fShader = glCreateShader(GL_FRAGMENT_SHADER);
	glShaderSource(fShader, 1, &fsSource, NULL);
	glCompileShader(fShader);

	Program2 = glCreateProgram();
	glAttachShader(Program2, vShader);
	glAttachShader(Program2, fShader);
	glLinkProgram(Program2);

	int link_ok;
	glGetProgramiv(Program2, GL_LINK_STATUS, &link_ok);
	if (!link_ok)
	{
		std::cout << "error attach shaders \n";
		return;
	}
}

void initShader3()
{
	std::string readed = readfile("vertex.shader");
	const char* vsSource = readed.c_str();

	std::string readed2 = readfile("fragment3.shader");
	const char* fsSource = readed2.c_str();

	GLuint vShader, fShader;
	vShader = glCreateShader(GL_VERTEX_SHADER);
	glShaderSource(vShader, 1, &vsSource, NULL);
	glCompileShader(vShader);

	fShader = glCreateShader(GL_FRAGMENT_SHADER);
	glShaderSource(fShader, 1, &fsSource, NULL);
	glCompileShader(fShader);

	Program3 = glCreateProgram();
	glAttachShader(Program3, vShader);
	glAttachShader(Program3, fShader);
	glLinkProgram(Program3);

	int link_ok;
	glGetProgramiv(Program3, GL_LINK_STATUS, &link_ok);
	if (!link_ok)
	{
		std::cout << "error attach shaders \n";
		return;
	}

	const char* tex1_name = "ourTexture1";
	Unif_tex_1 = glGetUniformLocation(Program3, tex1_name);
	if (Unif_tex_1 == -1)
	{
		std::cout << "could not bind uniform " << tex1_name << std::endl;
		return;
	}
	const char* tex2_name = "ourTexture2";
	Unif_tex_2 = glGetUniformLocation(Program3, tex2_name);
	if (Unif_tex_2 == -1)
	{
		std::cout << "could not bind uniform " << tex2_name << std::endl;
		return;
	}
	checkOpenGLerror();
}

void freeShader()
{
	glUseProgram(0);
	glDeleteProgram(Program1);
	glDeleteProgram(Program2);
	glDeleteProgram(Program3);
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
	/*GLfloat vertices[] = {
		// Позиции          // Цвета             // Текстурные координаты
		 0.5f,  0.5f, 0.0f,   1.0f, 0.0f, 0.0f,   1.0f, 1.0f,   // Верхний правый
		 0.5f, -0.5f, 0.0f,   0.0f, 1.0f, 0.0f,   1.0f, 0.0f,   // Нижний правый
		-0.5f, -0.5f, 0.0f,   0.0f, 0.0f, 1.0f,   0.0f, 0.0f,   // Нижний левый
		-0.5f,  0.5f, 0.0f,   1.0f, 1.0f, 0.0f,   0.0f, 1.0f    // Верхний левый
	};*/

	GLfloat vertices[] = { -0.5f, -0.5f, -1.0f,   1.0f, 0.0f, 0.0f,   0.0f, 0.0f,
				 0.5f, -0.5f, -1.0f,   0.0f, 1.0f, 0.0f,   1.0f, 0.0f,
				 0.0f,  0.5f, -1.0f,   0.0f, 0.0f, 1.0f,   0.5f, 1.0f
	};
	glGenBuffers(1, &VBO);
	// 0. Копируем массив с вершинами в буфер OpenGL
	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);
	// 1. Затем установим указатели на вершинные атрибуты
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(GLfloat), (GLvoid*)0);
	glEnableVertexAttribArray(0);

	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(GLfloat), (GLvoid*)(3 * sizeof(GLfloat)));
	glEnableVertexAttribArray(1);

	glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(GLfloat), (GLvoid*)(6 * sizeof(GLfloat)));
	glEnableVertexAttribArray(2);

	glGenVertexArrays(1, &VAO);
	glBindVertexArray(VAO);
	// 2. Копируем наш массив вершин в буфер для OpenGL
	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);
	//// 3. Устанавливаем указатели на вершинные атрибуты 
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(GLfloat), (GLvoid*)0);
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(GLfloat), (GLvoid*)(3 * sizeof(GLfloat)));
	glEnableVertexAttribArray(1);
	glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(GLfloat), (GLvoid*)(6 * sizeof(GLfloat)));
	glEnableVertexAttribArray(2);
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
	else if (mode == 2)
	{
		glUseProgram(Program2);
		glBindTexture(GL_TEXTURE_2D, texture1);
	}
	else if (mode == 3)
	{
		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, texture1);
		glUniform1i(Unif_tex_1, 0);
		glActiveTexture(GL_TEXTURE1);
		glBindTexture(GL_TEXTURE_2D, texture2);
		glUniform1i(Unif_tex_2, 1);
	}
	glDrawArrays(GL_TRIANGLES, 0, 3);
	

	//фиг знает почему не работает
	/*glBindVertexArray(VAO);
	glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
	glBindVertexArray(0);*/

	glUseProgram(0);
	checkOpenGLerror();
	glDisable(GL_TEXTURE_2D);
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
	initBuffers();
	makeTextureImage();
	initShader1();
	initShader2();
	initShader3();
	init();
	glutDisplayFunc(display);
	glutReshapeFunc(reshape);
	glutKeyboardFunc(keyboard);
	glutMainLoop();

	freeShader();
}