#include<iostream>
#include<Windows.h> //�̰� GUI ������ �ʿ��Ѱǵ�.
#include<gl\GL.h>
#include<gl\GLU.h>
#include<gl\glut.h>
#include<gl\GLAUX.H>
#include<math.h> //�ﰢ�Լ� ������

/*
LNK2019 ERR >> Property -> Configuration Properties -> Linker -> Input -> Additional Dependencies -> add ��LEGACY_STDIO_DEFINITIONS.LIB��
+glaux.lib �߰�������Ѵ�.
*/

/*
* gl.h (OpenGL Core Library) - ������ ����� �����ϴ� ���̺귯��
* glu.h (OpenGL Utility Library) - GL ���̺귯���� ����� ����
* glut.h (OpenGL Utility Toolkit) - ����� �Է��� �޾Ƶ��̰ų� ȭ�� �����츦 �����ϱ� ���� ���̺귯��
*/

using namespace std;

GLfloat vertices[8][3] = { { -1, -1, 1 },{ -1, 1, 1 },{ 1, 1, 1 },{ 1, -1, 1 },{ -1, -1, -1 },{ -1, 1, -1 },{ 1, 1, -1 },{ 1, -1, -1 } };
GLfloat colors[8][3] = { { 0, 0, 1 },{ 0, 1, 1 },{ 1, 1, 1 },{ 1, 0, 1 },{ 0, 0, 0 },{ 0, 1, 0 },{ 1, 1, 0 },{ 1, 0, 0 } };
//{ {255,255,255},{ 255,255,255 },{ 255,255,255 },{ 255,255,255 },{ 255,255,255 },{ 255,255,255 },{ 255,255,255 },{ 255,255,255 } };


static float m_vertices[8][3], m_colors[8][4], m_normals[6][3];
GLubyte m_image[64][64][3];

AUX_RGBImageRec *texRec[2];
GLuint texID[2];

void m_v()
{
	m_vertices[0][0] = -1; m_vertices[0][1] = -1; m_vertices[0][2] = 1; 
	m_vertices[1][0] = -1; m_vertices[1][1] = 1; m_vertices[1][2] = 1; 
	m_vertices[2][0] = 1; m_vertices[2][1] = 1; m_vertices[2][2] = 1; 
	m_vertices[3][0] = 1; m_vertices[3][1] = -1; m_vertices[3][2] = 1; 
	m_vertices[4][0] = -1; m_vertices[4][1] = -1; m_vertices[4][2] = -1;
	m_vertices[5][0] = -1; m_vertices[5][1] = 1; m_vertices[5][2] = -1;
	m_vertices[6][0] = 1; m_vertices[6][1] = 1; m_vertices[6][2] = -1;
	m_vertices[7][0] = 1; m_vertices[7][1] = -1; m_vertices[7][2] = -1;

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

	glNormal3fv(m_normals[a]);//�̰� �ʿ䰡 ���µ� ���ִ°��� �𸣰ڳ�..
	//x-y���� ��� ��Ÿ�����ĸ� ���°� ������...

	glColor3fv(colors[a]);
	glVertex3fv(vertices[a]);

	glColor3fv(colors[b]);
	glVertex3fv(vertices[b]);

	glColor3fv(colors[c]);
	glVertex3fv(vertices[c]);

	glColor3fv(colors[d]);
	glVertex3fv(vertices[d]);

	glEnd();
}

