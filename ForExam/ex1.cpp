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


	float a=1;


	glPolygonMode(GL_FRONT_AND_BACK,GL_LINE);
	glLineWidth(4);
	
	//glPointSize(4);
	glBegin(GL_QUAD_STRIP);
	{
		glColor3f(1, 0, 0);
		glVertex2f(0, 0);
		glVertex2f(0.5, 1);

		glVertex2f(1, 0);
		glVertex2f(1.5, 1);

		glVertex2f(2, 0);
		glVertex2f(2.5, 1);

		glVertex2f(3, 0);
		glVertex2f(3.5, 1);

		glVertex2f(4, 0);

		
	}
	glEnd();

	glFlush();//세팅을 다했다 이제 그려도 된다.
}
void SetupRC(void) { glClearColor(1.0f, 1.0f, 1.0f, 1.0f); }

void ChangeSize(int w, int h)
{
	if (h == 0)// 0으로 나누어지는거 막기위해서
		h = 1;

	glViewport(0, 0, w, h);

	glMatrixMode(GL_PROJECTION); //이부분이 PROJECTION인 이유는
	glLoadIdentity();//좌표계 초기화

	if (w <= h)
		glOrtho(-2.0, 2.0, -2.0*(float)h / (float)w,
			2.0*(float)h / (float)w, -10.0, 10.0);
	else
		glOrtho(-2.0*(float)w / (float)h, 2.0*(float)w / (float)h,
			-2.0, 2.0, -10.0, 10.0);
}

void init(void)
{
}
void main(int argc, char * argv[])
{
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_SINGLE | GLUT_RGB);
	glutInitWindowSize(1500, 600);

	glutCreateWindow("Simple");
	glutDisplayFunc(RenderScene);
	glutReshapeFunc(ChangeSize);//비율을 바꿔줘야 화면을 늘렸을떄도 무넺가 없다.

	SetupRC();
	glutMainLoop();
}
