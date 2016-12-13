#include<iostream>
#include<Windows.h> //이건 GUI 때문에 필요한건데.
#include<gl\GL.h>
#include<gl\GLU.h>
#include<gl\glut.h>
#include<gl\GLAUX.H>
#include<math.h> //삼각함수 쓰려고

/*
LNK2019 ERR >> Property -> Configuration Properties -> Linker -> Input -> Additional Dependencies -> add “LEGACY_STDIO_DEFINITIONS.LIB”
+glaux.lib 추가해줘야한다.
*/

/*
* gl.h (OpenGL Core Library) - 렌더링 기능을 제공하는 라이브러리
* glu.h (OpenGL Utility Library) - GL 라이브러러의 도우미 역할
* glut.h (OpenGL Utility Toolkit) - 사용자 입력을 받아들이거나 화면 윈도우를 제어하기 위한 라이브러리
*/

using namespace std;


static float vertices[8][3], colors[8][4], normals[6][3];
GLubyte m_image[64][64][3];

AUX_RGBImageRec *texRec[2];
GLuint texID[2];

void m_v()
{
	vertices[0][0] = -1; vertices[0][1] = -1; vertices[0][2] = 1; vertices[1][0] = -1; vertices[1][1] = 1; vertices[1][2] = 1; vertices[2][0] = 1; vertices[2][1] = 1; vertices[2][2] = 1; vertices[3][0] = 1; vertices[3][1] = -1; vertices[3][2] = 1; vertices[4][0] = -1; vertices[4][1] = -1; vertices[4][2] = -1; vertices[5][0] = -1; vertices[5][1] = 1; vertices[5][2] = -1; vertices[6][0] = 1; vertices[6][1] = 1; vertices[6][2] = -1; vertices[7][0] = 1; vertices[7][1] = -1; vertices[7][2] = -1;
	colors[0][0] = 0; colors[0][1] = 0; colors[0][2] = 1; colors[0][3] = 1; colors[1][0] = 0; colors[1][1] = 1; colors[1][2] = 1; colors[1][3] = 1; colors[2][0] = 1; colors[2][1] = 1; colors[2][2] = 1; colors[2][3] = 1; colors[3][0] = 1; colors[3][1] = 0; colors[3][2] = 1; colors[3][3] = 1; colors[4][0] = 0; colors[4][1] = 0; colors[4][2] = 0; colors[4][3] = 1; colors[5][0] = 0; colors[5][1] = 1; colors[5][2] = 0; colors[5][3] = 0; colors[6][0] = 1; colors[6][1] = 1; colors[6][2] = 0; colors[6][3] = 1; colors[7][0] = 1; colors[7][1] = 0; colors[7][2] = 0; colors[7][3] = 1;
	normals[0][0] = 0; normals[0][1] = 0; normals[0][2] = 1;// (0, 3, 2, 1) normals[1][0] = 0;normals[1][1] = 1;normals[1][2] = 0;// (1, 2, 6, 5) normals[2][0] = 1;normals[2][1] = 0;normals[2][2] = 0;// (2, 3, 7, 6) normals[3][0] = 0;normals[3][1] = -1;normals[3][2] = 0;// (3, 0, 4, 7) normals[4][0] = 0;normals[4][1] = 0;normals[4][2] = -1;// (4, 5, 6, 7) normals[5][0] = -1;normals[5][1] = 0;normals[5][2] = 0;// (5, 4, 0, 1)
}

void drawCube(int a, int b, int c, int d)
{
	glBegin(GL_QUADS);

	glNormal3fv(normals[a]);//이거 필요가 없는데 왜있는건지 모르겠네..
							//x-y축을 어떻게 나타내느냐를 보는게 좋을듯...

	glColor4fv(colors[a]);
	glVertex3fv(vertices[a]);

	glColor4fv(colors[b]);
	glVertex3fv(vertices[b]);

	glColor4fv(colors[c]);
	glVertex3fv(vertices[c]);

	glColor4fv(colors[d]);
	glVertex3fv(vertices[d]);

	glEnd();
}

