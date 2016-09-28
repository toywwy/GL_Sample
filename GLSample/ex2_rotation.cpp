#include<WIndows.h>
#include<gl/GL.h>
#include<gl/GLU.h>
#include<gl/glut.h>
/*
GLUT �������� MFC���� �߾�����ߴµ� ������ glut�� �̿��ؼ� �����츦 ��� ���̴�.
*/
#include <math.h>
static bool mouseLeftDown;
static float point[2][2];
static float theta;
void RenderScene(void)
{
	glClear(GL_COLOR_BUFFER_BIT);
	float cos_th = cos(theta * 3.14159 / 180.0);
	float sin_th = sin(theta * 3.14159 / 180.0);
	glColor3f(1, 0, 0);
	glBegin(GL_POLYGON);
	glVertex2f(cos_th, sin_th);
	glVertex2f(-sin_th, cos_th);
	glVertex2f(-cos_th, sin_th);
	glVertex2f(sin_th, -cos_th);
	glEnd();
	glFlush();
}
void SetupRC(void)
{
	glClearColor(1.0f, 1.0f, 1.0f, 1.0f);//���÷��� �Լ��� �ֱ������� ��ӱ׸��°Ŵ� �̱۹��ۿ��ٰ�,
										 //�׷��� �Խ� �׸��� �������ϴµ� �׸� �����̴µ� �ڸ��� �ʱ�ȭ ���ϸ� �ܻ��� ���⋚���� �ʱ�ȭ������Ѵ�.
										 //���� ..���Ѱ� �ص��ȴ�.
}
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
			if (!mouseLeftDown)
			{
				double viewport[4];
				glGetDoublev(GL_VIEWPORT, viewport);
				point[0][0] = x / (float)viewport[2] * 500;
				point[0][1] = (viewport[3] - y) / (float)viewport[3] * 500;
				point[1][0] = point[0][0];
				point[1][1] = point[0][1];

				mouseLeftDown = true;
			}
		}
		else if (state == GLUT_UP)
		{
			if (mouseLeftDown)
				mouseLeftDown = false;
		}
	}
	else if (button = GLUT_RIGHT_BUTTON)
	{
		if (state = GLUT_DOWN)
		{
		}
		else if (state == GLUT_UP)
		{
		}
	}
	glutPostRedisplay(); //ȭ�� ������ �ʿ��� �� ���Լ��� �θ��� display �ݹ� �Լ��� �ٽ� �� �� ����
}

void mouseMotion(int x, int y)
{
	if (mouseLeftDown)//left ��ư�� true�� �����϶� �� ���´�.
	{
		double viewport[4];
		glGetDoublev(GL_VIEWPORT, viewport);
		point[1][0] = x / (float)viewport[2] * 500;
		point[1][1] = (viewport[3] - y) / (float)viewport[3] * 500;
		//���� �׸��°Ŵ�.
	}
	glutPostRedisplay(); //ȭ���� �������ش�.
}

void timer(int value)
{
	theta += 2.0;
	if (theta >= 360.0)
		theta -= 360.0;
	glutTimerFunc(1000 / 30, timer, 1);
	glutPostRedisplay();
}
void init(void)
{
	mouseLeftDown = false;
	point[0][0] = 0;
	point[0][1] = 0;
	point[1][0] = 0;
	point[1][1] = 0;
	theta = 0;
}
void main(int argc, char * argv[])
{
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_SINGLE | GLUT_RGB);
	glutInitWindowSize(600, 600);
	
	glutCreateWindow("Simple");
	glutDisplayFunc(RenderScene);
	glutReshapeFunc(ChangeSize);//������ �ٲ���� ȭ���� �÷������� ������ ����.
	glutMouseFunc(mouseButton); //��ư�� ��������
	glutMotionFunc(mouseMotion); //���� �ϋ� �߻��ϴ� �Ͱ���.
	
	timer(1000);
	SetupRC();
	glutMainLoop();
	
	//�⺻���� Ʋ������ �ܿ��ʿ���� ���迡�ȳ��´� ���躼�� �ش�.
}
