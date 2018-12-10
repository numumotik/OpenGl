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
//#include<SOIL.h>
#include <vector>
#include <glm/glm.hpp>
#include <string>
#include <sstream>
#include <fstream>

unsigned char* image;
GLuint texture;
std::vector<glm::vec3> vertices;
std::vector<glm::vec2> uvs;
std::vector<glm::vec3> normals;
std::vector<unsigned int> vertex_indices;
GLuint vertexbuffer;
GLint indices_count;

// ID атрибута вершин
GLint Attrib_vertex;
//// ID атрибута цветов
//GLint Attrib_color;
//// ID юниформ матрицы проекции
//GLint Unif_matrix;
// ID Vertex Buffer Object
GLuint VBO_vertex;
//// ID Vertex Buffer Object
//GLuint VBO_color;
// ID VBO for element indices
GLuint VBO_element;

const double pi = 3.14159265358979323846;
int light_num = 0;
std::vector<float> light_angle { 0, 90, 0 };
std::vector<float> light_pos{ 5, 5, 5 };
std::vector<float> light_rad{ 10, 10, 10 };
float model_angle = 0;
int is_ahead = 0;
int is_back = 0;
int width = 0, height = 0;
int model_num = 0;

/*void makeTextureImage()
{
	texture = SOIL_load_OGL_texture
	(
		"texture2.jpg",
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
		"car_texture.jpg",
		SOIL_LOAD_AUTO,
		SOIL_CREATE_NEW_ID,
		SOIL_FLAG_MIPMAPS | SOIL_FLAG_INVERT_Y | SOIL_FLAG_NTSC_SAFE_RGB | SOIL_FLAG_COMPRESS_TO_DXT
	);
}*/

void loadOBJ(const std::string & path, std::vector<glm::vec3> & out_vertices, std::vector<glm::vec2> & out_uvs, std::vector<glm::vec3> & out_normals, std::vector<unsigned int> & vertex_indices)
{
    std::vector<unsigned int> /*vertex_indices, */uv_indices, normal_indices;
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
            vertex.x *= 8;
            vertex.y *= 8;
            vertex.z *= 8;
            //temp_vertices.push_back(vertex);
            out_vertices.push_back(vertex);
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
            temp_normals.push_back(normal);
        }
        else if (lineHeader == "f")
        {
            unsigned int vertex_index[3], uv_index[3], normal_index[3];
            char slash;
            ss >> vertex_index[0] >> slash >> uv_index[0] >> slash >> normal_index[0] >> vertex_index[1] >> slash >> uv_index[1] >> slash >> normal_index[1] >> vertex_index[2] >> slash >> uv_index[2] >> slash >> normal_index[2];

            vertex_indices.push_back(vertex_index[0] - 1);
            vertex_indices.push_back(vertex_index[1] - 1);
            vertex_indices.push_back(vertex_index[2] - 1);
            uv_indices.push_back(uv_index[0]);
            uv_indices.push_back(uv_index[1]);
            uv_indices.push_back(uv_index[2]);
            normal_indices.push_back(normal_index[0]);
            normal_indices.push_back(normal_index[1]);
            normal_indices.push_back(normal_index[2]);
        }
    }

    //// For each vertex of each triangle
    //for (unsigned int i = 0; i < vertex_indices.size(); i++)
    //{
    //    unsigned int vertexIndex = vertex_indices[i];
    //    glm::vec3 vertex = temp_vertices[vertexIndex - 1];
    //    out_vertices.push_back(vertex);

    //    unsigned int uvIndex = uv_indices[i];
    //    glm::vec2 uv = temp_uvs[uvIndex - 1];
    //    out_uvs.push_back(uv);

    //    unsigned int normalIndex = normal_indices[i];
    //    glm::vec3 normal = temp_normals[normalIndex - 1];
    //    out_normals.push_back(normal);
    //}
}

