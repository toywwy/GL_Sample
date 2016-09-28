/*
������ �׸��� ���� 3���� ���� �ﰢ���� ������ �ƴ�  �� �̻��� ������ ����Ͽ� �ٰ����� �����Ҷ����� ���������δ� �ﰢ�� ���� ������� �׷�����. �ﰢ���� �ƴϸ� ����� ����� ���� �����̴�.

BeginMode
#define GL_POINTS                         0x0000
//�� ������ ��ġ�� ���� ��´�. glPointSize(5.0f); �� ũ�� ũ��

#define GL_LINES                             0x0001
//�� �ΰ��� �ѽ��� �Ǿ �������� ����ȴ�. ¦�� ������ ���õȴ�.

#define GL_LINE_LOOP                   0x0002
//�� ������� ������ �׾��ش��� �������� ������ ������Ѽ� �ݾ��ش�.

#define GL_LINE_STRIP                    0x0003
//�� ������� ������ �׾��ش�. �̵���� ������ �����ٶ� �� �� �ִ�.

#define GL_TRIANGLES                    0x0004
//���� �̾ �ﰢ���� �̾��ش�.   ���ڰ� �ٸ��� �����Ѵ�.

#define GL_TRIANGLE_STRIP           0x0005
//����� �ﰢ���� �׸���.  ������ ���鶧 ������ ���ڿ� ���δ�.

#define GL_TRIANGLE_FAN               0x0006
//ù��° ���� �߽����� �ٸ� ������� ���� ���� ���� �ﰢ���� �׷��ش�.

#define GL_QUADS                            0x0007
//���� 4���� �̿��Ͽ� �簢���� �����.

#define GL_QUAD_STRIP                   0x0008
//�簢������ ����� ������� ������ش�.

#define GL_POLYGON                         0x0009
//���� ������ ������ش�. �ٰ���.

viewport ���� �۾��� �� ���� ������ ���÷��̿� ���� ���ϴ� ũ��� �ѷ��ִ� ������ �۾�.
glViewport(0,0,iWidth, iHeight); -> â�� ���� ���� ũ�⸦ ����ȭ ���״�.
*/

#include<iostream>
#include<WIndows.h>
#include<gl/GL.h>
#include<gl/GLU.h>
#include<gl/glut.h>

using namespace std;

void RenderScene(void)
{
	glClear(GL_COLOR_BUFFER_BIT);

	//�׸��� ������ �����ұ�???/
	glPointSize(5);//�̰Թ���/
				   //f
				   //v ���ʹ� 3ĭ¥�� �迭�� �ִ´� ..�׷��� �˾Ƽ� �Ѵ�.
				   //d ����
				   //������ �ʿ� �ϵ�..

	glEnable(GL_LINE_STIPPLE); //�������� �׷������Ѵ�.
	glPolygonMode(GL_FRONT_AND_BACK, GL_LINE); //�̰��ϸ� �ٸ� �׷�����

	glLineWidth(4); //�β� �����Ѵ�.

	glLineStipple(3, 0xcccc);
	//���� ��� ���� glColor3f�� �����ߴٰ� �ϸ� �Ʒ����� ���� �� ������ �����ȴ�.
	//glColor3f(1,0,0); �̷����ϸ� �׳� Flat Coloring�̵Ǵ°��̰�...�Ʒ��� �����ϸ� ...
	glBegin(GL_TRIANGLE_FAN);
	{//�̷������� ���̽��� �����ټ��ִ�.
		// glColor�� �������� �ٷ� �ڿ����� ���ý����� �Ϳ� ������ �Ǵ� ���̴�.
		// �ᱹ �� 4���� ��� �Ǵ� ���ε�

		glColor3f(1, 0, 0); //�� ����
		glVertex2f(-100, -100); //��ġ ����

		glColor3f(0, 1, 0);
		glVertex2f(0, -50);

		glColor3f(0, 0, 1);
		glVertex2f(0, 0);

		glColor3f(1, 1, 1);
		glVertex2f(-50, 0);
	}
	glEnd();

	glFlush();//���ۿ��ٰ� �׷����� ���⿡�� FLUSH �ϸ� ȭ�鿡 �����°Ŵ�.

}
void SetupRC(void){glClearColor(1.0f, 1.0f, 1.0f, 1.0f);}

