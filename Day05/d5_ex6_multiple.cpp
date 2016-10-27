#include<iostream>
#include<WIndows.h>
//#include<gl/gL.h>
//#include<gl/gLU.h> //�׷��� �տ��� �빮�ڷ� �ᵵ �ҹ��ڷ� �ᵵ ����� �����ǰ�???
#include<gl/glut.h>//�� �̰͸� ������ �Ǵ°ǰ�?

#include <math.h>

using namespace std;

GLUquadricObj *quadricObj;//��� �׸��� ����ϴ� ������ ���ߵ� ���� �Ǹ��� �׸��� ���

static float angle = 5.0f;

bool baseToggle[2];
bool lowerToggle[2];
bool upperToggle[2];

static float theta[3];

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

	gluLookAt(0, 2.0, 2.0, 0, 0, 0, 0, 1.0, 0);// ���鿡��������
											   //gluLookAt(0, 0, 2.0, 0, 0, 0, 0, 1.0, 0);// ���鿡��������
											   //gluLookAt(1, 1, 2.0, 0, 0, 0, 0, 1.0, 0);// �ణ������ ����.


											   /*glRotatef(theta[2], 0, 0, 1);
											   glRotatef(theta[1], 0, 1, 0);
											   glRotatef(theta[0], 1, 0, 0);*/

//------------------------------------------------------------
//GL_LIGHT0

	glEnable(GL_LIGHTING);
	glEnable(GL_LIGHT0);

	//GLfloat position0[4] = { 2.0f,2.0f,2.0f,1.0f };
	//GLfloat ambient0[4] = { 0.1f,0.1f,0.1f,1.0f };
	//GLfloat diffuse0[4] = { 1.0f,1.0f,0.0f,1.0f };
	//GLfloat specular0[4] = { 1.0f ,1.0f ,1.0f ,1.0f };

	//glLightfv(GL_LIGHT0, GL_POSITION, position0);
	//glLightfv(GL_LIGHT0, GL_AMBIENT, ambient0);
	//glLightfv(GL_LIGHT0, GL_DIFFUSE, diffuse0);
	//glLightfv(GL_LIGHT0, GL_SPECULAR, specular0);

	//////���� ���� 1~0 ���� ���� ��� ...���谡 �Ǵ°� ...
	//////glLightf(GL_LIGHT0, GL_CONSTANT_ATTENUATION, 0.2);
	//////glLightf(GL_LIGHT0, GL_LINEAR_ATTENUATION, 0.1);
	//////glLightf(GL_LIGHT0, GL_QUADRATIC_ATTENUATION, 0.05);


	//GLfloat spot_direction[3] = { -1,-1 ,-1 };
	//glLightfv(GL_LIGHT0, GL_SPOT_DIRECTION, spot_direction);
	//glLightf(GL_LIGHT0, GL_SPOT_CUTOFF, 15.0);
	//glLightf(GL_LIGHT0, GL_SPOT_EXPONENT, 100.0);//�̷��� �θ�..

	//------------------------------------------------------------
	//GL_LIGHT1


	//glEnable(GL_LIGHT1);
	//GLfloat position1[4] = { -2,-2,-2,1 };
	//GLfloat ambient1[4] = { 0.1,0.1,0.1,1 };
	//GLfloat diffuse1[4] = { 1,1,1,1 };
	//GLfloat specular1[4] = { 1,1,1,1 };

	//glLightfv(GL_LIGHT1, GL_POSITION, position1);
	//glLightfv(GL_LIGHT1, GL_AMBIENT, ambient1);
	//glLightfv(GL_LIGHT1, GL_DIFFUSE, diffuse1);
	//glLightfv(GL_LIGHT1, GL_SPECULAR, specular1);


	//------------------------------------------------------------
	//Matirial

	////�ݻ縦 ���ؼ� ���� ����� ���̴�....
	//GLfloat mat_ambient[4] = { 0.3,0.0,0.0,1.0 };
	//GLfloat mat_diffuse[4] = { 1,0.5,0.5,1.0 };
	//GLfloat mat_specular[4] = { 1,1,1,1 };
	//GLfloat mat_shininess = 100.0;


	////Material ��Ƽ���󿡴� GL_FORNT, GL_BACK ,GL_FRONT_AND_BACK�� �����Ѵ�. ����
	////��Ƽ������ specifying�Ѵ�.
	//glMaterialfv(GL_FRONT, GL_AMBIENT, mat_ambient);
	//glMaterialfv(GL_FRONT, GL_DIFFUSE, mat_diffuse);
	//glMaterialfv(GL_FRONT, GL_SPECULAR, mat_specular);
	//glMaterialf(GL_FRONT, GL_SHININESS, mat_shininess);


	//�굵 Light�� Enable �Ƚ����ָ� ��ü�� ����...������ ����???

	//glEnable(GL_LIGHTING);
	//glEnable(GL_LIGHT0);//�곻 �Ѵ� �־����.
	//����ϳ� ...GL_COLOR_MATERIAL�� �ϸ� color�� ���� ����Ҽ����ְԵȴ�.
	
	glColorMaterial(GL_FRONT, GL_AMBIENT_AND_DIFFUSE);
	glEnable(GL_COLOR_MATERIAL);



	GLfloat mat_specular[4] = { 1,1,1,1 };
	GLfloat mat_shininess = 25.0;
	glMaterialfv(GL_FRONT, GL_SPECULAR, mat_specular);//�ݻ縦���� ��¦�̴°� ����
	glMaterialf(GL_FRONT, GL_SHININESS, mat_shininess);//��� �׳� ���ε�?

	/*glPushMatrix();
	{
		glTranslatef(-1.3, 0, 0);
		glColor3f(1, 0, 0);
		glutSolidTeapot(0.4);
	}
	glPopMatrix();


	glColor3f(0, 1, 0);
	glutSolidTeapot(0.4);


	glTranslatef(1.3, 0, 0);
	glColor3f(0, 0, 1);
	glutSolidTeapot(0.4);*/


	glPushMatrix();
	{
		glTranslatef(-1.3, 0, 0);
		glColor3f(1, 0, 0);
		glutSolidTeapot(0.4);
	}
	glPopMatrix();


	glPushMatrix();
	{
		glTranslatef(0, 0, 0);
		glColor3f(0, 1, 0);
		glutSolidTeapot(0.4);
	}
	glPopMatrix();

	glPushMatrix();
	{
		glTranslatef(1.3, 0, 0);
		glColor3f(0, 0, 1);
		glutSolidTeapot(0.4);
	}
	glPopMatrix();


	glutSwapBuffers(); //GLUT_DOUBLE ���۸� ������ؼ� �����ؼ������ ���̴�.
}

void SetupRC(void) {
	glClearColor(1.0f, 1.0f, 1.0f, 1.0f);
	glEnable(GL_DEPTH_TEST);
}

void init(void)
{

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
	glutDisplayFunc(RenderScene);//�������Ǵ� �κ�
	glutReshapeFunc(ChangeSize);//SIZE�� �ٲ𸶴� ȣ���� �ȴ�.
								//	glutSpecialFunc(specialKeyboard);//����� Ű���� �̺�Ʈ�� �ݹ�
								//	glutKeyboardFunc(keyboard);//Ű���� �̺�Ʈ �ݹ�
	init();
	SetupRC();
	glutMainLoop();
	//�⺻���� Ʋ������ �ܿ��ʿ���� ���迡�ȳ��´� ���躼�� �ش�.
}
