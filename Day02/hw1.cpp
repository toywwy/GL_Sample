
#include<iostream>
#include<Windows.h>
#include<gl/GL.h>
#include<gl/GLU.h>
#include<gl/glut.h>
/*
GLUT 이전에는 MFC에서 했었어야했는데 이제는 glut를 이용해서 윈도우를 띄울 것이다.
*/
#include <math.h>

using namespace std;


static bool mouseLeftDown;
static float theta;
static float rC, gC, bC;
void RenderScene(void)
{
	glClear(GL_COLOR_BUFFER_BIT);
	//이런 수학 부분을 이해해야할 것 같은데..
	float cos_th = cos(theta * 3.14159 / 180.0);
	float sin_th = sin(theta * 3.14159 / 180.0);

	glColor3ub(rC, gC, bC);

	glBegin(GL_POLYGON);
	{
		glVertex2f(cos_th, sin_th);
		glVertex2f(-sin_th, cos_th);
		glVertex2f(-cos_th, -sin_th);
		glVertex2f(sin_th, -cos_th);
		//점을 4개 차례대로 찍은거고
		//다각형을 만든 것이다.
	}
	glEnd();
	glFlush();
}

void SetupRC(void) { glClearColor(1.0f, 1.0f, 1.0f, 1.0f); }

void ChangeSize(int w, int h)
{
	GLfloat aspectRatio;
	if (h == 0)
		h = 1;

	glViewport(0, 0, w, h);
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();

	if (w <= h)
		gluOrtho2D(-2.0, 2.0, -2.0*(float)h / (float)w, 2.0*(float)h / (float)w);
	else
		gluOrtho2D(-2.0*(float)w / (float)h, -2.0*(float)w / (float)h, -2.0, 2.0);

	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
}


void mouseButton(int button, int state, int x, int y)
{
	if (button == GLUT_LEFT_BUTTON)
	{
		if (state == GLUT_DOWN)
		{
			rC = (char)rand() % 256;
			gC = (char)rand() % 256;
			bC = (char)rand() % 256;
		}
	}
	else if (button = GLUT_RIGHT_BUTTON)
	{
		if (state == GLUT_DOWN)
		{
			rC = (char)rand() % 256;
			gC = (char)rand() % 256;
			bC = (char)rand() % 256;
		}
	}
	glutPostRedisplay(); //화면 갱신이 필요할 때 이함수를 부르면 display 콜백 함수를 다시 한 번 실행
}

void timer(int value)
{

	if (mouseLeftDown)
		theta -= 4.0; //theta 값의 증감을 바꿔주면된다.
	else
		theta += 4.0; //theta 값의 증감을 바꿔주면된다.


	if (theta >= 360.0)
		theta -= 360.0;

	glutTimerFunc(1000 / 30, timer, 1);
	glutPostRedisplay();
}

void keyboard(unsigned char key, int x, int y)
{
	switch (key)
	{
	case 'q' | 'Q':
		exit(0); break; //종료 시킬수있음
	case 'r' | 'R':

		if (mouseLeftDown) mouseLeftDown = false;
		else mouseLeftDown = true;

		break;
	default:
		break;
	}
}
void init(void)
{
	mouseLeftDown = false;
	theta = 0;
	rC = 1;
	gC = bC = 0;

}
void main(int argc, char * argv[])
{
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_SINGLE | GLUT_RGB);
	glutInitWindowSize(600, 600);

	glutCreateWindow("Simple");
	glutDisplayFunc(RenderScene);
	glutReshapeFunc(ChangeSize);//비율을 바꿔줘야 화면을 늘렸을떄도 무넺가 없다.
	glutMouseFunc(mouseButton); //버튼을 눌렀을떄
	glutKeyboardFunc(keyboard);
	timer(1000);
	SetupRC();
	glutMainLoop();
}
