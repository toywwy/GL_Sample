#include<WIndows.h>
#include<gl/GL.h>
#include<gl/GLU.h>
#include<gl/glut.h>
/*
GLUT 이전에는 MFC에서 했었어야했는데 이제는 glut를 이용해서 윈도우를 띄울 것이다.
*/
#include <math.h>
static bool mouseLeftDown;
static float point[2][2];
static float theta;
void RenderScene(void)
{
	glClear(GL_COLOR_BUFFER_BIT);
	float cos_th = cos(theta * 3.14159 / 180.0);
	float sin_th = sin(theta * 3.14159 / 180.0);
	glColor3f(1, 0, 0);
	glBegin(GL_POLYGON);
	glVertex2f(cos_th, sin_th);
	glVertex2f(-sin_th, cos_th);
	glVertex2f(-cos_th, sin_th);
	glVertex2f(sin_th, -cos_th);
	glEnd();
	glFlush();
}
void SetupRC(void)
{
	glClearColor(1.0f, 1.0f, 1.0f, 1.0f);//디스플레이 함수가 주기적으로 계속그리는거다 싱글버퍼에다가,
										 //그래서 게쏘 그리고 지워야하는데 네모가 움직이는데 뒤면을 초기화 안하면 잔상이 남기떄문에 초기화해줘야한다.
										 //색은 ..편한거 해도된다.
}
void ChangeSize(int w, int h)
{
	GLfloat aspectRatio;
	if (h == 0)
		h = 1;

	glViewport(0, 0, w, h);
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();

	if (w <= h)
		gluOrtho2D(-2.0, 2.0, -2.0*(float)h / (float)w, 2.0*(float)h / (float)w);
	else
		gluOrtho2D(-2.0*(float)w / (float)h, -2.0*(float)w / (float)h, -2.0, 2.0);

	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
}


void mouseButton(int button, int state, int x, int y)
{
	if (button == GLUT_LEFT_BUTTON)
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
	else if (button = GLUT_RIGHT_BUTTON)
	{
		if (state = GLUT_DOWN)
		{
		}
		else if (state == GLUT_UP)
		{
		}
	}
	glutPostRedisplay(); //화면 갱신이 필요할 때 이함수를 부르면 display 콜백 함수를 다시 한 번 실행
}

void mouseMotion(int x, int y)
{
	if (mouseLeftDown)//left 버튼이 true인 상태일때 만 들어온다.
	{
		double viewport[4];
		glGetDoublev(GL_VIEWPORT, viewport);
		point[1][0] = x / (float)viewport[2] * 500;
		point[1][1] = (viewport[3] - y) / (float)viewport[3] * 500;
		//선을 그리는거다.
	}
	glutPostRedisplay(); //화면을 갱신해준다.
}

void timer(int value)
{
	theta += 2.0;
	if (theta >= 360.0)
		theta -= 360.0;
	glutTimerFunc(1000 / 30, timer, 1);
	glutPostRedisplay();
}
void init(void)
{
	mouseLeftDown = false;
	point[0][0] = 0;
	point[0][1] = 0;
	point[1][0] = 0;
	point[1][1] = 0;
	theta = 0;
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
	glutMotionFunc(mouseMotion); //움직 일떄 발생하는 것같다.
	
	timer(1000);
	SetupRC();
	glutMainLoop();
	
	//기본적인 틀같은거 외우필요없고 시험에안나온다 시험볼떄 준다.
}
