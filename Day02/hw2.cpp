#include<WIndows.h>
#include<gl/GL.h>
#include<gl/GLU.h>
#include<gl/glut.h>

static bool mouseLeftDown; //���콺 ��ư�� ���� Flag ���̴�.
static float point[3][3]; // ���� ���ؼ� ������ �ʿ���ִ�. 2 x 2��. 4���� ���� �Ҽ��ְ���...

static int idx = 0;// 0,1,2
static bool isDraw;

void RenderScene(void)
{
	glClear(GL_COLOR_BUFFER_BIT);
	glEnable(GL_LINE_STIPPLE);

	if (mouseLeftDown)
		glLineStipple(3, 0xcccc); //��������
	else
		glDisable(GL_LINE_STIPPLE); // �������� ���°� ����Ѵ�.

	glPointSize(10);
	glEnable(GL_POINT_SMOOTH);
	glLineWidth(2);

	if (isDraw)
	{
		glColor3f(1, 0, 0);
		glBegin(GL_POINTS);
		{
			for (int i = 0; i<3; i++)
				glVertex2fv(point[i]);
		}
		glEnd();

		glColor3f(0, 0, 1);
		glBegin(GL_TRIANGLES);
		{
			glVertex2fv(point[0]); //point ���� ��ȭ�ϴ� ���̴�. �׸��� ���� �׷��ִ� ������
			glVertex2fv(point[1]);
			glVertex2fv(point[2]);
		}
		glEnd();

	}
	else
	{
		glColor3f(1, 0, 0);
		glBegin(GL_POINTS);
		{
			for (int i = 0; i<idx; i++)
				glVertex2fv(point[i]);
		}
		glEnd();
	}


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

				idx %= 3;
				point[idx][0] = x / (float)viewport[2] * 500;
				point[idx][1] = (viewport[3] - y) / (float)viewport[3] * 500;

				mouseLeftDown = true;

				idx += 1;
				if (idx == 3)
					isDraw = true;
			}
		}
		else if (state == GLUT_UP)
		{
			if (mouseLeftDown)
				mouseLeftDown = false;
		}
	}
	glutPostRedisplay(); //ȭ�� ������ �ʿ��� �� �� �Լ��� �θ��� display �ݹ� �Լ��� �ٽ� �� �� ����
}

void init(void)
{
	mouseLeftDown = false; // �ʱ⿡�� �ȴ��� ������
	isDraw = false;
	idx = 0;
	point[0][0] = 0;
	point[0][1] = 0;
	point[1][0] = 0;
	point[1][1] = 0;
	point[2][0] = 0;
	point[2][1] = 0;
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

	SetupRC();
	glutMainLoop();
}