// Èíèöèàëèçàöèÿ
void init(void)
{
	glClearColor(0.0, 0.0, 0.0, 0.0);
	glEnable(GL_NORMALIZE);
	glEnable(GL_COLOR_MATERIAL);

	// Street lights
	const GLfloat light_ambient[] = { 0.0, 0.0, 0.0, 1 };
	const GLfloat light_diffuse[] = { 1.0, 1.0, 1.0, 1.0 };
	const GLfloat light_specular[] = { 1.0, 1.0, 1.0, 1.0 };
	glLightfv(GL_LIGHT1, GL_AMBIENT, light_ambient);
	glLightfv(GL_LIGHT1, GL_DIFFUSE, light_diffuse);
	glLightfv(GL_LIGHT1, GL_SPECULAR, light_specular);

	glLightfv(GL_LIGHT2, GL_AMBIENT, light_ambient);
	glLightfv(GL_LIGHT2, GL_DIFFUSE, light_diffuse);
	glLightfv(GL_LIGHT2, GL_SPECULAR, light_specular);

	glLightfv(GL_LIGHT3, GL_AMBIENT, light_ambient);
	glLightfv(GL_LIGHT3, GL_DIFFUSE, light_diffuse);
	glLightfv(GL_LIGHT3, GL_SPECULAR, light_specular);

	glEnable(GL_DEPTH_TEST);
	glEnable(GL_LIGHTING);
	glEnable(GL_LIGHT1);
	//glEnable(GL_LIGHT2);
	//glEnable(GL_LIGHT3);
	//makeTextureImage();

    // Read our.obj file
    loadOBJ("african_head.obj", vertices, uvs, normals, vertex_indices);
}

double gr_cos(float angle) noexcept
{
	return cos(angle / 180 * pi);
}

double gr_sin(float angle) noexcept
{
	return sin(angle / 180 * pi);
}

void setLight()
{
	double x = light_rad[0] *gr_cos(light_angle[0]);
	double z = light_rad[0] * gr_sin(light_angle[0]);
	//GLfloat position[] = { x, light_pos[0], z, 1 };
	GLfloat position[] = { 0, 0, 0, 1 };
	if (glIsEnabled(GL_LIGHT1))
	{
		glPushMatrix();
		glTranslatef(x, light_pos[0], z);
		glLightfv(GL_LIGHT1, GL_POSITION, position);
		glutWireCube(0.5f);
		glPopMatrix();
	}

	if (glIsEnabled(GL_LIGHT2))
	{
		glPushMatrix();
		x = light_rad[1] * gr_cos(light_angle[1]);
		z = light_rad[1] * gr_sin(light_angle[1]);
		//GLfloat position2[] = { x, light_pos[1], z, 1 };
		glTranslatef(x, light_pos[1], z);
		glLightfv(GL_LIGHT2, GL_POSITION, position);
		glutWireCube(0.5f);
		glPopMatrix();
	}

	if (glIsEnabled(GL_LIGHT3))
	{
		glPushMatrix();
		x = light_rad[2] * gr_cos(light_angle[2]);
		z = light_rad[2] * gr_sin(light_angle[2]);
		//GLfloat position3[] = { x, light_pos[2], z, 1 };
		glLightfv(GL_LIGHT3, GL_POSITION, position);
		glTranslatef(x, light_pos[2], z);
		glutWireCube(0.5f);
		glPopMatrix();
	}
}

// Èçìåíåíèå ðàçìåðîâ îêíà
void reshape(int w, int h)
{
	width = w; height = h;
	glViewport(0, 0, (GLsizei)w, (GLsizei)h);
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	gluPerspective(60.0, static_cast<GLfloat>(w) / static_cast<GLfloat>(h), 1.0, 100.0);
	gluLookAt(0, 10, 30, 0.0, 0.0, 0.0, 0.0, 1.0, 0.0);
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
}

float x = 0; float z = 0; float y = 0;
float SPEED = 0.1;

void draw_simple_model()
{
	glutSolidCube(5);
}

void draw_head()
{
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, VBO_element);
    // 1st attribute buffer : vertices
    //glEnableVertexAttribArray(0);
    glEnableVertexAttribArray(Attrib_vertex);
    //glBindBuffer(GL_ARRAY_BUFFER, vertexbuffer);
    glBindBuffer(GL_ARRAY_BUFFER, VBO_vertex);
    //glVertexAttribPointer(
    //    0,                  // attribute 0. No particular reason for 0, but must match the layout in the shader.
    //    3,                  // size
    //    GL_FLOAT,           // type
    //    GL_FALSE,           // normalized?
    //    0,                  // stride
    //    (void*)0            // array buffer offset
    //);
    glVertexAttribPointer(Attrib_vertex, 3, GL_FLOAT, GL_FALSE, 0, 0);

    // Draw the triangles!
    //glDrawArrays(GL_TRIANGLES, 0, vertices.size());
    glDrawElements(GL_TRIANGLES, vertex_indices.size(), GL_UNSIGNED_INT, 0);
    //glDisableVertexAttribArray(0);
    glDisableVertexAttribArray(Attrib_vertex);
}

