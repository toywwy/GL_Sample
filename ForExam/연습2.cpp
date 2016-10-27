#include<windows.h>
#include<gl/glut.h>

#include<iostream>

using namespace std;

float theta;


bool mouseLeftDown;
bool firstMouseLeftDown;


float point[100000][3];
int length = 0;
int idx = 0;
void RenderScene(void)
{
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();

	gluLookAt(2, 2, 2, 0, 0, 0, 0, 1, 0);

	glEnable(GL_LIGHTING);
	glEnable(GL_LIGHT0);
	glEnable(GL_COLOR_MATERIAL);

	glLoadIdentity();//내가 보는것과 상관없이 그리니까...

	glPushMatrix();
	{
		glColor3f(0.2, 0.8, 0.2);
		glRotatef(-90, 0, 1, 0);
		glRotatef(theta, 1, 0, 0);
		glutSolidTeapot(0.3);
	}glPopMatrix();


	glRotatef(theta, 0, 0, 1);
	glTranslatef(1, 0, 0);
	glPushMatrix();
	{
		glColor3f(0.8, 0.2, 0.2);
		glutSolidTeapot(0.3);
	}
	glPopMatrix();

	glRotatef(theta, 1, 0, 0);
	glColor3f(0.2, 0.2, 1);//파랭이
	glTranslatef(0, 1, 0);
	glutSolidTeapot(0.3);

	glLoadIdentity();//이걸 해주고해야 원점 0.0 으로부터 그릴수있따.

	glBegin(GL_LINE_STRIP);
	{
		for (int i = 0; i < length; i++)
			glVertex3fv(point[i]);
	}
	glEnd();


	if (!mouseLeftDown&&firstMouseLeftDown)
	{
		glPushMatrix();
		{
			if (idx < length) {
				glTranslatef(point[idx][0], point[idx][1], point[idx][2]);
				glColor3f(0.4, 0.2, 0.2);
				glutSolidTeapot(0.3);
				idx++;
			}
		}
		glPopMatrix();
	}

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
		glOrtho(-2.0*(w / h), 2.0*(w / h), -2.0, 2.0, -2.0, 2.0);

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
	glClearColor(1, 1, 1, 1);
	glEnable(GL_DEPTH_TEST);
}

void MouseClick(int button, int state, int x, int y)
{
	if (button == GLUT_LEFT_BUTTON)
	{
		if (state == GLUT_DOWN)
		{
			idx=length = 0;
			firstMouseLeftDown=mouseLeftDown = true;

		}
		else if (state == GLUT_UP) {
			if(mouseLeftDown)
				mouseLeftDown = false;

		}
	}
	else if (button == GLUT_RIGHT_BUTTON)
	{

	}
}

void MouseMove(int x, int y)
{
	cout <<"----------"<< x << " , " << y << endl;

	double viewport[4];
	glGetDoublev(GL_VIEWPORT, viewport);

	float xLoc = 0;
	float yLoc = 0;

	float width = viewport[2] / 2;
	float height = viewport[3] / 2;

	if (x > width)
	{
		xLoc = (x - width) / width;
	}
	else
	{
		xLoc =-( width-x) / width;

	}

	if (y > height)
	{
		yLoc =-( y - height) / height;
	}
	else
	{
		yLoc = (height-y) / height;
	}

	point[length][0] = xLoc*2.0;
	point[length][1] = yLoc*2.0;
	cout <<xLoc << " , " << yLoc << endl;

	length++;
}


void keyboard(char key, int x, int y)
{

}

void menu(int item)
{
	keyboard((unsigned char)item, 0, 0);
}

int main(int argc, char * argv[])
{
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB);
	glutInitWindowSize(600, 600);
	glutCreateWindow("EX01");
	glutDisplayFunc(RenderScene);
	glutReshapeFunc(ChangeSize);
	glutMouseFunc(MouseClick);
	glutMotionFunc(MouseMove);

	glutCreateMenu(menu); //메뉴마다 옵션을 넣어줘야한다. 그리고 키보드에 매핑도 가능한것이지..
	glutAddMenuEntry("HELELO", 1);
	glutAddMenuEntry("LEE", 2);
	glutAttachMenu(GLUT_RIGHT_BUTTON);
	

	init();
	SetupRC();
	glutMainLoop();

	return 0;

}