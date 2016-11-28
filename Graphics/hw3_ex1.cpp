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

typedef pair<float *, float *> P;
vector<P> v;

int cnt = 0;
vector<vector<float *>> vNormal;

int pCnt, iCnt; // position, indices 
float **positions;// [453][3];
int **indices;//[948][3];

int fFlag = 1;
int vFlag = 0;

int pFlag=1;
int dFlag=0;
int sFlag=0;

//이건 배열로 둬서 만들어도 될 듯하다.
int cFlagIdx = 0;
float cutOff[5] = { 5,10,15,20,25};
int nFlagIdx = 0;
float shininess[4] = { 0,30,60,100 };

static float theta;

GLfloat pX, pY, pZ;

float tx, ty,tz;

void timer(int value)
{
	theta += 90;
	if (theta >= 360.0) theta =0;
	
	nFlagIdx += 1;
	if (nFlagIdx == 4) nFlagIdx = 0;

	cFlagIdx += 1;
	if (cFlagIdx == 5) cFlagIdx = 0;

	glutPostRedisplay();//화면 갱신이 필요할 때 이함수를 부르면 display 콜백 함수를 다시 한 번 실행
	glutTimerFunc(1000, timer, 1);//다이머 시간 마다 갱신,  해준다 //30 frame 나온다.
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

void CrossProduct(float* a, float* b, float* c, float* &r)
{
	r[0] = (b[1] - a[1]) * (c[2] - a[2]) - (b[2] - a[2]) * (c[1] - a[1]);
	r[1] = (b[2] - a[2]) * (c[0] - a[0]) - (b[0] - a[0]) * (c[2] - a[2]);
	r[2] = (b[0] - a[0]) * (c[1] - a[1]) - (b[1] - a[1]) * (c[0] - a[0]);
	Normalize(r);
}

void fDrawFill()
{
	glBegin(GL_TRIANGLES);
	{
		for (int i = 0; i < iCnt; i++)
		{
			float * n = new float[3];

			CrossProduct(positions[indices[i][0]], positions[indices[i][1]], positions[indices[i][2]], n);
			glNormal3fv(n);
			glVertex3fv(positions[indices[i][0]]);
			glVertex3fv(positions[indices[i][1]]);
			glVertex3fv(positions[indices[i][2]]);
		}
	}
	glEnd();
}

void vDrawFill()
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
void drawLines()
{
	glBegin(GL_TRIANGLES);
	{
		for (int i = 0; i < iCnt; i++)
		{
			float * n = new float[3];

			CrossProduct(positions[indices[i][0]], positions[indices[i][1]], positions[indices[i][2]], n);
			//glNormal3fv(n);

			float *loc = new float[3];

			for (int j = 0; j < 3; j++)
			{
				glVertex3fv(positions[indices[i][j]]);
				vNormal[indices[i][j]].push_back(n);//점 3개 에다가 다 넣어줘야한다.
				loc[j] = 0;//초기화 해줘야함.
				for (int k = 0; k < 3; k++) loc[j] += positions[indices[i][k]][j];
				loc[j] /= 3;
			}
			v.push_back(P(loc, n)); //여기에 벡터 그릴거 다들어간다.
		}
	}
	glEnd();
}

void drawNormalVectors()
{
	for (P p : v)
	{
		glBegin(GL_LINES);
		{
			glVertex3f(p.first[0], p.first[1], p.first[2]);//노말 벡터와 쌍인 애들이다.
			glVertex3f(p.second[0], p.second[1], p.second[2]);//얘와 합쳐 졌을때 의미를 
		}
		glEnd();
	}
}

void drawNormalVertexVectors()
{
	for (int idx = 0; idx < vNormal.size(); idx++)
	{//idx가 인덱스다. 점에 대한 인덱스임
		float x, y, z, m;
		m = x = y = z = 0;
		//Gouraud Shading을 한다.
		//점의 인덱스 IDX에를 포함했던 노말 백터들을 이터레이터 돌면서 고루쉐이딩을 해준다.
		for (float * f : vNormal[idx])
		{
			x += f[0];
			y += f[1];
			z += f[2];
			m += abs(f[0]) + abs(f[1]) + abs(f[2]);//단위 벡터로 만들어주기 위한.
		}

		glBegin(GL_LINES);
		{
			glVertex3f(x / m*1.5, y / m*1.5, z / m*1.5);
			glVertex3fv(positions[idx]);
		}
		glEnd();

	}
}


void lightSettings()
{
	//광원의 위치 마지막 인자가 0이면 directional 임
	//감쇠가 없어지면 점 광원이나 방향 광원 모두 같아짐
	glDisable(GL_LIGHT1);
	GLfloat position0[4] = { pX,pY,pZ,pFlag };
	glLightfv(GL_LIGHT0, GL_POSITION, position0);

	//광원의 색을 결정한다. 얘가 있어야 색이있는거고...
	GLfloat ambient0[4] = { 0.1, 0.1, 0.1, 1.0f };
	GLfloat diffuse0[4] = { 0.4, 0.4, 0.4, 1 };
	GLfloat specular0[4] = { 0.2 ,0.2,0.2, 1.0 };

	glLightfv(GL_LIGHT0, GL_AMBIENT, ambient0);
	glLightfv(GL_LIGHT0, GL_DIFFUSE, diffuse0);
	glLightfv(GL_LIGHT0, GL_SPECULAR, specular0);

	//광원의 감쇠
	glLightf(GL_LIGHT0, GL_CONSTANT_ATTENUATION, 0.2);
	glLightf(GL_LIGHT0, GL_LINEAR_ATTENUATION, 0.1);
	glLightf(GL_LIGHT0, GL_QUADRATIC_ATTENUATION, 0.05);

	GLfloat mat_ambient[4] = { 0.3,0.0,0.0,1.0 };
	GLfloat mat_diffuse[4] = { 1,0.5,0.5,1.0 };
	GLfloat mat_specular[4] = { 1,1,1,1 };
	GLfloat mat_shininess = shininess[nFlagIdx];


	//Material 머티리얼에는 GL_FORNT, GL_BACK ,GL_FRONT_AND_BACK이 존재한다. 주의
	//머티리얼을 specifying한다.
	glMaterialfv(GL_FRONT, GL_AMBIENT, mat_ambient);
	glMaterialfv(GL_FRONT, GL_DIFFUSE, mat_diffuse);
	glMaterialfv(GL_FRONT, GL_SPECULAR, mat_specular);
	glMaterialf(GL_FRONT, GL_SHININESS, mat_shininess);
}

void spotLightSettings()
{

	glEnable(GL_LIGHT1);
	
	GLfloat position0[4] = { pX,pY,pZ,1 };
	glLightfv(GL_LIGHT1, GL_POSITION, position0);

	//광원의 색을 결정한다. 얘가 있어야 색이있는거고...
	GLfloat ambient0[4] = { 0.1, 0.1, 0.1, 1.0f };
	GLfloat diffuse0[4] = { 0.5, 0.5, 0.5, 1 };
	GLfloat specular0[4] = { 0.2 ,0.2,0.2, 1.0 };

	glLightfv(GL_LIGHT1, GL_AMBIENT, ambient0);
	glLightfv(GL_LIGHT1, GL_DIFFUSE, diffuse0);
	glLightfv(GL_LIGHT1, GL_SPECULAR, specular0);

	//광원의 감쇠
	glLightf(GL_LIGHT1, GL_CONSTANT_ATTENUATION, 0.2);
	glLightf(GL_LIGHT1, GL_LINEAR_ATTENUATION, 0.1);
	glLightf(GL_LIGHT1, GL_QUADRATIC_ATTENUATION, 0.05);

	GLfloat spot_direction[3] = { -1,-1 ,-1 };
	glLightfv(GL_LIGHT1, GL_SPOT_DIRECTION, spot_direction);
	glLightf(GL_LIGHT1, GL_SPOT_CUTOFF, cutOff[cFlagIdx]);
	glLightf(GL_LIGHT1, GL_SPOT_EXPONENT, 30);//이렇게 두면..//크게 한상태로하면 보임

	GLfloat mat_ambient[4] = { 0.3,0.0,0.0,1.0 };
	GLfloat mat_diffuse[4] = { 1,0.5,0.5,1.0 };
	GLfloat mat_specular[4] = { 1,1,1,1 };
	GLfloat mat_shininess = shininess[nFlagIdx];


	//Material 머티리얼에는 GL_FORNT, GL_BACK ,GL_FRONT_AND_BACK이 존재한다. 주의
	//머티리얼을 specifying한다.
	glMaterialfv(GL_FRONT, GL_AMBIENT, mat_ambient);
	glMaterialfv(GL_FRONT, GL_DIFFUSE, mat_diffuse);
	glMaterialfv(GL_FRONT, GL_SPECULAR, mat_specular);
	glMaterialf(GL_FRONT, GL_SHININESS, mat_shininess);

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

	gluLookAt(0.2, 0.2, 0.2, 0, 0, 0, 0, 1, 0); //보는 시점이다.


	float cos_th = cos(theta * 3.14159 / 180);
	float sin_th = sin(theta * 3.14159 / 180);

	float rotateMatrix[16] = {
		cos_th,	0,	-sin_th,	0,
		0,		1,	0,		0,
		sin_th,0,	cos_th,	0,
		0,		0,	0,		1
	};

	float translateMatrixPre[16] = {
		1,	0,	0,	0,
		0,	1,	0,	0,
		0,	0,	1,	0,
		tx,ty,tz,1,
	};

	float translateMatrixPost[16] = {
		1,	0,	0,	0,
		0,	1,	0,	0,
		0,	0,	1,	0,
		-tx,-ty,-tz,1,
	};

	glPushMatrix();
	{
		glMultMatrixf(translateMatrixPre);// 1,1,1 점으로 이동
		glMultMatrixf(rotateMatrix);// theta 만큼 회전
		glMultMatrixf(translateMatrixPost);//-1,-1,-1 원점으로 이동

		if (sFlag)
		{
			spotLightSettings();
		}
		else {
			lightSettings();
		}
	}
	glPopMatrix();
	


	glEnable(GL_POLYGON_OFFSET_FILL);// offset 쓰려면 enable 해줘야함
	glPolygonOffset(10, 0);//factor, units 이다.
						   //factor > create a variable depth offset for each polygon. 뎁스 오프셋을 줄수 있는거다 각 폴리곤에
						   //factor > Specifies a scale factor that is used to create a variable depth offset for each polygon. The initial value is zero.
						   //units > Specifies a value that is multiplied by an implementation-specific value to create a constant depth offset. The initial value is 0.

	glPolygonMode(GL_FRONT, GL_FILL);
	glColor3f(0.5, 0.5, 0.5);

	if (fFlag) fDrawFill(); //draw
	else vDrawFill();

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
	glEnable(GL_NORMALIZE);
	glCullFace(GL_BACK);


	//광원 활성화
	glEnable(GL_LIGHTING);
	glEnable(GL_LIGHT0);
	glShadeModel(GL_SMOOTH);

	lightSettings();
	//spotLightSettings();
}

void init(void)
{
	theta=0;
	pX = pY = pZ = 1;
	tx = ty = tz = 0;
	glutTimerFunc(1000, timer, 1);
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
			-1.0, 1.0, -1.0, 1.0);
}


