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

using namespace std;

typedef GLfloat point3[3];

static float theta[3];

int flag = 1;


void DrawTriangle(point3 &a, point3 &b, point3 &c);
void Normalize(point3 &p);
void DivideTriangle(point3 &a, point3 &b, point3 &c, int n);
void CrossProduct(point3 &a, point3 &b, point3 &c, point3 &r);

void Normalize(point3 &p)
{
	double d = p[0] * p[0] + p[1] * p[1] + p[2] * p[2];

	if (d > 0)
	{
		float len = (float)(1.0 / sqrt(d));
		p[0] *= len;
		p[1] *= len;
		p[2] *= len;
	}
}


void CrossProduct(point3 &a, point3 &b, point3 &c, point3 &r)
{
	r[0] = (b[1] - a[1]) * (c[2] - a[2]) - (b[2] - a[2]) * (c[1] - a[1]);
	r[1] = (b[2] - a[2]) * (c[0] - a[0]) - (b[0] - a[0]) * (c[2] - a[2]);
	r[2] = (b[0] - a[0]) * (c[1] - a[1]) - (b[1] - a[1]) * (c[0] - a[0]);

	Normalize(r);//유닛 벡터로 만드는건가 법선 벡터를
}

void DivideTriangle(point3 &a, point3 &b, point3 &c, int n)
{
	if (n > 0)
	{
		point3 v1, v2, v3;
		for (register int i = 0; i < 3; i++)
		{
			v1[i] = a[i] + b[i];
			v2[i] = b[i] + c[i];
			v3[i] = c[i] + a[i];
		}

		Normalize(v1);
		Normalize(v2);
		Normalize(v3);

		DivideTriangle(a, v1, v3, n - 1);
		DivideTriangle(b, v2, v1, n - 1);
		DivideTriangle(c, v3, v2, n - 1);
		DivideTriangle(v1, v2, v3, n - 1);
	}
	else
		DrawTriangle(a, b, c);
}



void drawSmooth(point3 &a, point3 &b, point3&c)
{
	//Smooth Shading
	glEnable(GL_LIGHTING);//꼭 LIGHT 켜줘야한다.

	glBegin(GL_TRIANGLES);
	{
		glNormal3fv(a); // 노말라이즈를 이용해서 Shading을 하는것이다.
		glVertex3fv(a);

		glNormal3fv(b);
		glVertex3fv(b);

		glNormal3fv(c);
		glVertex3fv(c);
	}
	glEnd();
}
void drawFlat(point3 &a, point3 &b, point3&c)
{

	//Flat Shading
	glEnable(GL_LIGHTING);//꼭 LIGHT 켜줘야한다.

	point3 n;//단위벡터??? 이게 뭐지 그냥 넣으면 되는건가??
	CrossProduct(a, b, c, n);

	glBegin(GL_TRIANGLES);
	{
		glNormal3fv(n);
		glVertex3fv(a);
		glVertex3fv(b);
		glVertex3fv(c);
	}
	glEnd();

}
void drawWireframe(point3 &a, point3 &b, point3&c)
{
	//WireFrame
	glDisable(GL_LIGHTING);
	glDisable(GL_LIGHT0);

	glBegin(GL_LINE_LOOP);
	{
		//glNormal3fv(a); //노말라이즈를 이용해서 Shading을 하는것이다.
		glVertex3fv(a);

		//glNormal3fv(b);
		glVertex3fv(b);

		//glNormal3fv(c);
		glVertex3fv(c);
	}
	glEnd();
}

void DrawTriangle(point3 &a, point3 &b, point3&c)
{
	if (flag == 1) drawWireframe(a, b, c);
	else if (flag == 2) drawFlat(a, b, c);
	else drawSmooth(a, b, c);
}

void timer(int value)
{
	glutPostRedisplay();//화면 갱신이 필요할 때 이함수를 부르면 display 콜백 함수를 다시 한 번 실행
	glutTimerFunc(1000 / 30, timer, 1);//다이머 시간 마다 갱신,  해준다 //30 frame 나온다.
									   //타이머가 작동하는데 다시 타이머를 동작시킴으로써 계쏙 작동하게 되는거지.
}

void RenderScene(void)
{
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	glMatrixMode(GL_MODELVIEW);//MODELVEIW는 그리는거고 projection은 투영하는거다.
	glLoadIdentity();

	gluLookAt(0, 0, 2.0, 0, 0, 0, 0, 1.0, 0);

	glEnable(GL_LIGHT0);
	glColor3f(0, 0, 0);
	glRotatef(theta[2], 0, 0, 1);
	glRotatef(theta[1], 0, 1, 0);
	glRotatef(theta[0], 1, 0, 0);

	point3 v[4] = {
		{ 0,			0,			1 },
		{ 0,			0.942809,	-0.333333 },
		{ -0.816497,	-0.471405,	-0.333333 },
		{ 0.816497,		-0.471405,	-0.333333 }
	};

	int n = 3;

	DivideTriangle(v[0], v[1], v[2], n);
	DivideTriangle(v[0], v[2], v[3], n);
	DivideTriangle(v[0], v[3], v[1], n);
	DivideTriangle(v[3], v[2], v[1], n);

	glutSwapBuffers(); //GLUT_DOUBLE 버퍼를 더블로해서 스왑해서사용할 것이다.
}

void SetupRC(void) {
	glClearColor(1.0f, 1.0f, 1.0f, 1.0f);
	glEnable(GL_DEPTH_TEST);
}

void init(void)
{
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
			2.0*(float)h / (float)w, -10.0, 10.0);
	else
		glOrtho(-2.0*(float)w / (float)h, 2.0*(float)w / (float)h,
			-2.0, 2.0, -10.0, 10.0);
}


void menu(int item)
{
	flag = item;
	glutPostRedisplay();
}

void menuInit()
{
	glutCreateMenu(menu); //메뉴마다 옵션을 넣어줘야한다. 그리고 키보드에 매핑도 가능한것이지..
	glutAddMenuEntry("WireFrame", 1);
	glutAddMenuEntry("Flat", 2);
	glutAddMenuEntry("Smooth", 3);
	//오른쪽 버튼 누르면 메뉴가 뜬다.
	glutAttachMenu(GLUT_RIGHT_BUTTON);
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
	menuInit();
	init();
	SetupRC();
	glutMainLoop();
	//기본적인 틀같은거 외우필요없고 시험에안나온다 시험볼떄 준다.
}
