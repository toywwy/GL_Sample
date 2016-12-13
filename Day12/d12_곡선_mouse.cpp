#include<iostream>
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

#define B0(t) ( (1-t) * (1-t) *(1-t))
#define B1(t) ( 3*(1-t) * (1-t) *(t))
#define B2(t) ( 3*(t) * (t) *(1-t))
#define B3(t) ( t*t*t )

using namespace std;

struct Point {
	float x = 0.0f;
	float y = 0.0f;
	float z = 0.0f;
};

bool isLeftMouseDown;
char * applicationName = " Simple Curvds";
int windowWidth = 500;
int windowHeight = 500;
int orthoSize = 2.0;

Point controlPoint[4];
Point mouseDropPoint;
int selectPoint = -1;

void drawControlPoints()
{
	glColor3f(1.0f, 0.0f, 0.0f);
	glPointSize(5.0f);

	for (int i = 0; i < 4; i++)
	{
		glBegin(GL_POINTS);
		{
			glVertex3f(controlPoint[i].x, controlPoint[i].y, controlPoint[i].z);
		}glEnd();
	}
}

void drawControlLines()
{
	glColor3f(0.0f, 0.0f, 0.0f);
	glLineStipple(3, 0xcccc);

	glEnable(GL_LINE_STIPPLE);

	for (int i = 0; i < 3; i++)
	{
		glBegin(GL_LINES);
		{
			glVertex3f(controlPoint[i].x, controlPoint[i].y, controlPoint[i].z);
			glVertex3f(controlPoint[i + 1].x, controlPoint[i + 1].y, controlPoint[i + 1].z);
		}glEnd();

	}
	glDisable(GL_LINE_STIPPLE);
}

//곡선으로 만들어 주는 부분이다.
void drawBezierCurve()
{
	float segments = 0.02f;// 
	float t = 0.0f;// 얘가 1.0이 될때 까지 ..

	Point p[2];
	p[1] = controlPoint[0];//그냥 시작 점을 얘로 할려고 이렇게 한거다.
	//p[1].x = p[1].y = p[1].z = 0;
	for (; t < 1.0f; t += segments)//뭐지 곡선이 있는거 같지 않은데..
	{
		p[0] = p[1];
		p[1].x = (controlPoint[0].x * B0(t)) + (controlPoint[1].x * B1(t)) + (controlPoint[2].x * B2(t)) + (controlPoint[3].x*B3(t));

		p[1].y = (controlPoint[0].y * B0(t)) + (controlPoint[1].y * B1(t)) + (controlPoint[2].y * B2(t)) + (controlPoint[3].y*B3(t));
		p[1].z = 0.0f;

		glColor3f(0.0f, 0.0f, 1.0f);

		glBegin(GL_LINES);
		{
			glVertex3f(p[0].x, p[0].y, p[0].z);
			glVertex3f(p[1].x, p[1].y, p[1].z);
		}
		glEnd();
	}
}

void drawMouseDropPoint()
{
	glColor3f(0.0f, 1.0f, 0.0f);
	glPointSize(5.0);

	glBegin(GL_POINTS);
	{
		glVertex3f(mouseDropPoint.x, mouseDropPoint.y, mouseDropPoint.z);
	}
	glEnd();
}

void mouseDrop(int _x, int _y)
{
	mouseDropPoint.x = (((float)windowWidth / 2) - ((float)windowWidth - _x)) / ((float)windowWidth / (orthoSize * 2));
	mouseDropPoint.y = (((float)windowHeight / 2) - _y) / ((float)windowHeight / (orthoSize * 2));
	mouseDropPoint.z = 0.0f;

	std::cout << mouseDropPoint.x << " " << mouseDropPoint.y << std::endl;
}


void timer(int value)
{
	glutPostRedisplay();//화면 갱신이 필요할 때 이함수를 부르면 display 콜백 함수를 다시 한 번 실행
	glutTimerFunc(1000 / 30, timer, 1);//다이머 시간 마다 갱신,  해준다 //30 frame 나온다.
									   //타이머가 작동하는데 다시 타이머를 동작시킴으로써 계쏙 작동하게 되는거지.
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

	drawControlPoints();
	drawControlLines();

	drawBezierCurve();//곡선 그리는 거 호출한다.

	if (isLeftMouseDown) drawMouseDropPoint();


	glutSwapBuffers(); //GLUT_DOUBLE 버퍼를 더블로해서 스왑해서사용할 것이다.
}

void SetupRC(void) {
	glClearColor(0.8f, 0.8f, 0.8f, 1.0f);
	glEnable(GL_DEPTH_TEST);
}

void init(void)
{

	isLeftMouseDown = false;
	controlPoint[0].x = -1.0f; controlPoint[0].y = -1.0f; controlPoint[0].z = 0.0f;
	controlPoint[1].x = -1.0f; controlPoint[1].y = 1.0f; controlPoint[1].z = 0.0f;
	controlPoint[2].x = 1.0f; controlPoint[2].y = 1.0f; controlPoint[2].z = 0.0f;
	controlPoint[3].x = 1.0f; controlPoint[3].y = -1.0f; controlPoint[3].z = 0.0f;


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
					 //근데 이게 보면 아는데 ,이러고 나서 좌표계를 초기화해줘야 하고 이런게 기억안날텐데...
					 //사실근데 안써도 상관없는거구나 ....좌표계를 조작하지 않은이상????
					 //여기에서 초기화 해주는 이유는 뭘까?? 어떤 좌표계를 초기화 시켜준다는 말이지??

	if (w <= h)
		glOrtho(-2.0, 2.0, -2.0*(float)h / (float)w,
			2.0*(float)h / (float)w, 10.0, -10.0);
	else
		glOrtho(-2.0*(float)w / (float)h, 2.0*(float)w / (float)h,
			-2.0, 2.0, 10.0, -10.0);
}

void mouse(int button, int state, int x, int y)
{
	if (button == GLUT_LEFT_BUTTON && state == GLUT_DOWN)
	{
		isLeftMouseDown = true;
		mouseDrop(x, y);
	}
	if (button == GLUT_LEFT_BUTTON && state == GLUT_UP)
	{
		isLeftMouseDown = false;
	}

	glutPostRedisplay();
}

void motion(int x, int y)
{
	if (isLeftMouseDown)
	{
		mouseDrop(x, y);
	}
	glutPostRedisplay();
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
								//glutKeyboardFunc(keyboard);//키보드 이벤트 콜백
	glutMouseFunc(mouse);
	glutMotionFunc(motion);
	//453, 948 //453개의 점과 948개의의 삼각형으로 나뉘어 져있다.....
	//저 점과 그점의 차이는 무엇인가???
	//

	//TODO 마우스를 이용해서 구현을 해보자
	//void drawBezierCurve() 얘가 곡선을 그리는건데 여기에서 들어오는 점을 바탕으로 곡선을 그리면 될 것같다.



	init();
	SetupRC();
	glutMainLoop();
	//기본적인 틀같은거 외우필요없고 시험에안나온다 시험볼떄 준다.
}
