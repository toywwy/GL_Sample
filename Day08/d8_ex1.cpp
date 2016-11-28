#define _CRT_SECURE_NO_WARNINGS
#pragma warning(disable:4996)

#include<iostream>
#include<cstdio>
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

int flag = 1;

static int listID;
static float theta;
static int base;

HDC hDC;
HWND hWnd;

GLfloat vertices[8][3] = { { -1, -1, 1 },{ -1, 1, 1 },{ 1, 1, 1 },{ 1, -1, 1 },{ -1, -1, -1 },{ -1, 1, -1 },{ 1, 1, -1 },{ 1, -1, -1 } };
GLfloat colors[8][3] = { { 0, 0, 1 },{ 0, 1, 1 },{ 1, 1, 1 },{ 1, 0, 1 },{ 0, 0, 0 },{ 0, 1, 0 },{ 1, 1, 0 },{ 1, 0, 0 } };

void drawCube()
{
	glBegin(GL_QUADS);


	glColor3fv(colors[0]); glVertex3fv(vertices[0]);
	glColor3fv(colors[3]); glVertex3fv(vertices[3]);
	glColor3fv(colors[2]); glVertex3fv(vertices[2]);
	glColor3fv(colors[1]); glVertex3fv(vertices[1]);



	glColor3fv(colors[2]); glVertex3fv(vertices[2]);
	glColor3fv(colors[3]); glVertex3fv(vertices[3]);
	glColor3fv(colors[7]); glVertex3fv(vertices[7]);
	glColor3fv(colors[6]); glVertex3fv(vertices[6]);


	glColor3fv(colors[3]); glVertex3fv(vertices[3]);
	glColor3fv(colors[0]); glVertex3fv(vertices[0]);
	glColor3fv(colors[4]); glVertex3fv(vertices[4]);
	glColor3fv(colors[7]); glVertex3fv(vertices[7]);


	glColor3fv(colors[1]); glVertex3fv(vertices[1]);
	glColor3fv(colors[2]); glVertex3fv(vertices[2]);
	glColor3fv(colors[6]); glVertex3fv(vertices[6]);
	glColor3fv(colors[5]); glVertex3fv(vertices[5]);



	glColor3fv(colors[4]); glVertex3fv(vertices[4]);
	glColor3fv(colors[5]); glVertex3fv(vertices[5]);
	glColor3fv(colors[6]); glVertex3fv(vertices[6]);
	glColor3fv(colors[7]); glVertex3fv(vertices[7]);


	glColor3fv(colors[5]); glVertex3fv(vertices[5]);
	glColor3fv(colors[4]); glVertex3fv(vertices[4]);
	glColor3fv(colors[0]); glVertex3fv(vertices[0]);
	glColor3fv(colors[1]); glVertex3fv(vertices[1]);


	glEnd();
}

void timer(int value)
{
	theta += 1;
	if (theta > 360) theta = 0;
	glutPostRedisplay();//ȭ�� ������ �ʿ��� �� ���Լ��� �θ��� display �ݹ� �Լ��� �ٽ� �� �� ����
	glutTimerFunc(1000 / 30, timer, 1);//���̸� �ð� ���� ����,  ���ش� //30 frame ���´�.
									   //Ÿ�̸Ӱ� �۵��ϴµ� �ٽ� Ÿ�̸Ӹ� ���۽�Ŵ���ν� ��� �۵��ϰ� �Ǵ°���.
}

void KillFont(GLvoid)
{
	glDeleteLists(base, 96);
}

//���� �Ķ���͸� �̿��ؼ� ��ǲ�� �޴´�.
void glPrint(const char*fmt, ...)
{
	char text[256];
	va_list ap;
	
	if (fmt == NULL) return;

	va_start(ap, fmt);
		vsprintf(text, fmt, ap);
	va_end(ap);

	glPushAttrib(GL_LIST_BIT);
		glListBase(base - 32);//������ 32���� �����ؼ� �ε��� ���� �ٷ��� �ѰͰ���
		glCallLists(strlen(text), GL_UNSIGNED_BYTE, text);//�̰� �� ��ȣ�� �����ͼ� �׷��ִ°��̴�.
	glPopAttrib();
}

//������ ���ڵ��� ���÷��� ��Ͽ� �������Ѵ�. !!!�̸� �����س��� �װ� ���߿� ���� ���� ������ ������ �����ҵ�
//�� 96�������� �����Ǵ°͵� �ƴϰ�
void BuildFont(void)
{
	HFONT font;
	HFONT oldfont;

	base = glGenLists(96);

	font = CreateFontA(-24, 0, 0, 0,
		FW_BOLD,
		FALSE,
		FALSE,
		FALSE,
		ANSI_CHARSET,
		OUT_TT_PRECIS,
		CLIP_DEFAULT_PRECIS,
		ANTIALIASED_QUALITY,
		FF_DONTCARE | DEFAULT_PITCH,
		"Courier New");
	oldfont = (HFONT)SelectObject(hDC, font);

	wglUseFontBitmaps(hDC, 32, 96, base); //�̰� �̿��ؼ� ���÷��� ��Ͽ� ������ �ϴ� ���̴�.
	SelectObject(hDC, oldfont);
	DeleteObject(font);
}


