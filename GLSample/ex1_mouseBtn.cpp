#include<WIndows.h>
#include<gl/GL.h>
#include<gl/GLU.h>
#include<gl/glut.h>

static bool mouseLeftDown; //마우스 버튼에 대한 Flag 값이다.
static float point[2][2]; // 점에 대해서 이해할 필요는있다. 2 x 2다. 4개를 저장 할수있겠지...


/*
화면 좌표계의 차이 
	윈도우 : 좌 상단이 원점(0,0) 
	OpenGL : 좌 하단이 원점(0,0)
	GLUT는 윈도우 화면 좌표계를 사용
*/

void RenderScene(void)
{
	glClear(GL_COLOR_BUFFER_BIT);
	glEnable(GL_LINE_STIPPLE);

	if (mouseLeftDown)
		glLineStipple(3, 0xcccc); //점선으로
	else
		glDisable(GL_LINE_STIPPLE); // 점선으로 쓰는거 취소한다.

	glColor3f(0, 0, 1);
	glLineWidth(2);

	glBegin(GL_LINES);
	{	
		glVertex2fv(point[0]); //point 값은 변화하는 값이다. 그리고 새로 그려주는 것이지
		glVertex2fv(point[1]);
	}
	glEnd();

	glFlush();
}
void SetupRC(void){glClearColor(1.0f, 1.0f, 1.0f, 1.0f);}

void ChangeSize(int w, int h)
{
	GLfloat aspectRatio;
	if (h == 0)
		h = 1;

	glViewport(0, 0, w, h);
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	if (w <= h)
		gluOrtho2D(0, 500, 0, 500 * (float)h / (float)w);
	else
		gluOrtho2D(0, 500 * (float)w / (float)h, 0, 500);

	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
}

//TODO callback mouse Click
void mouseButton(int button, int state, int x, int y)
{
	if (button == GLUT_LEFT_BUTTON) //마우스 왼쪽버튼 눌렀을때
	{
		if (state == GLUT_DOWN)
		{
			if (!mouseLeftDown)
			{
				double viewport[4];

				glGetDoublev(GL_VIEWPORT, viewport);

				point[0][0] = x / (float)viewport[2] * 500;
				point[0][1] = (viewport[3] - y) / (float)viewport[3] * 500;
				point[1][0] = point[0][0];
				point[1][1] = point[0][1];

				mouseLeftDown = true;
			}
		}
		else if (state == GLUT_UP)
		{
			if (mouseLeftDown)
				mouseLeftDown = false;
		}
	}
	else if (button = GLUT_RIGHT_BUTTON) //오른쪽 버튼 눌렀을때
	{
		if (state = GLUT_DOWN)
		{

		}
		else if (state == GLUT_UP)
		{

		}
	}
	glutPostRedisplay(); //화면 갱신이 필요할 때 이 함수를 부르면 display 콜백 함수를 다시 한 번 실행
}

//TODO Mouse motion Callback
void mouseMotion(int x, int y) //마우스 모션 콜백을 해놓으면 값이 리턴됨
{
	if (mouseLeftDown)
	{
		double viewport[4];
		glGetDoublev(GL_VIEWPORT, viewport);
		point[1][0] = x / (float)viewport[2] * 500;
		point[1][1] = (viewport[3] - y) / (float)viewport[3] * 500;
	}
	glutPostRedisplay();
}

void init(void)
{
	mouseLeftDown = false; // 초기에는 안눌린 값으로
	point[0][0] = 0;
	point[0][1] = 0;
	point[1][0] = 0;
	point[1][1] = 0;
}
void main(int argc, char * argv[])
{
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_SINGLE | GLUT_RGB);
	glutInitWindowSize(600, 600);

	glutCreateWindow("Simple");
	glutDisplayFunc(RenderScene);
	glutReshapeFunc(ChangeSize);//비율을 바꿔줘야 화면을 늘렸을떄도 무넺가 없다.
	glutMouseFunc(mouseButton); //버튼을 눌렀을떄
	glutMotionFunc(mouseMotion); //마우스가 움직일때 콜백 발생

	SetupRC();
	glutMainLoop();
}
