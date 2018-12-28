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

GLuint texture_fox, texture_couch, texture_cat, texture_wood, texture_cups, 
			texture_mandarin, texture_flowers;

int mode = 0;
int width = 0, height = 0;
//2 - без текстур по модели Фонга через фрагментный шейдер
//4 - с текстурами по модели Фонга через фрагментный шейдер
GLuint Program_phong, Program_phong_fun, Program_textured, Program_fun;
GLint Unif1, Unif2;
GLuint shader_program;
//transform
glm::mat4 model, viewProjection;
glm::mat3 normaltr;

float view_angle = -45, view_pos = 50, view_rad = 50;
float viewPosition[]{ 0,50,-50 };

//light
float light_angle = 0, light_pos = 50, light_rad = 50;
float light[]{ 0, 5, 0, 1};
float dirPos[]{-2, 2, 0, 0};
float ambient[]{ 0.2f, 0.2f,0.2f,1.0f };
float diffuse[]{ 1.0f,1.0f,1.0f,1.0f };
float specular[]{ 1.0f,1.0f,1.0f,1.0f };
float attenuation[]{ 1.0f,0.0f,0.0f };

std::string objname = "obj/obj.obj";

std::string objtex = "obj/fabric.jpg";
double obj_scale = 0.6;
float rotateX = 0, rotateY = 0, rotateZ = 0;
int lightType = 0;

