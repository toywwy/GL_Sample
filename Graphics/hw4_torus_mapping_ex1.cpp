#define _CRT_SECURE_NO_WARNINGS
#pragma warning(disable:4996)


#include<iostream>
#include<Windows.h> //이건 GUI 때문에 필요한건데.
#include<gl/GL.h>
#include<gl/GLU.h>
#include<gl/glut.h> //이건 유틸리티 툴킷인데, 사용자의 입력이나 화면 윈도우를 제어하기 위해서다.
#include <math.h> //삼각함수 쓰려고
#include<vector>
#include<string>

#define SIZE 512
#define TORUS 36
#define CIRCLE 18

/*
* gl.h (OpenGL Core Library) - 렌더링 기능을 제공하는 라이브러리
* glu.h (OpenGL Utility Library) - GL 라이브러러의 도우미 역할
* glut.h (OpenGL Utility Toolkit) - 사용자 입력을 받아들이거나 화면 윈도우를 제어하기 위한 라이브러리
*/

using namespace std;

float theta = 0;

//하나만 선언해놓고 재활용 해서 쓸거다 어차피 textures에 등록하고 나면 필요 없게 되니까!
unsigned char woodMaterial[SIZE][SIZE][3];
unsigned char grayMaterial[SIZE][SIZE];

GLuint textures[6];

int flag = 0;

struct Point {
	float p[3];
	Point() {}
	Point(float x, float y, float z)
	{
		this->p[0] = x; this->p[1] = y; this->p[2] = z;
	}


	void SetPoint(float a, float b, float c)
	{
		this->p[0] = a; this->p[1] = b; this->p[2] = c;
	}

	Point operator- (Point a)
	{
		Point retP;
		for(int i = 0 ; i < 3; i++) retP.p[i] = this->p[i] - a.p[i];
		return retP;
	}

	friend Point CrossProduct(Point a, Point b)//friend 선언 해놓으면 전역적으로 사용이 가능하다.
	{
		Point p(
			a.p[1] * b.p[2] - a.p[2] * b.p[1],
			a.p[2] * b.p[0] - a.p[0] * b.p[2],
			a.p[0] * b.p[1] - a.p[1] * b.p[0]
		);

		return getNormal(p);
	}

	friend Point getNormal(Point np) {
		float x, y, z, len = 0;

		x = np.p[0];
		y = np.p[1];
		z = np.p[2];

		len = x*x + y*y + z*z;
		len = sqrt(len);
		Point p(x / len, y / len, z / len);

		return p;
	}
};


vector<vector<Point>> v;
vector<Point> nv[TORUS][CIRCLE];//[j][i]에 다가 노말 값들을 저장해 놓는다.

void setTorusPoints()
{
	float move = 1.0;//원점에서 점을 이동한 것
	float r = 0.5;//반지름

	for (int yRot = 0; yRot <= 360; yRot = yRot + 10)//y축 회전 x,z만 변형 한다.
	{
		vector<Point> circlePoints;
		for (int zRot = 0; zRot <= 360; zRot += 20)//Circle 원에 관한것이다. // 얘는 z축 회전 x,y만 변형
		{
			//z축 회전 원을 만드는 것에 관해서는 점을 이동시켜줘야한다. +move 해준거
			float x = (move +  r* cos(zRot*3.141592 / 180.0)) * cos(yRot * 3.141592 / 180.0);
			float y = r * sin(zRot*3.141592 / 180.0);//얘는 변형이 z축 회전 에만 되면 된다.
			float z = (move + r * cos(zRot*3.141592 / 180.0)) * sin(yRot * 3.141592 / 180.0);
			//결국 이런 원리로 만들어 지는 것이다.

			circlePoints.push_back(Point(x, y, z));
		}
		v.push_back(circlePoints);//36개의 원이 존재한다.
	}
}


Point getVertexNormal(int j, int i)
{
	float x, y, z,len;

	len = x = y = z = 0 ;

	for (Point np : nv[j][i])
	{
		x += np.p[0];
		y += np.p[1];
		z += np.p[2];
	}

	len = abs(x) + abs(y) + abs(z);
	/*len = sqrt(len);//신기하게 얘를 해줄수록 빛의 밝기가 강해진다.
	len = sqrt(len);*/
	Point p(x / len, y / len, z / len);

	return p;
}

