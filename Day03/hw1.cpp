#include<WIndows.h>
#include<gl/GL.h>
#include<gl/GLU.h>
#include<gl/glut.h>

#include <math.h>

GLfloat vertices[8][3] = { { -1, -1, 1 },{ -1, 1, 1 },{ 1, 1, 1 },{ 1, -1, 1 },{ -1, -1, -1 },{ -1, 1, -1 },{ 1, 1, -1 },{ 1, -1, -1 } };
GLfloat colors[8][3] = { { 0, 0, 1 },{ 0, 1, 1 },{ 1, 1, 1 },{ 1, 0, 1 },{ 0, 0, 0 },{ 0, 1, 0 },{ 1, 1, 0 },{ 1, 0, 0 } };

static float theta;
static bool mouseLeftDown;
static char rotationAxisMode;

void quad(int a, int b, int c, int d)
{
	glColor3fv(colors[a]); glVertex3fv(vertices[a]);
	glColor3fv(colors[b]); glVertex3fv(vertices[b]);
	glColor3fv(colors[c]); glVertex3fv(vertices[c]);
	glColor3fv(colors[d]); glVertex3fv(vertices[d]);
}

void RenderScene(void)
{
	//������ �� �ʱ�ȭ
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	/*
	ī�޷��� ��ü�̵��� �ϱ����ؼ� GL_MODELVIEW�� �̿��ϰ�
	��ü�� ������Ű�� ���ؼ��� GL_PROJECTION ��尡 �ʿ��ϴ�.
	*/
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();

	//A transformation of the world coordinate
	gluLookAt(0.5, 0.5, 0.5, 0, 0, 0, 0, 1, 0);
	/*
	To Obtain a Desired View, Position and Orient the Camera within the World Coordinate System
	Eye point, at point, up vector
	*/


	if (rotationAxisMode == 0)
		glRotatef(theta, 1, 0, 0);//�̰� ���� �;��ϴ������� ����????
	else if (rotationAxisMode == 1)
		glRotatef(theta, 0, 1, 0);//�̰� ���� �;��ϴ������� ����????
	else if (rotationAxisMode == 2)
		glRotatef(theta, 0, 0, 1);//�̰� ���� �;��ϴ������� ����????
	else
		glRotatef(theta, 0, 1, 0);//�̰� ���� �;��ϴ������� ����????


	glBegin(GL_QUADS);
	quad(0, 3, 2, 1);
	quad(2, 3, 7, 6);
	quad(3, 0, 4, 7);
	quad(1, 2, 6, 5);
	quad(4, 5, 6, 7);
	quad(5, 4, 0, 1);
	glEnd();

	glutSwapBuffers();//���� ���� ����ϱ� �����̴�.
}

void mouseButton(int button, int state, int x, int y)
{
	if (button == GLUT_LEFT_BUTTON)
		if (state == GLUT_DOWN)
			(mouseLeftDown == true) ? mouseLeftDown = false : mouseLeftDown = true;
		else if (button = GLUT_RIGHT_BUTTON)
			if (state == GLUT_DOWN)
				(mouseLeftDown == true) ? mouseLeftDown = false : mouseLeftDown = true;

	glutPostRedisplay(); //ȭ�� ������ �ʿ��� �� ���Լ��� �θ��� display �ݹ� �Լ��� �ٽ� �� �� ����
}

void keyboard(unsigned char key, int x, int y)
{
	switch (key)
	{
	case 'q' | 'Q':
		exit(0); break; //���� ��ų������
	case 'x' | 'X':
		rotationAxisMode = 0;
		break;
	case 'y' | 'X':
		rotationAxisMode = 1;
		break;
	case 'z' | 'X':
		rotationAxisMode = 2;
		break;
	default:
		break;
	}
}


void SetupRC(void) {
	glClearColor(1.0f, 1.0f, 1.0f, 1.0f);
	glEnable(GL_DEPTH_TEST);
}

void ChangeSize(int w, int h)
{
	if (h == 0)// 0���� ���������°� �������ؼ�
		h = 1;

	glViewport(0, 0, w, h);

	glMatrixMode(GL_PROJECTION);//����� �����ؾ� �ϴ°Ŷ� �̷����ؾ���
	glLoadIdentity();//��ǥ�� �ʱ�ȭ, ó���� �̷�����������Ѵ�.

	if (w <= h)
		glOrtho(-2.0, 2.0, -2.0*(float)h / (float)w,
			2.0*(float)h / (float)w, -10.0, 10.0);//���� 10���� �ٲ۰� Ŭ�����ϱ����ؼ��̴�.
	else
		glOrtho(-2.0*(float)w / (float)h, 2.0*(float)w / (float)h, -2.0,
			2.0, -10.0, 10.0);
}


void timer(int value)
{
	(mouseLeftDown) ? theta -= 2 : theta += 2;

	if (theta >= 360.0)
		theta -= 360.0;

	if (theta <= -360.0)
		theta += 360.0;

	glutPostRedisplay();
	glutTimerFunc(1000 / 30, timer, 1);
}

void init(void)
{
	theta = 0.0f;
	rotationAxisMode = 1;
	mouseLeftDown = false;
	glutTimerFunc(1000 / 30, timer, 1);//Ÿ�̸� ù����
}

void main(int argc, char * argv[])
{
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB);
	glutInitWindowSize(600, 600);
	glutCreateWindow("Simple");

	glutDisplayFunc(RenderScene);
	glutReshapeFunc(ChangeSize);//�� �߰�������Ѵ�. �׷����� �̰͵� ���� ȣ���� �Ǳ� ������ ��
	glutMouseFunc(mouseButton);
	glutKeyboardFunc(keyboard);

	init();
	SetupRC();

	glutMainLoop();//�̰� ���� ��������
}
