#include<WIndows.h>
#include<gl/GL.h>
#include<gl/GLU.h>
#include<gl/glut.h>
/*
GLUT 이전에는 MFC에서 했었어야했는데 이ㅔㅈ는 glut를 이용해서 윈도우를 띄울 것이다.
*/
void RenderScene(void)
{
	glClear(GL_COLOR_BUFFER_BIT);

	//그리는 순서가 존재할까???/
	glPointSize(5);//이게뭐지/
				   //f
				   //v 백터는 3칸짜리 배열로 넣는다 ..그러면 알아서 한다.
				   //d 더블
				   //공부좀 필요 하듯..

				   //glEnable(GL_LINE_STIPPLE);
	glLineWidth(10);
	//glLineStipple(3, 0xcccc);
	glBegin(GL_TRIANGLE_STRIP);//STRIP은 연결되기 떄문에 붙어버린다. //트라이앵글과의 차이점알돌고
	{
		glColor3f(1, 0, 0); // 색넣으면 바로 뒤에오는 버택스같은 것에 적용이 되는 것이다.
		glVertex2f(-50, -50);
		glColor3f(0, 1, 0);
		glVertex2f(0, -50);
		glColor3f(0, 0, 1);
		glVertex2f(0, 0);
		glColor3f(0, 0, 0);
		glVertex2f(-50, 0);
	}
	glEnd();
	glFlush();//버퍼에다가 그래놓고 여기에서 FLUSH 하면 화면에 나오는거다.

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
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
}

void main(int argc, char * argv[])
{
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_SINGLE | GLUT_RGB);
	glutInitWindowSize(600, 600);
	glutCreateWindow("Simple");
	glutDisplayFunc(RenderScene);
	glutReshapeFunc(ChangeSize);//비율을 바꿔줘야 화면을 늘렸을떄도 무넺가 없다.
	SetupRC();
	glutMainLoop();
	//기본적인 틀같은거 외우필요없고 시험에안나온다 시험볼떄 준다.
}


/*
오프니젱ㄹ은 그냥 하는게 아니라 콜백함수를 등록해야함.*/

/*

GLQUADS
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
