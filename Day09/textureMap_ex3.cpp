#include<iostream>
#include<Windows.h> //�̰� GUI ������ �ʿ��Ѱǵ�.
#include<gl\GL.h>
#include<gl\GLU.h>
#include<gl\glut.h>
#include<gl\GLAUX.H>
#include<math.h> //�ﰢ�Լ� ������

/*
* gl.h (OpenGL Core Library) - ������ ����� �����ϴ� ���̺귯��
* glu.h (OpenGL Utility Library) - GL ���̺귯���� ����� ����
* glut.h (OpenGL Utility Toolkit) - ����� �Է��� �޾Ƶ��̰ų� ȭ�� �����츦 �����ϱ� ���� ���̺귯��
*/

/*
LNK2019 ERR >> Property -> Configuration Properties -> Linker -> Input -> Additional Dependencies -> add ��LEGACY_STDIO_DEFINITIONS.LIB��
+glaux.lib �߰�������Ѵ�.
*/

using namespace std;

GLfloat vertices[8][3] = { { -1, -1, 1 },{ -1, 1, 1 },{ 1, 1, 1 },{ 1, -1, 1 },{ -1, -1, -1 },{ -1, 1, -1 },{ 1, 1, -1 },{ 1, -1, -1 } };
GLfloat colors[8][3] = { { 0, 0, 1 },{ 0, 1, 1 },{ 1, 1, 1 },{ 1, 0, 1 },{ 0, 0, 0 },{ 0, 1, 0 },{ 1, 1, 0 },{ 1, 0, 0 } };

static float m_vertices[8][3], m_colors[8][4], m_normals[6][3];
GLubyte m_image[64][64][3];

AUX_RGBImageRec *texRec;
GLuint texID;

void m_v()
{
	m_vertices[0][0] = -1; m_vertices[0][1] = -1; m_vertices[0][2] = 1; m_vertices[1][0] = -1; m_vertices[1][1] = 1; m_vertices[1][2] = 1; m_vertices[2][0] = 1; m_vertices[2][1] = 1; m_vertices[2][2] = 1; m_vertices[3][0] = 1; m_vertices[3][1] = -1; m_vertices[3][2] = 1; m_vertices[4][0] = -1; m_vertices[4][1] = -1; m_vertices[4][2] = -1; m_vertices[5][0] = -1; m_vertices[5][1] = 1; m_vertices[5][2] = -1; m_vertices[6][0] = 1; m_vertices[6][1] = 1; m_vertices[6][2] = -1; m_vertices[7][0] = 1; m_vertices[7][1] = -1; m_vertices[7][2] = -1;
	m_colors[0][0] = 0; m_colors[0][1] = 0; m_colors[0][2] = 1; m_colors[0][3] = 1; m_colors[1][0] = 0; m_colors[1][1] = 1; m_colors[1][2] = 1; m_colors[1][3] = 1; m_colors[2][0] = 1; m_colors[2][1] = 1; m_colors[2][2] = 1; m_colors[2][3] = 1; m_colors[3][0] = 1; m_colors[3][1] = 0; m_colors[3][2] = 1; m_colors[3][3] = 1; m_colors[4][0] = 0; m_colors[4][1] = 0; m_colors[4][2] = 0; m_colors[4][3] = 1; m_colors[5][0] = 0; m_colors[5][1] = 1; m_colors[5][2] = 0; m_colors[5][3] = 1; m_colors[6][0] = 1; m_colors[6][1] = 1; m_colors[6][2] = 0; m_colors[6][3] = 1; m_colors[7][0] = 1; m_colors[7][1] = 0; m_colors[7][2] = 0; m_colors[7][3] = 1;
	m_normals[0][0] = 0; m_normals[0][1] = 0; m_normals[0][2] = 1;
	// (0, 3, 2, 1) 
	m_normals[1][0] = 0; m_normals[1][1] = 1; m_normals[1][2] = 0;
	// (1, 2, 6, 5) 
	m_normals[2][0] = 1; m_normals[2][1] = 0; m_normals[2][2] = 0;
	// (2, 3, 7, 6) 
	m_normals[3][0] = 0; m_normals[3][1] = -1; m_normals[3][2] = 0;
	// (3, 0, 4, 7) 
	m_normals[4][0] = 0; m_normals[4][1] = 0; m_normals[4][2] = -1;
	// (4, 5, 6, 7) 
	m_normals[5][0] = -1; m_normals[5][1] = 0; m_normals[5][2] = 0;
	// (5, 4, 0, 1)
}

void drawCube(int a, int b, int c, int d)
{
	glBegin(GL_QUADS);

	glNormal3fv(m_normals[a]);

	glColor3fv(colors[a]);
	glTexCoord2f(0.0, 0.0);
	glVertex3fv(vertices[a]);

	glColor3fv(colors[b]);
	glTexCoord2f(1.0, 0.0);
	glVertex3fv(vertices[b]);

	glColor3fv(colors[c]);
	glTexCoord2f(1.0, 1.0);
	glVertex3fv(vertices[c]);

	glColor3fv(colors[d]);
	glTexCoord2f(0.0, 1.0);
	glVertex3fv(vertices[d]);

	glEnd();
}

