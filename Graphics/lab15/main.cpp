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
#include <map>
#include <SOIL.h>
#include <glm/gtc/matrix_transform.hpp>

unsigned char* image;
GLuint texture1;
int width = 0, height = 0;
//1 - Ламберта
//2 - Блинна
//3 - Minnaert
//4 - Toon shading
//5 - Ами Гуч
//6 - Rim
int mode = 1;
GLuint Program1, Program2, Program3, Program4, Program5, Program6;
GLuint VAO;
//std::vector<GLushort> indices;
GLuint shader_program;
//transform
glm::mat4 model, viewProjection;
glm::mat3 normaltr;

float viewPosition[]{ 0,0,-50 };

//light
float light_angle = 0, light_pos = 0, light_rad = 50;
float light[]{ 0, 5, 0 };
float ambient[]{ 0.2f, 0.2f,0.2f,1.0f };
//float ambient[]{ 1,1,1,1 };
float diffuse[]{ 1.0f,1.0f,1.0f,1.0f };
float specular[]{ 1.0f,1.0f,1.0f,1.0f };
float attenuation[]{ 1.0f,0.0f,0.0f };

//tryig model
std::vector<glm::vec3> indexed_vertices;
std::vector<glm::vec2> indexed_uvs;
std::vector<glm::vec3> indexed_normals;
std::vector<unsigned short> indices;
GLuint vertexbuffer;
GLuint uvbuffer;
GLuint normalbuffer;
GLuint elementbuffer;

//std::string objname = "Penguin.obj";
//std::string objtex = "penguin.png";
//double obj_scale = 20;
//float rotateX = 0, rotateY = 180, rotateZ = 0;

std::string objname = "Cat.obj";
std::string objtex = "cat.jpg";
double obj_scale = 0.5;
float rotateX = -105, rotateY = 0, rotateZ = 180;


void makeTextureImage()
{
	texture1 = SOIL_load_OGL_texture
	(
		objtex.c_str(),
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

//1 - Ламберта
void initShader1()
{
	std::string readed = readfile("vertex1.shader");
	const char* vsSource = readed.c_str();

	std::string readed2 = readfile("fragment1.shader");
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
		std::cout << "(1)error attach shaders \n";
		GLchar infoLog[512];
		GLint size;
		glGetProgramInfoLog(Program1, 512, &size, infoLog);
		std::cout << infoLog;
		return;
	}

	checkOpenGLerror();
}

//2 - Блинна
void initShader2()
{
	std::string readed = readfile("vertex2.shader");
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
		std::cout << "(2)error attach shaders \n";
		GLchar infoLog[512];
		GLint size;
		glGetProgramInfoLog(Program2, 512, &size, infoLog);
		std::cout << infoLog;
		return;
	}

	checkOpenGLerror();
}

//3 - Minnaert
void initShader3()
{
	std::string readed = readfile("vertex3.shader");
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
		std::cout << "(3)error attach shaders \n";
		GLchar infoLog[512];
		GLint size;
		glGetProgramInfoLog(Program3, 512, &size, infoLog);
		std::cout << infoLog;
		return;
	}

	checkOpenGLerror();
}

//4 - Toon shading
void initShader4()
{
	std::string readed = readfile("vertex4.shader");
	const char* vsSource = readed.c_str();

	std::string readed2 = readfile("fragment4.shader");
	const char* fsSource = readed2.c_str();

	GLuint vShader, fShader;
	vShader = glCreateShader(GL_VERTEX_SHADER);
	glShaderSource(vShader, 1, &vsSource, NULL);
	glCompileShader(vShader);

	fShader = glCreateShader(GL_FRAGMENT_SHADER);
	glShaderSource(fShader, 1, &fsSource, NULL);
	glCompileShader(fShader);

	Program4 = glCreateProgram();
	glAttachShader(Program4, vShader);
	glAttachShader(Program4, fShader);
	glLinkProgram(Program4);

	int link_ok;
	glGetProgramiv(Program4, GL_LINK_STATUS, &link_ok);
	if (!link_ok)
	{
		std::cout << "(4)error attach shaders \n";
		GLchar infoLog[512];
		GLint size;
		glGetProgramInfoLog(Program4, 512, &size, infoLog);
		std::cout << infoLog;
		return;
	}

	checkOpenGLerror();
}

