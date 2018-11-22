#include<Windows.h>    
// first include Windows.h header file which is required    
#include<stdio.h>    
#include<gl/GL.h>   // GL.h header file    
#include<gl/GLU.h> // GLU.h header file    
#include<gl/glut.h>  // glut.h header file from freeglut\include\GL folder    
#include<conio.h>    
#include<stdio.h>    
#include<math.h>    
#include<string.h>    
#include<SOIL.h>

unsigned char* image;
GLuint texture;
void makeTextureImage()
{
	int width, height;
	image = SOIL_load_image("texture.jpg", &width, &height, 0, SOIL_LOAD_RGB);
	
	glGenTextures(1, &texture);
	glBindTexture(GL_TEXTURE_2D, texture);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, image);
	glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_REPLACE);
	//SOIL_free_image_data(image);
	//glBindTexture(GL_TEXTURE_2D, 0);
}

#define checkImageWidth 64
#define checkImageHeight 64
GLubyte checkImage[checkImageHeight][checkImageWidth][4];
GLuint texName;
void makeCheckImage()
{
	int i, j, c;
	for (i = 0; i<checkImageHeight; i++)
	{
		for (j = 0; j<checkImageWidth; j++)
		{
			c = (((i & 0x8) == 0) ^ ((j & 0x8) == 0)) * 55;
			checkImage[i][j][0] = (GLubyte)c;
			checkImage[i][j][1] = (GLubyte)c;
			checkImage[i][j][2] = (GLubyte)c;
			checkImage[i][j][3] = (GLubyte)255;
		}
	}
	glPixelStorei(GL_UNPACK_ALIGNMENT, 1);
	glGenTextures(1, &texName);
	glBindTexture(GL_TEXTURE_2D, texName);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, checkImageWidth, checkImageHeight,
		0, GL_RGBA, GL_UNSIGNED_BYTE, checkImage);

	glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_REPLACE);
}
float sun[3]{ 1, 1.0, 0 };
float camera_angle = 50;
float car_angle = 0;
int is_ahead = 0;
int is_back = 0;
//Инициализация
void init(void)
{
	glClearColor(0.0, 0.0, 0.0, 0.0);
	glEnable(GL_NORMALIZE);
	glEnable(GL_COLOR_MATERIAL);
	
	GLfloat light_ambient[] = { 0.0,0.0,0.0,1.0 };
	GLfloat light_diffuse[] = { 1.0,1.0,1.0,1.0 };
	GLfloat light_specular[] = { 1.0,1.0,1.0,1.0 };
	glLightfv(GL_LIGHT1, GL_AMBIENT, light_ambient);
	glLightfv(GL_LIGHT1, GL_DIFFUSE, light_diffuse);
	glLightfv(GL_LIGHT1, GL_SPECULAR, light_specular);
	glEnable(GL_LIGHT1);
	glLightfv(GL_LIGHT2, GL_AMBIENT, light_ambient);
	glLightfv(GL_LIGHT2, GL_DIFFUSE, light_diffuse);
	glLightfv(GL_LIGHT2, GL_SPECULAR, light_specular);
	glEnable(GL_LIGHT2);
	glLightfv(GL_LIGHT3, GL_AMBIENT, light_ambient);
	glLightfv(GL_LIGHT3, GL_DIFFUSE, light_diffuse);
	glLightfv(GL_LIGHT3, GL_SPECULAR, light_specular);
	glEnable(GL_LIGHT3);
	glEnable(GL_DEPTH_TEST);
	glEnable(GL_LIGHTING);

	//makeTextureImage();	
	makeCheckImage();
}
//Изменение размеров окна
void reshape(int w, int h)
{
	glViewport(0, 0, (GLsizei)w, (GLsizei)h);
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	gluPerspective(60.0, (GLfloat)w / (GLfloat)h, 1.0, 100.0);
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
}

void drawLights()
{
	GLfloat position[] = { 0,3,0,1 };
	glColor3f(1.0, 1.0, 0.0);

	glPushMatrix();
	glTranslatef(-5, 0, 0);
	glLightfv(GL_LIGHT1, GL_POSITION, position);
	glTranslatef(5, 0, -10);
	glLightfv(GL_LIGHT2, GL_POSITION, position);
	glTranslatef(3, 0, 13);
	glLightfv(GL_LIGHT3, GL_POSITION, position);
	glPopMatrix();

	glPushMatrix();
	glDisable(GL_LIGHTING);
	glTranslatef(-5, 3, 0);
	glutWireCube(0.1);
	glTranslatef(5, 0, -10);
	glutWireCube(0.1); 
	glTranslatef(3, 0, 13);
	glutWireCube(0.1);
	glEnable(GL_LIGHTING);
	glPopMatrix();
}

