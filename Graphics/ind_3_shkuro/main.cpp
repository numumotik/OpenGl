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


GLuint texture_bench, texture_ground, texture_flowers, texture_soil, texture_pine, texture_stone, texture_tree;
int width = 0, height = 0;
GLuint Program_no_t, Program_with_t, Program_floor;
GLint Unif1, Unif2;
GLuint shader_program;
//transform
glm::mat4 model, viewProjection;
glm::mat3 normaltr;

std::string objname;

std::string objtex;
double obj_scale = 1;

GLint Attrib_vertex, Unif_color;
GLint Unif_v_color1, Unif_v_color2;
GLint Unif_floor_color1, Unif_floor_color2;


GLuint VBO, VAO, IBO;

// camera
float camera_angle = 90;
float camera_pos = 20;
float camera_rad = -50;
float viewPosition[]{ 0,0,-50 };

//light
float light_angle = 0, light_pos = 50, light_rad = 50;
float light[]{ 0, 5, 0 };
float ambient[]{ 0.2f, 0.2f,0.2f,1.0f };
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

float rotateX = 0, rotateY = 0, rotateZ = 0;

const double pi = 3.14159265358979323846;


//std::string objname = "Cat.obj";
//std::string objtex = "cat.jpg";
//double obj_scale = 0.5;
//float rotateX = -105, rotateY = 0, rotateZ = 180;

