#include<Windows.h>    
// first include Windows.h header file which is required    
#include<stdio.h>
#include<gl/GL.h>   // GL.h header file    
#include<gl/GLU.h> // GLU.h header file    
#include<gl/glut.h>  // glut.h header file from freeglut\include\GL folder    
#include<conio.h>
#include<math.h>
#include<string.h>
//#include<SOIL.h>
#include <vector>
unsigned char* image;
GLuint texture;

const double pi = 3.14159265358979323846;
int light_num = 0;
std::vector<float> light_angle { 0, 90, 0 };
std::vector<float> light_pos{ 5, 5, 5 };
std::vector<float> light_rad{ 10, 10, 10 };
float model_angle = 0;
int is_ahead = 0;
int is_back = 0;
int width = 0, height = 0;

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

// Инициализация
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

// Изменение размеров окна
void reshape(int w, int h)
{
	width = w; height = h;
	glViewport(0, 0, (GLsizei)w, (GLsizei)h);
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	gluPerspective(60.0, static_cast<GLfloat>(w) / static_cast<GLfloat>(h), 1.0, 100.0);
	gluLookAt(0, 30, 30, 0.0, 0.0, 0.0, 0.0, 1.0, 0.0);
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
}

float x = 0; float z = 0; float y = 0;
float SPEED = 0.1;


void tryingTexturedCube(GLfloat size)
{
	glBindTexture(GL_TEXTURE_2D, texture);
	glEnable(GL_TEXTURE_2D);
	glBegin(GL_QUADS);

	// Front Face
	glNormal3f(0, 0, 1); glTexCoord2f(0.0f, 0.0f); glVertex3f(-size, -size, size);  // Bottom Left Of The Texture and Quad
	glNormal3f(0, 0, 1); glTexCoord2f(1.0f, 0.0f); glVertex3f(size, -size, size);  // Bottom Right Of The Texture and Quad
	glNormal3f(0, 0, 1); glTexCoord2f(1.0f, 1.0f); glVertex3f(size, size, size);  // Top Right Of The Texture and Quad
	glNormal3f(0, 0, 1); glTexCoord2f(0.0f, 1.0f); glVertex3f(-size, size, size);  // Top Left Of The Texture and Quad

																				   // Back Face
	glNormal3f(0, 0, -1); glTexCoord2f(1.0f, 0.0f); glVertex3f(-size, -size, -size);  // Bottom Right Of The Texture and Quad
	glNormal3f(0, 0, -1); glTexCoord2f(1.0f, 1.0f); glVertex3f(-size, size, -size);  // Top Right Of The Texture and Quad
	glNormal3f(0, 0, -1); glTexCoord2f(0.0f, 1.0f); glVertex3f(size, size, -size);  // Top Left Of The Texture and Quad
	glNormal3f(0, 0, -1); glTexCoord2f(0.0f, 0.0f); glVertex3f(size, -size, -size);  // Bottom Left Of The Texture and Quad

																					 // Top Face
	glNormal3f(0, 1, 0); glTexCoord2f(0.0f, 1.0f); glVertex3f(-size, size, -size);  // Top Left Of The Texture and Quad
	glNormal3f(0, 1, 0); glTexCoord2f(0.0f, 0.0f); glVertex3f(-size, size, size);  // Bottom Left Of The Texture and Quad
	glNormal3f(0, 1, 0); glTexCoord2f(1.0f, 0.0f); glVertex3f(size, size, size);  // Bottom Right Of The Texture and Quad
	glNormal3f(0, 1, 0); glTexCoord2f(1.0f, 1.0f); glVertex3f(size, size, -size);  // Top Right Of The Texture and Quad

																				   // Bottom Face
	glNormal3f(0, -1, 0); glTexCoord2f(1.0f, 1.0f); glVertex3f(-size, -size, -size);  // Top Right Of The Texture and Quad
	glNormal3f(0, -1, 0); glTexCoord2f(0.0f, 1.0f); glVertex3f(size, -size, -size);  // Top Left Of The Texture and Quad
	glNormal3f(0, -1, 0); glTexCoord2f(0.0f, 0.0f); glVertex3f(size, -size, size);  // Bottom Left Of The Texture and Quad
	glNormal3f(0, -1, 0); glTexCoord2f(1.0f, 0.0f); glVertex3f(-size, -size, size);  // Bottom Right Of The Texture and Quad

																					 // Right face
	glNormal3f(1, 0, 0); glTexCoord2f(1.0f, 0.0f); glVertex3f(size, -size, -size);  // Bottom Right Of The Texture and Quad
	glNormal3f(1, 0, 0); glTexCoord2f(1.0f, 1.0f); glVertex3f(size, size, -size);  // Top Right Of The Texture and Quad
	glNormal3f(1, 0, 0); glTexCoord2f(0.0f, 1.0f); glVertex3f(size, size, size);  // Top Left Of The Texture and Quad
	glNormal3f(1, 0, 0); glTexCoord2f(0.0f, 0.0f); glVertex3f(size, -size, size);  // Bottom Left Of The Texture and Quad

																				   // Left Face
	glNormal3f(-1, 0, 0); glTexCoord2f(0.0f, 0.0f); glVertex3f(-size, -size, -size);  // Bottom Left Of The Texture and Quad
	glNormal3f(-1, 0, 0); glTexCoord2f(1.0f, 0.0f); glVertex3f(-size, -size, size);  // Bottom Right Of The Texture and Quad
	glNormal3f(-1, 0, 0); glTexCoord2f(1.0f, 1.0f); glVertex3f(-size, size, size);  // Top Right Of The Texture and Quad
	glNormal3f(-1, 0, 0); glTexCoord2f(0.0f, 1.0f); glVertex3f(-size, size, -size);  // Top Left Of The Texture and Quad
	glEnd();
	glDisable(GL_TEXTURE_2D);
}

void draw_model()
{
	glPushMatrix();
	glTranslatef(x, y, z);
	glRotatef(model_angle, 0, 1, 0);

	glColor3f(0.8, 0.8, 0.8);

	//model here
	glutSolidCube(5);

	glPopMatrix();
}

// Отображение
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

// Реакция на клавиатуру
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

int main(int argc, char **argv)
{
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB);
	glutInitWindowSize(800, 800);
	glutInitWindowPosition(10, 10);
	glutCreateWindow("Lab 12");
	init();
	glutDisplayFunc(display);
	glutReshapeFunc(reshape);
	glutSpecialFunc(specialKeys);
	glutKeyboardFunc(keyboard);
	glutMainLoop();
	return 0;
}