#include<WIndows.h>
#include<gl/GL.h>
#include<gl/GLU.h>
#include<gl/glut.h>

static bool mouseLeftDown; //마우스 버튼에 대한 Flag 값이다.
static float point[3][3]; // 점에 대해서 이해할 필요는있다. 2 x 2다. 4개를 저장 할수있겠지...

static int idx = 0;// 0,1,2
static bool isDraw;

void RenderScene(void)
{
	glClear(GL_COLOR_BUFFER_BIT);
	glEnable(GL_LINE_STIPPLE);

	if (mouseLeftDown)
		glLineStipple(3, 0xcccc); //점선으로
	else
		glDisable(GL_LINE_STIPPLE); // 점선으로 쓰는거 취소한다.

	glPointSize(10);
	glEnable(GL_POINT_SMOOTH);
	glLineWidth(2);

	if (isDraw)
	{
		glColor3f(1, 0, 0);
		glBegin(GL_POINTS);
		{
			for (int i = 0; i<3; i++)
				glVertex2fv(point[i]);
		}
		glEnd();

		glColor3f(0, 0, 1);
		glBegin(GL_TRIANGLES);
		{
			glVertex2fv(point[0]); //point 값은 변화하는 값이다. 그리고 새로 그려주는 것이지
			glVertex2fv(point[1]);
			glVertex2fv(point[2]);
		}
		glEnd();

	}
	else
	{
		glColor3f(1, 0, 0);
		glBegin(GL_POINTS);
		{
			for (int i = 0; i<idx; i++)
				glVertex2fv(point[i]);
		}
		glEnd();
	}


	glFlush();
}
void SetupRC(void) { glClearColor(1.0f, 1.0f, 1.0f, 1.0f); }

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

				idx %= 3;
				point[idx][0] = x / (float)viewport[2] * 500;
				point[idx][1] = (viewport[3] - y) / (float)viewport[3] * 500;

				mouseLeftDown = true;

				idx += 1;
				if (idx == 3)
					isDraw = true;
			}
		}
		else if (state == GLUT_UP)
		{
			if (mouseLeftDown)
				mouseLeftDown = false;
		}
	}
	glutPostRedisplay(); //화면 갱신이 필요할 때 이 함수를 부르면 display 콜백 함수를 다시 한 번 실행
}

void init(void)
{
	mouseLeftDown = false; // 초기에는 안눌린 값으로
	isDraw = false;
	idx = 0;
	point[0][0] = 0;
	point[0][1] = 0;
	point[1][0] = 0;
	point[1][1] = 0;
	point[2][0] = 0;
	point[2][1] = 0;
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

	SetupRC();
	glutMainLoop();
}