void makeTextureImage()
{
	texture_bench = SOIL_load_OGL_texture
	(
		"models/park_bench/bench_wood_color.jpg",
		SOIL_LOAD_AUTO,
		SOIL_CREATE_NEW_ID,
		SOIL_FLAG_MIPMAPS | SOIL_FLAG_INVERT_Y | SOIL_FLAG_NTSC_SAFE_RGB | SOIL_FLAG_COMPRESS_TO_DXT
	);
	texture_ground = SOIL_load_OGL_texture
	(
		"models/ground3.jpg",
		SOIL_LOAD_AUTO,
		SOIL_CREATE_NEW_ID,
		SOIL_FLAG_MIPMAPS | SOIL_FLAG_INVERT_Y | SOIL_FLAG_NTSC_SAFE_RGB | SOIL_FLAG_COMPRESS_TO_DXT
	);
	texture_flowers = SOIL_load_OGL_texture
	(
		//"models/PRIM1P.png",
		"models/flower2.png",
		SOIL_LOAD_AUTO,
		SOIL_CREATE_NEW_ID,
		SOIL_FLAG_MIPMAPS | SOIL_FLAG_INVERT_Y | SOIL_FLAG_NTSC_SAFE_RGB | SOIL_FLAG_COMPRESS_TO_DXT
	);
	texture_soil = SOIL_load_OGL_texture
	(
		"models/soil.png",
		SOIL_LOAD_AUTO,
		SOIL_CREATE_NEW_ID,
		SOIL_FLAG_MIPMAPS | SOIL_FLAG_INVERT_Y | SOIL_FLAG_NTSC_SAFE_RGB | SOIL_FLAG_COMPRESS_TO_DXT
	);
	texture_pine = SOIL_load_OGL_texture
	(
		"models/pine.png",
		SOIL_LOAD_AUTO,
		SOIL_CREATE_NEW_ID,
		SOIL_FLAG_MIPMAPS | SOIL_FLAG_INVERT_Y | SOIL_FLAG_NTSC_SAFE_RGB | SOIL_FLAG_COMPRESS_TO_DXT
	);
	texture_stone = SOIL_load_OGL_texture
	(
		"models/stone.png",
		SOIL_LOAD_AUTO,
		SOIL_CREATE_NEW_ID,
		SOIL_FLAG_MIPMAPS | SOIL_FLAG_INVERT_Y | SOIL_FLAG_NTSC_SAFE_RGB | SOIL_FLAG_COMPRESS_TO_DXT
	);
	texture_tree = SOIL_load_OGL_texture
	(
		//"models/green.png",
		"models/tree_texture.png",
		SOIL_LOAD_AUTO,
		SOIL_CREATE_NEW_ID,
		SOIL_FLAG_MIPMAPS | SOIL_FLAG_INVERT_Y | SOIL_FLAG_NTSC_SAFE_RGB | SOIL_FLAG_COMPRESS_TO_DXT
	);
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

// без текстур по модели Фонга
void initShader_no_t()
{
	std::string readed = readfile("vertex24.shader");
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

	Program_no_t = glCreateProgram();
	glAttachShader(Program_no_t, vShader);
	glAttachShader(Program_no_t, fShader);
	glLinkProgram(Program_no_t);

	int link_ok;
	glGetProgramiv(Program_no_t, GL_LINK_STATUS, &link_ok);
	if (!link_ok)
	{
		std::cout << "(2)error attach shaders \n";
		GLchar infoLog[512];
		GLint size;
		glGetProgramInfoLog(Program_no_t, 512, &size, infoLog);
		std::cout << infoLog;
		return;
	}
	Unif2 = glGetUniformLocation(Program_no_t, "objColor");
	if (Unif2 == -1)
	{
		std::cout << "could not bind uniform objColor(2)" << std::endl;
		checkOpenGLerror();
		return;
	}
	checkOpenGLerror();
}

//с текстурами по модели Фонга
void initShader_with_t()
{
	std::string readed = readfile("vertex24.shader");
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

	Program_with_t = glCreateProgram();
	glAttachShader(Program_with_t, vShader);
	glAttachShader(Program_with_t, fShader);
	glLinkProgram(Program_with_t);

	int link_ok;
	glGetProgramiv(Program_with_t, GL_LINK_STATUS, &link_ok);
	if (!link_ok)
	{
		std::cout << "(4)error attach shaders \n";
		GLchar infoLog[512];
		GLint size;
		glGetProgramInfoLog(Program_with_t, 512, &size, infoLog);
		std::cout << infoLog;
		return;
	}

	checkOpenGLerror();
}

void initShader_ground()
{
	std::string readed = readfile("vertex_floor.shader");
	const char* vsSource = readed.c_str();

	std::string readed2 = readfile("fragment_floor.shader");
	const char* fsSource = readed2.c_str();

	GLuint vShader, fShader;
	vShader = glCreateShader(GL_VERTEX_SHADER);
	glShaderSource(vShader, 1, &vsSource, NULL);
	glCompileShader(vShader);

	fShader = glCreateShader(GL_FRAGMENT_SHADER);
	glShaderSource(fShader, 1, &fsSource, NULL);
	glCompileShader(fShader);

	Program_floor = glCreateProgram();
	glAttachShader(Program_floor, vShader);
	glAttachShader(Program_floor, fShader);
	glLinkProgram(Program_floor);

	int link_ok;
	glGetProgramiv(Program_floor, GL_LINK_STATUS, &link_ok);
	if (!link_ok)
	{
		std::cout << "error attach shaders \n";
		return;
	}

	const char* attr_name = "coord";
	Attrib_vertex = glGetAttribLocation(Program_floor, attr_name);
	if (Attrib_vertex == -1)
	{
		std::cout << "could not bind attrib " << attr_name << std::endl;
		return;
	}
	checkOpenGLerror();
}

void initShaders() {
	initShader_no_t();
	initShader_with_t();
	shader_program = Program_no_t;
}

void freeShader()
{
	glUseProgram(0);
	glDeleteProgram(Program_no_t);
	glDeleteProgram(Program_with_t);

}

void init(void)
{
	glClearColor(1.0, 1.0, 1.0, 1.0);

	glEnable(GL_DEPTH_TEST);
}

double gr_cos(float angle) noexcept
{
	return cos(angle / 180 * pi);
}

double gr_sin(float angle) noexcept
{
	return sin(angle / 180 * pi);
}

void setCamera()
{
	glLoadIdentity();
	const double x = camera_rad * gr_cos(camera_angle);
	const double z = camera_rad * gr_sin(camera_angle);
	gluLookAt(x, camera_pos, z, 0.0, 0.0, 0.0, 0.0, -50.0, 0.0);

	const GLfloat spot_position[] = { x, camera_pos, z , 1 };
	GLfloat spot_direction[] = { -x, -camera_pos, -z };
	const float length = sqrt(x * x + static_cast<double>(camera_pos) * static_cast<double>(camera_pos) + z * z);
	if (length != 0)
	{
		spot_direction[0] /= length;
		spot_direction[1] /= length;
		spot_direction[2] /= length;
	}
	glLightfv(GL_LIGHT0, GL_SPOT_DIRECTION, spot_direction);
	glLightfv(GL_LIGHT0, GL_POSITION, spot_position);
}

void reshape(int w, int h)
{
	width = w; height = h;
	glViewport(0, 0, w, h);
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	gluPerspective(60.0, static_cast<GLfloat>(w) / static_cast<GLfloat>(h), 1.0, 100.0);
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
	setCamera();
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
			/*normal.x *= -1;
			normal.y *= -1;
			normal.z *= -1;*/
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

GLuint VAO_bench;
std::vector<unsigned short> indices_bench;
void init_bench()
{
	objname = "models/park_bench/bench_v01.obj";
	obj_scale = 0.05;

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
	indexVBO(vertices, uvs, normals, indices_bench, indexed_vertices, indexed_uvs, indexed_normals);

	//gen
	glGenBuffers(1, &vertexbuffer);
	glGenBuffers(1, &uvbuffer);
	glGenBuffers(1, &normalbuffer);
	glGenBuffers(1, &elementbuffer);
	glGenVertexArrays(1, &VAO_bench);

	glBindVertexArray(VAO_bench);
	//binding
	glBindBuffer(GL_ARRAY_BUFFER, vertexbuffer);
	glBufferData(GL_ARRAY_BUFFER, indexed_vertices.size() * sizeof(glm::vec3), &indexed_vertices[0], GL_STATIC_DRAW);

	glBindBuffer(GL_ARRAY_BUFFER, uvbuffer);
	glBufferData(GL_ARRAY_BUFFER, indexed_uvs.size() * sizeof(glm::vec2), &indexed_uvs[0], GL_STATIC_DRAW);

	glBindBuffer(GL_ARRAY_BUFFER, normalbuffer);
	glBufferData(GL_ARRAY_BUFFER, indexed_normals.size() * sizeof(glm::vec3), &indexed_normals[0], GL_STATIC_DRAW);

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, elementbuffer);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices_bench.size() * sizeof(unsigned short), &indices_bench[0], GL_STATIC_DRAW);
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

GLuint VAO_ground;
std::vector<unsigned short> indices_ground;
void init_ground()
{
	GLuint VBO_f, IBO_f;
	GLfloat vertices[] = {
		-40.5f,  0.0f, -40.0f,   0.0f, 1.0f, 0.0f,   0.0f, 0.0f,
		-40.0f, -0.0f, 40.0f,   0.0f, 1.0f, 0.0f,   0.0f, 5.0f,
		40.0f, 0.0f, 40.0f,   0.0f, 1.0f, 0.0f,   5.0f, 5.0f,
		40.0f,  0.0f, -40.0f,   0.0f, 1.0f, 0.0f,   5.0f, 0.0f
	};

	indices_ground.assign({ 0, 1, 2, 3 });

	glGenBuffers(1, &VBO_f);
	glGenVertexArrays(1, &VAO_ground);
	glGenBuffers(1, &IBO_f);

	glBindVertexArray(VAO_ground);

	glBindBuffer(GL_ARRAY_BUFFER, VBO_f);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, IBO_f);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices_ground.size() * sizeof(GLushort), &indices_ground[0], GL_STATIC_DRAW);

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

