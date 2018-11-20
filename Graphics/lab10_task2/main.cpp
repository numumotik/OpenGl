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

int num = 0;
int width = 0, height = 0;
int rotate_mode = 0;
double rotate_x = 0;
double rotate_y = 0;
double rotate_z = 0;

// Init_OpenGL() function    
void Init_OpenGL()
{
	// set background color to Black    
	glClearColor(0.0, 0.0, 0.0, 0.0);
	glEnable(GL_DEPTH_TEST);
	glEnable(GL_LIGHTING);
	glEnable(GL_COLOR_MATERIAL);
	
	glEnable(GL_LIGHT0);
	glLightModelf(GL_LIGHT_MODEL_TWO_SIDE, GL_TRUE); 
	
	float light_ambient[] = { 0.0,0.0,0.0,1.0 }; 
	float light_diffuse[] = { 1.0,1.0,1.0,1.0 }; 
	float light_specular[] = { 1.0,1.0,1.0,1.0 }; 
	float light_position[] = { 0.0, 0.0, 0.0, 100.0 }; 
	glLightfv(GL_LIGHT0, GL_AMBIENT, light_ambient); 
	glLightfv(GL_LIGHT0, GL_DIFFUSE, light_diffuse); 
	glLightfv(GL_LIGHT0, GL_SPECULAR, light_specular);
	glLightfv(GL_LIGHT0, GL_POSITION, light_position);

	glEnable(GL_NORMALIZE);
}

// Display_Objects() function    
void Display_Objects(void)
{
	// clearing the window or remove all drawn objects    
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	
	if (num == 0)
	{
		glViewport(0, 0, width, height);
		glMatrixMode(GL_PROJECTION);
		glLoadIdentity();
		gluPerspective(65.0, width / height, 1.0, 1000.0);
		glMatrixMode(GL_MODELVIEW);
		glLoadIdentity();
		gluLookAt(-0.3, 0.5, 5.0, 0.0, 0.0, 0.0, 0.0, 1.0, 0.0);
	}
	else
	{
		glViewport(0, 0, width, height); 
		glMatrixMode(GL_PROJECTION); 
		glLoadIdentity(); 
		glOrtho(-5, 5, -5, 5, -5, 5); 
		glMatrixMode(GL_MODELVIEW); 
		glLoadIdentity();
	}

	glTranslatef(0, 0.0, 0.0);
	glColor3f(1, 1, 1);
	glutSolidCube(0.1);

	if (rotate_mode == 0)
	{
		glRotatef(rotate_x, 1, 0, 0);
		glRotatef(rotate_y, 0, 1, 0);
		glRotatef(rotate_z, 0, 0, 1);
	}
	/*glPushMatrix(), which copies the current matrix and adds
	the copy to the top of the stack, and
	glPopMatrix(), which discards the top matrix on the stack*/	    
	
	//gold
	glPushMatrix();
	glTranslatef(1, 0.0, 1.0);
	if (rotate_mode)
	{
		glRotatef(rotate_x, 1, 0, 0);
		glRotatef(rotate_y, 0, 1, 0);
		glRotatef(rotate_z, 0, 0, 1);
	}
	glColor3f(1, 0.78, 0.09); 
	float mat_specular[] = { 1,1,1,1 };
	glMaterialfv(GL_FRONT_AND_BACK, GL_SPECULAR, mat_specular);
	glMaterialf(GL_FRONT_AND_BACK, GL_SHININESS, 128.0);
	glutSolidCube(0.5);
	glPopMatrix();

	//silver
	glPushMatrix(); 
	if (rotate_mode == 1)
	{
		glTranslatef(1, 0.0, 1.0);
		glRotatef(rotate_x, 1, 0, 0);
		glRotatef(rotate_y, 0, 1, 0);
		glRotatef(rotate_z, 0, 0, 1);
		glTranslatef(-1, 0.0, -1.0);
	}
	glTranslatef(0.25, 0.0, 1.0);
	if (rotate_mode == 2)
	{
		glRotatef(rotate_x, 1, 0, 0);
		glRotatef(rotate_y, 0, 1, 0);
		glRotatef(rotate_z, 0, 0, 1);
	}
	glColor3f(0.75, 0.75, 0.75);
	glutSolidCube(0.5);
	glPopMatrix();

	//bronze
	glPushMatrix();
	if (rotate_mode == 1)
	{
		glTranslatef(1, 0.0, 1.0);
		glRotatef(rotate_x, 1, 0, 0);
		glRotatef(rotate_y, 0, 1, 0);
		glRotatef(rotate_z, 0, 0, 1);
		glTranslatef(-1, 0.0, -1.0);
	}
	glTranslatef(1.7, 0, 1.0);
	if (rotate_mode == 2)
	{
		glRotatef(rotate_x, 1, 0, 0);
		glRotatef(rotate_y, 0, 1, 0);
		glRotatef(rotate_z, 0, 0, 1);
	}
	glColor3f(0.8, 0.5, 0.2);
	glutSolidCube(0.5);
	glPopMatrix();

	glFlush();
	glutSwapBuffers();
}
// Reshape() function    
void Reshape(int w, int h)
{
	width = w; height = h;
	//adjusts the pixel rectangle for drawing to be the entire new window    
	glViewport(0, 0, (GLsizei)w, (GLsizei)h);
	//matrix specifies the projection transformation    
	glMatrixMode(GL_PROJECTION);
	// load the identity of matrix by clearing it.    
	glLoadIdentity();
	gluPerspective(65.0, (GLfloat)w / (GLfloat)h, 1.0, 1000.0);
	//matrix specifies the modelview transformation    
	glMatrixMode(GL_MODELVIEW);
	// again  load the identity of matrix    
	glLoadIdentity();
	// gluLookAt() this function is used to specify the eye.    
	// it is used to specify the coordinates to view objects from a specific position    
	gluLookAt(-0.3, 0.5, 5.0, 0.0, 0.0, 0.0, 0.0, 1.0, 0.0);
}

