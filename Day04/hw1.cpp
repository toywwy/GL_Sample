#include<iostream>
#include<WIndows.h>
#include<gl/gL.h>
#include<gl/gLU.h>
#include<gl/glut.h>

#include <math.h>

using namespace std;

GLUquadricObj *quadricObj;//��� �׸��� ����ϴ� ������ ���ߵ� ���� �Ǹ��� �׸��� ���

static float baseTheta;
static float lowerArmTheta;
static float upperArmTheta;

static float angle = 5.0f;

bool baseToggle[2];
bool lowerToggle[2];
bool upperToggle[2];


void base(void)
{
	if (baseToggle[0])
	{
		if (baseToggle[1])//left
			baseTheta -= angle;
		else//right key
			baseTheta += angle;

		baseToggle[0] = false;
	}

	glRotatef(baseTheta, 0, 1, 0);//�ٽ� pop �� �ʿ� ����. ��ü�� �� ���ƾ� �ϱ� ������

	glPushMatrix();
	{	
		glColor3f(1, 0, 0);
		glRotatef(-90, 1, 0, 0);
		gluCylinder(quadricObj, 0.5, 0.5, 0.3, 20, 1); 
		// ���� �׸��°� ���׸��� quadricObj�� �̿��ϴ� �� ����.
	}
	glPopMatrix();
}

void lower_arm(void)
{

	if (lowerToggle[0])
	{
		if (lowerToggle[1])//left
			lowerArmTheta += angle;
		else//right key
			lowerArmTheta -= angle;

		lowerToggle[0] = false;
	}

	glPushMatrix();
	{	
		glRotatef(lowerArmTheta, 1, 0, 0);
		glColor3f(0, 1, 0);
		glTranslatef(0, 0.7, 0);
		glScalef(0.2, 1, 0.2);
		glutWireCube(1);
	}
	glPopMatrix();

}

void upper_arm(void)
{

	if (upperToggle[0])
	{
		if (upperToggle[1])//left
			upperArmTheta += angle;
		else//right key
			upperArmTheta -= angle;
		upperToggle[0] = false;
	}
	
	glPushMatrix();
	{
		glRotatef(lowerArmTheta, 11, 0, 0);// lowerArmTheta�� ȸ���� �κб��� ȸ���� �Ŀ� �������� ȸ���Ѵ�.
		glTranslatef(0, 1.0, 0); //�׸��� �ٽ� ������ �÷���
		glRotatef(upperArmTheta, 11, 0, 0);// �������� ���� upper���� ȸ���� �κ�
		glTranslatef(0, 0.6, 0);//�׸��� ������ �߽����� �������ؼ�
		glColor3f(0, 0, 1);
		glScalef(0.2, 0.8, 0.2);
		glutWireCube(1);
	}
	glPopMatrix();
}



void keyboard(unsigned char c, int x, int y)
{
	switch (c)
	{
	case VK_ESCAPE:
		gluDeleteQuadric(quadricObj);
		exit(0);
		break;
	}
}

void specialKeyboard(int c, int x, int y)
{
	switch (c)
	{
	case GLUT_KEY_HOME:
		upperToggle[0] = true;
		upperToggle[1] = true;
		break;
	case GLUT_KEY_END:
		upperToggle[0] = true;
		upperToggle[1] = false;
		break;
	case GLUT_KEY_LEFT:
		baseToggle[0] = true;
		baseToggle[1] = true;

		break;
	case GLUT_KEY_RIGHT:
		baseToggle[0] = true;
		baseToggle[1] = false;
		break;
	case GLUT_KEY_UP:
		lowerToggle[0] = true;
		lowerToggle[1] = true;
		break;
	case GLUT_KEY_DOWN:
		lowerToggle[0] = true;
		lowerToggle[1] = false;
		break;
	}

}

void timer(int value)
{
	glutPostRedisplay();//ȭ�� ������ �ʿ��� �� ���Լ��� �θ��� display �ݹ� �Լ��� �ٽ� �� �� ����
	glutTimerFunc(1000 / 30, timer, 1);//���̸� �ð� ���� ����,  ���ش� //30 frame ���´�.
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

	gluLookAt(0.5, 0.5, 0.5, 0, 0, 0, 0, 1, 0);

	base();//�Ǹ���
	lower_arm();
	upper_arm();//lower_arm�� ����ϰ� �׷�����.

	glutSwapBuffers(); //GLUT_DOUBLE ���۸� ������ؼ� �����ؼ������ ���̴�.
}

void SetupRC(void) {
	glClearColor(1.0f, 1.0f, 1.0f, 1.0f);
	glEnable(GL_DEPTH_TEST);
}

void init(void)
{
	//�ʱ�ȭ
	baseTheta=0.0f;
	lowerArmTheta=0.0f;
	upperArmTheta=0.0f;

	quadricObj = gluNewQuadric();
	gluQuadricDrawStyle(quadricObj, GLU_LINE);
	glutTimerFunc(1000 / 30, timer, 1);
}

void ChangeSize(int w, int h)
{
	if (h == 0)// 0���� ���������°� �������ؼ�
		h = 1;

	glViewport(0, 0, w, h);

	glMatrixMode(GL_PROJECTION); //�̺κ��� PROJECTION�� ������
	glLoadIdentity();//��ǥ�� �ʱ�ȭ

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
	glutDisplayFunc(RenderScene);//������ �ٲ���� ȭ���� �÷������� ������ ����.
	glutReshapeFunc(ChangeSize);//�� �߰�������Ѵ�.
	
	glutSpecialFunc(specialKeyboard);
	glutKeyboardFunc(keyboard);

	init();
	SetupRC();
	glutMainLoop();
	//�⺻���� Ʋ������ �ܿ��ʿ���� ���迡�ȳ��´� ���躼�� �ش�.
}