GLuint VAO_soil;
std::vector<unsigned short> indices_soil;
void init_soil()
{
	GLuint VBO_f, IBO_f;
	GLfloat vertices[] = {
		-20.0f,  0.0f, -20.0f,   0.0f, 1.0f, 0.0f,   0.0f, 0.0f,
		-20.0f, 0.0f, 20.0f,   0.0f, 1.0f, 0.0f,   0.0f, 5.0f,
		20.0f, 0.0f, 20.0f,   0.0f, 1.0f, 0.0f,   5.0f, 5.0f,
		20.0f,  0.0f, -20.0f,   0.0f, 1.0f, 0.0f,   5.0f, 0.0f
	};


	indices_soil.assign({ 0, 1, 2, 3 });

	glGenBuffers(1, &VBO_f);
	glGenVertexArrays(1, &VAO_soil);
	glGenBuffers(1, &IBO_f);

	glBindVertexArray(VAO_soil);

	glBindBuffer(GL_ARRAY_BUFFER, VBO_f);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, IBO_f);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices_soil.size() * sizeof(GLushort), &indices_soil[0], GL_STATIC_DRAW);

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

GLuint VAO_flowers;
std::vector<unsigned short> indices_flowers;
void init_flowers()
{
	objname = "models/PrimroseP.obj";
	obj_scale = 4;

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
	indexVBO(vertices, uvs, normals, indices_flowers, indexed_vertices, indexed_uvs, indexed_normals);

	//gen
	glGenBuffers(1, &vertexbuffer);
	glGenBuffers(1, &uvbuffer);
	glGenBuffers(1, &normalbuffer);
	glGenBuffers(1, &elementbuffer);
	glGenVertexArrays(1, &VAO_flowers);

	glBindVertexArray(VAO_flowers);
	//binding
	glBindBuffer(GL_ARRAY_BUFFER, vertexbuffer);
	glBufferData(GL_ARRAY_BUFFER, indexed_vertices.size() * sizeof(glm::vec3), &indexed_vertices[0], GL_STATIC_DRAW);

	glBindBuffer(GL_ARRAY_BUFFER, uvbuffer);
	glBufferData(GL_ARRAY_BUFFER, indexed_uvs.size() * sizeof(glm::vec2), &indexed_uvs[0], GL_STATIC_DRAW);

	glBindBuffer(GL_ARRAY_BUFFER, normalbuffer);
	glBufferData(GL_ARRAY_BUFFER, indexed_normals.size() * sizeof(glm::vec3), &indexed_normals[0], GL_STATIC_DRAW);

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, elementbuffer);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices_flowers.size() * sizeof(unsigned short), &indices_flowers[0], GL_STATIC_DRAW);
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

