#include<iostream>
#include<WIndows.h> //�̰� GUI ������ �ʿ��Ѱǵ�.
#include<gl/GL.h>
#include<gl/GLU.h>
#include<gl/glut.h> //�̰� ��ƿ��Ƽ ��Ŷ�ε�, ������� �Է��̳� ȭ�� �����츦 �����ϱ� ���ؼ���.
#include <math.h> //�ﰢ�Լ� ������

/*
* gl.h (OpenGL Core Library) - ������ ����� �����ϴ� ���̺귯��
* glu.h (OpenGL Utility Library) - GL ���̺귯���� ����� ����
* glut.h (OpenGL Utility Toolkit) - ����� �Է��� �޾Ƶ��̰ų� ȭ�� �����츦 �����ϱ� ���� ���̺귯��
*/

using namespace std;

typedef GLfloat point3[3];

static float theta[3];

void DrawTriangle(point3 &a, point3 &b, point3 &c);
void Normalize(point3 &p);
void DivideTriangle(point3 &a, point3 &b, point3 &c, int n);
void CrossProduct(point3 &a, point3 &b, point3 &c, point3 &r);

void Normalize(point3 &p)
{
	double d = p[0] * p[0] + p[1] * p[1] + p[2] * p[2];

	if (d > 0)
	{
		float len = (float)(1.0 / sqrt(d));
		p[0] *= len;
		p[1] *= len;
		p[2] *= len;
	}
}


void CrossProduct(point3 &a, point3 &b, point3 &c, point3 &r)
{
	r[0] = (b[1] - a[1]) * (c[2] - a[2]) - (b[2] - a[2]) * (c[1] - a[1]);
	r[1] = (b[2] - a[2]) * (c[0] - a[0]) - (b[0] - a[0]) * (c[2] - a[2]);
	r[2] = (b[0] - a[0]) * (c[1] - a[1]) - (b[1] - a[1]) * (c[0] - a[0]);
	Normalize(r);
}

void DivideTriangle(point3 &a, point3 &b, point3 &c, int n)
{
	if (n > 0)
	{
		point3 v1, v2, v3;
		for (register int i = 0; i < 3; i++)
		{
			v1[i] = a[i] + b[i];
			v2[i] = b[i] + c[i];
			v3[i] = c[i] + a[i];
		}

		Normalize(v1);
		Normalize(v2);
		Normalize(v3);

		DivideTriangle(a, v1, v3, n - 1);
		DivideTriangle(b, v2, v1, n - 1);
		DivideTriangle(c, v3, v2, n - 1);
		DivideTriangle(v1, v2, v3, n - 1);
	}
	else
		DrawTriangle(a,b,c);
}

void DrawTriangle(point3 &a, point3 &b, point3&c)
{

	glEnable(GL_LIGHTING);//�� LIGHT ������Ѵ�.
	
	//Flat Shading
	//�̰� ��Ī�� �÷� ���̵� �̿��� �ſ���.
	point3 n;//��������??? �̰� ���� �׳� ������ �Ǵ°ǰ�??
	CrossProduct(a, b, c, n);
	glBegin(GL_TRIANGLES);
	{
		glNormal3fv(n);
		glVertex3fv(a);
		glVertex3fv(b);
		glVertex3fv(c);
	}
	glEnd();


	////Smooth Shading
	//
	//glBegin(GL_TRIANGLES);
	//{
	//	glNormal3fv(a); // �븻����� �̿��ؼ� Shading�� �ϴ°��̴�.
	//	glVertex3fv(a);

	//	glNormal3fv(b);
	//	glVertex3fv(b);

	//	glNormal3fv(c);
	//	glVertex3fv(c);
	//}
	//glEnd();
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

	gluLookAt(0, 0, 2.0, 0, 0, 0, 0, 1.0, 0); //���� �����̴�.
											  //ó���� ��, at�� ����κ��� �����ΰ� �̱⶧���� ���� �Ȱǵ鿩..
											  //up�� ���� ���ϴ°Ŵ�...��������� �����ΰ�.

	//----------------------

	glEnable(GL_LIGHT0);
	glRotatef(theta[2], 0, 0, 1);
	glRotatef(theta[1], 0, 1, 0);
	glRotatef(theta[0], 1, 0, 0);

	point3 v[4] = {
		{0,0,1},
		{ 0,			0.942809,-0.333333 },
		{-0.816497,		-0.471405,-0.333333},
		{0.816497,		-0.471405,-0.333333}
	};

	int n = 3;

	DivideTriangle(v[0], v[1], v[2], n);
	DivideTriangle(v[0], v[2], v[3], n);
	DivideTriangle(v[0], v[3], v[1], n);
	DivideTriangle(v[3], v[2], v[1], n);

	glutSwapBuffers(); //GLUT_DOUBLE ���۸� ������ؼ� �����ؼ������ ���̴�.
}

void SetupRC(void) {
	glClearColor(1.0f, 1.0f, 1.0f, 1.0f);
	glEnable(GL_DEPTH_TEST);
}

void init(void)
{
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
			2.0*(float)h / (float)w, -10.0, 10.0);
	else
		glOrtho(-2.0*(float)w / (float)h, 2.0*(float)w / (float)h,
			-2.0, 2.0, -10.0, 10.0);
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
								//	glutKeyboardFunc(keyboard);//Ű���� �̺�Ʈ �ݹ�

	init();
	SetupRC();
	glutMainLoop();
	//�⺻���� Ʋ������ �ܿ��ʿ���� ���迡�ȳ��´� ���躼�� �ش�.
}