void ChangeSize(int w, int h)
{//x,y�� ���� ������ ���̰�
	GLfloat aspectRatio;
	if (h == 0)
		h = 1;

	glViewport(0, 0, w, h);//����� ���°ɷ� ����Ʈ�� ������ ������ ���ش�.
	glMatrixMode(GL_PROJECTION);//3���� �������� ��ü�� 2������ ������Ű�� ������ �ʿ��� matrix�̴�.
	//glMatrixmode(GL_MODELVIEW)�� �ϸ� �� ������ ������ OpenGL ��ɾ�� �ٷ� �� MODELVIEW matrix�� ���õ� operation�� �����Ѵٰ� OpenGL���� �˷��ִ°�
	//(��ü�� ��ġ ��ȯ�̳� ī�޶��� ��ġ ��ȭ)

	glLoadIdentity(); //��ǥ�踦 �ʱ�ȭ�Ѵ�.

	//���� / ����
	aspectRatio = (GLfloat)w / (GLfloat)h; //���ο� ���θ� ���� �־���.
	cout <<"ratio = "<<aspectRatio<<"width = " << w << " , height = " << h << endl;

	if (w <= h) //���ΰ� �� ��ų� ����.
	{
		//���� < 1
		float windowWidth = 100;
		float windowHeight = 100 / aspectRatio; //������ ���ؼ� �ٿ��ִ°ž� �������� �÷����� ���θ� �ٿ��־���Ѵ�.
		glOrtho(-100.0, 100.0, -windowHeight, windowHeight, 1.0, -1.0);
		//z���� ���ְ� �׷��ִ°��̴�. ��鿡�� ���̰� ���翵�Ѵٰ� �����ϸ�ȴ�.

		//1,3,5    2,4,6 �̷��� ���� ���� �Ʒ��� ������ ������ �����ϰ��ִ�.

	}
	else // ���ΰ� �� ����
	{
		//���� > 1
		float windowWidth = 100 * aspectRatio;//������ ���ؼ� �ٿ�����Ѵ�. //���ΰ� ����� ������ �����ֳ�
		float windowHeight = 100;
		//windowWith �� ���� ��ġ ����ش�. 0�����ϸ� ȭ�鿡 �� �پ���� //�ٵ� �����Ѵٰ� �Ⱥ��̴°Ծƴ� ..
		//-100������������ �ߺ����� ������ 0�̵Ǹ� �ϳ����Ⱥ���
		glOrtho(-windowWidth, windowWidth, -100.0, 100.0, 1.0, -1.0);
		//glOrtho ( GLdouble left,Gldouble right, GLdouble bottom,  GLdouble top, GLdouble nearVal, GLdouble farVal)
		//far�� ����� �־� �Z���� ������� near�� ����� �־��൵ ����� ����.
		//near, far�� Ŭ���� ������ ���ʰ� ���� ��ǥ�� ���Ѵ�.

	}
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();

	/*
		ī�޷��� ��ü�̵��� �ϱ����ؼ� GL_MODELVIEW�� �̿��ϰ�
		��ü�� ������Ű�� ���ؼ��� GL_PROJECTION ��尡 �ʿ��ϴ�.
	*/

}

void main(int argc, char * argv[])
{
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_SINGLE | GLUT_RGB);
	glutInitWindowSize(600, 600);
	glutCreateWindow("Simple");
	glutDisplayFunc(RenderScene);
	glutReshapeFunc(ChangeSize);
	SetupRC();
	glutMainLoop();
	//�⺻���� Ʋ������ �ܿ��ʿ���� ���迡�ȳ��´� ���躼�� �ش�.
}

/*
glColor3f(1,0,0)
GL_QUADS

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