GLuint VAO_pine;
std::vector<unsigned short> indices_pine;
void init_pine()
{
	objname = "models/pine.obj";
	obj_scale = 4;

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
	indexVBO(vertices, uvs, normals, indices_pine, indexed_vertices, indexed_uvs, indexed_normals);

	//gen
	glGenBuffers(1, &vertexbuffer);
	glGenBuffers(1, &uvbuffer);
	glGenBuffers(1, &normalbuffer);
	glGenBuffers(1, &elementbuffer);
	glGenVertexArrays(1, &VAO_pine);

	glBindVertexArray(VAO_pine);
	//binding
	glBindBuffer(GL_ARRAY_BUFFER, vertexbuffer);
	glBufferData(GL_ARRAY_BUFFER, indexed_vertices.size() * sizeof(glm::vec3), &indexed_vertices[0], GL_STATIC_DRAW);

	glBindBuffer(GL_ARRAY_BUFFER, uvbuffer);
	glBufferData(GL_ARRAY_BUFFER, indexed_uvs.size() * sizeof(glm::vec2), &indexed_uvs[0], GL_STATIC_DRAW);

	glBindBuffer(GL_ARRAY_BUFFER, normalbuffer);
	glBufferData(GL_ARRAY_BUFFER, indexed_normals.size() * sizeof(glm::vec3), &indexed_normals[0], GL_STATIC_DRAW);

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, elementbuffer);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices_pine.size() * sizeof(unsigned short), &indices_pine[0], GL_STATIC_DRAW);
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

GLuint VAO_stone;
std::vector<unsigned short> indices_stone;
void init_stone()
{
	objname = "models/stone.obj";
	obj_scale = 0.1;

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
	indexVBO(vertices, uvs, normals, indices_stone, indexed_vertices, indexed_uvs, indexed_normals);

	//gen
	glGenBuffers(1, &vertexbuffer);
	glGenBuffers(1, &uvbuffer);
	glGenBuffers(1, &normalbuffer);
	glGenBuffers(1, &elementbuffer);
	glGenVertexArrays(1, &VAO_stone);

	glBindVertexArray(VAO_stone);
	//binding
	glBindBuffer(GL_ARRAY_BUFFER, vertexbuffer);
	glBufferData(GL_ARRAY_BUFFER, indexed_vertices.size() * sizeof(glm::vec3), &indexed_vertices[0], GL_STATIC_DRAW);

	glBindBuffer(GL_ARRAY_BUFFER, uvbuffer);
	glBufferData(GL_ARRAY_BUFFER, indexed_uvs.size() * sizeof(glm::vec2), &indexed_uvs[0], GL_STATIC_DRAW);

	glBindBuffer(GL_ARRAY_BUFFER, normalbuffer);
	glBufferData(GL_ARRAY_BUFFER, indexed_normals.size() * sizeof(glm::vec3), &indexed_normals[0], GL_STATIC_DRAW);

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, elementbuffer);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices_stone.size() * sizeof(unsigned short), &indices_stone[0], GL_STATIC_DRAW);
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

GLuint VAO_tree;
std::vector<unsigned short> indices_tree;
void init_tree()
{
	objname = "models/lowtree1.obj";
	obj_scale = 0.04;

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
	indexVBO(vertices, uvs, normals, indices_tree, indexed_vertices, indexed_uvs, indexed_normals);

	//gen
	glGenBuffers(1, &vertexbuffer);
	glGenBuffers(1, &uvbuffer);
	glGenBuffers(1, &normalbuffer);
	glGenBuffers(1, &elementbuffer);
	glGenVertexArrays(1, &VAO_tree);

	glBindVertexArray(VAO_tree);
	//binding
	glBindBuffer(GL_ARRAY_BUFFER, vertexbuffer);
	glBufferData(GL_ARRAY_BUFFER, indexed_vertices.size() * sizeof(glm::vec3), &indexed_vertices[0], GL_STATIC_DRAW);

	glBindBuffer(GL_ARRAY_BUFFER, uvbuffer);
	glBufferData(GL_ARRAY_BUFFER, indexed_uvs.size() * sizeof(glm::vec2), &indexed_uvs[0], GL_STATIC_DRAW);

	glBindBuffer(GL_ARRAY_BUFFER, normalbuffer);
	glBufferData(GL_ARRAY_BUFFER, indexed_normals.size() * sizeof(glm::vec3), &indexed_normals[0], GL_STATIC_DRAW);

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, elementbuffer);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices_tree.size() * sizeof(unsigned short), &indices_tree[0], GL_STATIC_DRAW);
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

void initBuffers()
{
	init_bench();
	init_ground();
	init_soil();
	init_flowers();
	init_pine();
	init_stone();
	init_tree();
}