//5 - Ами Гуч
void initShader5()
{
	std::string readed = readfile("vertex5.shader");
	const char* vsSource = readed.c_str();

	std::string readed2 = readfile("fragment5.shader");
	const char* fsSource = readed2.c_str();

	GLuint vShader, fShader;
	vShader = glCreateShader(GL_VERTEX_SHADER);
	glShaderSource(vShader, 1, &vsSource, NULL);
	glCompileShader(vShader);

	fShader = glCreateShader(GL_FRAGMENT_SHADER);
	glShaderSource(fShader, 1, &fsSource, NULL);
	glCompileShader(fShader);

	Program5 = glCreateProgram();
	glAttachShader(Program5, vShader);
	glAttachShader(Program5, fShader);
	glLinkProgram(Program5);

	int link_ok;
	glGetProgramiv(Program5, GL_LINK_STATUS, &link_ok);
	if (!link_ok)
	{
		std::cout << "(5)error attach shaders \n";
		GLchar infoLog[512];
		GLint size;
		glGetProgramInfoLog(Program5, 512, &size, infoLog);
		std::cout << infoLog;
		return;
	}

	checkOpenGLerror();
}

//6 - Rim
void initShader6()
{
	std::string readed = readfile("vertex6.shader");
	const char* vsSource = readed.c_str();

	std::string readed2 = readfile("fragment6.shader");
	const char* fsSource = readed2.c_str();

	GLuint vShader, fShader;
	vShader = glCreateShader(GL_VERTEX_SHADER);
	glShaderSource(vShader, 1, &vsSource, NULL);
	glCompileShader(vShader);

	fShader = glCreateShader(GL_FRAGMENT_SHADER);
	glShaderSource(fShader, 1, &fsSource, NULL);
	glCompileShader(fShader);

	Program6 = glCreateProgram();
	glAttachShader(Program6, vShader);
	glAttachShader(Program6, fShader);
	glLinkProgram(Program6);

	int link_ok;
	glGetProgramiv(Program6, GL_LINK_STATUS, &link_ok);
	if (!link_ok)
	{
		std::cout << "(6)error attach shaders \n";
		GLchar infoLog[512];
		GLint size;
		glGetProgramInfoLog(Program6, 512, &size, infoLog);
		std::cout << infoLog;
		return;
	}

	checkOpenGLerror();
}

void initShaders() {
	initShader1();
	initShader2();
	initShader3();
	initShader4();
	initShader5();
	initShader6();
	shader_program = Program1;
}

void freeShader()
{
	glUseProgram(0);
	glDeleteProgram(Program1);
	glDeleteProgram(Program2);
	glDeleteProgram(Program3);
	glDeleteProgram(Program4);
	glDeleteProgram(Program5);
	glDeleteProgram(Program6);
}

void init(void)
{
	glClearColor(0.0, 0.0, 0.0, 1.0);

	glEnable(GL_DEPTH_TEST);
}

void reshape(int w, int h)
{
	width = w; height = h;
	glViewport(0, 0, w, h);
}

struct PackedVertex
{
	glm::vec3 position;
	glm::vec2 uv;
	glm::vec3 normal;
	bool operator<(const PackedVertex that) const
	{
		return memcmp((void*)this, (void*)&that, sizeof(PackedVertex)) > 0;
	};
};

bool getSimilarVertexIndex_fast(
	PackedVertex & packed,
	std::map<PackedVertex, unsigned short> & VertexToOutIndex,
	unsigned short & result
)
{
	std::map<PackedVertex, unsigned short>::iterator it = VertexToOutIndex.find(packed);
	if (it == VertexToOutIndex.end())
	{
		return false;
	}
	else
	{
		result = it->second;
		return true;
	}
}

