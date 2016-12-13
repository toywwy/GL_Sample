#include<iostream>
#include<Windows.h> //�̰� GUI ������ �ʿ��Ѱǵ�.
#include<gl/GL.h>
#include<gl/GLU.h>
#include<gl/glut.h> //�̰� ��ƿ��Ƽ ��Ŷ�ε�, ������� �Է��̳� ȭ�� �����츦 �����ϱ� ���ؼ���.
#include <math.h> //�ﰢ�Լ� ������

/*
* gl.h (OpenGL Core Library) - ������ ����� �����ϴ� ���̺귯��
* glu.h (OpenGL Utility Library) - GL ���̺귯���� ����� ����
* glut.h (OpenGL Utility Toolkit) - ����� �Է��� �޾Ƶ��̰ų� ȭ�� �����츦 �����ϱ� ���� ���̺귯��
*/

using namespace std;


struct Point {
	float p[3];
	Point() {}
	Point(float x, float y, float z)
	{
		this->p[0] = x; this->p[1] = y; this->p[2] = z;
	}

	Point operator- (Point a)
	{
		Point retP;
		for (int i = 0; i < 3; i++) retP.p[i] = this->p[i] - a.p[i];
		return retP;
	}

};


void timer(int value)
{
	glutPostRedisplay();//ȭ�� ������ �ʿ��� �� ���Լ��� �θ��� display �ݹ� �Լ��� �ٽ� �� �� ����
	glutTimerFunc(1000 / 30, timer, 1);//���̸� �ð� ���� ����,  ���ش� //30 frame ���´�.
									   //Ÿ�̸Ӱ� �۵��ϴµ� �ٽ� Ÿ�̸Ӹ� ���۽�Ŵ���ν� ��� �۵��ϰ� �Ǵ°���.
}

Point getNormal(Point np)
{
	float x, y, z;
	float len;
	len = 0;

	x = np.p[0];
	y = np.p[1];
	z = np.p[2];

	len = x*x + y*y + z*z;
	len = sqrt(len);
	Point p(x / len, y / len, z / len);

	return p;
}

//�����Ѱǵ� �̰� �ɰž� �ȵ� ������ ����.
Point CrossProduct(Point a, Point b)
{
	Point p(
		a.p[1] * b.p[2] - a.p[2] * b.p[1],
		a.p[2] * b.p[0] - a.p[0] * b.p[2],
		a.p[0] * b.p[1] - a.p[1] * b.p[0]
	);

	return getNormal(p);
}

void RenderScene(void)
{
	//Clear the window with current clearing color
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT); //������ ������ �����.
														/*  DEPTH_BUFFER�� �ξ �ȼ����� ���� ���� �����ϵ��� �Ͽ���. �̸� �̿��ؼ� � ��ü�� �׸����� �Ҷ� �׾տ� ��ü�� ������ ������ ���̸�
														���Ͽ� DEPTH�� ���� �ﰢ������ �׷�����, DEPTH�� ū �簢������ �׸��� �ʰ� �� �� �ְԵǴ� ���̴�.
														(������ ��ǥ�� �ȼ��� ������ ���̿� ���� �׷��� ���� ���ƾ� ���� �����ϰ� �Ǵ� ���̴�.) */

	glMatrixMode(GL_MODELVIEW);//MODELVEIW�� �׸��°Ű� projection�� �����ϴ°Ŵ�.
	glLoadIdentity();

	gluLookAt(0, 0, 2.0, 0, 0, 0, 0, 1.0, 0); //���� �����̴�.
											  //ó���� ��, at�� ����κ��� �����ΰ� �̱⶧���� ���� �Ȱǵ鿩..
											  //up�� ���� ���ϴ°Ŵ�...��������� �����ΰ�.


	Point p1(0, 0, 0);
	Point p2(1, 0, 0);
	Point p3(1, 1, 0);
	Point p4(0,1,0);
	
	//���������� �����Ѵ� ���� ������ ������ �ٲ� ���� �ִ�.
	
	//���̰Ǿȵ�.. �ϱ� �������� ���� ��� �����ϸ� �׳� ���̵Ǿ� ������ �Ŵϱ�..
	Point ps1 = p1 - p2;
	Point ps2 = p3 - p2;

	//Point ps1 = p1 - p2;
	//Point ps2 = p3 - p2;

	//������ ������ �Ѵ� ....
	Point np = CrossProduct(ps1, ps2); //�� �� ������ �߿��Ѱ���....??
	glColor3f(0, 1, 0);
	glBegin(GL_TRIANGLES);//�ֻ簢�� �׸��� �� ������ �߿������ʴ°���...
	{
		glNormal3fv(np.p);
		glVertex3fv(p1.p);
		glVertex3fv(p2.p);
		glVertex3fv(p3.p);
	}
	glEnd();


	glutSwapBuffers(); //GLUT_DOUBLE ���۸� ������ؼ� �����ؼ������ ���̴�.
}

void SetupRC(void) {
	glClearColor(1.0f, 1.0f, 1.0f, 1.0f);
	glEnable(GL_DEPTH_TEST);
	glEnable(GL_LIGHTING);
	glEnable(GL_LIGHT0);
	
	//glEnable(GL_NORMALIZE);

	glColorMaterial(GL_FRONT, GL_AMBIENT_AND_DIFFUSE);
	glEnable(GL_COLOR_MATERIAL);

	GLfloat mat_specular[4] = { 1,1,1,1 };
	GLfloat mat_shininess = 100;

	glMaterialfv(GL_FRONT, GL_SPECULAR, mat_specular);
	glMaterialf(GL_FRONT, GL_SHININESS, mat_shininess);


}

void init(void)
{
	glutTimerFunc(1000 / 30, timer, 1);
}

void ChangeSize(int w, int h)
{
	if (h == 0)// 0���� ���������°� �������ؼ�
		h = 1;

	glViewport(0, 0, w, h); // ������ �������� �κ��� ����Ʈ�� ����Ѵ�.

	glMatrixMode(GL_PROJECTION); //�̺κ��� PROJECTION�� ������ 3D�� 2D�� �������Ѿ��Ѵ�.
								 //glOrtho�� ���ؼ�
	glLoadIdentity();//��ǥ�� �ʱ�ȭ
					 //�ٵ� �̰� ���� �ƴµ� ,�̷��� ���� ��ǥ�踦 �ʱ�ȭ����� �ϰ� �̷��� ���ȳ��ٵ�...
					 //��Ǳٵ� �Ƚᵵ ������°ű��� ....��ǥ�踦 �������� �����̻�????
					 //���⿡�� �ʱ�ȭ ���ִ� ������ ����?? � ��ǥ�踦 �ʱ�ȭ �����شٴ� ������??

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

	glutDisplayFunc(RenderScene);//�������Ǵ� �κ�
	glutReshapeFunc(ChangeSize);//SIZE�� �ٲ𸶴� ȣ���� �ȴ�.
								//	glutSpecialFunc(specialKeyboard);//����� Ű���� �̺�Ʈ�� �ݹ�
								//	glutKeyboardFunc(keyboard);//Ű���� �̺�Ʈ �ݹ�

	//453, 948 //453���� ���� 948������ �ﰢ������ ������ ���ִ�.....
	//�� ���� ������ ���̴� �����ΰ�???
	//
	
	//�Է¿Ϸ�



	init();
	SetupRC();
	glutMainLoop();
	//�⺻���� Ʋ������ �ܿ��ʿ���� ���迡�ȳ��´� ���躼�� �ش�.
}
