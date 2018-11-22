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
	//glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, image);
	//SOIL_free_image_data(image);
	glBindTexture(GL_TEXTURE_2D, 0);
}

float year = 0, day = 0;
float sun[3]{ 1, 1.0, 0 };
float planet[3]{ 0, 1.0, 0 };
//Инициализация
void init(void)
{
	glClearColor(0.0, 0.0, 0.0, 0.0);
	glEnable(GL_NORMALIZE);
	//glEnable(GL_COLOR_MATERIAL);
	//glEnable(GL_LIGHT0);

	//glEnable(GL_LIGHTING);
	
	glEnable(GL_DEPTH_TEST);


	makeTextureImage();	
}
//Изменение размеров окна
void reshape(int w, int h)
{
	glViewport(0, 0, (GLsizei)w, (GLsizei)h);
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	gluPerspective(60.0, (GLfloat)w / (GLfloat)h, 1.0, 20.0);
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
	gluLookAt(0, 5, 10, 0.0, 0.0, 0.0, 0.0, 1.0, 0.0);
}

//Отображение
void display(void)
{
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	glEnable(GL_TEXTURE_2D);
	glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_REPLACE);
	glBindTexture(GL_TEXTURE_2D, texture);
	glBegin(GL_QUADS);
	glTexCoord2f(0.0, 0.0); glVertex3f(-10.0, -0, -10.0);
	glTexCoord2f(0.0, 1.0); glVertex3f(-10.0, 0, 10.0);
	glTexCoord2f(1.0, 1.0); glVertex3f(10.0, 0.0, 10);
	glTexCoord2f(1.0, 0.0); glVertex3f(10.0, 0.0, -10.0);
	
	glEnd();
	glDisable(GL_TEXTURE_2D);

	glPushMatrix();
	//Рисуем солнце
	glColor3fv(sun);
	glutSolidSphere(1.0, 20, 16);
	glRotatef(year, 0.0, 1.0, 0.0);
	glTranslatef(2.0, 0.0, 0.0);
	glRotatef(day, 0.0, 1.0, 0.0);
	//Рисуем планету
	glColor3fv(planet);
	glutSolidSphere(0.2, 10, 8);
	glPopMatrix();

	glutSwapBuffers();
	
}
//Реакция на клавиатуру
void specialKeys(int key, int x, int y) {
	switch (key) {
	case GLUT_KEY_UP: day+=1; break;
	case GLUT_KEY_DOWN: day-= 1; break;
	case GLUT_KEY_RIGHT: year += 5; break;
	case GLUT_KEY_LEFT: year -= 5; break;
	}
	glutPostRedisplay();
}

int main(int argc, char **argv)
{
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB);
	glutInitWindowSize(500, 500);
	glutInitWindowPosition(100, 100);
	glutCreateWindow("Planetary System");
	init();
	glutDisplayFunc(display);
	glutReshapeFunc(reshape);
	glutSpecialFunc(specialKeys);
	glutMainLoop();
	return 0;
}