void loadOBJ(const std::string & path, std::vector<glm::vec3> & out_vertices, std::vector<glm::vec2> & out_uvs, std::vector<glm::vec3> & out_normals)
{
	std::vector<unsigned int> vertex_indices, uv_indices, normal_indices;
	std::vector<glm::vec3> temp_vertices;
	std::vector<glm::vec2> temp_uvs;
	std::vector<glm::vec3> temp_normals;

	std::ifstream infile(path);
	std::string line;
	while (getline(infile, line))
	{
		std::stringstream ss(line);
		std::string lineHeader;
		getline(ss, lineHeader, ' ');
		if (lineHeader == "v")
		{
			glm::vec3 vertex;
			ss >> vertex.x >> vertex.y >> vertex.z;

			//if (object == Cat)
			//{
				//obj_scale = 2;
			//}
			vertex.x *= obj_scale;
			vertex.y *= obj_scale;
			vertex.z *= obj_scale;
			temp_vertices.push_back(vertex);
		}
		else if (lineHeader == "vt")
		{
			glm::vec2 uv;
			ss >> uv.x >> uv.y;
			temp_uvs.push_back(uv);
		}
		else if (lineHeader == "vn")
		{
			glm::vec3 normal;
			ss >> normal.x >> normal.y >> normal.z;
			//normal.x *= -1;
			//normal.y *= -1;
			//normal.z *= -1;
			temp_normals.push_back(normal);
		}
		else if (lineHeader == "f")
		{
			unsigned int vertex_index, uv_index, normal_index;
			char slash;
			while (ss >> vertex_index >> slash >> uv_index >> slash >> normal_index)
			{
				vertex_indices.push_back(vertex_index);
				uv_indices.push_back(uv_index);
				normal_indices.push_back(normal_index);
			}
		}
	}

	// For each vertex of each triangle
	for (unsigned int i = 0; i < vertex_indices.size(); i++)
	{
		unsigned int vertexIndex = vertex_indices[i];
		glm::vec3 vertex = temp_vertices[vertexIndex - 1];
		out_vertices.push_back(vertex);

		unsigned int uvIndex = uv_indices[i];
		glm::vec2 uv = temp_uvs[uvIndex - 1];
		out_uvs.push_back(uv);

		unsigned int normalIndex = normal_indices[i];
		glm::vec3 normal = temp_normals[normalIndex - 1];
		out_normals.push_back(normal);
	}
}

void indexVBO(
	std::vector<glm::vec3> & in_vertices,
	std::vector<glm::vec2> & in_uvs,
	std::vector<glm::vec3> & in_normals,

	std::vector<unsigned short> & out_indices,
	std::vector<glm::vec3> & out_vertices,
	std::vector<glm::vec2> & out_uvs,
	std::vector<glm::vec3> & out_normals
)
{
	std::map<PackedVertex, unsigned short> VertexToOutIndex;

	// For each input vertex
	for (unsigned int i = 0; i < in_vertices.size(); i++)
	{
		PackedVertex packed = { in_vertices[i], in_uvs[i], in_normals[i] };

		// Try to find a similar vertex in out_XXXX
		unsigned short index;
		bool found = getSimilarVertexIndex_fast(packed, VertexToOutIndex, index);

		if (found)
		{ // A similar vertex is already in the VBO, use it instead!
			out_indices.push_back(index);
		}
		else
		{ // If not, it needs to be added in the output data.
			out_vertices.push_back(in_vertices[i]);
			out_uvs.push_back(in_uvs[i]);
			out_normals.push_back(in_normals[i]);
			unsigned short newindex = (unsigned short)out_vertices.size() - 1;
			out_indices.push_back(newindex);
			VertexToOutIndex[packed] = newindex;
		}
	}
}