void freeBuffers()
{
	//glDeleteBuffers(1, &VBO);
	glDeleteBuffers(1, &VAO_bench);
	glDeleteBuffers(1, &VAO_ground);

	//glDeleteBuffers(1, &IBO);
	//glDeleteBuffers(1, &vertexbuffer);
	//glDeleteBuffers(1, &uvbuffer);
	//glDeleteBuffers(1, &normalbuffer);
	//glDeleteBuffers(1, &elementbuffer);

	//glDisableVertexAttribArray(0);
	//glDisableVertexAttribArray(1);
	//glDisableVertexAttribArray(2);
}

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
	double x = camera_rad * glm::cos(camera_angle / 180 * pi);
	double z = camera_rad * glm::sin(camera_angle / 180 * pi);
    viewPosition[0] = x;
	viewPosition[1] = camera_pos;
	viewPosition[2] = z;
}

void setTransform(GLuint program)
{
	/* in shader:
	uniform struct Transform{
	mat4 model;
	mat4 viewProjection;
	mat3 normal;
	vec3 viewPosition;
	} transform;
	*/
	GLint s_model = glGetUniformLocation(program, "transform.model");
	GLint s_proj = glGetUniformLocation(program, "transform.viewProjection");
	GLint s_normal = glGetUniformLocation(program, "transform.normal");
	GLint s_view = glGetUniformLocation(program, "transform.viewPosition");

	glUniformMatrix4fv(s_model, 1, GL_FALSE, &model[0][0]);
	glUniformMatrix4fv(s_proj, 1, GL_FALSE, &viewProjection[0][0]);
	glUniformMatrix3fv(s_normal, 1, GL_FALSE, &normaltr[0][0]);
	glUniform3fv(s_view, 1, viewPosition);
}

void setPointLight(GLuint program)
{
	/* in shader:
	uniform struct PointLight {
		vec4 position;
		vec4 ambient;
		vec4 diffuse;
		vec4 specular;
		vec3 attenuation;
	} light;*/

	GLint s_position = glGetUniformLocation(program, "light.position");
	GLint s_ambient = glGetUniformLocation(program, "light.ambient");
	GLint s_diffuse = glGetUniformLocation(program, "light.diffuse");
	GLint s_specular = glGetUniformLocation(program, "light.specular");
	GLint s_attenuation = glGetUniformLocation(program, "light.attenuation");

	glUniform4fv(s_position, 1, light);
	glUniform4fv(s_ambient, 1, ambient);
	glUniform4fv(s_diffuse, 1, diffuse);
	glUniform4fv(s_specular, 1, specular);
	glUniform3fv(s_attenuation, 1, attenuation);
}

void setMaterial(GLuint program, float* m_ambient, float* m_diffuse, float* m_specular, float* m_emission, float m_shiness)
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

	GLint s_ambient = glGetUniformLocation(program, "material.ambient");
	GLint s_diffuse = glGetUniformLocation(program, "material.diffuse");
	GLint s_specular = glGetUniformLocation(program, "material.specular");
	GLint s_emission = glGetUniformLocation(program, "material.emission");
	GLint s_shiness = glGetUniformLocation(program, "material.shiness");

	glUniform4fv(s_ambient, 1, m_ambient);
	glUniform4fv(s_diffuse, 1, m_diffuse);
	glUniform4fv(s_specular, 1, m_specular);
	glUniform4fv(s_emission, 1, m_emission);
	glUniform1f(s_shiness, m_shiness);
}