void drawLamps()
{
	glColor3f(0.5, 0.5, 0.5);

	glPushMatrix();
	glTranslatef(-5, 0, 0);
	glRotatef(-90, 1, 0, 0);
	glutSolidCone(0.2, 3, 10, 10);
	glPopMatrix();

	glPushMatrix();
	glTranslatef(0, 0, -10);
	glRotatef(-90, 1, 0, 0);
	glutSolidCone(0.2, 3, 10, 10);
	glPopMatrix();

	glPushMatrix();
	glTranslatef(3, 0, 3);
	glRotatef(-90, 1, 0, 0);
	glutSolidCone(0.2, 3, 10, 10);
	glPopMatrix();
}

double gr_cos(float angle)
{
	return cos(angle / 180 * 3.14);
}

double gr_sin(float angle)
{
	return sin(angle / 180 * 3.14);
}

void setCamera()
{
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
	double x = 10 * gr_cos(camera_angle);
	double y = 10 * gr_sin(camera_angle);
	gluLookAt(x, 5, y, 0.0, 0.0, 0.0, 0.0, 1.0, 0.0);
}

float x = 0; float z = 0;
float SPEED = 0.1;

void drawCar()
{
	glPushMatrix();
	glColor3f(1, 1, 0);
	if (is_ahead)
	{
		x += SPEED*gr_sin(car_angle);
		z += SPEED*gr_cos(car_angle);
		is_ahead = 0;
	}
	if (is_back)
	{
		x -= SPEED*gr_sin(car_angle);
		z -= SPEED*gr_cos(car_angle);
		is_back = 0;
	}
	glTranslatef(x, 0, z);
	glRotatef(car_angle, 0, 1, 0);
	glutSolidCube(1);
	glColor3f(1, 0, 0);
	glTranslatef(0, 0, 0.5);
	glutSolidCube(0.5);
	glPopMatrix();
}
//Отображение
void display(void)
{
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	setCamera();
	drawLights();
	glEnable(GL_TEXTURE_2D);
	glBegin(GL_QUADS);
	glTexCoord2f(0.0, 0.0); glVertex3f(-10.0, -0, -10.0);
	glTexCoord2f(0.0, 1.0); glVertex3f(-10.0, 0, 10.0);
	glTexCoord2f(1.0, 1.0); glVertex3f(10.0, 0.0, 10);
	glTexCoord2f(1.0, 0.0); glVertex3f(10.0, 0.0, -10.0);
	glEnd();
	glDisable(GL_TEXTURE_2D);
	drawCar();
	drawLamps();	
	glutSwapBuffers();
	
}
//Реакция на клавиатуру
void specialKeys(int key, int x, int y) {
	switch (key) {
	case GLUT_KEY_UP: is_ahead = 1; break;
	case GLUT_KEY_DOWN: is_back = 1; break;
	case GLUT_KEY_RIGHT: car_angle -= 5; break;
	case GLUT_KEY_LEFT: car_angle += 5; break;
	case GLUT_KEY_F1: 
		if (glIsEnabled(GL_LIGHT1))
			glDisable(GL_LIGHT1);
		else glEnable(GL_LIGHT1);
		break;
	case GLUT_KEY_F2:
		if (glIsEnabled(GL_LIGHT2))
			glDisable(GL_LIGHT2);
		else glEnable(GL_LIGHT2);
		break;
	case GLUT_KEY_F3:
		if (glIsEnabled(GL_LIGHT3))
			glDisable(GL_LIGHT3);
		else glEnable(GL_LIGHT3);
		break;
	case GLUT_KEY_PAGE_UP:
		camera_angle += 1;
		break;
	case GLUT_KEY_PAGE_DOWN:
		camera_angle -= 1;
		break;
	}
	glutPostRedisplay();
}

void keyboard(unsigned char key, int x, int y)
{
	switch (key)
	{
	case 'l':
	case 'L':
		camera_angle += 0.5;
		break;
	case 'r':
	case 'R':
		camera_angle -= 0.5;
		break;
	case 'a':
	case 'A':
		car_angle += 3;
		break;
	case 'd':
	case 'D':
		car_angle -= 3;
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
	glutCreateWindow("Planetary System");
	init();
	glutDisplayFunc(display);
	glutReshapeFunc(reshape);
	glutSpecialFunc(specialKeys);
	glutKeyboardFunc(keyboard);
	glutMainLoop();
	return 0;
}