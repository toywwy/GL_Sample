#define _CRT_SECURE_NO_WARNINGS
#pragma warning(disable:4996)

#include<iostream>
#include<cstdio>
#include<Windows.h> //이건 GUI 때문에 필요한건데.
#include<gl/GL.h>
#include<gl/GLU.h>
#include<gl/glut.h> //이건 유틸리티 툴킷인데, 사용자의 입력이나 화면 윈도우를 제어하기 위해서다.
#include <math.h> //삼각함수 쓰려고

/*
* gl.h (OpenGL Core Library) - 렌더링 기능을 제공하는 라이브러리
* glu.h (OpenGL Utility Library) - GL 라이브러러의 도우미 역할
* glut.h (OpenGL Utility Toolkit) - 사용자 입력을 받아들이거나 화면 윈도우를 제어하기 위한 라이브러리
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
	glutPostRedisplay();//화면 갱신이 필요할 때 이함수를 부르면 display 콜백 함수를 다시 한 번 실행
	glutTimerFunc(1000 / 30, timer, 1);//다이머 시간 마다 갱신,  해준다 //30 frame 나온다.
									   //타이머가 작동하는데 다시 타이머를 동작시킴으로써 계쏙 작동하게 되는거지.
}

void KillFont(GLvoid)
{
	glDeleteLists(base, 96);
}

//가변 파라미터를 이용해서 인풋을 받는다.
void glPrint(const char*fmt, ...)
{
	char text[256];
	va_list ap;
	
	if (fmt == NULL) return;

	va_start(ap, fmt);
		vsprintf(text, fmt, ap);
	va_end(ap);

	glPushAttrib(GL_LIST_BIT);
		glListBase(base - 32);//시작이 32부터 시작해서 인덱스 맞춰 줄려고 한것같애
		glCallLists(strlen(text), GL_UNSIGNED_BYTE, text);//이게 그 번호를 가져와서 그려주는것이다.
	glPopAttrib();
}

//각각의 문자들을 디스플레이 목록에 저장을한다. !!!미리 저장해놓고 그거 나중에 갖아 쓰기 때문에 빠르게 가능할듯
//뭐 96개정도야 무리되는것도 아니고
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

	wglUseFontBitmaps(hDC, 32, 96, base); //이걸 이용해서 디스플레이 목록에 저장을 하는 것이다.
	SelectObject(hDC, oldfont);
	DeleteObject(font);
}


void RenderScene(void)
{
	//Clear the window with current clearing color
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT); //버퍼의 조합을 지운다.
														/*  DEPTH_BUFFER를 두어서 픽셀별로 깊이 값을 저장하도록 하였다. 이를 이용해서 어떤 물체를 그리고자 할때 그앞에 물체가 있으면 있을때 깊이를
												F		비교하여 DEPTH가 작은 삼각형쪽은 그려지고, DEPTH가 큰 사각형쪽은 그리지 않게 할 수 있게되는 것이다.
														(동일한 좌표의 픽셀이 있을때 깊이에 따라 그려야 할지 말아야 할지 선택하게 되는 것이다.) */

	glMatrixMode(GL_MODELVIEW);//MODELVEIW는 그리는거고 projection은 투영하는거다.
	glLoadIdentity();

	gluLookAt(3, 3, 3, 0, 0, 0, 0, 1, 0); //보는 시점이다.
										  //처음이 눈, at은 어느부분을 볼것인가 이기때문에 거의 안건들여..
										  //up은 축을 말하는거다...어느축으로 볼것인가.

										  //viewer x y z 에서 0,0,0 을 y축을 세워서 바라 보는 것이다.
										  //이게 glu LookAt이다.
	
	glColor3f(1, 0, 0);
	glRasterPos2f(-0.0, -3.0);
	glPrint("theta - %7.2f", theta);


	glRotatef(theta, 0, 1, 0);
	drawCube();
	

	

	glutSwapBuffers(); //GLUT_DOUBLE 버퍼를 더블로해서 스왑해서사용할 것이다.
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

	//이거왜하는거지?? 뉴리스트??
	/*glNewList(listID, GL_COMPILE);
	glEndList();*/

	BuildFont(); //이건 필수 적으로 해줘야 한다.

	glutTimerFunc(1000 / 30, timer, 1);
}

void ChangeSize(int w, int h)
{
	if (h == 0)// 0으로 나누어지는거 막기위해서
		h = 1;

	glViewport(0, 0, w, h); // 실제로 보여지는 부분을 뷰포트라 명시한다.

	glMatrixMode(GL_PROJECTION); //이부분이 PROJECTION인 이유는 3D를 2D로 투영시켜야한다.
								 //glOrtho를 통해서
	glLoadIdentity();//좌표계 초기화

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
	glutCreateMenu(menu); //메뉴마다 옵션을 넣어줘야한다. 그리고 키보드에 매핑도 가능한것이지..
	glutAddMenuEntry("orthographic", 1);
	glutAddMenuEntry("perspective", 2);

	//오른쪽 버튼 누르면 메뉴가 뜬다.
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

	glutDisplayFunc(RenderScene);//렌더링되는 부분
	glutReshapeFunc(ChangeSize);//SIZE가 바뀔마다 호출이 된다.
								//glutSpecialFunc(specialKeyboard);//스페셜 키보드 이벤트를 콜백
	glutKeyboardFunc(keyboard);
	menuInit();

	SetupRC();
	init();
	glutMainLoop();
	//기본적인 틀같은거 외우필요없고 시험에안나온다 시험볼떄 준다.
}