void readFile()
{
	freopen("bunny_origin.txt", "r", stdin);

	cin >> pCnt >> iCnt;

	positions = new float*[pCnt]; ///453
	indices = new int *[iCnt]; ///948
	vNormal.resize(pCnt);
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
	cout << "input key = " << c << endl;
	switch (c)
	{
	case VK_ESCAPE:
		exit(0);
		break;

	case 'f'://Draw face normal vectors. ‘f’ key to toggle. (a)
		fFlag = 1;
		break;

	case 'v'://Draw vertex normal vectors.‘v’ key to toggle. (b)
		fFlag = 0;
		break;
	case 'p': // toggle point
		sFlag = 0; //이렇게 되면 아예다 꺼지는 상황 발생
		pFlag = 1;
		break;

	case 'd'://toggle directional
		pFlag = sFlag = 0;
		pFlag = 0;
		break;

	case 's'://toggle spot 각각 나머지는 모두 꺼지게하자.
		pFlag = 1;
		if (sFlag) sFlag = 0;
		else sFlag = 1;

		break;

	case 'c'://토글이 된다. Change the cut-off angle of the spot light
		//5 10 15 20 25 1초에 5도 정도씩 변화하면 될듯하다.
		cFlagIdx += 1;
		if (cFlagIdx == 5) cFlagIdx = 0;

		break;

	case 'n'://Change the shininess coefficient of the spot light 
		//n 키로 토글이 가능하면서 
		//4초당 하나의 사이클 4개 해서 하면 될듯하고
		nFlagIdx += 1;
		if (nFlagIdx == 4) nFlagIdx = 0;

	case 'z':
		if (tx == 1) tx = ty = tz = 0;
		else tx = ty = tz = 1;

		cout << tx << ", " << ty << ", " << tz << endl;

		break;
	}

	glutPostRedisplay();
}


void main(int argc, char * argv[])
{
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB);
	glutInitWindowSize(800, 800);
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
