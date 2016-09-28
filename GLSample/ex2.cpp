/*
도형을 그릴때 정점 3개를 통한 삼각형의 조합이 아닌  그 이상의 정점을 사용하여 다각형을 구성할때에도 내부적으로는 삼각형 조합 방식으로 그려진다. 삼각형이 아니면 계산할 방법이 없기 때문이다.

BeginMode
#define GL_POINTS                         0x0000
//각 정점의 위치에 점을 찍는다. glPointSize(5.0f); 점 크기 크게

#define GL_LINES                             0x0001
//점 두개가 한쌍이 되어서 직선으로 연결된다. 짝이 없으면 무시된다.

#define GL_LINE_LOOP                   0x0002
//값 순서대로 직선을 그어준다음 시작점과 끝점을 연결시켜서 닫아준다.

#define GL_LINE_STRIP                    0x0003
//값 순서대로 직선을 그어준다. 이동경로 같은거 보여줄때 쓸 수 있다.

#define GL_TRIANGLES                    0x0004
//점을 이어서 삼각형을 이어준다.   숫자가 다르면 무시한다.

#define GL_TRIANGLE_STRIP           0x0005
//연결된 삼각형을 그린다.  엔진을 만들때 지형의 격자에 쓰인다.

#define GL_TRIANGLE_FAN               0x0006
//첫번째 점을 중심으로 다른 점들까지 선을 이은 다음 삼각형을 그려준다.

#define GL_QUADS                            0x0007
//정점 4개를 이용하여 사각형을 만든다.

#define GL_QUAD_STRIP                   0x0008
//사각형들이 연결된 모양으로 만들어준다.

#define GL_POLYGON                         0x0009
//닫힌 도형을 만들어준다. 다각형.

viewport 선행 작업이 다 끝난 다음에 디스플레이에 내가 원하는 크기로 뿌려주는 마지막 작업.
glViewport(0,0,iWidth, iHeight); -> 창의 가로 세로 크기를 정규화 시켰다.
*/

#include<iostream>
#include<WIndows.h>
#include<gl/GL.h>
#include<gl/GLU.h>
#include<gl/glut.h>

using namespace std;

void RenderScene(void)
{
	glClear(GL_COLOR_BUFFER_BIT);

	//그리는 순서가 존재할까???/
	glPointSize(5);//이게뭐지/
				   //f
				   //v 백터는 3칸짜리 배열로 넣는다 ..그러면 알아서 한다.
				   //d 더블
				   //공부좀 필요 하듯..

	glEnable(GL_LINE_STIPPLE); //점선으로 그려지게한다.
	glPolygonMode(GL_FRONT_AND_BACK, GL_LINE); //이거하면 줄만 그려지네

	glLineWidth(4); //두께 설정한다.

	glLineStipple(3, 0xcccc);
	//만약 비긴 전에 glColor3f를 설정했다고 하면 아래꺼가 전부 그 색으로 설정된다.
	//glColor3f(1,0,0); 이렇게하면 그냥 Flat Coloring이되는것이고...아래서 설정하면 ...
	glBegin(GL_TRIANGLE_FAN);
	{//이런식으로 블레이스로 묶어줄수있다.
		// glColor에 색넣으면 바로 뒤에오는 버택스같은 것에 적용이 되는 것이다.
		// 결국 점 4개를 찍게 되는 것인데

		glColor3f(1, 0, 0); //색 설정
		glVertex2f(-100, -100); //위치 설정

		glColor3f(0, 1, 0);
		glVertex2f(0, -50);

		glColor3f(0, 0, 1);
		glVertex2f(0, 0);

		glColor3f(1, 1, 1);
		glVertex2f(-50, 0);
	}
	glEnd();

	glFlush();//버퍼에다가 그래놓고 여기에서 FLUSH 하면 화면에 나오는거다.

}
void SetupRC(void){glClearColor(1.0f, 1.0f, 1.0f, 1.0f);}