void initBuffers()
{
	/*GLfloat vertices[] = {
		// Позиции          // normals             // Текстурные координаты
		 0.5f,  0.5f, 0.0f,   0.0f, 0.0f, 1.0f,   1.0f, 1.0f,   // Верхний правый
		 0.5f, -0.5f, 0.0f,   0.0f, 0.0f, 1.0f,   1.0f, 0.0f,   // Нижний правый
		-0.5f, -0.5f, 0.0f,   0.0f, 0.0f, 1.0f,   0.0f, 0.0f,   // Нижний левый
		-0.5f,  0.5f, 0.0f,   0.0f, 0.0f, 1.0f,   0.0f, 1.0f    // Верхний левый
	};

	indices.assign({ 0,1,2,3 });

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

	glBindVertexArray(0);*/

	// Read our .obj file
	std::vector<glm::vec3> vertices;
	std::vector<glm::vec2> uvs;
	std::vector<glm::vec3> normals;
	loadOBJ(objname.c_str(), vertices, uvs, normals);
	indexVBO(vertices, uvs, normals, indices, indexed_vertices, indexed_uvs, indexed_normals);

	//gen
	glGenBuffers(1, &vertexbuffer);
	glGenBuffers(1, &uvbuffer);
	glGenBuffers(1, &normalbuffer);
	glGenBuffers(1, &elementbuffer);
	glGenVertexArrays(1, &VAO);

	glBindVertexArray(VAO);
	//binding
	glBindBuffer(GL_ARRAY_BUFFER, vertexbuffer);
	glBufferData(GL_ARRAY_BUFFER, indexed_vertices.size() * sizeof(glm::vec3), &indexed_vertices[0], GL_STATIC_DRAW);

	glBindBuffer(GL_ARRAY_BUFFER, uvbuffer);
	glBufferData(GL_ARRAY_BUFFER, indexed_uvs.size() * sizeof(glm::vec2), &indexed_uvs[0], GL_STATIC_DRAW);

	glBindBuffer(GL_ARRAY_BUFFER, normalbuffer);
	glBufferData(GL_ARRAY_BUFFER, indexed_normals.size() * sizeof(glm::vec3), &indexed_normals[0], GL_STATIC_DRAW);

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, elementbuffer);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices.size() * sizeof(unsigned short), &indices[0], GL_STATIC_DRAW);
	//pointers
	// 1rst attribute buffer : vertices
	glBindBuffer(GL_ARRAY_BUFFER, vertexbuffer);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, 0);
	glEnableVertexAttribArray(0);

	// 3rd attribute buffer : normals
	glBindBuffer(GL_ARRAY_BUFFER, normalbuffer);
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 0, 0);
	glEnableVertexAttribArray(1);

	// 2nd attribute buffer : UVs
	glBindBuffer(GL_ARRAY_BUFFER, uvbuffer);
	glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 0, 0);
	glEnableVertexAttribArray(2);



	glBindVertexArray(0);
}

void freeBuffers()
{
	//glDeleteBuffers(1, &VBO);
	glDeleteBuffers(1, &VAO);
	//glDeleteBuffers(1, &IBO);
	glDeleteBuffers(1, &vertexbuffer);
	glDeleteBuffers(1, &uvbuffer);
	glDeleteBuffers(1, &normalbuffer);
	glDeleteBuffers(1, &elementbuffer);

	glDisableVertexAttribArray(0);
	glDisableVertexAttribArray(1);
	glDisableVertexAttribArray(2);
}

const double pi = 3.14159265358979323846;
void recountLightPos()
{
	double x = light_rad * glm::cos(light_angle / 180 * pi);
	double z = light_rad * glm::sin(light_angle / 180 * pi);
	light[0] = x;
	light[1] = light_pos;
	light[2] = z;
}