void RenderScene(void)
{
	//Clear the window with current clearing color
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT); //������ ������ �����.
														/*  DEPTH_BUFFER�� �ξ �ȼ����� ���� ���� �����ϵ��� �Ͽ���. �̸� �̿��ؼ� � ��ü�� �׸����� �Ҷ� �׾տ� ��ü�� ������ ������ ���̸�
												F		���Ͽ� DEPTH�� ���� �ﰢ������ �׷�����, DEPTH�� ū �簢������ �׸��� �ʰ� �� �� �ְԵǴ� ���̴�.
														(������ ��ǥ�� �ȼ��� ������ ���̿� ���� �׷��� ���� ���ƾ� ���� �����ϰ� �Ǵ� ���̴�.) */

	glMatrixMode(GL_MODELVIEW);//MODELVEIW�� �׸��°Ű� projection�� �����ϴ°Ŵ�.
	glLoadIdentity();

	gluLookAt(3, 3, 3, 0, 0, 0, 0, 1, 0); //���� �����̴�.
										  //ó���� ��, at�� ����κ��� �����ΰ� �̱⶧���� ���� �Ȱǵ鿩..
										  //up�� ���� ���ϴ°Ŵ�...��������� �����ΰ�.

										  //viewer x y z ���� 0,0,0 �� y���� ������ �ٶ� ���� ���̴�.
										  //�̰� glu LookAt�̴�.
	
	glColor3f(1, 0, 0);
	glRasterPos2f(-0.0, -3.0);
	glPrint("theta - %7.2f", theta);


	glRotatef(theta, 0, 1, 0);
	drawCube();
	

	

	glutSwapBuffers(); //GLUT_DOUBLE ���۸� �������ؼ� �����ؼ������ ���̴�.
}

void SetupRC(void) {
	glClearColor(1.0f, 1.0f, 1.0f, 1.0f);
	glEnable(GL_DEPTH_TEST);
}

void init(void)
{
	theta = 0.0f;
	hWnd = GetActiveWindow();
	hDC = GetDC(hWnd);

	//listID = glGenLists(1);

	//�̰ſ��ϴ°���?? ������Ʈ??
	/*glNewList(listID, GL_COMPILE);
	glEndList();*/

	BuildFont(); //�̰� �ʼ� ������ ����� �Ѵ�.

	glutTimerFunc(1000 / 30, timer, 1);
}

void ChangeSize(int w, int h)
{
	if (h == 0)// 0���� ���������°� �������ؼ�
		h = 1;

	glViewport(0, 0, w, h); // ������ �������� �κ��� ����Ʈ�� �����Ѵ�.

	glMatrixMode(GL_PROJECTION); //�̺κ��� PROJECTION�� ������ 3D�� 2D�� �������Ѿ��Ѵ�.
								 //glOrtho�� ���ؼ�
	glLoadIdentity();//��ǥ�� �ʱ�ȭ

	if (w <= h)
		glOrtho(-3.0, 3.0, -3.0*(float)h / (float)w,
			3.0*(float)h / (float)w, 1, 10.0);
	else
		glOrtho(-3.0*(float)w / (float)h, 3.0*(float)w / (float)h,
			-3.0, 3.0, 1.0, 10.0);

	/*if (w <= h)
		glFrustum(-2.0, 2.0, -2.0*(float)h / (float)w,
			2.0*(float)h / (float)w, 1, 10.0);
	else
		glFrustum(-2.0*(float)w / (float)h, 2.0*(float)w / (float)h,
			-2.0, 2.0, 1.0, 10.0);*/
}

void menu(int item)
{
	flag = item;
	//ChangeSize(glutGet(GLUT_WINDOW_WIDTH), glutGet(GLUT_WINDOW_HEIGHT));
	glutPostRedisplay();
}

void menuInit()
{
	glutCreateMenu(menu); //�޴����� �ɼ��� �־�����Ѵ�. �׸��� Ű���忡 ���ε� �����Ѱ�����..
	glutAddMenuEntry("orthographic", 1);
	glutAddMenuEntry("perspective", 2);

	//������ ��ư ������ �޴��� ���.
	glutAttachMenu(GLUT_RIGHT_BUTTON);
}


void keyboard(unsigned char c, int x, int y)
{
	switch (c)
	{
	case VK_ESCAPE:
		KillFont();
		exit(0);
	}
}

void main(int argc, char * argv[])
{
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB);
	glutInitWindowSize(600, 600);
	glutCreateWindow("Simple");

	glutDisplayFunc(RenderScene);//�������Ǵ� �κ�
	glutReshapeFunc(ChangeSize);//SIZE�� �ٲ𸶴� ȣ���� �ȴ�.
								//glutSpecialFunc(specialKeyboard);//����� Ű���� �̺�Ʈ�� �ݹ�
	glutKeyboardFunc(keyboard);
	menuInit();

	SetupRC();
	init();
	glutMainLoop();
	//�⺻���� Ʋ������ �ܿ��ʿ���� ���迡�ȳ��´� ���躼�� �ش�.
}