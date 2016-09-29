
/*
* windows.h - Master include file for Windowsapplication
* gl.h (OpenGL Core Library) - 렌더링 기능을 제공하는 라이브러리
* glu.h (OpenGL Utility Library) - GL 라이브러러의 도우미 역할
* glut.h (OpenGL Utility Toolkit) - 사용자 입력을 받아들이거나 화면 윈도우를 제어하기 위한 라이브러리
GL(OpenGL Core Library)
랜더링 기능을 제공하는 함수.  라이브러리로 GL은 단지 랜더링 기능만 수행할 뿐 사용자의 입력을 받는 기능과 출력하는 기능이 없다.

GLU(OpenGL Utility Library)
GL 함수로 작성되어 있는 고급기능을 제공하는 함수들의 집합

GLUT(OpenGL Utility Toolkit)
사용자의 입력을 받거나 화면 윈도우를 제어하기 위한 함수들로 운영체제를 호출한다.


앞에 gl 만 붙는 것은 공용으로 쓰이는 것이고

wgl처럼 gl 앞에 다른 것이 붙는 경우 특수 환경에 따라 쓰임새가 있는 것이다.


--


*/

#include<WIndows.h>
#include<gl/GL.h>
#include<gl/GLU.h>
#include<gl/glut.h>

/*
GLUT 이전에는 MFC에서 했었어야했는데 이제는 glut를 이용해서 윈도우를 띄울 것이다.
좀더 작업하기가 편해진거지
*/
void RenderScene(void)
{
	//Clear the window with current clearing color
	glClear(GL_COLOR_BUFFER_BIT); //버퍼의 조합을 지운다.
	glColor3f(1, 0, 0);// 색설정

	glBegin(GL_POLYGON);
	glVertex2f(-50, -50);
	glVertex2f(0, -50);
	glVertex2f(0, 0);
	glVertex2f(-50, 0);
	glEnd();

	glFlush();//flush 하면서 버퍼에 있는걸 그리는거다.
}

void SetupRC(void)
{
	glClearColor(1.0f, 1.0f, 1.0f, 1.0f);
	//디스플레이 함수가 주기적으로 계속그리는거다 싱글버퍼에다가,
	//그래서 계속 그리고 지워야하는데 네모가 움직이는데 뒤면을 초기화 안하면 잔상이 남기 때문에 초기화해줘야한다.
	//색은 ..편한거 해도된다.
}

void ChangeSize(int w, int h)
{
	GLfloat aspectRatio; //비율

	if (h == 0)
		h = 1;

	glViewport(0, 0, w, h); //뷰포트는 말그대로 화면에 보여주는 그 화면인거다.
	glMatrixMode(GL_PROJECTION); //투영을 위한 행렬을 변환하느 부분
	glLoadIdentity();//행렬 처리가 이루어지기전에 좌표계를 초기화한다.

	aspectRatio = (GLfloat)w / (GLfloat)h;

	if (w <= h)
	{
		float windowWidth = 100;
		float windowHeight = 100 / aspectRatio;
		glOrtho(-100.0, 100.0, -windowHeight, windowHeight, 1.0, -1.0);
	}
	else
	{
		float windowWidth = 100 * aspectRatio;
		float windowHeight = 100;
		glOrtho(-windowWidth, windowWidth, -100.0, 100.0, 1.0, -1.0);
	}

	glMatrixMode(GL_MODELVIEW); //이런데 뭐가들어가는지 이런거 봐야하는데...
	glLoadIdentity();
}

void main(int argc, char * argv[])
{
	glutInit(&argc, argv);//반드시 써야 한다고 들었다.

	glutInitDisplayMode(GLUT_SINGLE | GLUT_RGB);//싱글 버퍼 쓸것임, 색상모드는 RGB를 이용한다.
	glutInitWindowSize(600, 600);//윈도우 사이즈 처음에

	glutCreateWindow("Simple");
	glutDisplayFunc(RenderScene);//이부분이 콜백을 호출하게 되는것이다.
	glutReshapeFunc(ChangeSize);//비율을 바꿔줘야 화면을 늘렸을때도 문제가없다.
								//새로운 창의 너비와 높이를 받아서 좌표계와 화면좌표의 매핑에 이용된다.

	SetupRC();
	glutMainLoop();//void RenderScene(void) 이부분을 반복적으로 수행하는게 아닐까????
				   //glut framework를 실행한다.
				   //GLUT GL루틴

				   //기본적인 틀같은거 외우필요없고 시험에안나온다 시험볼떄 준다.
}
/*
* void glutInit(int *argc, char **argv) - GLUT 라이브러리를 초기화 한 후 윈도우 시스템과 세션을 연다.
* void glutInitDisplayMode(unsigned int mode) - 생성될 윈도우의 디스플레이 모드를 설정
* void glutInitWindowSize(int width, int height) - 윈도우 크기 설정
* void glutCreateWindow(const char *tilte) - 새로운 윈도우 생성
* void glutDisplayFunc(void (*func)(void)) - 화면 디스플레이 이벤트가 발생 했을 시 실행될 콜백 등록 함수
* void glutReshapeFunc(void (*func)(int, int)) - 윈도우 크기가 변경되었을 시 발생 했을 시 실행될 콜백 등록 함수
* void SetupRC() / void Init() - 초기화 함수로서 관습적으로 사용 (개발자 임의)
* glutMainLoop() - GLUT, GL 루틴
*/

