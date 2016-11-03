#include<iostream>
#include<Windows.h>
#include<gl/gL.h>
#include<gl/gLU.h>
#include<gl/glut.h>

#include <math.h>

using namespace std;

GLUquadricObj *quadricObj;//��� �׸��� ����ϴ� ������ ���ߵ� ���� �Ǹ��� �׸��� ���
float r[20];
float colors[20][3];

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

	gluLookAt(2, 2, 2, 0, 0, 0, 0, 1.0, 0);// ���鿡��������

	glEnable(GL_LIGHTING);
	glEnable(GL_LIGHT0);

	glColorMaterial(GL_FRONT, GL_AMBIENT_AND_DIFFUSE);
	glEnable(GL_COLOR_MATERIAL);

	GLfloat mat_specular[4] = { 1,1,1,1 };
	GLfloat mat_shininess = 25.0;
	glMaterialfv(GL_FRONT, GL_SPECULAR, mat_specular);
	glMaterialf(GL_FRONT, GL_SHININESS, mat_shininess);

	float dy[] = { -2.0,-1.0,0,1.0,2.0 };
	float d[] = { -2.0,-1.0,0,1.0 };
	int idx = 0;

	for (int i = 0; i < 5; i++)
	{
		for (int j = 0; j < 4; j++)
		{
			glPushMatrix();
			{
				glTranslatef(d[j], dy[i], 0);
				//�̵��� ��ġ���� ȸ���� �ؾ��Ѵ�.
				if (i == 0)
					glRotatef(r[idx], 1, 0, 0);
				else if (i == 1)
					glRotatef(r[idx], 0, 1, 0);
				else if (i == 2)
					glRotatef(r[idx], 0, 0, 1);
				else if (i == 3)
					glRotatef(r[idx], 0, 1, 0);
				else
					glRotatef(r[idx], 0, 0, 1);

				glColor3fv(colors[idx]);

				glutSolidTeapot(0.3);//�������� �ؾ��Ѵ�.
				r[idx++] += 2.0;
			}
			glPopMatrix();
		}
	}

	glutSwapBuffers(); //GLUT_DOUBLE ���۸� ������ؼ� �����ؼ������ ���̴�.
}

void SetupRC(void) {
	glClearColor(1.0f, 1.0f, 1.0f, 1.0f);
	glEnable(GL_DEPTH_TEST);
}

void init(void)
{

	for (int i = 0; i < 20; i++)
	{
		for (int j = 0; j < 3; j++)
		{
			float a = rand() % 10;
			colors[i][j] = a / 10.0;
		}
	}


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
	glutInitWindowSize(800, 600);
	glutCreateWindow("Simple");
	glutDisplayFunc(RenderScene);//�������Ǵ� �κ�
	glutReshapeFunc(ChangeSize);//SIZE�� �ٲ𸶴� ȣ���� �ȴ�.
								//	glutSpecialFunc(specialKeyboard);//����� Ű���� �̺�Ʈ�� �ݹ�
								//	glutKeyboardFunc(keyboard);//Ű���� �̺�Ʈ �ݹ�
	init();
	SetupRC();
	glutMainLoop();
}
