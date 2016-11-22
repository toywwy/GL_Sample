#include<iostream>
#include<Windows.h> //이건 GUI 때문에 필요한건데.
#include<gl\GL.h>
#include<gl\GLU.h>
#include<gl\glut.h>
#include<gl\GLAUX.H>
#include<math.h> //삼각함수 쓰려고

/*
* gl.h (OpenGL Core Library) - 렌더링 기능을 제공하는 라이브러리
* glu.h (OpenGL Utility Library) - GL 라이브러러의 도우미 역할
* glut.h (OpenGL Utility Toolkit) - 사용자 입력을 받아들이거나 화면 윈도우를 제어하기 위한 라이브러리
*/

/*
LNK2019 ERR >> Property -> Configuration Properties -> Linker -> Input -> Additional Dependencies -> add “LEGACY_STDIO_DEFINITIONS.LIB”
+glaux.lib 추가해줘야한다.
*/

using namespace std;

GLfloat vertices[8][3] = { { -1, -1, 1 },{ -1, 1, 1 },{ 1, 1, 1 },{ 1, -1, 1 },{ -1, -1, -1 },{ -1, 1, -1 },{ 1, 1, -1 },{ 1, -1, -1 } };
GLfloat colors[8][3] = { { 0, 0, 1 },{ 0, 1, 1 },{ 1, 1, 1 },{ 1, 0, 1 },{ 0, 0, 0 },{ 0, 1, 0 },{ 1, 1, 0 },{ 1, 0, 0 } };

static float m_vertices[8][3], m_colors[8][4], m_normals[6][3];
GLubyte m_image[64][64][3];

AUX_RGBImageRec *texRec;
GLuint texID;

void m_v()
{
	m_vertices[0][0] = -1; m_vertices[0][1] = -1; m_vertices[0][2] = 1; m_vertices[1][0] = -1; m_vertices[1][1] = 1; m_vertices[1][2] = 1; m_vertices[2][0] = 1; m_vertices[2][1] = 1; m_vertices[2][2] = 1; m_vertices[3][0] = 1; m_vertices[3][1] = -1; m_vertices[3][2] = 1; m_vertices[4][0] = -1; m_vertices[4][1] = -1; m_vertices[4][2] = -1; m_vertices[5][0] = -1; m_vertices[5][1] = 1; m_vertices[5][2] = -1; m_vertices[6][0] = 1; m_vertices[6][1] = 1; m_vertices[6][2] = -1; m_vertices[7][0] = 1; m_vertices[7][1] = -1; m_vertices[7][2] = -1;
	m_colors[0][0] = 0; m_colors[0][1] = 0; m_colors[0][2] = 1; m_colors[0][3] = 1; m_colors[1][0] = 0; m_colors[1][1] = 1; m_colors[1][2] = 1; m_colors[1][3] = 1; m_colors[2][0] = 1; m_colors[2][1] = 1; m_colors[2][2] = 1; m_colors[2][3] = 1; m_colors[3][0] = 1; m_colors[3][1] = 0; m_colors[3][2] = 1; m_colors[3][3] = 1; m_colors[4][0] = 0; m_colors[4][1] = 0; m_colors[4][2] = 0; m_colors[4][3] = 1; m_colors[5][0] = 0; m_colors[5][1] = 1; m_colors[5][2] = 0; m_colors[5][3] = 1; m_colors[6][0] = 1; m_colors[6][1] = 1; m_colors[6][2] = 0; m_colors[6][3] = 1; m_colors[7][0] = 1; m_colors[7][1] = 0; m_colors[7][2] = 0; m_colors[7][3] = 1;
	m_normals[0][0] = 0; m_normals[0][1] = 0; m_normals[0][2] = 1;
	// (0, 3, 2, 1) 
	m_normals[1][0] = 0; m_normals[1][1] = 1; m_normals[1][2] = 0;
	// (1, 2, 6, 5) 
	m_normals[2][0] = 1; m_normals[2][1] = 0; m_normals[2][2] = 0;
	// (2, 3, 7, 6) 
	m_normals[3][0] = 0; m_normals[3][1] = -1; m_normals[3][2] = 0;
	// (3, 0, 4, 7) 
	m_normals[4][0] = 0; m_normals[4][1] = 0; m_normals[4][2] = -1;
	// (4, 5, 6, 7) 
	m_normals[5][0] = -1; m_normals[5][1] = 0; m_normals[5][2] = 0;
	// (5, 4, 0, 1)
}

void drawCube(int a, int b, int c, int d)
{
	glBegin(GL_QUADS);

	glNormal3fv(m_normals[a]);

	glColor3fv(colors[a]);
	glTexCoord2f(0.0, 0.0);
	glVertex3fv(vertices[a]);

	glColor3fv(colors[b]);
	glTexCoord2f(1.0, 0.0);
	glVertex3fv(vertices[b]);

	glColor3fv(colors[c]);
	glTexCoord2f(1.0, 1.0);
	glVertex3fv(vertices[c]);

	glColor3fv(colors[d]);
	glTexCoord2f(0.0, 1.0);
	glVertex3fv(vertices[d]);

	glEnd();
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

	gluLookAt(2.0, 2.0, 2.0, 0, 0, 0, 0, 1.0, 0); //보는 시점이다.
												  //처음이 눈, at은 어느부분을 볼것인가 이기때문에 거의 안건들여..
												  //up은 축을 말하는거다...어느축으로 볼것인가.

	drawCube(0, 3, 2, 1);
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
}


void LoadTexture()
{
	//TEXT("image/monalisa.bmp")
	texRec = auxDIBImageLoad(TEXT("image/monalisa.bmp"));
	//LNK2019 ERR >> Property -> Configuration Properties -> Linker -> Input -> Additional Dependencies -> add “LEGACY_STDIO_DEFINITIONS.LIB”
	//이거를 한다음에 TEXT를 해주니까 문제해결 되었다.

	glGenTextures(1, &texID);
	glBindTexture(GL_TEXTURE_2D, texID);

	glTexImage2D(GL_TEXTURE_2D, 0, 3, texRec->sizeX, texRec->sizeY, 0, GL_RGB, GL_UNSIGNED_BYTE, texRec->data);

	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP);
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP);

	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);

}

void init(void)
{
	//	glutTimerFunc(1000 / 30, timer, 1);
	glColorMaterial(GL_FRONT, GL_AMBIENT_AND_DIFFUSE);
	glEnable(GL_COLOR_MATERIAL);

	glEnable(GL_TEXTURE_2D);
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

								//453, 948 //453개의 점과 948개의의 삼각형으로 나뉘어 져있다.....
								//저 점과 그점의 차이는 무엇인가???
								//

								//입력완료



	init();
	SetupRC();
	glutMainLoop();
	//기본적인 틀같은거 외우필요없고 시험에안나온다 시험볼떄 준다.
}
