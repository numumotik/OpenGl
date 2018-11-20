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
#include <random>
#include <ctime>


int width = 0, height = 0;
int num = 0;
float color_r = 0.8, color_g = 0.8, color_b = 0.8;
double rotate_x = 0;
double rotate_y = 0;
double rotate_z = 0;

void reshape(int w, int h)
{
	width = w; height = h;
	//adjusts the pixel rectangle for drawing to be the entire new window    
	glViewport(0, 0, w, h);
	//matrix specifies the projection transformation    
	glMatrixMode(GL_PROJECTION);
	// load the identity of matrix by clearing it.    
	glLoadIdentity();
	gluPerspective(65.0f, w/h, 1.0f, 1000.0f);
	//matrix specifies the modelview transformation    
	glMatrixMode(GL_MODELVIEW);
	// again  load the identity of matrix    
	glLoadIdentity();
	// gluLookAt() this function is used to specify the eye.    
	// it is used to specify the coordinates to view objects from a specific position    
	gluLookAt(-0.3, 0.5, 5.0, 0.0, 0.0, 0.0, 0.0, 1.0, 0.0);
}

void draw_triangle()
{
	glBegin(GL_TRIANGLES);
	glColor3f(color_r, color_g, color_b);
	glVertex3f(-0.75, -0.50, 0);
	glVertex3f(-0.75, 0.50, 0);
	glVertex3f(0.75, -0.50, 0);
	glEnd();
}

void draw_quad()
{
	glBegin(GL_POLYGON);
	glColor3f(color_r, color_g, color_b);
	glVertex3f(-0.75, -0.50, 0);
	glVertex3f(-0.75, 0.50, 0);
	glVertex3f(0.75, 0.50, 0);
	glVertex3f(0.75, -0.50, 0);
	glEnd();
}

void draw_rainbow_triangle()
{
	glBegin(GL_TRIANGLES);
	glColor3f(1, 0, 0);
	glVertex3f(-0.75, -0.50, 0);
	glColor3f(0, 1, 0);
	glVertex3f(-0.75, 0.50, 0);
	glColor3f(0, 0, 1);
	glVertex3f(0.75, -0.50, 0);
	glEnd();
}

void draw_primitive()
{
	glColor3f(color_r, color_g, color_b);
	switch (num)
	{
	case 0: glutWireCube(0.5); break;
	case 1: draw_triangle(); break;
	case 2: draw_quad(); break;
	case 3: draw_rainbow_triangle(); break;
	case 4: glutWireSphere(0.5, 10, 10); break;
	case 5: glutWireIcosahedron(); break;
	case 6: glutWireTorus(0.2, 0.5, 10, 10); break;
	default: glutWireTeapot(0.5);
		break;
	}

}

void my_display()
{
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glLoadIdentity();
	glRotatef(rotate_x, 1.0, 0.0, 0.0);
	glRotatef(rotate_y, 0.0, 1.0, 0.0);
	glRotatef(rotate_z, 0.0, 0.0, 1.0);
	draw_primitive();
	glFlush();
	glutSwapBuffers();
}

void specialKeys(int key, int x, int y) {
	switch (key) {
	case GLUT_KEY_UP: rotate_x+= 5; break;
	case GLUT_KEY_DOWN: rotate_x -= 5; break;
	case GLUT_KEY_RIGHT: rotate_y += 5; break;
	case GLUT_KEY_LEFT: rotate_y -= 5; break;
	case GLUT_KEY_PAGE_UP: rotate_z += 5; break;
	case GLUT_KEY_PAGE_DOWN: rotate_z -= 5; break;
	}
	glutPostRedisplay();
}

void Mouse(int button, int state, int x, int y) 
{
	if (state == GLUT_DOWN)
	{
		++num;
		rotate_x = rotate_y = rotate_z = 0;
		num %= 8;
		color_r = (float)rand() / (RAND_MAX);
		color_g = (float)rand() / (RAND_MAX);
		color_b = (float)rand() / (RAND_MAX);

	}
	glutPostRedisplay();
}

// main function    
int main(int argc, char** argv)
{
	srand(time(0));
	// initialize glut    
	glutInit(&argc, argv);
	glutInitWindowPosition(100, 100);
	glutInitWindowSize(800, 600);
	glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB | GLUT_DEPTH);
	// create window with window text    
	glutCreateWindow("OpenGL");   
	
	//glutIdleFunc(my_idle);   
	glutDisplayFunc(my_display);  
	glutSpecialFunc(specialKeys);
	//glutReshapeFunc(reshape);
	glutMouseFunc(Mouse);
	//init(); 
	glutMainLoop();
	return 0;
}