void makeTextureImage()
{
	texture_fox = SOIL_load_OGL_texture
	(
		"obj/fabric.jpg",
		SOIL_LOAD_AUTO,
		SOIL_CREATE_NEW_ID,
		SOIL_FLAG_MIPMAPS | SOIL_FLAG_INVERT_Y | SOIL_FLAG_NTSC_SAFE_RGB | SOIL_FLAG_COMPRESS_TO_DXT
	);
	texture_couch = SOIL_load_OGL_texture
	(
		"obj/Pokemon.jpg",
		SOIL_LOAD_AUTO,
		SOIL_CREATE_NEW_ID,
		SOIL_FLAG_MIPMAPS | SOIL_FLAG_INVERT_Y | SOIL_FLAG_NTSC_SAFE_RGB | SOIL_FLAG_COMPRESS_TO_DXT
	);	
	texture_cat = SOIL_load_OGL_texture
	(
		"obj/cat.jpg",
		SOIL_LOAD_AUTO,
		SOIL_CREATE_NEW_ID,
		SOIL_FLAG_MIPMAPS | SOIL_FLAG_INVERT_Y | SOIL_FLAG_NTSC_SAFE_RGB | SOIL_FLAG_COMPRESS_TO_DXT
	);
	texture_wood = SOIL_load_OGL_texture
	(
		"obj/wood.jpg",
		SOIL_LOAD_AUTO,
		SOIL_CREATE_NEW_ID,
		SOIL_FLAG_MIPMAPS | SOIL_FLAG_INVERT_Y | SOIL_FLAG_NTSC_SAFE_RGB | SOIL_FLAG_COMPRESS_TO_DXT
	);	
	texture_cups = SOIL_load_OGL_texture
	(
		"obj/cups.png",
		SOIL_LOAD_AUTO,
		SOIL_CREATE_NEW_ID,
		SOIL_FLAG_MIPMAPS | SOIL_FLAG_INVERT_Y | SOIL_FLAG_NTSC_SAFE_RGB | SOIL_FLAG_COMPRESS_TO_DXT
	);
	texture_mandarin = SOIL_load_OGL_texture
	(
		"obj/mandarin.png",
		SOIL_LOAD_AUTO,
		SOIL_CREATE_NEW_ID,
		SOIL_FLAG_MIPMAPS | SOIL_FLAG_INVERT_Y | SOIL_FLAG_NTSC_SAFE_RGB | SOIL_FLAG_COMPRESS_TO_DXT
	);
	texture_flowers = SOIL_load_OGL_texture
	(
		"obj/flower4.png",
		SOIL_LOAD_AUTO,
		SOIL_CREATE_NEW_ID,
		SOIL_FLAG_MIPMAPS | SOIL_FLAG_INVERT_Y | SOIL_FLAG_NTSC_SAFE_RGB | SOIL_FLAG_COMPRESS_TO_DXT
	);
	//glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	//glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	//glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	//glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
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


// без текстур 
void initShader2()
{
	std::string readed = readfile("vertex_phong_gen.shader");
	const char* vsSource = readed.c_str();

	std::string readed2 = readfile("fragment_phong_gen.shader");
	//std::string readed2 = readfile("fragment_phong_bricks.shader");
	//std::string readed2 = readfile("fragment_phong_bathroom.shader");
	const char* fsSource = readed2.c_str();

	GLuint vShader, fShader;
	vShader = glCreateShader(GL_VERTEX_SHADER);
	glShaderSource(vShader, 1, &vsSource, NULL);
	glCompileShader(vShader);

	fShader = glCreateShader(GL_FRAGMENT_SHADER);
	glShaderSource(fShader, 1, &fsSource, NULL);
	glCompileShader(fShader);

	Program_phong = glCreateProgram();
	glAttachShader(Program_phong, vShader);
	glAttachShader(Program_phong, fShader);
	glLinkProgram(Program_phong);

	int link_ok;
	glGetProgramiv(Program_phong, GL_LINK_STATUS, &link_ok);
	if (!link_ok)
	{
		std::cout << "(2)error attach shaders \n";
		GLchar infoLog[512];
		GLint size;
		glGetProgramInfoLog(Program_phong, 512, &size, infoLog);
		std::cout << infoLog;
		return;
	}
	checkOpenGLerror();
}

void initShader3()
{
	std::string readed = readfile("vertex_phong_gen.shader");
	const char* vsSource = readed.c_str();

	std::string readed2 = readfile("fragment_phong_gen_toon.shader");
	//std::string readed2 = readfile("fragment_phong_bricks.shader");
	//std::string readed2 = readfile("fragment_phong_bathroom.shader");
	const char* fsSource = readed2.c_str();

	GLuint vShader, fShader;
	vShader = glCreateShader(GL_VERTEX_SHADER);
	glShaderSource(vShader, 1, &vsSource, NULL);
	glCompileShader(vShader);

	fShader = glCreateShader(GL_FRAGMENT_SHADER);
	glShaderSource(fShader, 1, &fsSource, NULL);
	glCompileShader(fShader);

	Program_phong_fun = glCreateProgram();
	glAttachShader(Program_phong_fun, vShader);
	glAttachShader(Program_phong_fun, fShader);
	glLinkProgram(Program_phong_fun);

	int link_ok;
	glGetProgramiv(Program_phong_fun, GL_LINK_STATUS, &link_ok);
	if (!link_ok)
	{
		std::cout << "(3)error attach shaders \n";
		GLchar infoLog[512];
		GLint size;
		glGetProgramInfoLog(Program_phong_fun, 512, &size, infoLog);
		std::cout << infoLog;
		return;
	}
	checkOpenGLerror();
}

void initShader4()
{
	std::string readed = readfile("vertex_phong.shader");
	const char* vsSource = readed.c_str();

	std::string readed2 = readfile("fragment_phong_textured.shader");
	const char* fsSource = readed2.c_str();

	GLuint vShader, fShader;
	vShader = glCreateShader(GL_VERTEX_SHADER);
	glShaderSource(vShader, 1, &vsSource, NULL);
	glCompileShader(vShader);

	fShader = glCreateShader(GL_FRAGMENT_SHADER);
	glShaderSource(fShader, 1, &fsSource, NULL);
	glCompileShader(fShader);

	Program_textured = glCreateProgram();
	glAttachShader(Program_textured, vShader);
	glAttachShader(Program_textured, fShader);
	glLinkProgram(Program_textured);

	int link_ok;
	glGetProgramiv(Program_textured, GL_LINK_STATUS, &link_ok);
	if (!link_ok)
	{
		std::cout << "(4)error attach shaders \n";
		GLchar infoLog[512];
		GLint size;
		glGetProgramInfoLog(Program_textured, 512, &size, infoLog);
		std::cout << infoLog;
		return;
	}

	checkOpenGLerror();
}

void initShader6()
{
	std::string readed = readfile("vertex_phong.shader");
	const char* vsSource = readed.c_str();

	std::string readed2 = readfile("fragment_phong_toon.shader");
	const char* fsSource = readed2.c_str();

	GLuint vShader, fShader;
	vShader = glCreateShader(GL_VERTEX_SHADER);
	glShaderSource(vShader, 1, &vsSource, NULL);
	glCompileShader(vShader);

	fShader = glCreateShader(GL_FRAGMENT_SHADER);
	glShaderSource(fShader, 1, &fsSource, NULL);
	glCompileShader(fShader);

	Program_fun = glCreateProgram();
	glAttachShader(Program_fun, vShader);
	glAttachShader(Program_fun, fShader);
	glLinkProgram(Program_fun);

	int link_ok;
	glGetProgramiv(Program_fun, GL_LINK_STATUS, &link_ok);
	if (!link_ok)
	{
		std::cout << "(6)error attach shaders \n";
		GLchar infoLog[512];
		GLint size;
		glGetProgramInfoLog(Program_fun, 512, &size, infoLog);
		std::cout << infoLog;
		return;
	}

	checkOpenGLerror();
}

void initShaders() {
	initShader2();
	initShader3();
	initShader4();
	initShader6();
	shader_program = Program_phong;
}

void freeShader()
{
	glUseProgram(0);
	glDeleteProgram(Program_phong);
	glDeleteProgram(Program_textured);
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
			int i = 0;
			while (ss >> vertex_index >> slash >> uv_index >> slash >> normal_index)
			{
				++i;
				vertex_indices.push_back(vertex_index);
				uv_indices.push_back(uv_index);
				normal_indices.push_back(normal_index);
			}
			if (i > 0 && i < 4)
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

GLuint VAO_mandarin;
std::vector<unsigned short> indices_mandarin;
void initMandarin()
{
	objname = "obj/mandarin.obj";
	obj_scale = 0.05;
	//tryig model
	std::vector<glm::vec3> indexed_vertices;
	std::vector<glm::vec2> indexed_uvs;
	std::vector<glm::vec3> indexed_normals;
	GLuint vertexbuffer;
	GLuint uvbuffer;
	GLuint normalbuffer;
	GLuint elementbuffer;
	// Read our .obj file
	std::vector<glm::vec3> vertices;
	std::vector<glm::vec2> uvs;
	std::vector<glm::vec3> normals;
	loadOBJ(objname.c_str(), vertices, uvs, normals);
	indexVBO(vertices, uvs, normals, indices_mandarin, indexed_vertices, indexed_uvs, indexed_normals);

	//gen
	glGenBuffers(1, &vertexbuffer);
	glGenBuffers(1, &uvbuffer);
	glGenBuffers(1, &normalbuffer);
	glGenBuffers(1, &elementbuffer);
	glGenVertexArrays(1, &VAO_mandarin);

	glBindVertexArray(VAO_mandarin);
	//binding
	glBindBuffer(GL_ARRAY_BUFFER, vertexbuffer);
	glBufferData(GL_ARRAY_BUFFER, indexed_vertices.size() * sizeof(glm::vec3), &indexed_vertices[0], GL_STATIC_DRAW);

	glBindBuffer(GL_ARRAY_BUFFER, uvbuffer);
	glBufferData(GL_ARRAY_BUFFER, indexed_uvs.size() * sizeof(glm::vec2), &indexed_uvs[0], GL_STATIC_DRAW);

	glBindBuffer(GL_ARRAY_BUFFER, normalbuffer);
	glBufferData(GL_ARRAY_BUFFER, indexed_normals.size() * sizeof(glm::vec3), &indexed_normals[0], GL_STATIC_DRAW);

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, elementbuffer);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices_mandarin.size() * sizeof(unsigned short), &indices_mandarin[0], GL_STATIC_DRAW);
	//pointers
	// 1rst attribute buffer : vertices
	glBindBuffer(GL_ARRAY_BUFFER, vertexbuffer);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, 0);
	glEnableVertexAttribArray(0);

	// 2rd attribute buffer : normals
	glBindBuffer(GL_ARRAY_BUFFER, normalbuffer);
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 0, 0);
	glEnableVertexAttribArray(1);

	// 3nd attribute buffer : UVs
	glBindBuffer(GL_ARRAY_BUFFER, uvbuffer);
	glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 0, 0);
	glEnableVertexAttribArray(2);

	glBindVertexArray(0);

	glDeleteBuffers(1, &vertexbuffer);
	glDeleteBuffers(1, &uvbuffer);
	glDeleteBuffers(1, &normalbuffer);
	glDeleteBuffers(1, &elementbuffer);
}