void setTransform()
{
	/* in shader:
	uniform struct Transform{
	mat4 model;
	mat4 viewProjection;
	mat3 normal;
	vec3 viewPosition;
	} transform;
	*/
	GLint s_model = glGetUniformLocation(shader_program, "transform.model");
	GLint s_proj = glGetUniformLocation(shader_program, "transform.viewProjection");
	GLint s_normal = glGetUniformLocation(shader_program, "transform.normal");
	GLint s_view = glGetUniformLocation(shader_program, "transform.viewPosition");

	glUniformMatrix4fv(s_model, 1, GL_FALSE, &model[0][0]);
	glUniformMatrix4fv(s_proj, 1, GL_FALSE, &viewProjection[0][0]);
	glUniformMatrix3fv(s_normal, 1, GL_FALSE, &normaltr[0][0]);
	glUniform3fv(s_view, 1, viewPosition);

	checkOpenGLerror();
}

void setPointLight()
{
	/* in shader:
	uniform struct PointLight {
		vec4 position;
		vec4 ambient;
		vec4 diffuse;
		vec4 specular;
		vec3 attenuation;
	} light;*/

	GLint s_position = glGetUniformLocation(shader_program, "light.position");
	GLint s_ambient = glGetUniformLocation(shader_program, "light.ambient");
	GLint s_diffuse = glGetUniformLocation(shader_program, "light.diffuse");
	GLint s_specular = glGetUniformLocation(shader_program, "light.specular");
	GLint s_attenuation = glGetUniformLocation(shader_program, "light.attenuation");

	glUniform4fv(s_position, 1, light);
	glUniform4fv(s_ambient, 1, ambient);
	glUniform4fv(s_diffuse, 1, diffuse);
	glUniform4fv(s_specular, 1, specular);
	glUniform3fv(s_attenuation, 1, attenuation);
}

void setMaterial(float* m_ambient, float* m_diffuse, float* m_specular, float* m_emission, float m_shiness)
{
	/* shader:
	uniform struct Material {
		sampler2D texture;
		vec4 ambient;
		vec4 diffuse;
		vec4 specular;
		vec4 emission;
		float shiness;
	} material;*/

	GLint s_ambient = glGetUniformLocation(shader_program, "material.ambient");
	GLint s_diffuse = glGetUniformLocation(shader_program, "material.diffuse");
	GLint s_specular = glGetUniformLocation(shader_program, "material.specular");
	GLint s_emission = glGetUniformLocation(shader_program, "material.emission");
	GLint s_shiness = glGetUniformLocation(shader_program, "material.shiness");

	glUniform4fv(s_ambient, 1, m_ambient);
	glUniform4fv(s_diffuse, 1, m_diffuse);
	glUniform4fv(s_specular, 1, m_specular);
	glUniform4fv(s_emission, 1, m_emission);
	glUniform1f(s_shiness, m_shiness);
}

void Lambert()
{
	glUseProgram(shader_program);
	float fColor[4] = { 1.0f,0.0f,1.0f,1.0f };
	GLint color = glGetUniformLocation(shader_program, "diffColor");
	glUniform4fv(color, 1, fColor);
	checkOpenGLerror();

	setTransform();
	//set point light
	GLint s_position = glGetUniformLocation(shader_program, "light.position");
	glUniform4fv(s_position, 1, light);
	checkOpenGLerror();
}

void Blinn()
{
	glUseProgram(shader_program);
	float fColor[4] = { 1.0f,0.0f,1.0f,1.0f };
	GLint color = glGetUniformLocation(shader_program, "diffColor");
	glUniform4fv(color, 1, fColor);
	checkOpenGLerror();

	setTransform();
	//set point light
	GLint s_position = glGetUniformLocation(shader_program, "light.position");
	glUniform4fv(s_position, 1, light);
	checkOpenGLerror();

	//set material
	GLint s_diffuse = glGetUniformLocation(shader_program, "material.diffuse");
	GLint s_specular = glGetUniformLocation(shader_program, "material.specular");
	GLint s_shiness = glGetUniformLocation(shader_program, "material.shiness");

	float m_diffuse[]{ 0.5f,0.0f,0.0f,1.0f };
	float m_specular[]{ 0.7f,0.7f,0.0f,1.0f };
	float m_shiness = 30;

	glUniform4fv(s_diffuse, 1, m_diffuse);
	glUniform4fv(s_specular, 1, m_specular);
	glUniform1f(s_shiness, m_shiness);
}

