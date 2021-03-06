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


struct Point {
	float p[3];
	Point() {}
	Point(float x, float y, float z)
	{
		this->p[0] = x; this->p[1] = y; this->p[2] = z;
	}

	Point operator- (Point a)
	{
		Point retP;
		for (int i = 0; i < 3; i++) retP.p[i] = this->p[i] - a.p[i];
		return retP;
	}

};


void timer(int value)
{
	glutPostRedisplay();//화면 갱신이 필요할 때 이함수를 부르면 display 콜백 함수를 다시 한 번 실행
	glutTimerFunc(1000 / 30, timer, 1);//다이머 시간 마다 갱신,  해준다 //30 frame 나온다.
									   //타이머가 작동하는데 다시 타이머를 동작시킴으로써 계쏙 작동하게 되는거지.
}

Point getNormal(Point np)
{
	float x, y, z;
	float len;
	len = 0;

	x = np.p[0];
	y = np.p[1];
	z = np.p[2];

	len = x*x + y*y + z*z;
	len = sqrt(len);
	Point p(x / len, y / len, z / len);

	return p;
}

//내가한건데 이거 될거야 안될 이유가 없다.
Point CrossProduct(Point a, Point b)
{
	Point p(
		a.p[1] * b.p[2] - a.p[2] * b.p[1],
		a.p[2] * b.p[0] - a.p[0] * b.p[2],
		a.p[0] * b.p[1] - a.p[1] * b.p[0]
	);

	return getNormal(p);
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

	gluLookAt(0, 0, 2.0, 0, 0, 0, 0, 1.0, 0); //보는 시점이다.
											  //처음이 눈, at은 어느부분을 볼것인가 이기때문에 거의 안건들여..
											  //up은 축을 말하는거다...어느축으로 볼것인가.


	Point p1(0, 0, 0);
	Point p2(1, 0, 0);
	Point p3(1, 1, 0);
	Point p4(0,1,0);
	
	//외적순서에 의존한다 법선 벡터의 방향이 바뀔 수가 있다.
	
	//헐이건안돼.. 하긴 점과점을 빼면 어떻게 생각하면 그냥 점이되어 버리는 거니까..
	Point ps1 = p1 - p2;
	Point ps2 = p3 - p2;

	//Point ps1 = p1 - p2;
	//Point ps2 = p3 - p2;

	//순서에 의존을 한다 ....
	Point np = CrossProduct(ps1, ps2); //헐 왜 순서가 중요한거지....??
	glColor3f(0, 1, 0);
	glBegin(GL_TRIANGLES);//왜사각형 그릴때 이 순서는 중요하지않는거지...
	{
		glNormal3fv(np.p);
		glVertex3fv(p1.p);
		glVertex3fv(p2.p);
		glVertex3fv(p3.p);
	}
	glEnd();


	glutSwapBuffers(); //GLUT_DOUBLE 버퍼를 더블로해서 스왑해서사용할 것이다.
}

void SetupRC(void) {
	glClearColor(1.0f, 1.0f, 1.0f, 1.0f);
	glEnable(GL_DEPTH_TEST);
	glEnable(GL_LIGHTING);
	glEnable(GL_LIGHT0);
	
	//glEnable(GL_NORMALIZE);

	glEnable(GL_COLOR_MATERIAL);//앵 얘는 없으면안되네??
	//빛과 머티리얼은 관련이 있다는건가???

	/*
	마지막 라인인 GL_COLOR_MATRIAL은 텍스쳐 맵핑에 색을 추가하도록 해 준다. 
	이 재질의 컬러링을 가능하지 않게 한다면, 텍스쳐는 항상 원래의 색상으로 나타난다. 
	즉 glColor3f(r,g,b)가 아무런 효과를 내지 못한다. 
	그래서 이것을 가능하게 해주는 것이 중요하다.
	
	*/



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