void draw_ground()
{
	//glUseProgram(Program_floor);

	//glColor3f(1, 1, 1);

	////glBindTexture(GL_TEXTURE_2D, texture_floor);
	//glEnable(GL_TEXTURE_2D);
	//glBegin(GL_QUADS);
	//glNormal3f(0, 1, 0); glTexCoord2f(0.0, 0.0); glVertex3f(-40.0, -0, -40.0);
	//glNormal3f(0, 1, 0); glTexCoord2f(0.0, 10.0); glVertex3f(-40.0, 0, 40.0);
	//glNormal3f(0, 1, 0); glTexCoord2f(10.0, 10.0); glVertex3f(40.0, 0.0, 40);
	//glNormal3f(0, 1, 0); glTexCoord2f(10.0, 0.0); glVertex3f(40.0, 0.0, -40.0);
	//glEnd();
	//glDisable(GL_TEXTURE_2D);

	model = glm::mat4(1.0f);

	//rotate scene here
	model = glm::rotate(model, glm::radians(rotateX), glm::vec3(1, 0, 0));
	model = glm::rotate(model, glm::radians(rotateY), glm::vec3(0, 1, 0));
	model = glm::rotate(model, glm::radians(rotateZ), glm::vec3(0, 0, 1));

	normaltr = glm::transpose(glm::inverse(model));

	float red[4] = { 0.7f, 0.7f, 0.7f, 1.0f };

	shader_program = Program_with_t;
	glUseProgram(shader_program);
	glEnable(GL_TEXTURE_2D);
	glBindTexture(GL_TEXTURE_2D, texture_ground);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);

	setTransform(shader_program);
	setPointLight(shader_program);
	float m_ambient[]{ 0.2f,0.2f,0.2f,1.0f };
	float m_diffuse[]{ 1.0f,1.0f,1.0f,1.0f };
	float m_specular[]{ 0.2f,0.2f,0.2f,1.0f };
	float m_emission[]{ 0.0f,0.0f,0.0f,1.0f };
	float m_shiness = 0;
	setMaterial(shader_program, m_ambient, m_diffuse, m_specular, m_emission, m_shiness);

	glBindVertexArray(VAO_ground);
	glDrawElements(GL_QUADS, indices_ground.size(), GL_UNSIGNED_SHORT, 0);
	glBindVertexArray(0);

	glUseProgram(0);
	glDisable(GL_TEXTURE_2D);

	/*shader_program = Program_no_t;
	glUseProgram(shader_program);
	glUniform4fv(Unif2, 1, red);

	glUseProgram(shader_program);
	setTransform(shader_program);
	setPointLight(shader_program);
	float m_ambient[]{ 0.2f,0.2f,0.2f,1.0f };
	float m_diffuse[]{ 1.0f,1.0f,1.0f,1.0f };
	float m_specular[]{ 0.2f,0.2f,0.2f,1.0f };
	float m_emission[]{ 0.0f,0.0f,0.0f,1.0f };
	float m_shiness = 0;
	setMaterial(shader_program, m_ambient, m_diffuse, m_specular, m_emission, m_shiness);

	glBindVertexArray(VAO_ground);
	glDrawElements(GL_QUADS, indices_ground.size(), GL_UNSIGNED_SHORT, 0);
	glBindVertexArray(0);

	glUseProgram(0);*/

}

void draw_soil()
{
	model = glm::mat4(1.0f);

	//rotate scene here
	model = glm::rotate(model, glm::radians(rotateX), glm::vec3(1, 0, 0));
	model = glm::rotate(model, glm::radians(rotateY), glm::vec3(0, 1, 0));
	model = glm::rotate(model, glm::radians(rotateZ), glm::vec3(0, 0, 1));

	model = glm::translate(model, glm::vec3(0, 0.1, 0));

	normaltr = glm::transpose(glm::inverse(model));

	shader_program = Program_with_t;
	glUseProgram(shader_program);
	glEnable(GL_TEXTURE_2D);
	glBindTexture(GL_TEXTURE_2D, texture_soil);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);

	setTransform(shader_program);
	setPointLight(shader_program);
	float m_ambient[]{ 0.2f,0.2f,0.2f,1.0f };
	float m_diffuse[]{ 1.0f,1.0f,1.0f,1.0f };
	float m_specular[]{ 0.2f,0.2f,0.2f,1.0f };
	float m_emission[]{ 0.0f,0.0f,0.0f,1.0f };
	float m_shiness = 0;
	setMaterial(shader_program, m_ambient, m_diffuse, m_specular, m_emission, m_shiness);

	glBindVertexArray(VAO_soil);
	glDrawElements(GL_QUADS, indices_soil.size(), GL_UNSIGNED_SHORT, 0);
	glBindVertexArray(0);

	glUseProgram(0);
	glDisable(GL_TEXTURE_2D);
}

void draw_bench()
{
	model = glm::mat4(1.0f);

	//rotate scene here
	model = glm::rotate(model, glm::radians(rotateX), glm::vec3(1, 0, 0));
	model = glm::rotate(model, glm::radians(rotateY), glm::vec3(0, 1, 0));
	model = glm::rotate(model, glm::radians(rotateZ), glm::vec3(0, 0, 1));

	//translate&rotate model
	model = glm::translate(model, glm::vec3(0, 0, -22));
	//model = glm::rotate(model, glm::radians(rotateX), glm::vec3(1, 0, 0));
	model = glm::rotate(model, glm::radians(90.0f), glm::vec3(0, 1, 0));
	//model = glm::rotate(model, glm::radians(rotateZ), glm::vec3(0, 0, 1));

	normaltr = glm::transpose(glm::inverse(model));

	shader_program = Program_with_t;
	glUseProgram(shader_program);
	glEnable(GL_TEXTURE_2D);
	glBindTexture(GL_TEXTURE_2D, texture_bench);

	setTransform(shader_program);
	setPointLight(shader_program);
	float m_ambient[]{ 0.2f,0.2f,0.2f,1.0f };
	float m_diffuse[]{ 1.0f,1.0f,1.0f,1.0f };
	float m_specular[]{ 0.2f,0.2f,0.2f,1.0f };
	float m_emission[]{ 0.0f,0.0f,0.0f,1.0f };
	float m_shiness = 0;
	setMaterial(shader_program, m_ambient, m_diffuse, m_specular, m_emission, m_shiness);

	glBindVertexArray(VAO_bench);
	glDrawElements(GL_QUADS, indices_bench.size(), GL_UNSIGNED_SHORT, 0);
	glBindVertexArray(0);

	glUseProgram(0);
	glDisable(GL_TEXTURE_2D);
}