GLuint VAO_cups;
std::vector<unsigned short> indices_cups;
void initCups()
{
	objname = "obj/cups.obj";
	obj_scale = 10;
	//tryig model
	std::vector<glm::vec3> indexed_vertices;
	std::vector<glm::vec2> indexed_uvs;
	std::vector<glm::vec3> indexed_normals;
	GLuint vertexbuffer;
	GLuint uvbuffer;
	GLuint normalbuffer;
	GLuint elementbuffer;
	// Read our .obj file
	std::vector<glm::vec3> vertices;
	std::vector<glm::vec2> uvs;
	std::vector<glm::vec3> normals;
	loadOBJ(objname.c_str(), vertices, uvs, normals);
	indexVBO(vertices, uvs, normals, indices_cups, indexed_vertices, indexed_uvs, indexed_normals);

	//gen
	glGenBuffers(1, &vertexbuffer);
	glGenBuffers(1, &uvbuffer);
	glGenBuffers(1, &normalbuffer);
	glGenBuffers(1, &elementbuffer);
	glGenVertexArrays(1, &VAO_cups);

	glBindVertexArray(VAO_cups);
	//binding
	glBindBuffer(GL_ARRAY_BUFFER, vertexbuffer);
	glBufferData(GL_ARRAY_BUFFER, indexed_vertices.size() * sizeof(glm::vec3), &indexed_vertices[0], GL_STATIC_DRAW);

	glBindBuffer(GL_ARRAY_BUFFER, uvbuffer);
	glBufferData(GL_ARRAY_BUFFER, indexed_uvs.size() * sizeof(glm::vec2), &indexed_uvs[0], GL_STATIC_DRAW);

	glBindBuffer(GL_ARRAY_BUFFER, normalbuffer);
	glBufferData(GL_ARRAY_BUFFER, indexed_normals.size() * sizeof(glm::vec3), &indexed_normals[0], GL_STATIC_DRAW);

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, elementbuffer);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices_cups.size() * sizeof(unsigned short), &indices_cups[0], GL_STATIC_DRAW);
	//pointers
	// 1rst attribute buffer : vertices
	glBindBuffer(GL_ARRAY_BUFFER, vertexbuffer);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, 0);
	glEnableVertexAttribArray(0);

	// 2rd attribute buffer : normals
	glBindBuffer(GL_ARRAY_BUFFER, normalbuffer);
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 0, 0);
	glEnableVertexAttribArray(1);

	// 3nd attribute buffer : UVs
	glBindBuffer(GL_ARRAY_BUFFER, uvbuffer);
	glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 0, 0);
	glEnableVertexAttribArray(2);

	glBindVertexArray(0);

	glDeleteBuffers(1, &vertexbuffer);
	glDeleteBuffers(1, &uvbuffer);
	glDeleteBuffers(1, &normalbuffer);
	glDeleteBuffers(1, &elementbuffer);
}

GLuint VAO_table;
std::vector<unsigned short> indices_table;
void initTable()
{
	objname = "obj/table.obj";
	obj_scale = 10;
	//tryig model
	std::vector<glm::vec3> indexed_vertices;
	std::vector<glm::vec2> indexed_uvs;
	std::vector<glm::vec3> indexed_normals;
	GLuint vertexbuffer;
	GLuint uvbuffer;
	GLuint normalbuffer;
	GLuint elementbuffer;
	// Read our .obj file
	std::vector<glm::vec3> vertices;
	std::vector<glm::vec2> uvs;
	std::vector<glm::vec3> normals;
	loadOBJ(objname.c_str(), vertices, uvs, normals);
	indexVBO(vertices, uvs, normals, indices_table, indexed_vertices, indexed_uvs, indexed_normals);

	//gen
	glGenBuffers(1, &vertexbuffer);
	glGenBuffers(1, &uvbuffer);
	glGenBuffers(1, &normalbuffer);
	glGenBuffers(1, &elementbuffer);
	glGenVertexArrays(1, &VAO_table);

	glBindVertexArray(VAO_table);
	//binding
	glBindBuffer(GL_ARRAY_BUFFER, vertexbuffer);
	glBufferData(GL_ARRAY_BUFFER, indexed_vertices.size() * sizeof(glm::vec3), &indexed_vertices[0], GL_STATIC_DRAW);

	glBindBuffer(GL_ARRAY_BUFFER, uvbuffer);
	glBufferData(GL_ARRAY_BUFFER, indexed_uvs.size() * sizeof(glm::vec2), &indexed_uvs[0], GL_STATIC_DRAW);

	glBindBuffer(GL_ARRAY_BUFFER, normalbuffer);
	glBufferData(GL_ARRAY_BUFFER, indexed_normals.size() * sizeof(glm::vec3), &indexed_normals[0], GL_STATIC_DRAW);

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, elementbuffer);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices_table.size() * sizeof(unsigned short), &indices_table[0], GL_STATIC_DRAW);
	//pointers
	// 1rst attribute buffer : vertices
	glBindBuffer(GL_ARRAY_BUFFER, vertexbuffer);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, 0);
	glEnableVertexAttribArray(0);

	// 2rd attribute buffer : normals
	glBindBuffer(GL_ARRAY_BUFFER, normalbuffer);
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 0, 0);
	glEnableVertexAttribArray(1);

	// 3nd attribute buffer : UVs
	glBindBuffer(GL_ARRAY_BUFFER, uvbuffer);
	glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 0, 0);
	glEnableVertexAttribArray(2);

	glBindVertexArray(0);

	glDeleteBuffers(1, &vertexbuffer);
	glDeleteBuffers(1, &uvbuffer);
	glDeleteBuffers(1, &normalbuffer);
	glDeleteBuffers(1, &elementbuffer);
}

