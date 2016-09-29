
/*
* windows.h - Master include file for Windowsapplication
* gl.h (OpenGL Core Library) - ������ ����� �����ϴ� ���̺귯��
* glu.h (OpenGL Utility Library) - GL ���̺귯���� ����� ����
* glut.h (OpenGL Utility Toolkit) - ����� �Է��� �޾Ƶ��̰ų� ȭ�� �����츦 �����ϱ� ���� ���̺귯��
GL(OpenGL Core Library)
������ ����� �����ϴ� �Լ�.  ���̺귯���� GL�� ���� ������ ��ɸ� ������ �� ������� �Է��� �޴� ��ɰ� ����ϴ� ����� ����.

GLU(OpenGL Utility Library)
GL �Լ��� �ۼ��Ǿ� �ִ� ��ޱ���� �����ϴ� �Լ����� ����

GLUT(OpenGL Utility Toolkit)
������� �Է��� �ްų� ȭ�� �����츦 �����ϱ� ���� �Լ���� �ü���� ȣ���Ѵ�.


�տ� gl �� �ٴ� ���� �������� ���̴� ���̰�

wgló�� gl �տ� �ٸ� ���� �ٴ� ��� Ư�� ȯ�濡 ���� ���ӻ��� �ִ� ���̴�.


--


*/

#include<WIndows.h>
#include<gl/GL.h>
#include<gl/GLU.h>
#include<gl/glut.h>

/*
GLUT �������� MFC���� �߾�����ߴµ� ������ glut�� �̿��ؼ� �����츦 ��� ���̴�.
���� �۾��ϱⰡ ����������
*/
void RenderScene(void)
{
	//Clear the window with current clearing color
	glClear(GL_COLOR_BUFFER_BIT); //������ ������ �����.
	glColor3f(1, 0, 0);// ������

	glBegin(GL_POLYGON);
	glVertex2f(-50, -50);
	glVertex2f(0, -50);
	glVertex2f(0, 0);
	glVertex2f(-50, 0);
	glEnd();

	glFlush();//flush �ϸ鼭 ���ۿ� �ִ°� �׸��°Ŵ�.
}

void SetupRC(void)
{
	glClearColor(1.0f, 1.0f, 1.0f, 1.0f);
	//���÷��� �Լ��� �ֱ������� ��ӱ׸��°Ŵ� �̱۹��ۿ��ٰ�,
	//�׷��� ��� �׸��� �������ϴµ� �׸� �����̴µ� �ڸ��� �ʱ�ȭ ���ϸ� �ܻ��� ���� ������ �ʱ�ȭ������Ѵ�.
	//���� ..���Ѱ� �ص��ȴ�.
}

void ChangeSize(int w, int h)
{
	GLfloat aspectRatio; //����

	if (h == 0)
		h = 1;

	glViewport(0, 0, w, h); //����Ʈ�� ���״�� ȭ�鿡 �����ִ� �� ȭ���ΰŴ�.
	glMatrixMode(GL_PROJECTION); //������ ���� ����� ��ȯ�ϴ� �κ�
	glLoadIdentity();//��� ó���� �̷���������� ��ǥ�踦 �ʱ�ȭ�Ѵ�.

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

	glMatrixMode(GL_MODELVIEW); //�̷��� ���������� �̷��� �����ϴµ�...
	glLoadIdentity();
}

void main(int argc, char * argv[])
{
	glutInit(&argc, argv);//�ݵ�� ��� �Ѵٰ� �����.

	glutInitDisplayMode(GLUT_SINGLE | GLUT_RGB);//�̱� ���� ������, ������� RGB�� �̿��Ѵ�.
	glutInitWindowSize(600, 600);//������ ������ ó����

	glutCreateWindow("Simple");
	glutDisplayFunc(RenderScene);//�̺κ��� �ݹ��� ȣ���ϰ� �Ǵ°��̴�.
	glutReshapeFunc(ChangeSize);//������ �ٲ���� ȭ���� �÷������� ����������.
								//���ο� â�� �ʺ�� ���̸� �޾Ƽ� ��ǥ��� ȭ����ǥ�� ���ο� �̿�ȴ�.

	SetupRC();
	glutMainLoop();//void RenderScene(void) �̺κ��� �ݺ������� �����ϴ°� �ƴұ�????
				   //glut framework�� �����Ѵ�.
				   //GLUT GL��ƾ

				   //�⺻���� Ʋ������ �ܿ��ʿ���� ���迡�ȳ��´� ���躼�� �ش�.
}
/*
* void glutInit(int *argc, char **argv) - GLUT ���̺귯���� �ʱ�ȭ �� �� ������ �ý��۰� ������ ����.
* void glutInitDisplayMode(unsigned int mode) - ������ �������� ���÷��� ��带 ����
* void glutInitWindowSize(int width, int height) - ������ ũ�� ����
* void glutCreateWindow(const char *tilte) - ���ο� ������ ����
* void glutDisplayFunc(void (*func)(void)) - ȭ�� ���÷��� �̺�Ʈ�� �߻� ���� �� ����� �ݹ� ��� �Լ�
* void glutReshapeFunc(void (*func)(int, int)) - ������ ũ�Ⱑ ����Ǿ��� �� �߻� ���� �� ����� �ݹ� ��� �Լ�
* void SetupRC() / void Init() - �ʱ�ȭ �Լ��μ� ���������� ��� (������ ����)
* glutMainLoop() - GLUT, GL ��ƾ
*/

