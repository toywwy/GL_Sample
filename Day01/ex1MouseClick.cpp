#include<WIndows.h>
#include<gl/GL.h>
#include<gl/GLU.h>
#include<gl/glut.h>

static bool mouseLeftDown; //���콺 ��ư�� ���� Flag ���̴�.
static float point[2][2]; // ���� ���ؼ� ������ �ʿ���ִ�. 2 x 2��. 4���� ���� �Ҽ��ְ���...


						  /*
						  ȭ�� ��ǥ���� ����
						  ������ : �� ����� ����(0,0)
						  OpenGL : �� �ϴ��� ����(0,0)
						  GLUT�� ������ ȭ�� ��ǥ�踦 ���
						  */

void RenderScene(void)
{
	glClear(GL_COLOR_BUFFER_BIT);
	glEnable(GL_LINE_STIPPLE);

	if (mouseLeftDown)
		glLineStipple(3, 0xcccc); //��������
	else
		glDisable(GL_LINE_STIPPLE); // �������� ���°� ����Ѵ�.

	glColor3f(0, 0, 1);
	glLineWidth(2);

	glBegin(GL_LINES);
	{
		glVertex2fv(point[0]); //point ���� ��ȭ�ϴ� ���̴�. �׸��� ���� �׷��ִ� ������
		glVertex2fv(point[1]);
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
		gluOrtho2D(0, 500, 0, 500 * (float)h / (float)w);
	else
		gluOrtho2D(0, 500 * (float)w / (float)h, 0, 500);

	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
}

//TODO callback mouse Click
void mouseButton(int button, int state, int x, int y)
{
	if (button == GLUT_LEFT_BUTTON) //���콺 ���ʹ�ư ��������
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
	else if (button = GLUT_RIGHT_BUTTON) //������ ��ư ��������
	{
		if (state = GLUT_DOWN)
		{

		}
		else if (state == GLUT_UP)
		{

		}
	}
	glutPostRedisplay(); //ȭ�� ������ �ʿ��� �� �� �Լ��� �θ��� display �ݹ� �Լ��� �ٽ� �� �� ����
}

//TODO Mouse motion Callback
void mouseMotion(int x, int y) //���콺 ��� �ݹ��� �س����� ���� ���ϵ�
{
	if (mouseLeftDown)
	{
		double viewport[4];
		glGetDoublev(GL_VIEWPORT, viewport);
		point[1][0] = x / (float)viewport[2] * 500;
		point[1][1] = (viewport[3] - y) / (float)viewport[3] * 500;
	}
	glutPostRedisplay();
}

void init(void)
{
	mouseLeftDown = false; // �ʱ⿡�� �ȴ��� ������
	point[0][0] = 0;
	point[0][1] = 0;
	point[1][0] = 0;
	point[1][1] = 0;
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
	glutMotionFunc(mouseMotion); //���콺�� �����϶� �ݹ� �߻�

	SetupRC();
	glutMainLoop();
}