GLuint VAO_cat;
std::vector<unsigned short> indices_cat;
void initCat()
{
	objname = "obj/Cat.obj";
	obj_scale = 0.5;
	//tryig model
	std::vector<glm::vec3> indexed_vertices;
	std::vector<glm::vec2> indexed_uvs;
	std::vector<glm::vec3> indexed_normals;
	GLuint vertexbuffer;
	GLuint uvbuffer;
	GLuint normalbuffer;
	GLuint elementbuffer;
	// Read our .obj file
	std::vector<glm::vec3> vertices;
	std::vector<glm::vec2> uvs;
	std::vector<glm::vec3> normals;
	loadOBJ(objname.c_str(), vertices, uvs, normals);
	indexVBO(vertices, uvs, normals, indices_cat, indexed_vertices, indexed_uvs, indexed_normals);

	//gen
	glGenBuffers(1, &vertexbuffer);
	glGenBuffers(1, &uvbuffer);
	glGenBuffers(1, &normalbuffer);
	glGenBuffers(1, &elementbuffer);
	glGenVertexArrays(1, &VAO_cat);

	glBindVertexArray(VAO_cat);
	//binding
	glBindBuffer(GL_ARRAY_BUFFER, vertexbuffer);
	glBufferData(GL_ARRAY_BUFFER, indexed_vertices.size() * sizeof(glm::vec3), &indexed_vertices[0], GL_STATIC_DRAW);

	glBindBuffer(GL_ARRAY_BUFFER, uvbuffer);
	glBufferData(GL_ARRAY_BUFFER, indexed_uvs.size() * sizeof(glm::vec2), &indexed_uvs[0], GL_STATIC_DRAW);

	glBindBuffer(GL_ARRAY_BUFFER, normalbuffer);
	glBufferData(GL_ARRAY_BUFFER, indexed_normals.size() * sizeof(glm::vec3), &indexed_normals[0], GL_STATIC_DRAW);

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, elementbuffer);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices_cat.size() * sizeof(unsigned short), &indices_cat[0], GL_STATIC_DRAW);
	//pointers
	// 1rst attribute buffer : vertices
	glBindBuffer(GL_ARRAY_BUFFER, vertexbuffer);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, 0);
	glEnableVertexAttribArray(0);

	// 2rd attribute buffer : normals
	glBindBuffer(GL_ARRAY_BUFFER, normalbuffer);
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 0, 0);
	glEnableVertexAttribArray(1);

	// 3nd attribute buffer : UVs
	glBindBuffer(GL_ARRAY_BUFFER, uvbuffer);
	glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 0, 0);
	glEnableVertexAttribArray(2);

	glBindVertexArray(0);

	glDeleteBuffers(1, &vertexbuffer);
	glDeleteBuffers(1, &uvbuffer);
	glDeleteBuffers(1, &normalbuffer);
	glDeleteBuffers(1, &elementbuffer);
}

GLuint VAO_couch;
std::vector<unsigned short> indices_couch;
void initCouch()
{
	objname = "obj/couch.obj";
	obj_scale = 0.5;
	//tryig model
	std::vector<glm::vec3> indexed_vertices;
	std::vector<glm::vec2> indexed_uvs;
	std::vector<glm::vec3> indexed_normals;
	GLuint vertexbuffer;
	GLuint uvbuffer;
	GLuint normalbuffer;
	GLuint elementbuffer;
	// Read our .obj file
	std::vector<glm::vec3> vertices;
	std::vector<glm::vec2> uvs;
	std::vector<glm::vec3> normals;
	loadOBJ(objname.c_str(), vertices, uvs, normals);
	indexVBO(vertices, uvs, normals, indices_couch, indexed_vertices, indexed_uvs, indexed_normals);

	//gen
	glGenBuffers(1, &vertexbuffer);
	glGenBuffers(1, &uvbuffer);
	glGenBuffers(1, &normalbuffer);
	glGenBuffers(1, &elementbuffer);
	glGenVertexArrays(1, &VAO_couch);

	glBindVertexArray(VAO_couch);
	//binding
	glBindBuffer(GL_ARRAY_BUFFER, vertexbuffer);
	glBufferData(GL_ARRAY_BUFFER, indexed_vertices.size() * sizeof(glm::vec3), &indexed_vertices[0], GL_STATIC_DRAW);

	glBindBuffer(GL_ARRAY_BUFFER, uvbuffer);
	glBufferData(GL_ARRAY_BUFFER, indexed_uvs.size() * sizeof(glm::vec2), &indexed_uvs[0], GL_STATIC_DRAW);

	glBindBuffer(GL_ARRAY_BUFFER, normalbuffer);
	glBufferData(GL_ARRAY_BUFFER, indexed_normals.size() * sizeof(glm::vec3), &indexed_normals[0], GL_STATIC_DRAW);

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, elementbuffer);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices_couch.size() * sizeof(unsigned short), &indices_couch[0], GL_STATIC_DRAW);
	//pointers
	// 1rst attribute buffer : vertices
	glBindBuffer(GL_ARRAY_BUFFER, vertexbuffer);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, 0);
	glEnableVertexAttribArray(0);

	// 2rd attribute buffer : normals
	glBindBuffer(GL_ARRAY_BUFFER, normalbuffer);
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 0, 0);
	glEnableVertexAttribArray(1);

	// 3nd attribute buffer : UVs
	glBindBuffer(GL_ARRAY_BUFFER, uvbuffer);
	glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 0, 0);
	glEnableVertexAttribArray(2);

	glBindVertexArray(0);

	glDeleteBuffers(1, &vertexbuffer);
	glDeleteBuffers(1, &uvbuffer);
	glDeleteBuffers(1, &normalbuffer);
	glDeleteBuffers(1, &elementbuffer);
}

