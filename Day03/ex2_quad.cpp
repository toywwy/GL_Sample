#include<WIndows.h>
#include<gl/GL.h>
#include<gl/GLU.h>
#include<gl/glut.h>

#include <math.h>

GLfloat vertices[8][3] = { { -1, -1, 1 },{ -1, 1, 1 },{ 1, 1, 1 },{ 1, -1, 1 },{ -1, -1, -1 },{ -1, 1, -1 },{ 1, 1, -1 },{ 1, -1, -1 } };
GLfloat colors[8][3] = { { 0, 0, 1 },{ 0, 1, 1 },{ 1, 1, 1 },{ 1, 0, 1 },{ 0, 0, 0 },{ 0, 1, 0 },{ 1, 1, 0 },{ 1, 0, 0 } };

static float theta;

void quad(int a,int b,int c,int d)
{
	glColor3fv(colors[a]); glVertex3fv(vertices[a]);
	glColor3fv(colors[b]); glVertex3fv(vertices[b]);
	glColor3fv(colors[c]); glVertex3fv(vertices[c]);
	glColor3fv(colors[d]); glVertex3fv(vertices[d]);
}

void RenderScene(void)
{
	glClear(GL_COLOR_BUFFER_BIT|GL_DEPTH_BUFFER_BIT);
	
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();

	gluLookAt( 0.5, 0.5, 0.5, 0, 0, 0, 0, 1, 0);//보통 마지막은 0,0으로

	glRotatef(theta, 0, 1, 0);//이게 먼저 와야하는이유는 뭘까????


	glBegin(GL_QUADS);
		quad(0, 3, 2, 1);
		quad(2, 3, 7, 6);
		quad(3, 0, 4, 7);
		quad(1, 2, 6, 5);
		quad(4, 5, 6, 7);
		quad(5, 4, 0, 1);
	glEnd();

	glutSwapBuffers();//더블 버퍼 사용하기 떄문이다.
}



void SetupRC(void) {
	glClearColor(1.0f, 1.0f, 1.0f, 1.0f);
	glEnable(GL_DEPTH_TEST);
}

void ChangeSize(int w, int h)
{
	if (h == 0)// 0으로 나누어지는거 막기위해서
		h = 1;

	glViewport(0, 0, w, h);

	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();

	if (w <= h)
		glOrtho(-2.0, 2.0, -2.0*(float)h / (float)w, 
			2.0*(float)h / (float)w, -10.0, 10.0);//위에 10으로 바꾼거 클리핑하기위해서이다.
	else
		glOrtho(-2.0*(float)w / (float)h, 2.0*(float)w / (float)h, -2.0, 
			2.0, -10.0, 10.0);
}


void timer(int value)
{
	theta += 2.0;
	if (theta >= 360.0)
		theta -= 360.0;
	glutPostRedisplay();
	glutTimerFunc(1000 / 30, timer, 1);
}

void init(void)
{
	theta = 0.0f;
	glutTimerFunc(1000 / 30, timer, 1);
}

void main(int argc, char * argv[])
{
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB);
	glutInitWindowSize(600, 600);
	glutCreateWindow("Simple");
	
	glutDisplayFunc(RenderScene);
	glutReshapeFunc(ChangeSize);//꼭 추가해줘야한다. 그려질떄 이것도 같이 호출이 되기 떄문에 꼭

	init();
	SetupRC();
	glutMainLoop();
}