void ChangeSize(int w, int h)
{//x,y로 값이 들어오는 것이고
	GLfloat aspectRatio;
	if (h == 0)
		h = 1;

	glViewport(0, 0, w, h);//사이즈를 들어온걸로 뷰포트를 새로이 설정을 해준다.
	glMatrixMode(GL_PROJECTION);//3차원 공간상의 물체를 2차원에 투영시키기 때문에 필요한 matrix이다.
	//glMatrixmode(GL_MODELVIEW)로 하면 그 다음에 나오는 OpenGL 명령어는 바로 이 MODELVIEW matrix와 관련된 operation을 수행한다고 OpenGL에게 알려주는것
	//(물체의 위치 변환이나 카메라의 위치 변화)

	glLoadIdentity(); //좌표계를 초기화한다.

	//가로 / 세로
	aspectRatio = (GLfloat)w / (GLfloat)h; //가로에 세로를 나눠 주었다.
	cout <<"ratio = "<<aspectRatio<<"width = " << w << " , height = " << h << endl;

	if (w <= h) //세로가 더 길거나 같다.
	{
		//비율 < 1
		float windowWidth = 100;
		float windowHeight = 100 / aspectRatio; //비율을 통해서 줄여주는거야 위쪽으로 늘렸을때 세로를 줄여주어야한다.
		glOrtho(-100.0, 100.0, -windowHeight, windowHeight, 1.0, -1.0);
		//z값을 없애고 그려주는것이다. 평면에서 보이게 정사영한다고 생각하면된다.

		//1,3,5    2,4,6 이렇게 각가 왼쪽 아래면 오른쪽 윗면을 참고하고있다.

	}
	else // 가로가 더 긴경우
	{
		//비율 > 1
		float windowWidth = 100 * aspectRatio;//비율을 통해서 줄여줘야한다. //가로가 긴경우는 비율을 곱해주네
		float windowHeight = 100;
		//windowWith 는 왼쪽 위치 잡아준다. 0으로하면 화면에 딱 붙어버려 //근데 음수한다고 안보이는게아님 ..
		//-100으로했을떄도 잘보였다 오히려 0이되면 하나도안보임
		glOrtho(-windowWidth, windowWidth, -100.0, 100.0, 1.0, -1.0);
		//glOrtho ( GLdouble left,Gldouble right, GLdouble bottom,  GLdouble top, GLdouble nearVal, GLdouble farVal)
		//far에 양수를 넣어 줫더니 가까워짐 near에 양수를 넣어줘도 가까워 졌다.
		//near, far는 클리핑 영역의 앞쪽과 뒤쪽 좌표를 말한다.

	}
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();

	/*
		카메러나 물체이동을 하기위해서 GL_MODELVIEW를 이용하고
		물체를 투영시키기 위해서는 GL_PROJECTION 모드가 필요하다.
	*/

}

void main(int argc, char * argv[])
{
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_SINGLE | GLUT_RGB);
	glutInitWindowSize(600, 600);
	glutCreateWindow("Simple");
	glutDisplayFunc(RenderScene);
	glutReshapeFunc(ChangeSize);
	SetupRC();
	glutMainLoop();
	//기본적인 틀같은거 외우필요없고 시험에안나온다 시험볼떄 준다.
}

/*
glColor3f(1,0,0)
GL_QUADS

glVertex2f(-80, -30);
glVertex2f(-50, -30);
glVertex2f(-50, 0);
glVertex2f(-80, 0);
glVertex2f(-20, -30);
glVertex2f(10, -30);
glVertex2f(10, 0);
glVertex2f(-20, 0);


GL_QUAD_STRIP
glVertex2f(-80, -30);
glVertex2f(-80, 0);
glVertex2f(-50, -30);
glVertex2f(-50, 0);
glVertex2f(-20, -30);
glVertex2f(-20, 0);
glVertex2f(10, -30);
glVertex2f(10, 0);

*/