void draw_flowers()
{
	model = glm::mat4(1.0f);

	//rotate scene here
	model = glm::rotate(model, glm::radians(rotateX), glm::vec3(1, 0, 0));
	model = glm::rotate(model, glm::radians(rotateY), glm::vec3(0, 1, 0));
	model = glm::rotate(model, glm::radians(rotateZ), glm::vec3(0, 0, 1));

	//translate&rotate model
	model = glm::translate(model, glm::vec3(10, -0.5, -15));
	//model = glm::rotate(model, glm::radians(rotateX), glm::vec3(1, 0, 0));
	model = glm::rotate(model, glm::radians(180.0f), glm::vec3(0, 1, 0));
	//model = glm::rotate(model, glm::radians(rotateZ), glm::vec3(0, 0, 1));

	normaltr = glm::transpose(glm::inverse(model));

	shader_program = Program_with_t;
	glUseProgram(shader_program);
	glEnable(GL_TEXTURE_2D);
	glBindTexture(GL_TEXTURE_2D, texture_flowers);

	setTransform(shader_program);
	setPointLight(shader_program);
	float m_ambient[]{ 0.2f,0.2f,0.2f,1.0f };
	float m_diffuse[]{ 1.0f,1.0f,1.0f,1.0f };
	float m_specular[]{ 0.2f,0.2f,0.2f,1.0f };
	float m_emission[]{ 0.0f,0.0f,0.0f,1.0f };
	float m_shiness = 0;
	setMaterial(shader_program, m_ambient, m_diffuse, m_specular, m_emission, m_shiness);

	glBindVertexArray(VAO_flowers);
	glDrawElements(GL_QUADS, indices_flowers.size(), GL_UNSIGNED_SHORT, 0);
	glBindVertexArray(0);

	glUseProgram(0);
	glDisable(GL_TEXTURE_2D);
}

void draw_pine()
{
	model = glm::mat4(1.0f);

	//rotate scene here
	model = glm::rotate(model, glm::radians(rotateX), glm::vec3(1, 0, 0));
	model = glm::rotate(model, glm::radians(rotateY), glm::vec3(0, 1, 0));
	model = glm::rotate(model, glm::radians(rotateZ), glm::vec3(0, 0, 1));

	//translate&rotate model
	model = glm::translate(model, glm::vec3(10, 0, 10));
	model = glm::rotate(model, glm::radians(180.0f), glm::vec3(0, 1, 0));

	normaltr = glm::transpose(glm::inverse(model));

	shader_program = Program_with_t;
	glUseProgram(shader_program);
	glEnable(GL_TEXTURE_2D);
	glBindTexture(GL_TEXTURE_2D, texture_pine);

	setTransform(shader_program);
	setPointLight(shader_program);
	float m_ambient[]{ 0.2f,0.2f,0.2f,1.0f };
	float m_diffuse[]{ 1.0f,1.0f,1.0f,1.0f };
	float m_specular[]{ 0.2f,0.2f,0.2f,1.0f };
	float m_emission[]{ 0.0f,0.0f,0.0f,1.0f };
	float m_shiness = 0;
	setMaterial(shader_program, m_ambient, m_diffuse, m_specular, m_emission, m_shiness);

	glBindVertexArray(VAO_pine);
	glDrawElements(GL_QUADS, indices_pine.size(), GL_UNSIGNED_SHORT, 0);
	glBindVertexArray(0);

	// second pine
	model = glm::translate(model, glm::vec3(20, 0, 20));
	model = glm::rotate(model, glm::radians(90.0f), glm::vec3(0, 1, 0));

	normaltr = glm::transpose(glm::inverse(model));

	glBindTexture(GL_TEXTURE_2D, texture_pine);

	setTransform(shader_program);
	setPointLight(shader_program);
	setMaterial(shader_program, m_ambient, m_diffuse, m_specular, m_emission, m_shiness);

	glBindVertexArray(VAO_pine);
	glDrawElements(GL_QUADS, indices_pine.size(), GL_UNSIGNED_SHORT, 0);
	glBindVertexArray(0);

	// third pine
	model = glm::translate(model, glm::vec3(5, 0, -20));
	model = glm::rotate(model, glm::radians(90.0f), glm::vec3(0, 1, 0));

	normaltr = glm::transpose(glm::inverse(model));

	glBindTexture(GL_TEXTURE_2D, texture_pine);

	setTransform(shader_program);
	setPointLight(shader_program);
	setMaterial(shader_program, m_ambient, m_diffuse, m_specular, m_emission, m_shiness);

	glBindVertexArray(VAO_pine);
	glDrawElements(GL_QUADS, indices_pine.size(), GL_UNSIGNED_SHORT, 0);
	glBindVertexArray(0);

	glUseProgram(0);
	glDisable(GL_TEXTURE_2D);
}

