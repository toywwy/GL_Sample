#include<Windows.h>
#include<gl/GL.h>
#include<gl/GLU.h>
#include<gl/glut.h>
#include<iostream>
#include <math.h>

using namespace std;

GLfloat vertices[8][3] = { { -4, -4, 4 },{ -4, 4, 4 },{ 4, 4, 4 },{ 4, -4, 4 },{ -4, -4, -4 },{ -4, 4, -4 },{ 4, 4, -4 },{ 4, -4, -4 } };
GLfloat colors[8][3] = { { 0, 0, 1 },{ 0, 1, 1 },{ 1, 1, 1 },{ 1, 0, 1 },{ 0, 0, 0 },{ 0, 1, 0 },{ 1, 1, 0 },{ 1, 0, 0 } };

static float theta;
static bool mouseLeftDown;
static char rotationAxisMode;

void quad(int a, int b, int c, int d)
{
	glColor3fv(colors[a]); glVertex3fv(vertices[a]);
	glColor3fv(colors[b]); glVertex3fv(vertices[b]);
	glColor3fv(colors[c]); glVertex3fv(vertices[c]);
	glColor3fv(colors[d]); glVertex3fv(vertices[d]);
}

void RenderScene(void)
{
	//윈도우 색 초기화
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	/*
	카메러나 물체이동을 하기위해서 GL_MODELVIEW를 이용하고
	물체를 투영시키기 위해서는 GL_PROJECTION 모드가 필요하다.
	*/
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();

	//A transformation of the world coordinate
	gluLookAt(0.5, 0.5, 0.5, 0, 0, 0, 0, 1, 0);
	/*
	To Obtain a Desired View, Position and Orient the Camera within the World Coordinate System
	Eye point, at point, up vector
	*/



	//이렇게 cos 값을 sin 값을 이용할 수가 있는건데.... 행렬을 만들어서
	//glMultMatrixf()여기에 행렬을 넣어주는거지

	//glRotatef(theta, 0, 1, 0);
	
	//-1,-1,-1 한다음.. 회전하고 다시 1,1,1 해주면 된다.
	float tx, ty, tz;
	tx = ty = tz = 1;
	float translateMatrixPre[16] = {
		1,	0,	0,	0,
		0,	1,	0,	0,
		0,	0,	1,	0,
		-tx,-ty,-tz,1,
	};
	
	float translateMatrixPost[16] = {
		1,	0,	0,	0,
		0,	1,	0,	0,
		0,	0,	1,	0,
		tx,	ty,	tz,	1,
	};//이게 그 순서 생각해서 입력해야 되는구나 ....


	float cos_th = cos(theta * 3.14159 / 180);
	float sin_th = sin(theta * 3.14159 / 180);

	float rotateMatrix[16] = {
		cos_th,	0,	-sin_th,	0,
		0,		1,	0,		0,
		sin_th,0,	cos_th,	0,
		0,		0,	0,		1
	};

	//만약 이동을 해야 된다고 하면 

	GLfloat *glf =new GLfloat[16];
	glf[0] = cos_th; glf[1] = 0;  glf[2] = sin_th; glf[3] = 0;
	glf[4] = 0; glf[5] = 1; glf[6] = 0; glf[7] = 0;
	glf[8] = -sin_th; glf[9] = 0; glf[10] = cos_th; glf[11] = 0;
	glf[12] = 0; glf[13] = 0; glf[14] = 0; glf[15] = 1;


	glMultMatrixf(translateMatrixPre);
	glMultMatrixf(rotateMatrix);
	glMultMatrixf(translateMatrixPost);


	glBegin(GL_QUADS);
	quad(0, 3, 2, 1);
	quad(2, 3, 7, 6);
	quad(3, 0, 4, 7);
	quad(1, 2, 6, 5);
	quad(4, 5, 6, 7);
	quad(5, 4, 0, 1);
	glEnd();



	glutSwapBuffers();//더블 버퍼 사용하기 떄문이다.
}

void mouseButton(int button, int state, int x, int y)
{
	if (button == GLUT_LEFT_BUTTON)
		if (state == GLUT_DOWN)
			(mouseLeftDown == true) ? mouseLeftDown = false : mouseLeftDown = true;
		else if (button = GLUT_RIGHT_BUTTON)
			if (state == GLUT_DOWN)
				(mouseLeftDown == true) ? mouseLeftDown = false : mouseLeftDown = true;

	glutPostRedisplay(); //화면 갱신이 필요할 때 이함수를 부르면 display 콜백 함수를 다시 한 번 실행
}

void keyboard(unsigned char key, int x, int y)
{
	switch (key)
	{
	case 'q' | 'Q':
		exit(0); break; //종료 시킬수있음
	case 'x' | 'X':
		rotationAxisMode = 0;
		break;
	case 'y' | 'X':
		rotationAxisMode = 1;
		break;
	case 'z' | 'X':
		rotationAxisMode = 2;
		break;
	default:
		break;
	}
}


void SetupRC(void) {
	glClearColor(1.0f, 1.0f, 1.0f, 1.0f);
	glEnable(GL_DEPTH_TEST);
}

void ChangeSize(int w, int h)
{
	if (h == 0)// 0으로 나누어지는거 막기위해서
		h = 1;

	glViewport(0, 0, w, h);

	glMatrixMode(GL_PROJECTION);//오쏘는 투영해야 하는거라 이렇게해야함
	glLoadIdentity();//좌표계 초기화, 처리가 이루어지기전에한다.

	if (w <= h)
		glOrtho(-10, 10, -10*(float)h / (float)w,
			10*(float)h / (float)w, -10.0, 10.0);//위에 10으로 바꾼거 클리핑하기위해서이다.
	else
		glOrtho(-10*(float)w / (float)h, 10*(float)w / (float)h, -10,
			10, -10.0, 10.0);
}


void timer(int value)
{
	(mouseLeftDown) ? theta -= 2 : theta += 2;

	if (theta >= 360.0)
		theta -= 360.0;

	if (theta <= -360.0)
		theta += 360.0;

	glutPostRedisplay();
	glutTimerFunc(1000 / 30, timer, 1);
}

void init(void)
{
	theta = 0.0f;
	rotationAxisMode = 1;
	mouseLeftDown = false;
	glutTimerFunc(1000 / 30, timer, 1);//타이머 첫동작
}

void main(int argc, char * argv[])
{
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB);
	glutInitWindowSize(600, 600);
	glutCreateWindow("Simple");

	glutDisplayFunc(RenderScene);
	glutReshapeFunc(ChangeSize);//꼭 추가해줘야한다. 그려질떄 이것도 같이 호출이 되기 때문에 꼭
	glutMouseFunc(mouseButton);
	glutKeyboardFunc(keyboard);

	init();
	SetupRC();

	glutMainLoop();//이게 제일 마지막에
}
