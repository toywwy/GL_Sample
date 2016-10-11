#include<WIndows.h>
#include<gl/gL.h>
#include<gl/gLU.h>
#include<gl/glut.h>


GLfloat vertices[8][3] = { { -1, -1, 1 },{ -1, 1, 1 },{ 1, 1, 1 },{ 1, -1, 1 },{ -1, -1, -1 },{ -1, 1, -1 },{ 1, 1, -1 },{ 1, -1, -1 } };
GLfloat colors[8][3] = { { 0, 0, 1 },{ 0, 1, 1 },{ 1, 1, 1 },{ 1, 0, 1 },{ 0, 0, 0 },{ 0, 1, 0 },{ 1, 1, 0 },{ 1, 0, 0 } };

void quad(char a, char b, char c, char d)
{
	glColor3fv(colors[a]); glVertex3fv(vertices[a]);
	glColor3fv(colors[b]); glVertex3fv(vertices[b]);
	glColor3fv(colors[c]); glVertex3fv(vertices[c]);
	glColor3fv(colors[d]); glVertex3fv(vertices[d]);
}
void RenderScene(void)
{
	//Clear the window with current clearing color
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT); //������ ������ �����.

	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();

	gluLookAt(0.5, 0.5, 0.5, 0, 0, 0, 0, 1, 0);
	glPushMatrix();//Ǫ�� �ߴ��� �� �־��µ� ����� �߾ȳ���. ������ �������ִ°ǰ�???
	{
		glTranslatef(0.0f, -1.0f, 0.0f); //y������ -1.0f �̵� //�Ʒ��� �̵��Ѵ�.
		glColor3f(1.0f, 0.0f, 0.0f);
		glutWireTetrahedron();
	}
	glPopMatrix(); //Ǫ���ߴ��� �ٽ� �����ִ��ǹ� ���� -1.0 ��ŭ y������ �̵����ִ� ��ǥ�踦 �ٽ� ���ؼ� 0,0,0�� ���·� ������ִ°���
	//push pop�� �뵵�� �����ϱ�???
	glPushMatrix();
	{
	glTranslatef(0.0f,0.0f, 0.0f);//y������ 1 ���� �̵��Ѵ�.
	glColor3f(0.0f, 1.0f, 0.0f);
	glutWireCube(0.5);
	}
	glPopMatrix();//�̰͵� �ٽ� �����༭ �������� �����.

	glutSwapBuffers(); //DETH_BUFFER�� �̿��ߴ��� ������
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

	SetupRC();
	glutMainLoop();
	//�⺻���� Ʋ������ �ܿ��ʿ���� ���迡�ȳ��´� ���躼�� �ش�.
}