void timer(int value)
{
	glutPostRedisplay();//ȭ�� ������ �ʿ��� �� ���Լ��� �θ��� display �ݹ� �Լ��� �ٽ� �� �� ����
	glutTimerFunc(1000 / 30, timer, 1);//���̸� �ð� ���� ����,  ���ش� //30 frame ���´�.
									   //Ÿ�̸Ӱ� �۵��ϴµ� �ٽ� Ÿ�̸Ӹ� ���۽�Ŵ���ν� ��� �۵��ϰ� �Ǵ°���.
}

void RenderScene(void)
{
	//Clear the window with current clearing color
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT); //������ ������ �����.
														/*  DEPTH_BUFFER�� �ξ �ȼ����� ���� ���� �����ϵ��� �Ͽ���. �̸� �̿��ؼ� � ��ü�� �׸����� �Ҷ� �׾տ� ��ü�� ������ ������ ���̸�
														���Ͽ� DEPTH�� ���� �ﰢ������ �׷�����, DEPTH�� ū �簢������ �׸��� �ʰ� �� �� �ְԵǴ� ���̴�.
														(������ ��ǥ�� �ȼ��� ������ ���̿� ���� �׷��� ���� ���ƾ� ���� �����ϰ� �Ǵ� ���̴�.) */

	glMatrixMode(GL_MODELVIEW);//MODELVEIW�� �׸��°Ű� projection�� �����ϴ°Ŵ�.
	glLoadIdentity();

	gluLookAt(2.0, 2.0, 2.0, 0, 0, 0, 0, 1.0, 0); //���� �����̴�.
												  //ó���� ��, at�� ����κ��� �����ΰ� �̱⶧���� ���� �Ȱǵ鿩..
												  //up�� ���� ���ϴ°Ŵ�...��������� �����ΰ�.

	drawCube(0, 3, 2, 1);
	drawCube(1, 2, 6, 5);
	drawCube(2, 3, 7, 6);
	drawCube(3, 0, 4, 7);
	drawCube(4, 5, 6, 7);
	drawCube(5, 4, 0, 1);


	glutSwapBuffers(); //GLUT_DOUBLE ���۸� ������ؼ� �����ؼ������ ���̴�.
}

void SetupRC(void) {
	glClearColor(1.0f, 1.0f, 1.0f, 1.0f);
	glEnable(GL_DEPTH_TEST);
}


void LoadTexture()
{
	//TEXT("image/monalisa.bmp")
	texRec = auxDIBImageLoad(TEXT("image/monalisa.bmp"));
	//LNK2019 ERR >> Property -> Configuration Properties -> Linker -> Input -> Additional Dependencies -> add ��LEGACY_STDIO_DEFINITIONS.LIB��
	//�̰Ÿ� �Ѵ����� TEXT�� ���ִϱ� �����ذ� �Ǿ���.

	glGenTextures(1, &texID);
	glBindTexture(GL_TEXTURE_2D, texID);

	glTexImage2D(GL_TEXTURE_2D, 0, 3, texRec->sizeX, texRec->sizeY, 0, GL_RGB, GL_UNSIGNED_BYTE, texRec->data);

	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP);
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP);

	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);

}

void init(void)
{
	//	glutTimerFunc(1000 / 30, timer, 1);
	glColorMaterial(GL_FRONT, GL_AMBIENT_AND_DIFFUSE);
	glEnable(GL_COLOR_MATERIAL);

	glEnable(GL_TEXTURE_2D);
	LoadTexture();

}

void ChangeSize(int w, int h)
{
	if (h == 0)// 0���� ���������°� �������ؼ�
		h = 1;

	glViewport(0, 0, w, h); // ������ �������� �κ��� ����Ʈ�� ����Ѵ�.

	glMatrixMode(GL_PROJECTION); //�̺κ��� PROJECTION�� ������ 3D�� 2D�� �������Ѿ��Ѵ�.
								 //glOrtho�� ���ؼ�
	glLoadIdentity();//��ǥ�� �ʱ�ȭ
					 //�ٵ� �̰� ���� �ƴµ� ,�̷��� ���� ��ǥ�踦 �ʱ�ȭ����� �ϰ� �̷��� ���ȳ��ٵ�...
					 //��Ǳٵ� �Ƚᵵ ������°ű��� ....��ǥ�踦 �������� �����̻�????
					 //���⿡�� �ʱ�ȭ ���ִ� ������ ����?? � ��ǥ�踦 �ʱ�ȭ �����شٴ� ������??

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

	glutDisplayFunc(RenderScene);//�������Ǵ� �κ�
	glutReshapeFunc(ChangeSize);//SIZE�� �ٲ𸶴� ȣ���� �ȴ�.
								//	glutSpecialFunc(specialKeyboard);//����� Ű���� �̺�Ʈ�� �ݹ�
								//	glutKeyboardFunc(keyboard);//Ű���� �̺�Ʈ �ݹ�

								//453, 948 //453���� ���� 948������ �ﰢ������ ������ ���ִ�.....
								//�� ���� ������ ���̴� �����ΰ�???
								//

								//�Է¿Ϸ�



	init();
	SetupRC();
	glutMainLoop();
	//�⺻���� Ʋ������ �ܿ��ʿ���� ���迡�ȳ��´� ���躼�� �ش�.
}
