#include<iostream>
#include<WIndows.h>
//#include<gl/gL.h>
//#include<gl/gLU.h> //그래서 앞에를 대문자로 써도 소문자로 써도 상관이 없던건가???
#include<gl/glut.h>//잉 이것만 있으면 되는건가?

#include <math.h>

using namespace std;

GLUquadricObj *quadricObj;//곡면 그릴떄 사용하는 것으로 유추됨 원통 실린더 그릴때 사용

static float angle = 5.0f;

bool baseToggle[2];
bool lowerToggle[2];
bool upperToggle[2];

static float theta[3];

void keyboard(unsigned char c, int x, int y)
{
	switch (c)
	{
	case VK_ESCAPE:
		gluDeleteQuadric(quadricObj);
		exit(0);
		break;
	}
}

void specialKeyboard(int c, int x, int y)
{
	switch (c)
	{
	case GLUT_KEY_HOME:
		upperToggle[0] = true;
		upperToggle[1] = true;
		break;
	case GLUT_KEY_END:
		upperToggle[0] = true;
		upperToggle[1] = false;
		break;
	case GLUT_KEY_LEFT:
		baseToggle[0] = true;
		baseToggle[1] = true;

		break;
	case GLUT_KEY_RIGHT:
		baseToggle[0] = true;
		baseToggle[1] = false;
		break;
	case GLUT_KEY_UP:
		lowerToggle[0] = true;
		lowerToggle[1] = true;
		break;
	case GLUT_KEY_DOWN:
		lowerToggle[0] = true;
		lowerToggle[1] = false;
		break;
	}

}

void timer(int value)
{
	glutPostRedisplay();//화면 갱신이 필요할 때 이함수를 부르면 display 콜백 함수를 다시 한 번 실행
	glutTimerFunc(1000 / 30, timer, 1);//다이머 시간 마다 갱신,  해준다 //30 frame 나온다.
}

void RenderScene(void)
{
	//Clear the window with current clearing color
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT); //버퍼의 조합을 지운다.
														/*  DEPTH_BUFFER를 두어서 픽셀별로 깊이 값을 저장하도록 하였다. 이를 이용해서 어떤 물체를 그리고자 할때 그앞에 물체가 있으면 있을때 깊이를
														비교하여 DEPTH가 작은 삼각형쪽은 그려지고, DEPTH가 큰 사각형쪽은 그리지 않게 할 수 있게되는 것이다.
														(동일한 좌표의 픽셀이 있을때 깊이에 따라 그려야 할지 말아야 할지 선택하게 되는 것이다.) */

	glMatrixMode(GL_MODELVIEW);//MODELVEIW는 그리는거고 projection은 투영하는거다.
	glLoadIdentity();

	gluLookAt(0, 2.0, 2.0, 0, 0, 0, 0, 1.0, 0);// 정면에서보려고
											   //gluLookAt(0, 0, 2.0, 0, 0, 0, 0, 1.0, 0);// 정면에서보려고
											   //gluLookAt(1, 1, 2.0, 0, 0, 0, 0, 1.0, 0);// 약간위에서 본다.


											   /*glRotatef(theta[2], 0, 0, 1);
											   glRotatef(theta[1], 0, 1, 0);
											   glRotatef(theta[0], 1, 0, 0);*/


	glEnable(GL_LIGHTING);
	glEnable(GL_LIGHT0);


	float f = 2.0f;

	GLfloat position0[4] = { 2.0f,2.0f,2.0f,1.0f };
	GLfloat ambient0[4] = { 0.1f,0.1f,0.1f,1.0f };
	GLfloat diffuse0[4] = { 1.0f,1.0f,0.0f,1.0f };
	GLfloat specular0[4] = { 1.0f ,1.0f ,1.0f ,1.0f };



	glLightfv(GL_LIGHT0, GL_POSITION, position0);
	glLightfv(GL_LIGHT0, GL_AMBIENT, ambient0);
	glLightfv(GL_LIGHT0, GL_DIFFUSE, diffuse0);
	glLightfv(GL_LIGHT0, GL_SPECULAR, specular0);

	//float 값 타입의 vector라고 명시하는 것이다.

	//glColor3f(1, 0, 0); //어차피 해도 안먹힌다.

	glutSolidTeapot(1.0);

	glutSwapBuffers(); //GLUT_DOUBLE 버퍼를 더블로해서 스왑해서사용할 것이다.
}

void SetupRC(void) {
	glClearColor(1.0f, 1.0f, 1.0f, 1.0f);
	glEnable(GL_DEPTH_TEST);
}

void init(void)
{

	quadricObj = gluNewQuadric();
	gluQuadricDrawStyle(quadricObj, GLU_LINE);
	glutTimerFunc(1000 / 30, timer, 1);
}

void ChangeSize(int w, int h)
{
	if (h == 0)// 0으로 나누어지는거 막기위해서
		h = 1;

	glViewport(0, 0, w, h);

	glMatrixMode(GL_PROJECTION); //이부분이 PROJECTION인 이유는
	glLoadIdentity();//좌표계 초기화

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
	glutDisplayFunc(RenderScene);//렌더링되는 부분
	glutReshapeFunc(ChangeSize);//SIZE가 바뀔마다 호출이 된다.
								//	glutSpecialFunc(specialKeyboard);//스페셜 키보드 이벤트를 콜백
								//	glutKeyboardFunc(keyboard);//키보드 이벤트 콜백
	init();
	SetupRC();
	glutMainLoop();
	//기본적인 틀같은거 외우필요없고 시험에안나온다 시험볼떄 준다.
}