void getNormal()
{
	for (int j = 0; j < TORUS; j++)
	{
		for (int i = 0; i < CIRCLE; i++)
		{
			//범위초과시 0으로 바꾸어 줘야 런타임 에러 막는다.
			int I = (i + 1) % CIRCLE;
			int J = (j + 1) % TORUS;

				
			//glVertex3fv(v[j][i].p);//p1
			//glVertex3fv(v[j][i + 1].p);//얘를 P0로 쓰면된다.
			//glVertex3fv(v[j + 1][i + 1].p);//p2

			//(p1-p0) x (p2-p0) 해서 j,i에서의 법선 벡터를 얻게 되는 것이다. 
			//[36][18].push(normal) 나중에 이 noraml 값에 대해서 평균 내고 정규화 하면 되는 것이다.

			Point l1 = v[j][i] - v[j][i+1];
			Point l2 = v[j+1][i+1] - v[j][i+1];//반시계로 돌려야 하니까 얘가 먼저

			Point np = CrossProduct(l2,l1);//외적은 순서에 영향을 미친다!!! 안쪽 법선벡터인지 바깥쪽 법선 벡터인지.
			nv[j][i].push_back(np);
			nv[J][i].push_back(np);
			nv[j][I].push_back(np);
			nv[J][I].push_back(np);

		}
	}

	for (int j = 0; j < TORUS; j++)
		for (int i = 0; i < CIRCLE; i++)
			nv[j][i][0] = getVertexNormal(j,i);

}

void drawTorus()
{
	glPushMatrix();

	float s = 1.0 / TORUS; //매핑할떄 가로의 하나의 텍셀의 길이이다.
	//크기 * TORUS를 하면 전체 매핑을하는 것이다.
	float t = 1.0 / CIRCLE;//매핑할떄 가로의 하나의 텍셀의 크기

	glBindTexture(GL_TEXTURE_2D, textures[flag]);//텍스쳐를 매핑하는데 필수가 되는 요소이다.
	//어떠한 텍스쳐를 바인딩해서 사용할 것인가를 정의한다.
	//textures[flag] 여기에는 텍스쳐가 저장이 되어있다.

	//사실 가장 쉽게 텍스쳐 매핑 하는게 GL QUADS로 하는 방법이다.
	for (int j = 0; j < TORUS; j++)
	{
		for (int i = 0; i < CIRCLE; i++)
		{
			glBegin(GL_QUADS);//쿼드는 무조건 사각형 그리면 ... 순서가 상관없는건가??
			{// 이론상은 반시계로 돌아야 하니까 (j1,i1)->(j2,i1)->(j2,i2)->(j1,i2) 이렇게 해서 한바퀴를 돌게 되는 구조이다.

				int I = (i + 1) % CIRCLE;
				int J = (j + 1) % TORUS;

				glNormal3fv(nv[j][i][0].p);
				glTexCoord2f(s*j, t*i);
				glVertex3fv(v[j][i].p);

				glNormal3fv(nv[j][I][0].p);
				glTexCoord2f(s*j, t*(i + 1));
				glVertex3fv(v[j][i + 1].p);

				glNormal3fv(nv[J][I][0].p);
				glTexCoord2f(s*(j + 1), t*(i + 1));
				glVertex3fv(v[j + 1][i + 1].p);

				glNormal3fv(nv[J][I][0].p);
				glTexCoord2f(s*(j + 1), t*i);
				glVertex3fv(v[j + 1][i].p);
			}
			glEnd();
		}
	}

	glPopMatrix();
}


void timer(int value)
{
	glutPostRedisplay();//화면 갱신이 필요할 때 이함수를 부르면 display 콜백 함수를 다시 한 번 실행
	glutTimerFunc(1000, timer, 1);//다이머 시간 마다 갱신,  해준다 //30 frame 나온다.
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
	gluLookAt(0, 2, 2, 0, 0, 0, 0, 1.0, 0); //보는 시점이다.
											//drawAxes();

	drawTorus();
	glutSwapBuffers(); //GLUT_DOUBLE 버퍼를 더블로해서 스왑해서사용할 것이다.
}