void draw_stone()
{
	model = glm::mat4(1.0f);

	//rotate scene here
	model = glm::rotate(model, glm::radians(rotateX), glm::vec3(1, 0, 0));
	model = glm::rotate(model, glm::radians(rotateY), glm::vec3(0, 1, 0));
	model = glm::rotate(model, glm::radians(rotateZ), glm::vec3(0, 0, 1));

	//translate&rotate model
	model = glm::translate(model, glm::vec3(15, 0, -15));
	//model = glm::rotate(model, glm::radians(rotateX), glm::vec3(1, 0, 0));
	//model = glm::rotate(model, glm::radians(180.0f), glm::vec3(0, 1, 0));
	//model = glm::rotate(model, glm::radians(rotateZ), glm::vec3(0, 0, 1));

	normaltr = glm::transpose(glm::inverse(model));

	shader_program = Program_with_t;
	glUseProgram(shader_program);
	glEnable(GL_TEXTURE_2D);
	glBindTexture(GL_TEXTURE_2D, texture_stone);

	setTransform(shader_program);
	setPointLight(shader_program);
	float m_ambient[]{ 0.2f,0.2f,0.2f,1.0f };
	float m_diffuse[]{ 1.0f,1.0f,1.0f,1.0f };
	float m_specular[]{ 0.2f,0.2f,0.2f,1.0f };
	float m_emission[]{ 0.0f,0.0f,0.0f,1.0f };
	float m_shiness = 0;
	setMaterial(shader_program, m_ambient, m_diffuse, m_specular, m_emission, m_shiness);

	glBindVertexArray(VAO_stone);
	glDrawElements(GL_QUADS, indices_stone.size(), GL_UNSIGNED_SHORT, 0);
	glBindVertexArray(0);

	glUseProgram(0);
	glDisable(GL_TEXTURE_2D);
}

void draw_tree()
{
	model = glm::mat4(1.0f);

	//rotate scene here
	model = glm::rotate(model, glm::radians(rotateX), glm::vec3(1, 0, 0));
	model = glm::rotate(model, glm::radians(rotateY), glm::vec3(0, 1, 0));
	model = glm::rotate(model, glm::radians(rotateZ), glm::vec3(0, 0, 1));

	//translate&rotate model
	model = glm::translate(model, glm::vec3(-10, 0, 7));
	model = glm::rotate(model, glm::radians(170.0f), glm::vec3(0, 1, 0));

	normaltr = glm::transpose(glm::inverse(model));

	shader_program = Program_with_t;
	glUseProgram(shader_program);
	glEnable(GL_TEXTURE_2D);
	glBindTexture(GL_TEXTURE_2D, texture_tree);

	setTransform(shader_program);
	setPointLight(shader_program);
	float m_ambient[]{ 0.2f,0.2f,0.2f,1.0f };
	float m_diffuse[]{ 1.0f,1.0f,1.0f,1.0f };
	float m_specular[]{ 0.2f,0.2f,0.2f,1.0f };
	float m_emission[]{ 0.0f,0.0f,0.0f,1.0f };
	float m_shiness = 0;
	setMaterial(shader_program, m_ambient, m_diffuse, m_specular, m_emission, m_shiness);

	glBindVertexArray(VAO_tree);
	glDrawElements(GL_QUADS, indices_tree.size(), GL_UNSIGNED_SHORT, 0);
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

	draw_ground();
	draw_soil();
	draw_bench();
	draw_flowers();
	draw_pine();
	draw_stone();
	draw_tree();
	glutSwapBuffers();
}

//Организовать управление источником света (перемещение, вращение источника вокруг центра сцены) с помощью шейдеров
void special(int key, int x, int y)
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
	recountCameraPos();
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
	case 'a':
	case 'A':
		camera_angle += 3; break;
	case 'd':
	case 'D':
		camera_angle -= 3; break;
	case 'w':
	case 'W':
		camera_pos += 0.5; break;
	case 's':
	case 'S':
		camera_pos -= 0.5; break;
	case 'q':
	case 'Q':
		camera_rad += 0.5; break;
	case 'e':
	case 'E':
		camera_rad -= 0.5; break;
	default:
		break;
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
	glutCreateWindow("Individual 3");

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
	recountCameraPos();
	recountLightPos();

	glutDisplayFunc(display);
	glutReshapeFunc(reshape);
	glutSpecialFunc(special);
	glutKeyboardFunc(keyboard);

	glutMainLoop();

	freeShader();
	freeBuffers();
}