GLuint VAO_floor;
std::vector<unsigned short> indices_floor; 
void initFloor()
{
	GLuint VBO_f, IBO_f;
	GLfloat vertices[] = {
		// Позиции          // normals             // Текстурные координаты
		-40.5f,  0.0f, -40.0f,   0.0f, 1.0f, 0.0f,   0.0f, 0.0f,   // Верхний правый
		-40.0f, -0.0f, 40.0f,   0.0f, 1.0f, 0.0f,   0.0f, 1.0f,   // Нижний правый
		40.0f, 0.0f, 40.0f,   0.0f, 1.0f, 0.0f,   1.0f, 1.0f,   // Нижний левый
		40.0f,  0.0f, -40.0f,   0.0f, 1.0f, 0.0f,   1.0f, 0.0f    // Верхний левый
	};

	indices_floor.assign({ 0,1,2,3 });

	glGenBuffers(1, &VBO_f);
	glGenVertexArrays(1, &VAO_floor);
	glGenBuffers(1, &IBO_f);

	glBindVertexArray(VAO_floor);

	glBindBuffer(GL_ARRAY_BUFFER, VBO_f);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, IBO_f);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices_floor.size() * sizeof(GLushort), &indices_floor[0], GL_STATIC_DRAW);

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
	glDeleteBuffers(1, &VBO_f);
	glDeleteBuffers(1, &IBO_f);
}

GLuint VAO_fox;
std::vector<unsigned short> indices_fox;
void initFox()
{
	objname = "obj/obj.obj";
	obj_scale = 0.5;
	//tryig model
	std::vector<glm::vec3> indexed_vertices;
	std::vector<glm::vec2> indexed_uvs;
	std::vector<glm::vec3> indexed_normals;
	GLuint vertexbuffer;
	GLuint uvbuffer;
	GLuint normalbuffer;
	GLuint elementbuffer;
	// Read our .obj file
	std::vector<glm::vec3> vertices;
	std::vector<glm::vec2> uvs;
	std::vector<glm::vec3> normals;
	loadOBJ(objname.c_str(), vertices, uvs, normals);
	indexVBO(vertices, uvs, normals, indices_fox, indexed_vertices, indexed_uvs, indexed_normals);

	//gen
	glGenBuffers(1, &vertexbuffer);
	glGenBuffers(1, &uvbuffer);
	glGenBuffers(1, &normalbuffer);
	glGenBuffers(1, &elementbuffer);
	glGenVertexArrays(1, &VAO_fox);

	glBindVertexArray(VAO_fox);
	//binding
	glBindBuffer(GL_ARRAY_BUFFER, vertexbuffer);
	glBufferData(GL_ARRAY_BUFFER, indexed_vertices.size() * sizeof(glm::vec3), &indexed_vertices[0], GL_STATIC_DRAW);

	glBindBuffer(GL_ARRAY_BUFFER, uvbuffer);
	glBufferData(GL_ARRAY_BUFFER, indexed_uvs.size() * sizeof(glm::vec2), &indexed_uvs[0], GL_STATIC_DRAW);

	glBindBuffer(GL_ARRAY_BUFFER, normalbuffer);
	glBufferData(GL_ARRAY_BUFFER, indexed_normals.size() * sizeof(glm::vec3), &indexed_normals[0], GL_STATIC_DRAW);

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, elementbuffer);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices_fox.size() * sizeof(unsigned short), &indices_fox[0], GL_STATIC_DRAW);
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

	glDeleteBuffers(1, &vertexbuffer);
	glDeleteBuffers(1, &uvbuffer);
	glDeleteBuffers(1, &normalbuffer);
	glDeleteBuffers(1, &elementbuffer);
}

void initBuffers()
{
	initFox();
	initFloor();
	initCouch();
	//initCat();
	initTable();
	initCups();
	initMandarin();
}

void freeBuffers()
{
	glDeleteBuffers(1, &VAO_floor);
	glDeleteBuffers(1, &VAO_fox);
	//glDeleteBuffers(1, &vertexbuffer);
	//glDeleteBuffers(1, &uvbuffer);
	//glDeleteBuffers(1, &normalbuffer);
	//glDeleteBuffers(1, &elementbuffer);

	//glDisableVertexAttribArray(0);
	//glDisableVertexAttribArray(1);
	//glDisableVertexAttribArray(2);
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

void recountCameraPos()
{
	double x = view_rad * glm::cos(view_angle / 180 * pi);
	double z = view_rad * glm::sin(view_angle / 180 * pi);
	viewPosition[0] = x;
	viewPosition[1] = view_pos;
	viewPosition[2] = z;
}

void setTransform(GLuint progr)
{
	/* in shader:
	uniform struct Transform{
	mat4 model;
	mat4 viewProjection;
	mat3 normal;
	vec3 viewPosition;
	} transform;
	*/
	GLint s_model = glGetUniformLocation(progr, "transform.model");
	GLint s_proj = glGetUniformLocation(progr, "transform.viewProjection");
	GLint s_normal = glGetUniformLocation(progr, "transform.normal");
	GLint s_view = glGetUniformLocation(progr, "transform.viewPosition");

	glUniformMatrix4fv(s_model, 1, GL_FALSE, &model[0][0]);
	glUniformMatrix4fv(s_proj, 1, GL_FALSE, &viewProjection[0][0]);
	glUniformMatrix3fv(s_normal, 1, GL_FALSE, &normaltr[0][0]);
	glUniform3fv(s_view, 1, viewPosition);
}

void setLight(GLuint progr)
{
	/* in shader:
	uniform struct PointLight {
		int type;
		vec4 position;
		vec4 ambient;
		vec4 diffuse;
		vec4 specular;
		vec3 attenuation;
		vec3 spotDirection;
		float spotCutoff;
		float spotExponent;
	} light;*/

	GLint s_type = glGetUniformLocation(progr, "light.type");
	GLint s_position = glGetUniformLocation(progr, "light.position");
	GLint s_ambient = glGetUniformLocation(progr, "light.ambient");
	GLint s_diffuse = glGetUniformLocation(progr, "light.diffuse");
	GLint s_specular = glGetUniformLocation(progr, "light.specular");
	GLint s_attenuation = glGetUniformLocation(progr, "light.attenuation");
	GLint s_spD = glGetUniformLocation(progr, "light.spotDirection");
	GLint s_spC = glGetUniformLocation(progr, "light.spotCutoff");
	GLint s_spE = glGetUniformLocation(progr, "light.spotExponent");

	glUniform1i(s_type, lightType);
	if (lightType == 2)
	{
		//direction
		glUniform4fv(s_position, 1, dirPos);
	}
	else
		glUniform4fv(s_position, 1, light);
	glUniform4fv(s_ambient, 1, ambient);
	glUniform4fv(s_diffuse, 1, diffuse);
	glUniform4fv(s_specular, 1, specular);
	glUniform3fv(s_attenuation, 1, attenuation);

	float spDir[]{ 0,0,0 };
	spDir[0] = -light[0];
	spDir[1] = -light[1];
	spDir[2] = -light[2];
	float spCutoff = 30.0f;
	float spExp = 10;
	glUniform3fv(s_spD, 1, spDir);
	glUniform1f(s_spC, spCutoff);
	glUniform1f(s_spE, spExp);
}

void setMaterial(GLuint progr,float* m_ambient, float* m_diffuse, float* m_specular, float* m_emission, float m_shiness)
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

	GLint s_ambient = glGetUniformLocation(progr, "material.ambient");
	GLint s_diffuse = glGetUniformLocation(progr, "material.diffuse");
	GLint s_specular = glGetUniformLocation(progr, "material.specular");
	GLint s_emission = glGetUniformLocation(progr, "material.emission");
	GLint s_shiness = glGetUniformLocation(progr, "material.shiness");

	glUniform4fv(s_ambient, 1, m_ambient);
	glUniform4fv(s_diffuse, 1, m_diffuse);
	glUniform4fv(s_specular, 1, m_specular);
	glUniform4fv(s_emission, 1, m_emission);
	glUniform1f(s_shiness, m_shiness);
}

