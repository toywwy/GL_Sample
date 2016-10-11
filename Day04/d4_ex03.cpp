#include<WIndows.h>
#include<gl/gL.h>
#include<gl/gLU.h>
#include<gl/glut.h>

#include <math.h>

GLUquadricObj *p;

void base(void)
{
	glPushMatrix();
	{
		glColor3f(1, 0, 0);
		glRotatef(-90, 1, 0, 0);
		gluCylinder(p, 0.5, 0.5, 0.3, 20, 1);
	}
	glPopMatrix();
}

void lower_arm(void)
{
	glPushMatrix();
	{
		glColor3f(0, 1, 0);
		glTranslatef(0, 0.5, 0);
		glScalef(0.2, 1, 0.2);
		glutWireCube(1);
	}
	glPopMatrix();
}

void upper_arm(void)
{
	glPushMatrix();
	{
		glColor3f(0, 0, 1);
		glTranslatef(0, 0.4, 0);
		glScalef(0.2, 0.8, 0.2);
		glutWireCube(1);
	}
	glPopMatrix();
}

void init(void)
{
	p = gluNewQuadric();
	gluQuadricDrawStyle(p, GLU_LINE);
}

void keyboard(unsigned char c, int x, int y)
{
	switch (c)
	{
	case VK_ESCAPE:
		gluDeleteQuadric(p);
		exit(0);
	}
}
void RenderScene(void)
{
	//Clear the window with current clearing color
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT); //������ ������ �����.

	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();

	gluLookAt(0.5, 0.5, 0.5, 0, 0, 0, 0, 1, 0);

	base();
	lower_arm();
	upper_arm();

	glutSwapBuffers();
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

	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();

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

	init();
	SetupRC();
	glutMainLoop();
	//�⺻���� Ʋ������ �ܿ��ʿ���� ���迡�ȳ��´� ���躼�� �ش�.
}
