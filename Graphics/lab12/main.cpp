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
#include <SOIL.h>
#include <vector>
#include <glm/glm.hpp>
#include <string>
#include <sstream>
#include <fstream>
#include <map>
#include <utility>

unsigned char* image;
GLuint texture;
std::vector<glm::vec3> indexed_vertices;
std::vector<glm::vec2> indexed_uvs;
std::vector<glm::vec3> indexed_normals;
std::vector<unsigned short> indices;
GLuint vertexbuffer;
GLuint uvbuffer;
GLuint normalbuffer;
GLuint elementbuffer;

enum Object
{
    Head, Cat, Penguin, Diablo
};
Object object = Diablo;

const double pi = 3.14159265358979323846;
int light_num = 0;
std::vector<float> light_angle { 0, 90, 0 };
std::vector<float> light_pos{ 5, 5, 5 };
std::vector<float> light_rad{ 10, 10, 10 };
float model_angle = 0;
int is_ahead = 0;
int is_back = 0;
int width = 0, height = 0;
//int model_num = 0;

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
            double scale = 8;
            if (object == Cat)
            {
                scale = 0.5;
            }
            vertex.x *= scale;
            vertex.y *= scale;
            vertex.z *= scale;
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

void init(void)
{
	//glClearColor(1.0, 1.0, 1.0, 1.0);
	glClearColor(0.3, 0.3, 0.3, 1.0);
	glEnable(GL_NORMALIZE);
	glEnable(GL_COLOR_MATERIAL);
    glEnable(GL_CULL_FACE);

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

    std::string tex_file, obj_file;
    switch (object)
    {
        case Head:
            tex_file = "african_head_SSS.jpg";
            obj_file = "african_head.obj";
            break;
        case Cat:
            tex_file = "Pallas_Cat_dif.jpg";
            obj_file = "13578_Pallas_Cat_v1_L3.obj";
            break;
        case Penguin:
            tex_file = "Penguin Diffuse Color.png";
            obj_file = "PenguinBaseMesh.obj";
            break;
        case Diablo:
            tex_file = "f-texture.png";
            obj_file = "diablo3_pose.obj";
            break;
        default:
            break;
    }
    texture = SOIL_load_OGL_texture
    (
        tex_file.c_str(),
        SOIL_LOAD_AUTO,
        SOIL_CREATE_NEW_ID,
        SOIL_FLAG_MIPMAPS | SOIL_FLAG_INVERT_Y | SOIL_FLAG_NTSC_SAFE_RGB | SOIL_FLAG_COMPRESS_TO_DXT
    );

    // Read our .obj file
    std::vector<glm::vec3> vertices;
    std::vector<glm::vec2> uvs;
    std::vector<glm::vec3> normals;
    loadOBJ(obj_file, vertices, uvs, normals);
    indexVBO(vertices, uvs, normals, indices, indexed_vertices, indexed_uvs, indexed_normals);
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

void draw_head()
{
    glGenBuffers(1, &vertexbuffer);
    glBindBuffer(GL_ARRAY_BUFFER, vertexbuffer);
    glBufferData(GL_ARRAY_BUFFER, indexed_vertices.size() * sizeof(glm::vec3), &indexed_vertices[0], GL_STATIC_DRAW);

    glGenBuffers(1, &uvbuffer);
    glBindBuffer(GL_ARRAY_BUFFER, uvbuffer);
    glBufferData(GL_ARRAY_BUFFER, indexed_uvs.size() * sizeof(glm::vec2), &indexed_uvs[0], GL_STATIC_DRAW);

    glGenBuffers(1, &normalbuffer);
    glBindBuffer(GL_ARRAY_BUFFER, normalbuffer);
    glBufferData(GL_ARRAY_BUFFER, indexed_normals.size() * sizeof(glm::vec3), &indexed_normals[0], GL_STATIC_DRAW);

    glGenBuffers(1, &elementbuffer);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, elementbuffer);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices.size() * sizeof(unsigned short), &indices[0], GL_STATIC_DRAW);

    //glActiveTexture(GL_TEXTURE0);
    glEnable(GL_TEXTURE_2D);
    glBindTexture(GL_TEXTURE_2D, texture);

    // 1rst attribute buffer : vertices
    glEnableVertexAttribArray(0);
    glBindBuffer(GL_ARRAY_BUFFER, vertexbuffer);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, 0);

    // 2nd attribute buffer : UVs
    glEnableClientState(GL_TEXTURE_COORD_ARRAY);
    glEnableVertexAttribArray(1);
    glBindBuffer(GL_ARRAY_BUFFER, uvbuffer);
    glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 0, 0);
    glTexCoordPointer(2, GL_FLOAT, 0, 0);

    // 3rd attribute buffer : normals
    glEnableClientState(GL_NORMAL_ARRAY);
    glEnableVertexAttribArray(2);
    glBindBuffer(GL_ARRAY_BUFFER, normalbuffer);
    glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, 0, 0);
    glNormalPointer(GL_FLOAT, 0, NULL); // Normal start position address

    // Index buffer
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, elementbuffer);

    // Draw the triangles!

    if (object != Penguin && object != Cat)
        glDrawElements(GL_TRIANGLES, indices.size(), GL_UNSIGNED_SHORT, 0);
    else
        glDrawElements(GL_QUADS, indices.size(), GL_UNSIGNED_SHORT, 0);
	glDisable(GL_TEXTURE_2D);
	glDeleteBuffers(1, &vertexbuffer);
	glDeleteBuffers(1, &uvbuffer);
	glDeleteBuffers(1, &normalbuffer);
	glDeleteBuffers(1, &elementbuffer);
	//glDeleteTextures(1, &texture);

	glDisableVertexAttribArray(0);
	glDisableVertexAttribArray(1);
	glDisableVertexAttribArray(2);
}

void draw_model()
{
	glPushMatrix();
	glTranslatef(x, y, z);
	glRotatef(model_angle, 0, 1, 0);
    if (object == Cat)
        glRotatef(-90, 1, 0, 0);

	glColor3f(0.8, 0.8, 0.8);

	draw_head();

	glPopMatrix();
}

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

void disable_all()
{
	glDeleteBuffers(1, &vertexbuffer);
	glDeleteBuffers(1, &uvbuffer);
	glDeleteBuffers(1, &normalbuffer);
	glDeleteBuffers(1, &elementbuffer);
	//glDeleteTextures(1, &texture);

	glDisableVertexAttribArray(0);
	glDisableVertexAttribArray(1);
	glDisableVertexAttribArray(2);
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
	glutMainLoop();
	//disable_all();
	return 0;
}