void drawFloor()
{
	model = glm::mat4(1.0f);

	//rotate scene here
	model = glm::rotate(model, glm::radians(rotateX), glm::vec3(1, 0, 0));
	model = glm::rotate(model, glm::radians(rotateY), glm::vec3(0, 1, 0));
	model = glm::rotate(model, glm::radians(rotateZ), glm::vec3(0, 0, 1));

	normaltr = glm::transpose(glm::inverse(model));

	float red[4] = { 0.7f, 0.7f, 0.7f, 1.0f };
	if (mode)
		shader_program = Program_phong_fun;
	else
		shader_program = Program_phong;
	glUseProgram(shader_program);
	//glUniform4fv(Unif2, 1, red);

	glUseProgram(shader_program);
	setTransform(shader_program);
	setLight(shader_program);
	float m_ambient[]{ 0.2f,0.2f,0.2f,1.0f };
	float m_diffuse[]{ 1.0f,1.0f,1.0f,1.0f };
	float m_specular[]{ 0.2f,0.2f,0.2f,1.0f };
	float m_emission[]{ 0.0f,0.0f,0.0f,1.0f };
	float m_shiness = 0;
	setMaterial(shader_program, m_ambient, m_diffuse, m_specular, m_emission, m_shiness);

	glBindVertexArray(VAO_floor);
	glDrawElements(GL_QUADS, indices_floor.size(), GL_UNSIGNED_SHORT, 0);
	glBindVertexArray(0);

	glUseProgram(0);
}

void drawCat(GLuint progr)
{
	model = glm::mat4(1.0f);

	//rotate scene here
	model = glm::rotate(model, glm::radians(rotateX), glm::vec3(1, 0, 0));
	model = glm::rotate(model, glm::radians(rotateY), glm::vec3(0, 1, 0));
	model = glm::rotate(model, glm::radians(rotateZ), glm::vec3(0, 0, 1));

	//translate&rotate model
	//model = glm::translate(model, glm::vec3(10, 7, 10));
	//model = glm::rotate(model, glm::radians(rotateX), glm::vec3(1, 0, 0));
	model = glm::rotate(model, glm::radians(180.0f), glm::vec3(0, 1, 0));
	//model = glm::rotate(model, glm::radians(rotateZ), glm::vec3(0, 0, 1));

	normaltr = glm::transpose(glm::inverse(model));

	shader_program = progr;
	glUseProgram(shader_program);
	glEnable(GL_TEXTURE_2D);
	glBindTexture(GL_TEXTURE_2D, texture_cat);

	setTransform(shader_program);
	setLight(shader_program);
	float m_ambient[]{ 0.2f,0.2f,0.2f,1.0f };
	float m_diffuse[]{ 1.0f,1.0f,1.0f,1.0f };
	float m_specular[]{ 0.2f,0.2f,0.2f,1.0f };
	float m_emission[]{ 0.0f,0.0f,0.0f,1.0f };
	float m_shiness = 0;
	setMaterial(shader_program, m_ambient, m_diffuse, m_specular, m_emission, m_shiness);

	glBindVertexArray(VAO_cat);
	glDrawElements(GL_QUADS, indices_cat.size(), GL_UNSIGNED_SHORT, 0);
	glBindVertexArray(0);

	glUseProgram(0);
	glDisable(GL_TEXTURE_2D);
}

