#include<iostream>
#include<Windows.h> //이건 GUI 때문에 필요한건데.
#include<gl/GL.h>
#include<gl/GLU.h>
#include<gl/glut.h> //이건 유틸리티 툴킷인데, 사용자의 입력이나 화면 윈도우를 제어하기 위해서다.
#include<math.h> //삼각함수 쓰려고
#include<vector> 

#define _CRT_SECURE_NO_WARNINGS
#pragma warning(disable:4996)
/*
* gl.h (OpenGL Core Library) - 렌더링 기능을 제공하는 라이브러리
* glu.h (OpenGL Utility Library) - GL 라이브러러의 도우미 역할
* glut.h (OpenGL Utility Toolkit) - 사용자 입력을 받아들이거나 화면 윈도우를 제어하기 위한 라이브러리
*/

using namespace std;

typedef float point3[3];

typedef pair<point3, point3> P;
vector<P> v;

vector<vector<float *>> vNormal;


int pCnt, iCnt; // position, indices 
float **positions;// [453][3];
int **indices;//[948][3];



void timer(int value)
{
	glutPostRedisplay();//화면 갱신이 필요할 때 이함수를 부르면 display 콜백 함수를 다시 한 번 실행
	glutTimerFunc(1000 / 30, timer, 1);//다이머 시간 마다 갱신,  해준다 //30 frame 나온다.
									   //타이머가 작동하는데 다시 타이머를 동작시킴으로써 계쏙 작동하게 되는거지.
}

void Normalize(point3 p)
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

//이걸 가지고 그릴수있을 것 같애
void CrossProduct(float* a, float* b, float* c, float* &r)
{
	r[0] = (b[1] - a[1]) * (c[2] - a[2]) - (b[2] - a[2]) * (c[1] - a[1]);
	r[1] = (b[2] - a[2]) * (c[0] - a[0]) - (b[0] - a[0]) * (c[2] - a[2]);
	r[2] = (b[0] - a[0]) * (c[1] - a[1]) - (b[1] - a[1]) * (c[0] - a[0]);
	Normalize(r);
}

void drawLines()
{
	glBegin(GL_TRIANGLES);
	{
		for (int i = 0; i < iCnt; i++)
		{
			float * n = new float[3];

			CrossProduct(positions[indices[i][0]], positions[indices[i][1]], positions[indices[i][2]], n);
			for (int j = 0; j < 3; j++)
			{
				glVertex3fv(positions[indices[i][j]]);
				vNormal[indices[i][j]].push_back(n);
			}
		}
	}
	glEnd();
}
void drawFill()
{
	glBegin(GL_TRIANGLES);
	{
		for (int i = 0; i < iCnt; i++)
		{
			glNormal3fv(positions[indices[i][0]]);//이 노말 값 이용못하나?
			glVertex3fv(positions[indices[i][0]]);

			glNormal3fv(positions[indices[i][1]]);
			glVertex3fv(positions[indices[i][1]]);

			glNormal3fv(positions[indices[i][2]]);
			glVertex3fv(positions[indices[i][2]]);
		}
	}
	glEnd();
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

	glEnable(GL_LIGHT0);
	glEnable(GL_LIGHT1);

	//GLfloat position0[4] = { -1,-1,-1,1 };
	//glLightfv(GL_LIGHT0, GL_POSITION, position0);
	gluLookAt(0.2, 0.2, 0.2, 0, 0, 0, 0, 1, 0); //보는 시점이다.

	GLfloat position1[4] = { -1,-1,-1,1 };
	glLightfv(GL_LIGHT1, GL_POSITION, position1);


	glEnable(GL_POLYGON_OFFSET_FILL);// offset 쓰려면 enable 해줘야함
	glPolygonOffset(10, 0);//factor, units 이다.
						   //factor > create a variable depth offset for each polygon. 뎁스 오프셋을 줄수 있는거다 각 폴리곤에
						   //factor > Specifies a scale factor that is used to create a variable depth offset for each polygon. The initial value is zero.
						   //units > Specifies a value that is multiplied by an implementation-specific value to create a constant depth offset. The initial value is 0.

	glPolygonMode(GL_FRONT, GL_FILL);
	glColor3f(0.5, 0.5, 0.5);
	drawFill(); //draw
	glDisable(GL_POLYGON_OFFSET_FILL);

	//검정 색으로 선 그리는 부분
	glColor3f(0, 0, 0);
	glPolygonMode(GL_FRONT, GL_LINE);
	drawLines();//draw


	glutSwapBuffers(); //GLUT_DOUBLE 버퍼를 더블로해서 스왑해서사용할 것이다.
}

void SetupRC(void) {
	glClearColor(1.0f, 1.0f, 1.0f, 1.0f);
	glEnable(GL_DEPTH_TEST);

	//BACK을 CULL 하겠다 안그릴거야.
	glEnable(GL_CULL_FACE);
	glEnable(GL_LIGHTING);
	glEnable(GL_NORMALIZE);


	glCullFace(GL_BACK);
}

void init(void)
{
	//glutTimerFunc(1000 / 30, timer, 1);
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
		glOrtho(-1.0, 1.0, -1.0*(float)h / (float)w,
			1.0*(float)h / (float)w, -1.0, 1.0);
	else
		glOrtho(-1.0*(float)w / (float)h, 1.0*(float)w / (float)h,
			-1.1, 1.0, -1.0, 1.0);

	//	glFrustum(-5,5, -5, 5, 5, 100);

}


void readFile()
{
	freopen("bunny_origin.txt", "r", stdin);

	cin >> pCnt >> iCnt;

	positions = new float*[pCnt]; ///453
	indices = new int *[iCnt]; ///948

	vNormal.resize(pCnt);//점 갯수만큼 생성해준다.

	for (int i = 0; i < pCnt; i++) positions[i] = new float[3];
	for (int i = 0; i < iCnt; i++) indices[i] = new int[3];
	for (int i = 0; i < pCnt; i++) cin >> positions[i][0] >> positions[i][1] >> positions[i][2];

	for (int i = 0; i < iCnt; i++)
	{
		cin >> indices[i][0] >> indices[i][1] >> indices[i][2];
		indices[i][0]--; indices[i][1]--; indices[i][2]--;
	}
}

void keyboard(unsigned char c, int x, int y)
{
	switch (c)
	{
	case VK_ESCAPE:
		exit(0);
		break;
	//Default 가 wireframe 모드이고??

	case 'f'://Draw face normal vectors. ‘f’ key to toggle. (a)

		break;

	case 'v'://Draw vertex normal vectors.‘v’ key to toggle. (b)

		break;

	case 'w' ://‘w’ key to turn on/off the wireframe mode
		//wireframe 과 normal vector를 꺼주면 된다.
		//normal vector는 face와 vertex가 있다.
		break;

	}
}

void main(int argc, char * argv[])
{
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB);
	glutInitWindowSize(600, 600);
	glutCreateWindow("Simple");

	readFile();

	glutDisplayFunc(RenderScene);//렌더링되는 부분
	glutReshapeFunc(ChangeSize);//SIZE가 바뀔마다 호출이 된다.
								//	glutSpecialFunc(specialKeyboard);//스페셜 키보드 이벤트를 콜백
	glutKeyboardFunc(keyboard);//키보드 이벤트 콜백
	init();
	SetupRC();
	glutMainLoop();
	//기본적인 틀같은거 외우필요없고 시험에안나온다 시험볼떄 준다.
}
