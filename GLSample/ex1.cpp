#include<WIndows.h>
#include<gl/GL.h>
#include<gl/GLU.h>
#include<gl/glut.h>
/*
GLUT �������� MFC���� �߾�����ߴµ� �̤Ĥ��� glut�� �̿��ؼ� �����츦 ��� ���̴�.
*/
void RenderScene(void)
{
	glClear(GL_COLOR_BUFFER_BIT);
	glColor3f(1, 0, 0);
	glBegin(GL_POLYGON);
	glVertex2f(-50, -50);
	glVertex2f(0, -50);
	glVertex2f(0, 0);
	glVertex2f(-50, 0);
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
	aspectRatio = (GLfloat)w / (GLfloat)h;
	if (w <= h)
	{
		float windowWidth = 100;
		float windowHeight = 100 / aspectRatio;
		glOrtho(-100.0, 100.0, -windowHeight, windowHeight, 1.0, -1.0);
	}
	else
	{
		float windowWidth = 100 * aspectRatio;
		float windowHeight = 100;
		glOrtho(-windowWidth, windowWidth, -100.0, 100.0, 1.0, -1.0);
	}
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
}

void main(int argc, char * argv[])
{
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_SINGLE | GLUT_RGB);
	glutInitWindowSize(600, 600);
	glutCreateWindow("Simple");
	glutDisplayFunc(RenderScene);
	glutReshapeFunc(ChangeSize);//������ �ٲ���� ȭ���� �÷������� ������ ����.
	SetupRC();
	glutMainLoop();
	//�⺻���� Ʋ������ �ܿ��ʿ���� ���迡�ȳ��´� ���躼�� �ش�.
}


/*
������������ �׳� �ϴ°� �ƴ϶� �ݹ��Լ��� ����ؾ���.*/