void drawFox(GLuint progr)
{
	model = glm::mat4(1.0f);

	//rotate scene here
	model = glm::rotate(model, glm::radians(rotateX), glm::vec3(1, 0, 0));
	model = glm::rotate(model, glm::radians(rotateY), glm::vec3(0, 1, 0));
	model = glm::rotate(model, glm::radians(rotateZ), glm::vec3(0, 0, 1));
	
	//translate&rotate model
	model = glm::translate(model, glm::vec3(10,7,10));
	//model = glm::rotate(model, glm::radians(rotateX), glm::vec3(1, 0, 0));
	model = glm::rotate(model, glm::radians(180.0f), glm::vec3(0, 1, 0));
	//model = glm::rotate(model, glm::radians(rotateZ), glm::vec3(0, 0, 1));

	normaltr = glm::transpose(glm::inverse(model));

	shader_program = progr;
	glUseProgram(shader_program);
	glEnable(GL_TEXTURE_2D);
	glBindTexture(GL_TEXTURE_2D, texture_fox);

	setTransform(shader_program);
	setLight(shader_program);
	float m_ambient[]{ 0.2f,0.2f,0.2f,1.0f };
	float m_diffuse[]{ 1.0f,1.0f,1.0f,1.0f };
	float m_specular[]{ 0.2f,0.2f,0.2f,1.0f };
	float m_emission[]{ 0.0f,0.0f,0.0f,1.0f };
	float m_shiness = 2;
	setMaterial(shader_program, m_ambient, m_diffuse, m_specular, m_emission, m_shiness);

	glBindVertexArray(VAO_fox);
	glDrawElements(GL_QUADS, indices_fox.size(), GL_UNSIGNED_SHORT, 0);
	glBindVertexArray(0);

	//another fox
	model = glm::translate(model, glm::vec3(20, 0, 0));
	model = glm::rotate(model, glm::radians(-60.0f), glm::vec3(0, 1, 0));

	normaltr = glm::transpose(glm::inverse(model));

	glBindTexture(GL_TEXTURE_2D, texture_cups);

	setTransform(shader_program);
	setLight(shader_program);
	float m_ambient2[]{ 0.2f,0.2f,0.2f,1.0f };
	float m_diffuse2[]{ 0.2f,0.2f,0.2f,1.0f };
	float m_specular2[]{ 0.0f,0.0f,0.0f,1.0f };
	float m_emission2[]{ 0.0f,0.0f,0.0f,1.0f };
	float m_shiness2 = 0;
	setMaterial(shader_program, m_ambient2, m_diffuse2, m_specular2, m_emission2, m_shiness2);

	glBindVertexArray(VAO_fox);
	glDrawElements(GL_QUADS, indices_fox.size(), GL_UNSIGNED_SHORT, 0);
	glBindVertexArray(0);

	glUseProgram(0);
	glDisable(GL_TEXTURE_2D);
}

void drawTable(GLuint progr)
{
	model = glm::mat4(1.0f);

	//rotate scene here
	model = glm::rotate(model, glm::radians(rotateX), glm::vec3(1, 0, 0));
	model = glm::rotate(model, glm::radians(rotateY), glm::vec3(0, 1, 0));
	model = glm::rotate(model, glm::radians(rotateZ), glm::vec3(0, 0, 1));

	//translate&rotate model
	model = glm::translate(model, glm::vec3(8, 0, -10));
	//model = glm::rotate(model, glm::radians(rotateX), glm::vec3(1, 0, 0));
	//model = glm::rotate(model, glm::radians(180.0f), glm::vec3(0, 1, 0));
	//model = glm::rotate(model, glm::radians(rotateZ), glm::vec3(0, 0, 1));

	normaltr = glm::transpose(glm::inverse(model));

	shader_program = progr;
	glUseProgram(shader_program);
	glEnable(GL_TEXTURE_2D);
	glBindTexture(GL_TEXTURE_2D, texture_wood);

	setTransform(shader_program);
	setLight(shader_program);
	float m_ambient[]{ 0.2f,0.2f,0.2f,1.0f };
	float m_diffuse[]{ 1.0f,1.0f,1.0f,1.0f };
	float m_specular[]{ 0.2f,0.2f,0.2f,1.0f };
	float m_emission[]{ 0.0f,0.0f,0.0f,1.0f };
	float m_shiness = 0;
	setMaterial(shader_program, m_ambient, m_diffuse, m_specular, m_emission, m_shiness);

	glBindVertexArray(VAO_table);
	glDrawElements(GL_QUADS, indices_table.size(), GL_UNSIGNED_SHORT, 0);
	glBindVertexArray(0);

	glUseProgram(0);
	glDisable(GL_TEXTURE_2D);
}

void drawCups(GLuint progr)
{
	model = glm::mat4(1.0f);

	//rotate scene here
	model = glm::rotate(model, glm::radians(rotateX), glm::vec3(1, 0, 0));
	model = glm::rotate(model, glm::radians(rotateY), glm::vec3(0, 1, 0));
	model = glm::rotate(model, glm::radians(rotateZ), glm::vec3(0, 0, 1));

	//translate&rotate model
	model = glm::translate(model, glm::vec3(6, 10.2f, -4));
	//model = glm::rotate(model, glm::radians(rotateX), glm::vec3(1, 0, 0));
	model = glm::rotate(model, glm::radians(180.0f), glm::vec3(0, 1, 0));
	//model = glm::rotate(model, glm::radians(rotateZ), glm::vec3(0, 0, 1));

	normaltr = glm::transpose(glm::inverse(model));

	shader_program = progr;
	glUseProgram(shader_program);
	glEnable(GL_TEXTURE_2D);
	glBindTexture(GL_TEXTURE_2D, texture_cups);

	setTransform(shader_program);
	setLight(shader_program);
	float m_ambient[]{ 0.2f,0.2f,0.2f,1.0f };
	float m_diffuse[]{ 1.0f,1.0f,1.0f,1.0f };
	float m_specular[]{ 0.2f,0.2f,0.2f,1.0f };
	float m_emission[]{ 0.0f,0.0f,0.0f,1.0f };
	float m_shiness = 0;
	setMaterial(shader_program, m_ambient, m_diffuse, m_specular, m_emission, m_shiness);

	glBindVertexArray(VAO_cups);
	glDrawElements(GL_QUADS, indices_cups.size(), GL_UNSIGNED_SHORT, 0);
	glBindVertexArray(0);

	glUseProgram(0);
	glDisable(GL_TEXTURE_2D);
}