void specialKeys(int key, int x, int y) {
	switch (key) {
	case GLUT_KEY_UP: rotate_x += 5; break;
	case GLUT_KEY_DOWN: rotate_x -= 5; break;
	case GLUT_KEY_RIGHT: rotate_y += 5; break;
	case GLUT_KEY_LEFT: rotate_y -= 5; break;
	case GLUT_KEY_PAGE_UP: rotate_z += 5; break;
	case GLUT_KEY_PAGE_DOWN: rotate_z -= 5; break;
	case GLUT_KEY_F1: 
		rotate_x = rotate_y = rotate_z = 0;
		rotate_mode = 0; break;
	case GLUT_KEY_F2:
		rotate_x = rotate_y = rotate_z = 0; rotate_mode = 1; break;
	case GLUT_KEY_F3:
		rotate_x = rotate_y = rotate_z = 0; rotate_mode = 2; break;
	}
	glutPostRedisplay();
}

void Mouse(int button, int state, int x, int y)
{
	if (state == GLUT_DOWN)
	{
		num = (num+1)%2;
	}
	glutPostRedisplay();
}

void Keyboard(unsigned char key, int x, int y)
{
	if (key == '\1')
		rotate_mode = 1;
}
// main function    
int main(int argc, char** argv)
{
	glutInit(&argc, argv);
	glutInitWindowPosition(100, 100);
	glutInitWindowSize(800, 600);
	glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB | GLUT_DEPTH);
	glutCreateWindow("OpenGL Demo");
	Init_OpenGL();
	glutDisplayFunc(Display_Objects);
	glutReshapeFunc(Reshape);
	glutMouseFunc(Mouse);
	glutSpecialFunc(specialKeys);
	glutKeyboardFunc(Keyboard);
	glutMainLoop();
	return 0;
}