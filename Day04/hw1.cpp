#include<iostream>
#include<WIndows.h>
#include<gl/gL.h>
#include<gl/gLU.h>
#include<gl/glut.h>

#include <math.h>

using namespace std;

GLUquadricObj *quadricObj;//곡면 그릴떄 사용하는 것으로 유추됨 원통 실린더 그릴때 사용

static float baseTheta;
static float lowerArmTheta;
static float upperArmTheta;

static float angle = 5.0f;

bool baseToggle[2];
bool lowerToggle[2];
bool upperToggle[2];


void base(void)
{
	if (baseToggle[0])
	{
		if (baseToggle[1])//left
			baseTheta -= angle;
		else//right key
			baseTheta += angle;

		baseToggle[0] = false;
	}

	glRotatef(baseTheta, 0, 1, 0);//다시 pop 할 필요 없다. 전체가 다 돌아야 하기 때문에

	glPushMatrix();
	{	
		glColor3f(1, 0, 0);
		glRotatef(-90, 1, 0, 0);
		gluCylinder(quadricObj, 0.5, 0.5, 0.3, 20, 1); 
		// 원통 그리는거 곡면그릴떄 quadricObj를 이용하는 것 같다.
	}
	glPopMatrix();
}

void lower_arm(void)
{

	if (lowerToggle[0])
	{
		if (lowerToggle[1])//left
			lowerArmTheta += angle;
		else//right key
			lowerArmTheta -= angle;

		lowerToggle[0] = false;
	}

	glPushMatrix();
	{	
		glRotatef(lowerArmTheta, 1, 0, 0);
		glColor3f(0, 1, 0);
		glTranslatef(0, 0.7, 0);
		glScalef(0.2, 1, 0.2);
		glutWireCube(1);
	}
	glPopMatrix();

}

void upper_arm(void)
{

	if (upperToggle[0])
	{
		if (upperToggle[1])//left
			upperArmTheta += angle;
		else//right key
			upperArmTheta -= angle;
		upperToggle[0] = false;
	}
	
	glPushMatrix();
	{
		glRotatef(lowerArmTheta, 11, 0, 0);// lowerArmTheta가 회전한 부분까지 회전한 후에 원점에서 회전한다.
		glTranslatef(0, 1.0, 0); //그리고 다시 원점을 올려서
		glRotatef(upperArmTheta, 11, 0, 0);// 원점으로 부터 upper암이 회전한 부분
		glTranslatef(0, 0.6, 0);//그리고 도형의 중심으로 가기위해서
		glColor3f(0, 0, 1);
		glScalef(0.2, 0.8, 0.2);
		glutWireCube(1);
	}
	glPopMatrix();
}



void keyboard(unsigned char c, int x, int y)
{
	switch (c)
	{
	case VK_ESCAPE:
		gluDeleteQuadric(quadricObj);
		exit(0);
		break;
	}
}

void specialKeyboard(int c, int x, int y)
{
	switch (c)
	{
	case GLUT_KEY_HOME:
		upperToggle[0] = true;
		upperToggle[1] = true;
		break;
	case GLUT_KEY_END:
		upperToggle[0] = true;
		upperToggle[1] = false;
		break;
	case GLUT_KEY_LEFT:
		baseToggle[0] = true;
		baseToggle[1] = true;

		break;
	case GLUT_KEY_RIGHT:
		baseToggle[0] = true;
		baseToggle[1] = false;
		break;
	case GLUT_KEY_UP:
		lowerToggle[0] = true;
		lowerToggle[1] = true;
		break;
	case GLUT_KEY_DOWN:
		lowerToggle[0] = true;
		lowerToggle[1] = false;
		break;
	}

}

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

	gluLookAt(0.5, 0.5, 0.5, 0, 0, 0, 0, 1, 0);

	base();//실린더
	lower_arm();
	upper_arm();//lower_arm을 고려하고 그려야함.

	glutSwapBuffers(); //GLUT_DOUBLE 버퍼를 더블로해서 스왑해서사용할 것이다.
}

void SetupRC(void) {
	glClearColor(1.0f, 1.0f, 1.0f, 1.0f);
	glEnable(GL_DEPTH_TEST);
}

void init(void)
{
	//초기화
	baseTheta=0.0f;
	lowerArmTheta=0.0f;
	upperArmTheta=0.0f;

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
	glutInitWindowSize(600, 600);
	glutCreateWindow("Simple");
	glutDisplayFunc(RenderScene);//비율을 바꿔줘야 화면을 늘렸을떄도 문제가 없다.
	glutReshapeFunc(ChangeSize);//꼭 추가해줘야한다.
	
	glutSpecialFunc(specialKeyboard);
	glutKeyboardFunc(keyboard);

	init();
	SetupRC();
	glutMainLoop();
	//기본적인 틀같은거 외우필요없고 시험에안나온다 시험볼떄 준다.
}
