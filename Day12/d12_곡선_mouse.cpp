#include<iostream>
#include<Windows.h> //�̰� GUI ������ �ʿ��Ѱǵ�.
#include<gl/GL.h>
#include<gl/GLU.h>
#include<gl/glut.h> //�̰� ��ƿ��Ƽ ��Ŷ�ε�, ������� �Է��̳� ȭ�� �����츦 �����ϱ� ���ؼ���.
#include <math.h> //�ﰢ�Լ� ������

/*
* gl.h (OpenGL Core Library) - ������ ����� �����ϴ� ���̺귯��
* glu.h (OpenGL Utility Library) - GL ���̺귯���� ����� ����
* glut.h (OpenGL Utility Toolkit) - ����� �Է��� �޾Ƶ��̰ų� ȭ�� �����츦 �����ϱ� ���� ���̺귯��
*/

#define B0(t) ( (1-t) * (1-t) *(1-t))
#define B1(t) ( 3*(1-t) * (1-t) *(t))
#define B2(t) ( 3*(t) * (t) *(1-t))
#define B3(t) ( t*t*t )

using namespace std;

struct Point {
	float x = 0.0f;
	float y = 0.0f;
	float z = 0.0f;
};

bool isLeftMouseDown;
char * applicationName = " Simple Curvds";
int windowWidth = 500;
int windowHeight = 500;
int orthoSize = 2.0;

Point controlPoint[4];
Point mouseDropPoint;
int selectPoint = -1;

void drawControlPoints()
{
	glColor3f(1.0f, 0.0f, 0.0f);
	glPointSize(5.0f);

	for (int i = 0; i < 4; i++)
	{
		glBegin(GL_POINTS);
		{
			glVertex3f(controlPoint[i].x, controlPoint[i].y, controlPoint[i].z);
		}glEnd();
	}
}

void drawControlLines()
{
	glColor3f(0.0f, 0.0f, 0.0f);
	glLineStipple(3, 0xcccc);

	glEnable(GL_LINE_STIPPLE);

	for (int i = 0; i < 3; i++)
	{
		glBegin(GL_LINES);
		{
			glVertex3f(controlPoint[i].x, controlPoint[i].y, controlPoint[i].z);
			glVertex3f(controlPoint[i + 1].x, controlPoint[i + 1].y, controlPoint[i + 1].z);
		}glEnd();

	}
	glDisable(GL_LINE_STIPPLE);
}

//����� ����� �ִ� �κ��̴�.
void drawBezierCurve()
{
	float segments = 0.02f;// 
	float t = 0.0f;// �갡 1.0�� �ɶ� ���� ..

	Point p[2];
	p[1] = controlPoint[0];//�׳� ���� ���� ��� �ҷ��� �̷��� �ѰŴ�.
	//p[1].x = p[1].y = p[1].z = 0;
	for (; t < 1.0f; t += segments)//���� ��� �ִ°� ���� ������..
	{
		p[0] = p[1];
		p[1].x = (controlPoint[0].x * B0(t)) + (controlPoint[1].x * B1(t)) + (controlPoint[2].x * B2(t)) + (controlPoint[3].x*B3(t));

		p[1].y = (controlPoint[0].y * B0(t)) + (controlPoint[1].y * B1(t)) + (controlPoint[2].y * B2(t)) + (controlPoint[3].y*B3(t));
		p[1].z = 0.0f;

		glColor3f(0.0f, 0.0f, 1.0f);

		glBegin(GL_LINES);
		{
			glVertex3f(p[0].x, p[0].y, p[0].z);
			glVertex3f(p[1].x, p[1].y, p[1].z);
		}
		glEnd();
	}
}

void drawMouseDropPoint()
{
	glColor3f(0.0f, 1.0f, 0.0f);
	glPointSize(5.0);

	glBegin(GL_POINTS);
	{
		glVertex3f(mouseDropPoint.x, mouseDropPoint.y, mouseDropPoint.z);
	}
	glEnd();
}

void mouseDrop(int _x, int _y)
{
	mouseDropPoint.x = (((float)windowWidth / 2) - ((float)windowWidth - _x)) / ((float)windowWidth / (orthoSize * 2));
	mouseDropPoint.y = (((float)windowHeight / 2) - _y) / ((float)windowHeight / (orthoSize * 2));
	mouseDropPoint.z = 0.0f;

	std::cout << mouseDropPoint.x << " " << mouseDropPoint.y << std::endl;
}


void timer(int value)
{
	glutPostRedisplay();//ȭ�� ������ �ʿ��� �� ���Լ��� �θ��� display �ݹ� �Լ��� �ٽ� �� �� ����
	glutTimerFunc(1000 / 30, timer, 1);//���̸� �ð� ���� ����,  ���ش� //30 frame ���´�.
									   //Ÿ�̸Ӱ� �۵��ϴµ� �ٽ� Ÿ�̸Ӹ� ���۽�Ŵ���ν� ��� �۵��ϰ� �Ǵ°���.
}