void Minnaert()
{
	glUseProgram(shader_program);
	float fColor[4] = { 1.0f,1.0f,0.0f,1.0f };
	GLint color = glGetUniformLocation(shader_program, "diffColor");
	glUniform4fv(color, 1, fColor);
	checkOpenGLerror();

	setTransform();
	//set point light
	GLint s_position = glGetUniformLocation(shader_program, "light.position");
	glUniform4fv(s_position, 1, light);
	checkOpenGLerror();

	//set koef
	float koef = 0.8;
	GLint s_k = glGetUniformLocation(shader_program, "k");
	glUniform1f(s_k, koef);
	checkOpenGLerror();
}

void Toon()
{
	glUseProgram(shader_program);
	float fColor[4] = { 0.0f,1.0f,0.0f,1.0f };
	GLint color = glGetUniformLocation(shader_program, "diffColor");
	glUniform4fv(color, 1, fColor);
	checkOpenGLerror();

	setTransform();
	//set point light
	GLint s_position = glGetUniformLocation(shader_program, "light.position");
	glUniform4fv(s_position, 1, light);
	checkOpenGLerror();
}

void Gooch()
{
	glUseProgram(shader_program);
	float fColor[4] = { 0.75f,0.75f,0.75f,1.0f };
	GLint color = glGetUniformLocation(shader_program, "diffColor");
	glUniform4fv(color, 1, fColor);
	checkOpenGLerror();

	setTransform();
	//set point light
	GLint s_position = glGetUniformLocation(shader_program, "light.position");
	glUniform4fv(s_position, 1, light);
	checkOpenGLerror();

	//set other colors
	float oWarm[4] = { 0.6f,0.6f,0.0f, 1.0f };
	GLint warm = glGetUniformLocation(shader_program, "warm");
	glUniform4fv(warm, 1, oWarm);
	checkOpenGLerror();

	float oCold[4] = {0.0f,0.0f,0.6f, 1.0f};
	GLint cold = glGetUniformLocation(shader_program, "cold");
	glUniform4fv(cold, 1, oCold);
	checkOpenGLerror();

	float oDiffWarm = 0.45;
	GLint dwarm = glGetUniformLocation(shader_program, "diffwarm");
	glUniform1f(dwarm, oDiffWarm);
	checkOpenGLerror();

	float oDiffCold = 0.45;
	GLint dcold = glGetUniformLocation(shader_program, "diffcold");
	glUniform1f(dcold, oDiffCold);
	checkOpenGLerror();
}

void Rim()
{
	glUseProgram(shader_program);

	setTransform();
	//set point light
	GLint s_position = glGetUniformLocation(shader_program, "light.position");
	glUniform4fv(s_position, 1, light);
	checkOpenGLerror();

	//set other colors
	float diffc[4] = { 0.5f,0.0f,0.0f, 1.0f };
	GLint diff = glGetUniformLocation(shader_program, "diff");
	glUniform4fv(diff, 1, diffc);
	checkOpenGLerror();

	float specc[4] = { 0.7f,0.7f,0.0f, 1.0f };
	GLint spec = glGetUniformLocation(shader_program, "spec");
	glUniform4fv(spec, 1, specc);
	checkOpenGLerror();

	float ospecPow = 30.0f;
	GLint specpow = glGetUniformLocation(shader_program, "specPow");
	glUniform1f(specpow, ospecPow);
	checkOpenGLerror();

	float orimPow = 8.0f;
	GLint rimpow = glGetUniformLocation(shader_program, "rimPow");
	glUniform1f(rimpow, orimPow);
	checkOpenGLerror();

	float obias = 0.3f;
	GLint bias = glGetUniformLocation(shader_program, "bias");
	glUniform1f(bias, obias);
	checkOpenGLerror();
}

