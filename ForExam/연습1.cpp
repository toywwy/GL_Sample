#include<windows.h>
#include<gl/glut.h>

#include<iostream>

using namespace std;

float theta;

void RenderScene(void)
{
	glClear(GL_COLOR_BUFFER_BIT|GL_DEPTH_BUFFER_BIT);

	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();

	glEnable(GL_LIGHTING);
	glEnable(GL_LIGHT0);
	glEnable(GL_COLOR_MATERIAL);
	
	gluLookAt(0.5, 0.5 ,0.5,0, 0, 0, 0, 10, 0);
	
	glLoadIdentity();

	glPushMatrix();
	{
		glColor3f(0.2, 0.8, 0.2);
		glRotatef(-90,0, 1, 0);
		glRotatef(theta, 1, 0, 0);
		glutSolidTeapot(0.3);
	}glPopMatrix();


	glRotatef(theta, 0, 0,1);
	glTranslatef(1, 0, 0);
	glPushMatrix();
	{
		glColor3f(0.8, 0.2, 0.2);
		glutSolidTeapot(0.3);
	}
	glPopMatrix();

	glRotatef(theta, 1, 0, 0);
	glColor3f(0.2, 0.2, 1);//파랭이
	glTranslatef(0,1 , 0);
	glutSolidTeapot(0.3);
	
	glutSwapBuffers();
}


void ChangeSize(int w, int h)
{
	if (h == 0) h = 1;

	glViewport(0, 0, w, h);

	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();

	if (w <= h)
		glOrtho(-2.0, 2.0, -2.0*(h / w), 2.0*(h / w), -2.0, 2.0);
	else
		glOrtho(-2.0*(w / h), 2.0*(w/h) ,-2.0, 2.0, -2.0, 2.0);

	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
} 


void timer(int value)
{
	theta += 2;
	glutTimerFunc(1000 / 30, timer, 1);
	glutPostRedisplay();
}

void init()
{
	glutTimerFunc(1000 / 30, timer, 1);
}

void SetupRC()
{
	glClearColor(1, 1, 1,1);
	glEnable(GL_DEPTH_TEST);
}

void keyboard(char key, int x, int y)
{

}

void menu(int item)
{
	keyboard((unsigned char)item, 0, 0);
}

int main(int argc,char * argv[])
{
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB);
	glutInitWindowSize(600, 600);
	glutCreateWindow("EX01");
	glutDisplayFunc(RenderScene);
	glutReshapeFunc(ChangeSize);


	glutCreateMenu(menu); //메뉴마다 옵션을 넣어줘야한다. 그리고 키보드에 매핑도 가능한것이지..
	glutAddMenuEntry("HELELO", 1);
	glutAddMenuEntry("LEE", 2);
	glutAttachMenu(GLUT_RIGHT_BUTTON);

	init();
	SetupRC();
	glutMainLoop();

	return 0;

}