void timer(int value)
{
	glutPostRedisplay();//화면 갱신이 필요할 때 이함수를 부르면 display 콜백 함수를 다시 한 번 실행
	glutTimerFunc(1000 / 30, timer, 1);//다이머 시간 마다 갱신,  해준다 //30 frame 나온다.
									   //타이머가 작동하는데 다시 타이머를 동작시킴으로써 계쏙 작동하게 되는거지.
}
void drawPlanXY(int w, int h)
{
	float size = 8;

	if (w > h) size = 8 * w / h;
	else size = 8 * h / w;

	int z = 0;
	glColor3f(255, 255, 255);
	glBegin(GL_QUADS);

	glTexCoord2f(0.0, 0.0);
	glVertex3f(0, 0, z);

	glTexCoord2f(5, 0.0);
	glVertex3f(size, 0, z);

	glTexCoord2f(5, 5);
	glVertex3f(size, size, z);

	glTexCoord2f(0.0, 5);
	glVertex3f(0, size, z);


	glEnd();

}
void drawPlanYZ(int w, int h)
{
	float size = 8;

	if (w > h) size = 8 * w / h;
	else size = 8 * h / w;

	int z = 0;
	glColor3f(255, 255, 255);
	glBegin(GL_QUADS);

	glTexCoord2f(0.0, 0.0);
	glVertex3f(0, 0, 0);

	glTexCoord2f(5, 0.0);
	glVertex3f(0, 0, size);

	glTexCoord2f(5, 5);
	glVertex3f(0, size, size);

	glTexCoord2f(0.0, 5);
	glVertex3f(0, size, 0);


	glEnd();
}
void drawPlanXZ(int w, int h)
{
	float size = 8;

	if (w > h) size = 8 * w / h;
	else size = 8 * h / w;

	int z = 0;
	glColor3f(255, 255, 255);
	glBegin(GL_QUADS);

	glTexCoord2f(0.0, 0.0);
	glVertex3f(0, 0, 0);

	glTexCoord2f(5, 0.0);
	glVertex3f(0, 0, size);

	glTexCoord2f(5, 5);
	glVertex3f(size, 0, size);

	glTexCoord2f(0.0, 5);
	glVertex3f(size, 0, 0);


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

	gluLookAt(2.0, 2.0, 2.0, 0, 0, 0, 0, 1.0, 0); //보는 시점이다.
												  //처음이 눈, at은 어느부분을 볼것인가 이기때문에 거의 안건들여..
												  //up은 축을 말하는거다...어느축으로 볼것인가.
	int wWidth = glutGet(GLUT_WINDOW_WIDTH);
	int wHeight = glutGet(GLUT_WINDOW_HEIGHT);

	//여기서도 배경이 3차원으로 해주면된다..... 각 축의 면으로

	glBindTexture(GL_TEXTURE_2D, texID[0]);
	drawPlanXY(wWidth, wHeight);//이게가 안가네 매핑이안된다 다른거 다되는데

	glBindTexture(GL_TEXTURE_2D, texID[0]);
	drawPlanYZ(wWidth, wHeight);

	glBindTexture(GL_TEXTURE_2D, texID[1]);
	drawPlanXZ(wWidth, wHeight);

	//이부분이 점과 점을 바인드 해주는 것이다. 매핑 해주는게 이부분이지..
	drawCube(0, 3, 2, 1);//이쪽면에는 texID[0]를 매핑하고 ..그냥 다음 면에 그려질거 매핑해버린다.
	drawCube(1, 2, 6, 5);
	drawCube(2, 3, 7, 6);
	drawCube(3, 0, 4, 7);
	drawCube(4, 5, 6, 7);
	drawCube(5, 4, 0, 1);

	glutSwapBuffers(); //GLUT_DOUBLE 버퍼를 더블로해서 스왑해서사용할 것이다.
}

void SetupRC(void) {
	glClearColor(1.0f, 1.0f, 1.0f, 1.0f);
	glEnable(GL_DEPTH_TEST);

	//GL_BLEND 할때 추가해줘야 하는 부분!
	//glEnable(GL_BLEND);
	//glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	//

	//Source – polygon color, destination – background

	GLfloat fogColor[4] = { 0.75,0.75,0.75,1 };//색..
	glEnable(GL_FOG);//FOG할때, A effect of GL_FOG_MODE
	glFogi(GL_FOG_MODE, GL_EXP);
	glFogfv(GL_FOG_COLOR, fogColor);
	glFogf(GL_FOG_DENSITY, 0.35f);


	/*
	GL_EXP 는 지수승 아무래도 리니어보다는..

		GL_FOG_MODE		GL_LINEAR, GL_EXP, GL_EXP2
		GL_FOG_DENSITY	float(0~1)
		GL_FOG_START	float(0~1)
		GL_FOG_END		float(0~1)
		GL_FOG_COLOR	RGBA color
	*/
		


}


void LoadTexture()
{
	/*
	풀이법 그냥 ...배경을 먼저 그린뒤에, 그 다음에 큐브를 위치 시키면 될것같다 ....
	모나리자는 바둑판으로 배열을 해도 될 것... x-y를 이루는 평면 x-z를 이루는 평면 y-z를 이루는 평면 ..이렇게 3개를 만들면 되겠다.....
	오른 손 좌표계였었나... 오른 손좌표계 z-x 평면을 광운대로 매핑을 하면 될 듯 끝
	*/
	texRec[0] = auxDIBImageLoad(TEXT("image/monalisa.bmp"));
	texRec[1] = auxDIBImageLoad(TEXT("image/logo.bmp"));

	glGenTextures(2, &texID[0]);

	for (register int i = 0; i < 2; i++)
	{
		glBindTexture(GL_TEXTURE_2D, texID[i]);

		glTexImage2D(GL_TEXTURE_2D, 0, 3, texRec[i]->sizeX, texRec[i]->sizeY, 0, GL_RGB, GL_UNSIGNED_BYTE, texRec[i]->data);


		//이게그 coordinate s,t 그거구나 .... s,t 의ㅣ 방향을 GL_REPEAT으로하겠다... default는 REPEAT 이라는데?
		glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
		glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);//그러네 안해도된다. 

		glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
		glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);

		delete[] texRec[i]->data;
		delete texRec[i];
	}
}

void init(void)
{
	//	glutTimerFunc(1000 / 30, timer, 1);
	glColorMaterial(GL_FRONT, GL_AMBIENT_AND_DIFFUSE);
	glEnable(GL_COLOR_MATERIAL);

	glEnable(GL_TEXTURE_2D);

	m_v();
	for (int i = 0; i < 8; i++)
		for (int j = 0; j < 3; j++)
			vertices[i][j] += 1;

	LoadTexture();
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

	int s = 3;
	if (w <= h) glOrtho(-s, s, -s*(float)h / (float)w, s*(float)h / (float)w, -s, s);
	else glOrtho(-s*(float)w / (float)h, s*(float)w / (float)h, -s, s, -s, s);
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
