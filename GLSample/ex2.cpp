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

	//�׸��� ������ �����ұ�???/
	glPointSize(5);//�̰Թ���/
				   //f
				   //v ���ʹ� 3ĭ¥�� �迭�� �ִ´� ..�׷��� �˾Ƽ� �Ѵ�.
				   //d ����
				   //������ �ʿ� �ϵ�..

				   //glEnable(GL_LINE_STIPPLE);
	glLineWidth(10);
	//glLineStipple(3, 0xcccc);
	glBegin(GL_TRIANGLE_STRIP);//STRIP�� ����Ǳ� ������ �پ������. //Ʈ���̾ޱ۰��� �������˵���
	{
		glColor3f(1, 0, 0); // �������� �ٷ� �ڿ����� ���ý����� �Ϳ� ������ �Ǵ� ���̴�.
		glVertex2f(-50, -50);
		glColor3f(0, 1, 0);
		glVertex2f(0, -50);
		glColor3f(0, 0, 1);
		glVertex2f(0, 0);
		glColor3f(0, 0, 0);
		glVertex2f(-50, 0);
	}
	glEnd();
	glFlush();//���ۿ��ٰ� �׷����� ���⿡�� FLUSH �ϸ� ȭ�鿡 �����°Ŵ�.

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

/*

GLQUADS
glVertex2f(-80, -30);
glVertex2f(-50, -30);
glVertex2f(-50, 0);
glVertex2f(-80, 0);
glVertex2f(-20, -30);
glVertex2f(10, -30);
glVertex2f(10, 0);
glVertex2f(-20, 0);


GL_QUAD_STRIP
glVertex2f(-80, -30);
glVertex2f(-80, 0);
glVertex2f(-50, -30);
glVertex2f(-50, 0);
glVertex2f(-20, -30);
glVertex2f(-20, 0);
glVertex2f(10, -30);
glVertex2f(10, 0);

*/