void display(void)
{	
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glLoadIdentity();

	model = glm::mat4(1.0f);
	//rotate model here
	model = glm::rotate(model, glm::radians(rotateX), glm::vec3(1, 0, 0));
	model = glm::rotate(model, glm::radians(rotateY), glm::vec3(0, 1, 0));
	model = glm::rotate(model, glm::radians(rotateZ), glm::vec3(0, 0, 1));

	viewProjection = glm::perspective(45.0f, (float)width / (float)height, 0.1f, 100.0f);
	viewProjection *= glm::lookAt(
		glm::vec3(viewPosition[0], viewPosition[1], viewPosition[2]),
		glm::vec3(0, 0, 0),
		glm::vec3(0, 1, 0)
	);
	normaltr = glm::transpose(glm::inverse(model));
	
	float red[4] = { 1.0f, 0.0f, 0.0f, 1.0f };
	
	switch (mode)
	{
	case 1: Lambert();
		break;
	case 2: Blinn();
		break;
	case 3: Minnaert();
		break;
	case 4: Toon();
		break;
	case 5: Gooch();
		break;
	case 6: Rim();
		break;
	}

	/*setTransform();
	setPointLight();
	float m_ambient[]{ 1.0f,1.0f,1.0f,1.0f };
	float m_diffuse[]{ 1.0f,1.0f,1.0f,1.0f };
	float m_specular[]{ 0.2f,0.2f,0.2f,1.0f };
	float m_emission[]{ 0.0f,0.0f,0.0f,1.0f };
	float m_shiness = 0;
	setMaterial(m_ambient, m_diffuse, m_specular, m_emission, m_shiness);*/

	
	glBindVertexArray(VAO);
	glDrawElements(GL_QUADS, indices.size(), GL_UNSIGNED_SHORT, 0);
	glBindVertexArray(0);

	glUseProgram(0);
	checkOpenGLerror();
	//if (glIsEnabled(GL_TEXTURE_2D))
	//	glDisable(GL_TEXTURE_2D);
	glutSwapBuffers();
}

//Организовать управление источником света (перемещение, вращение источника вокруг центра сцены) с помощью шейдеров
//1 - Ламберта
//2 - Блинна
//3 - Minnaert
//4 - Toon shading
//5 - Ами Гуч
//6 - Rim
void special(int key, int x, int y)
{
	switch (key)
	{
	case GLUT_KEY_F1: mode = 1;
		shader_program = Program1;
		break;
	case GLUT_KEY_F2:
		mode = 2;
		shader_program = Program2;
		break;
	case GLUT_KEY_F3:
		mode = 3;
		shader_program = Program3;
		break;
	case GLUT_KEY_F4:
		mode = 4;
		shader_program = Program4;
		break;
	case GLUT_KEY_F5:
		mode = 5;
		shader_program = Program5;
		break;
	case GLUT_KEY_F6:
		mode = 6;
		shader_program = Program6;
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

void keyboard(unsigned char key, int x, int y)
{
	switch (key)
	{
	case '1':
		rotateX -= 1;
		break;
	case '2':
		rotateX += 1;
		break;
	case '3':
		rotateY -= 1;
		break;
	case '4':
		rotateY += 1;
		break;
	case '5':
		rotateZ -= 1;
		break;
	case '6':
		rotateZ += 1;
		break;
	default:
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
	initShaders();
	init();
	recountLightPos();

	glutDisplayFunc(display);
	glutReshapeFunc(reshape);
	glutSpecialFunc(special);
	glutKeyboardFunc(keyboard);

	glutMainLoop();

	freeShader();
	freeBuffers();
}