void drawCouch(GLuint progr)
{
	model = glm::mat4(1.0f);

	//rotate scene here
	model = glm::rotate(model, glm::radians(rotateX), glm::vec3(1, 0, 0));
	model = glm::rotate(model, glm::radians(rotateY), glm::vec3(0, 1, 0));
	model = glm::rotate(model, glm::radians(rotateZ), glm::vec3(0, 0, 1));

	//translate&rotate model
	model = glm::translate(model, glm::vec3(20, 1, 20));
	//model = glm::rotate(model, glm::radians(rotateX), glm::vec3(1, 0, 0));
	//model = glm::rotate(model, glm::radians(180.0f), glm::vec3(0, 1, 0));
	//model = glm::rotate(model, glm::radians(rotateZ), glm::vec3(0, 0, 1));

	normaltr = glm::transpose(glm::inverse(model));

	shader_program = progr;
	glUseProgram(shader_program);
	glEnable(GL_TEXTURE_2D);
	glBindTexture(GL_TEXTURE_2D, texture_couch);

	setTransform(shader_program);
	setLight(shader_program);
	float m_ambient[]{ 0.2f,0.2f,0.2f,1.0f };
	float m_diffuse[]{ 1.0f,1.0f,1.0f,1.0f };
	float m_specular[]{ 0.2f,0.2f,0.2f,1.0f };
	float m_emission[]{ 0.0f,0.0f,0.0f,1.0f };
	float m_shiness = 0;
	setMaterial(shader_program, m_ambient, m_diffuse, m_specular, m_emission, m_shiness);

	glBindVertexArray(VAO_couch);
	glDrawElements(GL_QUADS, indices_couch.size(), GL_UNSIGNED_SHORT, 0);
	glBindVertexArray(0);

	glUseProgram(0);
	glDisable(GL_TEXTURE_2D);
}

void drawMandarin(GLuint progr)
{
	model = glm::mat4(1.0f);

	//rotate scene here
	model = glm::rotate(model, glm::radians(rotateX), glm::vec3(1, 0, 0));
	model = glm::rotate(model, glm::radians(rotateY), glm::vec3(0, 1, 0));
	model = glm::rotate(model, glm::radians(rotateZ), glm::vec3(0, 0, 1));

	//translate&rotate model
	model = glm::translate(model, glm::vec3(12, 11.0f, -7));

	normaltr = glm::transpose(glm::inverse(model));

	shader_program = progr;
	glEnable(GL_TEXTURE_2D);
	glBindTexture(GL_TEXTURE_2D, texture_mandarin);

	glUseProgram(shader_program);
	setTransform(shader_program);
	setLight(shader_program);
	float m_ambient[]{ 0.2f,0.2f,0.2f,1.0f };
	float m_diffuse[]{ 1.0f,1.0f,1.0f,1.0f };
	float m_specular[]{ 0.2f,0.2f,0.2f,1.0f };
	float m_emission[]{ 0.5f,0.0f,0.0f,1.0f };
	float m_shiness = 8;
	setMaterial(shader_program, m_ambient, m_diffuse, m_specular, m_emission, m_shiness);

	glBindVertexArray(VAO_mandarin);
	glDrawElements(GL_QUADS, indices_mandarin.size(), GL_UNSIGNED_SHORT, 0);
	
	model = glm::translate(model, glm::vec3(3, 0, 0));
	setTransform(shader_program);
	glDrawElements(GL_QUADS, indices_mandarin.size(), GL_UNSIGNED_SHORT, 0);
	
	model = glm::translate(model, glm::vec3(3, 0, 0));
	setTransform(shader_program);
	glDrawElements(GL_QUADS, indices_mandarin.size(), GL_UNSIGNED_SHORT, 0);

	model = glm::translate(model, glm::vec3(-1.5, 0, 3));
	setTransform(shader_program);
	glDrawElements(GL_QUADS, indices_mandarin.size(), GL_UNSIGNED_SHORT, 0);

	model = glm::translate(model, glm::vec3(-3, 0, 0));
	setTransform(shader_program);
	glDrawElements(GL_QUADS, indices_mandarin.size(), GL_UNSIGNED_SHORT, 0);
	
	model = glm::translate(model, glm::vec3(1.5, 0, 3));
	setTransform(shader_program);
	glDrawElements(GL_QUADS, indices_mandarin.size(), GL_UNSIGNED_SHORT, 0);
	glBindVertexArray(0);

	glUseProgram(0);
	glDisable(GL_TEXTURE_2D);
}

void display(void)
{
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glLoadIdentity();
	
	viewProjection = glm::perspective(45.0f, (float)width / (float)height, 0.1f, 200.0f);
	viewProjection *= glm::lookAt(
		glm::vec3(viewPosition[0], viewPosition[1], viewPosition[2]),
		glm::vec3(0, 0, 0),
		glm::vec3(0, 1, 0)
	);
	
	
	drawFloor();
    if (mode)
		shader_program = Program_fun;
	else
	shader_program = Program_textured;
	drawFox(shader_program);
	drawCouch(shader_program);
	drawTable(shader_program);
	drawCups(shader_program);
	drawMandarin(shader_program);
	//drawCat(shader_program);
	glutSwapBuffers();
}

//Организовать управление источником света (перемещение, вращение источника вокруг центра сцены) с помощью шейдеров
void special(int key, int x, int y)
{
	if (lightType != 2)
	{
		switch (key)
		{
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
	case 'w': view_pos += 0.5; break;
	case 's': view_pos -= 0.5; break;
	case 'd': view_angle -= 1; break;
	case 'a': view_angle += 1; break;
	case 'r': view_rad -= 0.5; break;
	case 'f': view_rad += 0.5; break;
	case '=': 
		if (lightType != 2)
		light_pos += 0.5; break;
	case '-': 
		if (lightType != 2)
		light_pos -= 0.5; break;
	case '0': mode = 0; break;
	case '9': mode = 1; break;
	case 'z': lightType = 0; break;
	case 'x': lightType = 1; break;
	case 'c': lightType = 2; break;
	}
	recountCameraPos();
	glutPostRedisplay();
}

int main(int argc, char **argv)
{
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_DEPTH | GLUT_DOUBLE | GLUT_RGB);
	glutInitWindowSize(800, 800);
	glutInitWindowPosition(10, 10);
	glutCreateWindow("individual");

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
	recountCameraPos();

	glutDisplayFunc(display);
	glutReshapeFunc(reshape);
	glutSpecialFunc(special);
	glutKeyboardFunc(keyboard);

	glutMainLoop();

	freeShader();
	freeBuffers();
}