void draw_model()
{
	glPushMatrix();
	glTranslatef(x, y, z);
	glRotatef(model_angle, 0, 1, 0);

	glColor3f(0.8, 0.8, 0.8);

	//model here
	if (model_num)
		draw_head();
	else
		draw_simple_model();

	glPopMatrix();
}

// Îòîáðàæåíèå
void display(void)
{
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	//glMatrixMode(GL_MODELVIEW);
	if (is_ahead)
	{
		x += SPEED * gr_sin(model_angle);
		z += SPEED * gr_cos(model_angle);
		is_ahead = 0;
	}
	if (is_back)
	{
		x -= SPEED * gr_sin(model_angle);
		z -= SPEED * gr_cos(model_angle);
		is_back = 0;
	}

	setLight();
	draw_model();
	glutSwapBuffers();
}

// Ðåàêöèÿ íà êëàâèàòóðó
void specialKeys(int key, int x, int y)
{
	switch (key)
	{
	case GLUT_KEY_UP: light_pos[light_num] += 0.5; break;
	case GLUT_KEY_DOWN: light_pos[light_num] -= 0.5; break;
	case GLUT_KEY_RIGHT: light_angle[light_num] -= 3; break;
	case GLUT_KEY_LEFT: light_angle[light_num] += 3; break;
	case GLUT_KEY_PAGE_UP: light_rad[light_num] -= 0.5; break;
	case GLUT_KEY_PAGE_DOWN: light_rad[light_num] += 0.5; break;
	case GLUT_KEY_F1:
		light_num = 0;
		break;
	case GLUT_KEY_F2:
		light_num = 1;
		break;
	case GLUT_KEY_F3:
		light_num = 2;
		break;
	}
	glutPostRedisplay();
}

void keyboard(unsigned char key, int x, int y)
{
	switch (key)
	{
	case '1':
		if (glIsEnabled(GL_LIGHT1))
			glDisable(GL_LIGHT1);
		else glEnable(GL_LIGHT1);
		break;
	case '2':
		if (glIsEnabled(GL_LIGHT2))
			glDisable(GL_LIGHT2);
		else glEnable(GL_LIGHT2);
		break;
	case '3':
		if (glIsEnabled(GL_LIGHT3))
			glDisable(GL_LIGHT3);
		else glEnable(GL_LIGHT3);
		break;
	case 'a':
	case 'A':
		model_angle += 5;
		break;
	case 'd':
	case 'D':
		model_angle -= 5;
		break;
	case 'w':
	case 'W':
		is_ahead = 1;
		break;
	case 's':
	case 'S':
		is_back = 1;
		break;
	}
	glutPostRedisplay();
}

void mouseFunc(int button, int state, int x, int y)
{
	if (state == GLUT_DOWN)
    {
        model_num = (model_num + 1) % 2;
        if (model_num != 0)
        {
            // vertices
            //glGenBuffers(1, &vertexbuffer);
            //glBindBuffer(GL_ARRAY_BUFFER, vertexbuffer);
            //glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(glm::vec3), &vertices[0], GL_STATIC_DRAW);
            glGenBuffers(1, &VBO_vertex);
            glBindBuffer(GL_ARRAY_BUFFER, VBO_vertex);
            glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(glm::vec3), &vertices[0], GL_STATIC_DRAW);


            // indices
            glGenBuffers(1, &VBO_element);
            glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, VBO_element);
            glBufferData(GL_ELEMENT_ARRAY_BUFFER, vertex_indices.size() * sizeof(unsigned int), &vertex_indices[0], GL_STATIC_DRAW);
        }
        else
        {
            //glDeleteBuffers(1, &vertexbuffer);
            glDeleteBuffers(1, &VBO_vertex);
            glDeleteBuffers(1, &VBO_element);
        }
    }
}

int main(int argc, char **argv)
{
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB);
	glutInitWindowSize(800, 800);
	glutInitWindowPosition(10, 10);
	glutCreateWindow("Lab 12");
    glewInit();
	init();
	glutDisplayFunc(display);
	glutReshapeFunc(reshape);
	glutSpecialFunc(specialKeys);
	glutKeyboardFunc(keyboard);
	glutMouseFunc(mouseFunc);
	glutMainLoop();
	return 0;
}