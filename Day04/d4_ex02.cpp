#include<WIndows.h>
#include<gl/gL.h>
#include<gl/gLU.h>
#include<gl/glut.h>
#include <math.h>


GLfloat vertices[8][3] = { { -1, -1, 1 },{ -1, 1, 1 },{ 1, 1, 1 },{ 1, -1, 1 },{ -1, -1, -1 },{ -1, 1, -1 },{ 1, 1, -1 },{ 1, -1, -1 } };
GLfloat colors[8][3] = { { 0, 0, 1 },{ 0, 1, 1 },{ 1, 1, 1 },{ 1, 0, 1 },{ 0, 0, 0 },{ 0, 1, 0 },{ 1, 1, 0 },{ 1, 0, 0 } };

void quad(char a, char b, char c, char d)
{
	glColor3fv(colors[a]); glVertex3fv(vertices[a]);
	glColor3fv(colors[b]); glVertex3fv(vertices[b]);
	glColor3fv(colors[c]); glVertex3fv(vertices[c]);
	glColor3fv(colors[d]); glVertex3fv(vertices[d]);
}
void RenderScene(void)
{
	//Clear the window with current clearing color
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT); //버퍼의 조합을 지운다.

	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();

	gluLookAt(0.5, 0.5, 0.5, 0, 0, 0, 0, 1, 0);

	//Shearing //밀림

	GLfloat m[16];

	for (register int i = 0; i < 16; i++)
		m[i] = 0;

	m[0] = m[5] = m[10] = m[15] = 1;
	m[4] = 1.0 / tan(60 / 180.0 * 3.14159);
	
	glMultMatrixf(m);//이게 미는 것같음.
	glColor3f(1.0f, 0.0f, 0.0f);
	glutWireCube(2.0);

	glutSwapBuffers();
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
	glutDisplayFunc(RenderScene);//비율을 바꿔줘야 화면을 늘렸을떄도 무넺가 없다.
	glutReshapeFunc(ChangeSize);//꼭 추가해줘야한다.

	SetupRC();
	glutMainLoop();
	//기본적인 틀같은거 외우필요없고 시험에안나온다 시험볼떄 준다.
}
