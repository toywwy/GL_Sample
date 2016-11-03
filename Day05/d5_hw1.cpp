#include<iostream>
#include<Windows.h>
#include<gl/gL.h>
#include<gl/gLU.h>
#include<gl/glut.h>

#include <math.h>

using namespace std;

GLUquadricObj *quadricObj;//곡면 그릴떄 사용하는 것으로 유추됨 원통 실린더 그릴때 사용
float r[20];
float colors[20][3];

void timer(int value)
{
	glutPostRedisplay();//화면 갱신이 필요할 때 이함수를 부르면 display 콜백 함수를 다시 한 번 실행
	glutTimerFunc(1000 / 30, timer, 1);//다이머 시간 마다 갱신,  해준다 //30 frame 나온다.
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

	gluLookAt(2, 2, 2, 0, 0, 0, 0, 1.0, 0);// 정면에서보려고

	glEnable(GL_LIGHTING);
	glEnable(GL_LIGHT0);

	glColorMaterial(GL_FRONT, GL_AMBIENT_AND_DIFFUSE);
	glEnable(GL_COLOR_MATERIAL);

	GLfloat mat_specular[4] = { 1,1,1,1 };
	GLfloat mat_shininess = 25.0;
	glMaterialfv(GL_FRONT, GL_SPECULAR, mat_specular);
	glMaterialf(GL_FRONT, GL_SHININESS, mat_shininess);

	float dy[] = { -2.0,-1.0,0,1.0,2.0 };
	float d[] = { -2.0,-1.0,0,1.0 };
	int idx = 0;

	for (int i = 0; i < 5; i++)
	{
		for (int j = 0; j < 4; j++)
		{
			glPushMatrix();
			{
				glTranslatef(d[j], dy[i], 0);
				//이동한 위치에서 회전을 해야한다.
				if (i == 0)
					glRotatef(r[idx], 1, 0, 0);
				else if (i == 1)
					glRotatef(r[idx], 0, 1, 0);
				else if (i == 2)
					glRotatef(r[idx], 0, 0, 1);
				else if (i == 3)
					glRotatef(r[idx], 0, 1, 0);
				else
					glRotatef(r[idx], 0, 0, 1);

				glColor3fv(colors[idx]);

				glutSolidTeapot(0.3);//마지막에 해야한다.
				r[idx++] += 2.0;
			}
			glPopMatrix();
		}
	}

	glutSwapBuffers(); //GLUT_DOUBLE 버퍼를 더블로해서 스왑해서사용할 것이다.
}

void SetupRC(void) {
	glClearColor(1.0f, 1.0f, 1.0f, 1.0f);
	glEnable(GL_DEPTH_TEST);
}

void init(void)
{

	for (int i = 0; i < 20; i++)
	{
		for (int j = 0; j < 3; j++)
		{
			float a = rand() % 10;
			colors[i][j] = a / 10.0;
		}
	}


	quadricObj = gluNewQuadric();
	gluQuadricDrawStyle(quadricObj, GLU_LINE);
	glutTimerFunc(1000 / 30, timer, 1);
}

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


void main(int argc, char * argv[])
{
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB);
	glutInitWindowSize(800, 600);
	glutCreateWindow("Simple");
	glutDisplayFunc(RenderScene);//렌더링되는 부분
	glutReshapeFunc(ChangeSize);//SIZE가 바뀔마다 호출이 된다.
								//	glutSpecialFunc(specialKeyboard);//스페셜 키보드 이벤트를 콜백
								//	glutKeyboardFunc(keyboard);//키보드 이벤트 콜백
	init();
	SetupRC();
	glutMainLoop();
}