void RenderScene(void)
{
	//Clear the window with current clearing color
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT); //������ ������ �����.
														/*  DEPTH_BUFFER�� �ξ �ȼ����� ���� ���� �����ϵ��� �Ͽ���. �̸� �̿��ؼ� � ��ü�� �׸����� �Ҷ� �׾տ� ��ü�� ������ ������ ���̸�
														���Ͽ� DEPTH�� ���� �ﰢ������ �׷�����, DEPTH�� ū �簢������ �׸��� �ʰ� �� �� �ְԵǴ� ���̴�.
														(������ ��ǥ�� �ȼ��� ������ ���̿� ���� �׷��� ���� ���ƾ� ���� �����ϰ� �Ǵ� ���̴�.) */



	glMatrixMode(GL_MODELVIEW);//MODELVEIW�� �׸��°Ű� projection�� �����ϴ°Ŵ�.
	glLoadIdentity();

	drawControlPoints();
	drawControlLines();

	drawBezierCurve();//� �׸��� �� ȣ���Ѵ�.

	if (isLeftMouseDown) drawMouseDropPoint();


	glutSwapBuffers(); //GLUT_DOUBLE ���۸� ������ؼ� �����ؼ������ ���̴�.
}

void SetupRC(void) {
	glClearColor(0.8f, 0.8f, 0.8f, 1.0f);
	glEnable(GL_DEPTH_TEST);
}

void init(void)
{

	isLeftMouseDown = false;
	controlPoint[0].x = -1.0f; controlPoint[0].y = -1.0f; controlPoint[0].z = 0.0f;
	controlPoint[1].x = -1.0f; controlPoint[1].y = 1.0f; controlPoint[1].z = 0.0f;
	controlPoint[2].x = 1.0f; controlPoint[2].y = 1.0f; controlPoint[2].z = 0.0f;
	controlPoint[3].x = 1.0f; controlPoint[3].y = -1.0f; controlPoint[3].z = 0.0f;


	glutTimerFunc(1000 / 30, timer, 1);
}

void ChangeSize(int w, int h)
{
	if (h == 0)// 0���� ���������°� �������ؼ�
		h = 1;

	glViewport(0, 0, w, h); // ������ �������� �κ��� ����Ʈ�� ����Ѵ�.

	glMatrixMode(GL_PROJECTION); //�̺κ��� PROJECTION�� ������ 3D�� 2D�� �������Ѿ��Ѵ�.
								 //glOrtho�� ���ؼ�
	glLoadIdentity();//��ǥ�� �ʱ�ȭ
					 //�ٵ� �̰� ���� �ƴµ� ,�̷��� ���� ��ǥ�踦 �ʱ�ȭ����� �ϰ� �̷��� ���ȳ��ٵ�...
					 //��Ǳٵ� �Ƚᵵ ������°ű��� ....��ǥ�踦 �������� �����̻�????
					 //���⿡�� �ʱ�ȭ ���ִ� ������ ����?? � ��ǥ�踦 �ʱ�ȭ �����شٴ� ������??

	if (w <= h)
		glOrtho(-2.0, 2.0, -2.0*(float)h / (float)w,
			2.0*(float)h / (float)w, 10.0, -10.0);
	else
		glOrtho(-2.0*(float)w / (float)h, 2.0*(float)w / (float)h,
			-2.0, 2.0, 10.0, -10.0);
}

void mouse(int button, int state, int x, int y)
{
	if (button == GLUT_LEFT_BUTTON && state == GLUT_DOWN)
	{
		isLeftMouseDown = true;
		mouseDrop(x, y);
	}
	if (button == GLUT_LEFT_BUTTON && state == GLUT_UP)
	{
		isLeftMouseDown = false;
	}

	glutPostRedisplay();
}

void motion(int x, int y)
{
	if (isLeftMouseDown)
	{
		mouseDrop(x, y);
	}
	glutPostRedisplay();
}


void main(int argc, char * argv[])
{
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB);
	glutInitWindowSize(600, 600);
	glutCreateWindow("Simple");

	glutDisplayFunc(RenderScene);//�������Ǵ� �κ�
	glutReshapeFunc(ChangeSize);//SIZE�� �ٲ𸶴� ȣ���� �ȴ�.
								//	glutSpecialFunc(specialKeyboard);//����� Ű���� �̺�Ʈ�� �ݹ�
								//glutKeyboardFunc(keyboard);//Ű���� �̺�Ʈ �ݹ�
	glutMouseFunc(mouse);
	glutMotionFunc(motion);
	//453, 948 //453���� ���� 948������ �ﰢ������ ������ ���ִ�.....
	//�� ���� ������ ���̴� �����ΰ�???
	//

	//TODO ���콺�� �̿��ؼ� ������ �غ���
	//void drawBezierCurve() �갡 ��� �׸��°ǵ� ���⿡�� ������ ���� �������� ��� �׸��� �� �Ͱ���.



	init();
	SetupRC();
	glutMainLoop();
	//�⺻���� Ʋ������ �ܿ��ʿ���� ���迡�ȳ��´� ���躼�� �ش�.
}