void SetupRC(void) {
	glClearColor(1.0f, 1.0f, 1.0f, 1.0f);
	glEnable(GL_DEPTH_TEST);
	glEnable(GL_COLOR_MATERIAL);
	glEnable(GL_TEXTURE_2D);

	//노말이제대로 적용이 됐으면 라이팅이 제대로 먹혔을 것이다.
	glEnable(GL_LIGHTING);
	glEnable(GL_LIGHT0);
}


int readTexture(char *filename) {

	FILE *texture = fopen(filename, "rb");


	if (texture == NULL) {
		puts("File open failed");
		return -1;
	}
	else {
		puts("File open success");
		fread(woodMaterial, sizeof(unsigned char), SIZE * SIZE * 3, texture);

		fclose(texture);

		return 0;
	}

}

int readGrayTexture(char *filename) {


	FILE *texture = fopen(filename, "rb");

	if (texture == NULL) {
		puts("File open failed");
		return -1;
	}
	else {
		puts("File open success");
		fread(grayMaterial, sizeof(unsigned char), SIZE * SIZE, texture);
		fclose(texture);

		return 0;
	}

}

void init(void)
{

	//initCircle();
	setTorusPoints();
	getNormal();

	//readTexture(unsigned char *src, int width, int height, char *filename) {
	vector<char *> fileNames;
	fileNames.push_back("material/wood.raw");
	fileNames.push_back("material/check.raw");
	fileNames.push_back("material/donut.raw");
	fileNames.push_back("material/marble.raw");

	//여기서 부터는 그레이 스케일
	fileNames.push_back("material/logo2.raw");
	fileNames.push_back("material/grayscale_ornament.raw");

	glGenTextures(6, textures);//여러개의 텍스쳐를 사용하기위한 textures를 Generate한다.


	for (int i = 0; i < 6; i++)
	{
		if (i < 4)
		{
			readTexture(fileNames[i]);
			glBindTexture(GL_TEXTURE_2D, textures[i]);

			glTexImage2D(GL_TEXTURE_2D, 0, 3, 512, 512, 0, GL_RGB, GL_UNSIGNED_BYTE, woodMaterial);

			glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP);
			glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP);

			//GL_NEAREST is generally faster than GL_LINEAR, 
			//but it can produce textured images with sharper edges because the transition between texture elements is not as smooth. The initial value of GL_TEXTURE_MAG_FILTER is GL_LINEAR.
			glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
			glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
		}
		else
		{
			readGrayTexture(fileNames[i]);

			glBindTexture(GL_TEXTURE_2D, textures[i]);

			glTexImage2D(GL_TEXTURE_2D, 0, 3, 512, 512, 0, GL_LUMINANCE, GL_UNSIGNED_BYTE, grayMaterial);

			glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP);
			glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP);

			//GL_NEAREST is generally faster than GL_LINEAR, 
			//but it can produce textured images with sharper edges because the transition between texture elements is not as smooth. The initial value of GL_TEXTURE_MAG_FILTER is GL_LINEAR.
			glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
			glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
		}

	}
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

	if (w <= h)
		glOrtho(-2.0, 2.0, -2.0*(float)h / (float)w,
			2.0*(float)h / (float)w, -10.0, 10.0);
	else
		glOrtho(-2.0*(float)w / (float)h, 2.0*(float)w / (float)h,
			-2.0, 2.0, -10.0, 10.0);
}


void keyboard(unsigned char c, int x, int y)
{
	cout << "input key = " << c << endl;
	switch (c)
	{
	case VK_ESCAPE:
		exit(0);
		break;

	case 's':
		flag = 0;
		break;
	case 't':
		flag = 1;
		break;
	case 'u':
		flag = 2;
		break;
	case 'v':
		flag = 3;
		break;
	case 'w':
		flag = 4;
		break;
	case 'x':
		flag = 5;
		break;
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
	glutKeyboardFunc(keyboard);//키보드 이벤트 콜백


	init();
	SetupRC();
	glutMainLoop();
	//기본적인 틀같은거 외우필요없고 시험에안나온다 시험볼떄 준다.
}
