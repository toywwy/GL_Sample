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


	float a=1;


	glPolygonMode(GL_FRONT_AND_BACK,GL_LINE);
	glLineWidth(4);
	
	//glPointSize(4);
	glBegin(GL_QUAD_STRIP);
	{
		glColor3f(1, 0, 0);
		glVertex2f(0, 0);
		glVertex2f(0.5, 1);

		glVertex2f(1, 0);
		glVertex2f(1.5, 1);

		glVertex2f(2, 0);
		glVertex2f(2.5, 1);

		glVertex2f(3, 0);
		glVertex2f(3.5, 1);

		glVertex2f(4, 0);

		
	}
	glEnd();

	glFlush();//������ ���ߴ� ���� �׷��� �ȴ�.
}
void SetupRC(void) { glClearColor(1.0f, 1.0f, 1.0f, 1.0f); }

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

void init(void)
{
}
void main(int argc, char * argv[])
{
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_SINGLE | GLUT_RGB);
	glutInitWindowSize(1500, 600);

	glutCreateWindow("Simple");
	glutDisplayFunc(RenderScene);
	glutReshapeFunc(ChangeSize);//������ �ٲ���� ȭ���� �÷������� ������ ����.

	SetupRC();
	glutMainLoop();
}