void timer(int value)
{
	glutPostRedisplay();//ȭ�� ������ �ʿ��� �� ���Լ��� �θ��� display �ݹ� �Լ��� �ٽ� �� �� ����
	glutTimerFunc(1000 / 30, timer, 1);//���̸� �ð� ���� ����,  ���ش� //30 frame ���´�.
									   //Ÿ�̸Ӱ� �۵��ϴµ� �ٽ� Ÿ�̸Ӹ� ���۽�Ŵ���ν� ��� �۵��ϰ� �Ǵ°���.
}
void drawPlanXY(int w,int h)
{
	float size = 8;

	if (w > h) size = 8 * w / h;
	else size = 8 * h / w;

	int z = 0;
	glColor3f(255, 255, 255);
	glBegin(GL_QUADS);

	glTexCoord2f(0.0, 0.0);
	glVertex3f(0,0,z);

	glTexCoord2f(5, 0.0);
	glVertex3f(size, 0, z);
	
	glTexCoord2f(5, 5);
	glVertex3f(size, size,z);

	glTexCoord2f(0.0, 5);
	glVertex3f(0, size, z);

	
	glEnd();

}
void drawPlanYZ(int w,int h)
{
	float size = 8;

	if (w > h) size = 8 * w / h;
	else size = 8 * h / w;

	int z = 0;
	glColor3f(255, 255, 255);
	glBegin(GL_QUADS);

	glTexCoord2f(0.0, 0.0);
	glVertex3f(0, 0, 0);

	glTexCoord2f(5, 0.0);
	glVertex3f(0, 0, size);

	glTexCoord2f(5, 5);
	glVertex3f(0,size, size);

	glTexCoord2f(0.0, 5);
	glVertex3f(0, size, 0);


	glEnd();
}
void drawPlanXZ(int w, int h)
{
	float size = 8;

	if (w > h) size = 8 * w / h;
	else size = 8 * h / w;

	int z = 0;
	glColor3f(255, 255, 255);
	glBegin(GL_QUADS);
	
	glTexCoord2f(0.0, 0.0);
	glVertex3f(0, 0, 0);

	glTexCoord2f(5, 0.0);
	glVertex3f(0, 0, size);

	glTexCoord2f(5, 5);
	glVertex3f(size,0, size);

	glTexCoord2f(0.0, 5);
	glVertex3f(size,0 , 0);


	glEnd();
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
	int wWidth = glutGet(GLUT_WINDOW_WIDTH);
	int wHeight = glutGet(GLUT_WINDOW_HEIGHT);

	//���⼭�� ����� 3�������� ���ָ�ȴ�..... �� ���� ������

	glBindTexture(GL_TEXTURE_2D, texID[0]);
	drawPlanXY(wWidth, wHeight);//�̰԰� �Ȱ��� �����̾ȵȴ� �ٸ��� �ٵǴµ�

	glBindTexture(GL_TEXTURE_2D, texID[0]);
	drawPlanYZ(wWidth, wHeight);

	glBindTexture(GL_TEXTURE_2D, texID[1]);
	drawPlanXZ(wWidth, wHeight);
	
	//�̺κ��� ���� ���� ���ε� ���ִ� ���̴�. ���� ���ִ°� �̺κ�����..
	drawCube(0, 3, 2, 1);//���ʸ鿡�� texID[0]�� �����ϰ� ..�׳� ���� �鿡 �׷����� �����ع�����.
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
	/*
		Ǯ�̹� �׳� ...����� ���� �׸��ڿ�, �� ������ ť�긦 ��ġ ��Ű�� �ɰͰ��� ....
		�𳪸��ڴ� �ٵ������� �迭�� �ص� �� ��... x-y�� �̷�� ��� x-z�� �̷�� ��� y-z�� �̷�� ��� ..�̷��� 3���� ����� �ǰڴ�.....
		���� �� ��ǥ�迴����... ���� ����ǥ�� z-x ����� ������ ������ �ϸ� �� �� ��
	*/
	texRec[0] = auxDIBImageLoad(TEXT("image/monalisa.bmp"));
	texRec[1] = auxDIBImageLoad(TEXT("image/logo.bmp"));

	glGenTextures(2, &texID[0]);

	for (register int i = 0; i < 2; i++)
	{
		glBindTexture(GL_TEXTURE_2D, texID[i]);

		glTexImage2D(GL_TEXTURE_2D, 0, 3, texRec[i]->sizeX, texRec[i]->sizeY, 0, GL_RGB, GL_UNSIGNED_BYTE, texRec[i]->data);


		//�̰Ա� coordinate s,t �װű��� .... s,t �Ǥ� ������ GL_REPEAT�����ϰڴ�... default�� REPEAT �̶� ���� ���� ���ص��ȴ�.
		glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
		glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);

		glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
		glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);

		delete[] texRec[i]->data;
		delete texRec[i];
	}
}

void init(void)
{
	//	glutTimerFunc(1000 / 30, timer, 1);
	glColorMaterial(GL_FRONT, GL_AMBIENT_AND_DIFFUSE);
	glEnable(GL_COLOR_MATERIAL);

	glEnable(GL_TEXTURE_2D);

	m_v();
	for (int i = 0; i < 8; i++)
		for (int j = 0; j < 3; j++)
			vertices[i][j] += 1;

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

	int s = 3;
	if (w <= h) glOrtho(-s, s, -s*(float)h / (float)w, s*(float)h / (float)w, -s, s);
	else glOrtho(-s*(float)w / (float)h, s*(float)w / (float)h, -s, s, -s, s);
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

	init();
	SetupRC();
	glutMainLoop();
	//�⺻���� Ʋ������ �ܿ��ʿ���� ���迡�ȳ��´� ���躼�� �ش�.
}
