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


using namespace std;


static float viewer[3];


void keyboard(unsigned char c, int x, int y) {
	switch (c)
	{
	case 'z':
	case 'Z':
		viewer[0] -= 0.1;
		break;
	case 'c':
	case 'C':
		viewer[0] += 0.1;
		break;
	case 's':
	case 'S':
		viewer[1] -= 0.1;
		break;
	case 'x':
	case 'X':
		viewer[1] += 0.1;
		break;
	case 'a':
	case 'A':
		viewer[2] -= 0.1;
		break;
	case 'd':
	case 'D':
		viewer[2] += 0.1;
		break;
	case VK_ESCAPE:
		exit(0);
		break;
	}

	
	glutPostRedisplay();
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

	gluLookAt(viewer[0], viewer[1], viewer[2], 0, 0, 0, 0, 1, 0); //���� �����̴�.
											//ó���� ��, at�� ����κ��� �����ΰ� �̱⶧���� ���� �Ȱǵ鿩..
											//up�� ���� ���ϴ°Ŵ�...��������� �����ΰ�.

	//viewer x y z ���� 0,0,0 �� y���� ������ �ٶ� ���� ���̴�.
	//�̰� glu LookAt�̴�.


	GLfloat vertices[8][3] = { { -1, -1, 1 },{ -1, 1, 1 },{ 1, 1, 1 },{ 1, -1, 1 },{ -1, -1, -1 },{ -1, 1, -1 },{ 1, 1, -1 },{ 1, -1, -1 } };
	GLfloat colors[8][3] = { { 0, 0, 1 },{ 0, 1, 1 },{ 1, 1, 1 },{ 1, 0, 1 },{ 0, 0, 0 },{ 0, 1, 0 },{ 1, 1, 0 },{ 1, 0, 0 } };

	glBegin(GL_QUADS);


	glColor3fv(colors[0]); glVertex3fv(vertices[0]);
	glColor3fv(colors[3]); glVertex3fv(vertices[3]);
	glColor3fv(colors[2]); glVertex3fv(vertices[2]);
	glColor3fv(colors[1]); glVertex3fv(vertices[1]);



	glColor3fv(colors[2]); glVertex3fv(vertices[2]);
	glColor3fv(colors[3]); glVertex3fv(vertices[3]);
	glColor3fv(colors[7]); glVertex3fv(vertices[7]);
	glColor3fv(colors[6]); glVertex3fv(vertices[6]);


	glColor3fv(colors[3]); glVertex3fv(vertices[3]);
	glColor3fv(colors[0]); glVertex3fv(vertices[0]);
	glColor3fv(colors[4]); glVertex3fv(vertices[4]);
	glColor3fv(colors[7]); glVertex3fv(vertices[7]);


	glColor3fv(colors[1]); glVertex3fv(vertices[1]);
	glColor3fv(colors[2]); glVertex3fv(vertices[2]);
	glColor3fv(colors[6]); glVertex3fv(vertices[6]);
	glColor3fv(colors[5]); glVertex3fv(vertices[5]);



	glColor3fv(colors[4]); glVertex3fv(vertices[4]);
	glColor3fv(colors[5]); glVertex3fv(vertices[5]);
	glColor3fv(colors[6]); glVertex3fv(vertices[6]);
	glColor3fv(colors[7]); glVertex3fv(vertices[7]);


	glColor3fv(colors[5]); glVertex3fv(vertices[5]);
	glColor3fv(colors[4]); glVertex3fv(vertices[4]);
	glColor3fv(colors[0]); glVertex3fv(vertices[0]);
	glColor3fv(colors[1]); glVertex3fv(vertices[1]);


	glEnd();


	glutSwapBuffers(); //GLUT_DOUBLE ���۸� ������ؼ� �����ؼ������ ���̴�.
}

void SetupRC(void) {
	glClearColor(1.0f, 1.0f, 1.0f, 1.0f);
	glEnable(GL_DEPTH_TEST);
}

void init(void)
{
	glutTimerFunc(1000 / 30, timer, 1);
	viewer[0] = 1.5;
	viewer[1] = 1.5;
	viewer[2] = 1.5;
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
		glFrustum(-2.0, 2.0, -2.0*(float)h / (float)w,
			2.0*(float)h / (float)w, 1, 10.0);
	else
		glFrustum(-2.0*(float)w / (float)h, 2.0*(float)w / (float)h,
			-2.0, 2.0, 1.0, 10.0);
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
	glutKeyboardFunc(keyboard);//Ű���� �̺�Ʈ �ݹ�

								//453, 948 //453���� ���� 948������ �ﰢ������ ������ ���ִ�.....
								//�� ���� ������ ���̴� �����ΰ�???
								//

								//�Է¿Ϸ�



	init();
	SetupRC();
	glutMainLoop();
	//�⺻���� Ʋ������ �ܿ��ʿ���� ���迡�ȳ��´� ���躼�� �ش�.
}
