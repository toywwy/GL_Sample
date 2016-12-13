#define _CRT_SECURE_NO_WARNINGS
#pragma warning(disable:4996)


#include<iostream>
#include<Windows.h> //�̰� GUI ������ �ʿ��Ѱǵ�.
#include<gl/GL.h>
#include<gl/GLU.h>
#include<gl/glut.h> //�̰� ��ƿ��Ƽ ��Ŷ�ε�, ������� �Է��̳� ȭ�� �����츦 �����ϱ� ���ؼ���.
#include <math.h> //�ﰢ�Լ� ������
#include<vector>
#include<string>

#define SIZE 512
#define TORUS 36
#define CIRCLE 18

/*
* gl.h (OpenGL Core Library) - ������ ����� �����ϴ� ���̺귯��
* glu.h (OpenGL Utility Library) - GL ���̺귯���� ����� ����
* glut.h (OpenGL Utility Toolkit) - ����� �Է��� �޾Ƶ��̰ų� ȭ�� �����츦 �����ϱ� ���� ���̺귯��
*/

using namespace std;

float theta = 0;

//�ϳ��� �����س��� ��Ȱ�� �ؼ� ���Ŵ� ������ textures�� ����ϰ� ���� �ʿ� ���� �Ǵϱ�!
unsigned char woodMaterial[SIZE][SIZE][3];
unsigned char grayMaterial[SIZE][SIZE];

GLuint textures[6];

int flag = 0;

struct Point {
	float p[3];
	Point() {}
	Point(float x, float y, float z)
	{
		this->p[0] = x; this->p[1] = y; this->p[2] = z;
	}


	void SetPoint(float a, float b, float c)
	{
		this->p[0] = a; this->p[1] = b; this->p[2] = c;
	}

	Point operator- (Point a)
	{
		Point retP;
		for(int i = 0 ; i < 3; i++) retP.p[i] = this->p[i] - a.p[i];
		return retP;
	}

	friend Point CrossProduct(Point a, Point b)//friend ���� �س����� ���������� ����� �����ϴ�.
	{
		Point p(
			a.p[1] * b.p[2] - a.p[2] * b.p[1],
			a.p[2] * b.p[0] - a.p[0] * b.p[2],
			a.p[0] * b.p[1] - a.p[1] * b.p[0]
		);

		return getNormal(p);
	}

	friend Point getNormal(Point np) {
		float x, y, z, len = 0;

		x = np.p[0];
		y = np.p[1];
		z = np.p[2];

		len = x*x + y*y + z*z;
		len = sqrt(len);
		Point p(x / len, y / len, z / len);

		return p;
	}
};


vector<vector<Point>> v;
vector<Point> nv[TORUS][CIRCLE];//[j][i]�� �ٰ� �븻 ������ ������ ���´�.

void setTorusPoints()
{
	float move = 1.0;//�������� ���� �̵��� ��
	float r = 0.5;//������

	for (int yRot = 0; yRot <= 360; yRot = yRot + 10)//y�� ȸ�� x,z�� ���� �Ѵ�.
	{
		vector<Point> circlePoints;
		for (int zRot = 0; zRot <= 360; zRot += 20)//Circle ���� ���Ѱ��̴�. // ��� z�� ȸ�� x,y�� ����
		{
			//z�� ȸ�� ���� ����� �Ϳ� ���ؼ��� ���� �̵���������Ѵ�. +move ���ذ�
			float x = (move +  r* cos(zRot*3.141592 / 180.0)) * cos(yRot * 3.141592 / 180.0);
			float y = r * sin(zRot*3.141592 / 180.0);//��� ������ z�� ȸ�� ���� �Ǹ� �ȴ�.
			float z = (move + r * cos(zRot*3.141592 / 180.0)) * sin(yRot * 3.141592 / 180.0);
			//�ᱹ �̷� ������ ����� ���� ���̴�.

			circlePoints.push_back(Point(x, y, z));
		}
		v.push_back(circlePoints);//36���� ���� �����Ѵ�.
	}
}


Point getVertexNormal(int j, int i)
{
	float x, y, z,len;

	len = x = y = z = 0 ;

	for (Point np : nv[j][i])
	{
		x += np.p[0];
		y += np.p[1];
		z += np.p[2];
	}

	len = abs(x) + abs(y) + abs(z);
	/*len = sqrt(len);//�ű��ϰ� �긦 ���ټ��� ���� ��Ⱑ ��������.
	len = sqrt(len);*/
	Point p(x / len, y / len, z / len);

	return p;
}

void getNormal()
{
	for (int j = 0; j < TORUS; j++)
	{
		for (int i = 0; i < CIRCLE; i++)
		{
			//�����ʰ��� 0���� �ٲپ� ��� ��Ÿ�� ���� ���´�.
			int I = (i + 1) % CIRCLE;
			int J = (j + 1) % TORUS;

				
			//glVertex3fv(v[j][i].p);//p1
			//glVertex3fv(v[j][i + 1].p);//�긦 P0�� ����ȴ�.
			//glVertex3fv(v[j + 1][i + 1].p);//p2

			//(p1-p0) x (p2-p0) �ؼ� j,i������ ���� ���͸� ��� �Ǵ� ���̴�. 
			//[36][18].push(normal) ���߿� �� noraml ���� ���ؼ� ��� ���� ����ȭ �ϸ� �Ǵ� ���̴�.

			Point l1 = v[j][i] - v[j][i+1];
			Point l2 = v[j+1][i+1] - v[j][i+1];//�ݽð�� ������ �ϴϱ� �갡 ����

			Point np = CrossProduct(l2,l1);//������ ������ ������ ��ģ��!!! ���� ������������ �ٱ��� ���� ��������.
			nv[j][i].push_back(np);
			nv[J][i].push_back(np);
			nv[j][I].push_back(np);
			nv[J][I].push_back(np);

		}
	}

	for (int j = 0; j < TORUS; j++)
		for (int i = 0; i < CIRCLE; i++)
			nv[j][i][0] = getVertexNormal(j,i);

}

void drawTorus()
{
	glPushMatrix();

	float s = 1.0 / TORUS; //�����ҋ� ������ �ϳ��� �ؼ��� �����̴�.
	//ũ�� * TORUS�� �ϸ� ��ü �������ϴ� ���̴�.
	float t = 1.0 / CIRCLE;//�����ҋ� ������ �ϳ��� �ؼ��� ũ��

	glBindTexture(GL_TEXTURE_2D, textures[flag]);//�ؽ��ĸ� �����ϴµ� �ʼ��� �Ǵ� ����̴�.
	//��� �ؽ��ĸ� ���ε��ؼ� ����� ���ΰ��� �����Ѵ�.
	//textures[flag] ���⿡�� �ؽ��İ� ������ �Ǿ��ִ�.

	//��� ���� ���� �ؽ��� ���� �ϴ°� GL QUADS�� �ϴ� ����̴�.
	for (int j = 0; j < TORUS; j++)
	{
		for (int i = 0; i < CIRCLE; i++)
		{
			glBegin(GL_QUADS);//����� ������ �簢�� �׸��� ... ������ ������°ǰ�??
			{// �̷л��� �ݽð�� ���ƾ� �ϴϱ� (j1,i1)->(j2,i1)->(j2,i2)->(j1,i2) �̷��� �ؼ� �ѹ����� ���� �Ǵ� �����̴�.

				int I = (i + 1) % CIRCLE;
				int J = (j + 1) % TORUS;

				glNormal3fv(nv[j][i][0].p);
				glTexCoord2f(s*j, t*i);
				glVertex3fv(v[j][i].p);

				glNormal3fv(nv[j][I][0].p);
				glTexCoord2f(s*j, t*(i + 1));
				glVertex3fv(v[j][i + 1].p);

				glNormal3fv(nv[J][I][0].p);
				glTexCoord2f(s*(j + 1), t*(i + 1));
				glVertex3fv(v[j + 1][i + 1].p);

				glNormal3fv(nv[J][I][0].p);
				glTexCoord2f(s*(j + 1), t*i);
				glVertex3fv(v[j + 1][i].p);
			}
			glEnd();
		}
	}

	glPopMatrix();
}


void timer(int value)
{
	glutPostRedisplay();//ȭ�� ������ �ʿ��� �� ���Լ��� �θ��� display �ݹ� �Լ��� �ٽ� �� �� ����
	glutTimerFunc(1000, timer, 1);//���̸� �ð� ���� ����,  ���ش� //30 frame ���´�.
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
	gluLookAt(0, 2, 2, 0, 0, 0, 0, 1.0, 0); //���� �����̴�.
											//drawAxes();

	drawTorus();
	glutSwapBuffers(); //GLUT_DOUBLE ���۸� ������ؼ� �����ؼ������ ���̴�.
}

void SetupRC(void) {
	glClearColor(1.0f, 1.0f, 1.0f, 1.0f);
	glEnable(GL_DEPTH_TEST);
	glEnable(GL_COLOR_MATERIAL);
	glEnable(GL_TEXTURE_2D);

	//�븻������� ������ ������ �������� ����� ������ ���̴�.
	glEnable(GL_LIGHTING);
	glEnable(GL_LIGHT0);
}


int readTexture(char *filename) {

	FILE *texture = fopen(filename, "rb");


	if (texture == NULL) {
		puts("File open failed");
		return -1;
	}
	else {
		puts("File open success");
		fread(woodMaterial, sizeof(unsigned char), SIZE * SIZE * 3, texture);

		fclose(texture);

		return 0;
	}

}

int readGrayTexture(char *filename) {


	FILE *texture = fopen(filename, "rb");

	if (texture == NULL) {
		puts("File open failed");
		return -1;
	}
	else {
		puts("File open success");
		fread(grayMaterial, sizeof(unsigned char), SIZE * SIZE, texture);
		fclose(texture);

		return 0;
	}

}

void init(void)
{

	//initCircle();
	setTorusPoints();
	getNormal();

	//readTexture(unsigned char *src, int width, int height, char *filename) {
	vector<char *> fileNames;
	fileNames.push_back("material/wood.raw");
	fileNames.push_back("material/check.raw");
	fileNames.push_back("material/donut.raw");
	fileNames.push_back("material/marble.raw");

	//���⼭ ���ʹ� �׷��� ������
	fileNames.push_back("material/logo2.raw");
	fileNames.push_back("material/grayscale_ornament.raw");

	glGenTextures(6, textures);//�������� �ؽ��ĸ� ����ϱ����� textures�� Generate�Ѵ�.


	for (int i = 0; i < 6; i++)
	{
		if (i < 4)
		{
			readTexture(fileNames[i]);
			glBindTexture(GL_TEXTURE_2D, textures[i]);

			glTexImage2D(GL_TEXTURE_2D, 0, 3, 512, 512, 0, GL_RGB, GL_UNSIGNED_BYTE, woodMaterial);

			glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP);
			glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP);

			//GL_NEAREST is generally faster than GL_LINEAR, 
			//but it can produce textured images with sharper edges because the transition between texture elements is not as smooth. The initial value of GL_TEXTURE_MAG_FILTER is GL_LINEAR.
			glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
			glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
		}
		else
		{
			readGrayTexture(fileNames[i]);

			glBindTexture(GL_TEXTURE_2D, textures[i]);

			glTexImage2D(GL_TEXTURE_2D, 0, 3, 512, 512, 0, GL_LUMINANCE, GL_UNSIGNED_BYTE, grayMaterial);

			glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP);
			glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP);

			//GL_NEAREST is generally faster than GL_LINEAR, 
			//but it can produce textured images with sharper edges because the transition between texture elements is not as smooth. The initial value of GL_TEXTURE_MAG_FILTER is GL_LINEAR.
			glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
			glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
		}

	}
	glutTimerFunc(1000, timer, 1);
}

void ChangeSize(int w, int h)
{
	if (h == 0)// 0���� ���������°� �������ؼ�
		h = 1;

	glViewport(0, 0, w, h); // ������ �������� �κ��� ����Ʈ�� ����Ѵ�.

	glMatrixMode(GL_PROJECTION); //�̺κ��� PROJECTION�� ������ 3D�� 2D�� �������Ѿ��Ѵ�.
								 //glOrtho�� ���ؼ�
	glLoadIdentity();//��ǥ�� �ʱ�ȭ

	if (w <= h)
		glOrtho(-2.0, 2.0, -2.0*(float)h / (float)w,
			2.0*(float)h / (float)w, -10.0, 10.0);
	else
		glOrtho(-2.0*(float)w / (float)h, 2.0*(float)w / (float)h,
			-2.0, 2.0, -10.0, 10.0);
}


void keyboard(unsigned char c, int x, int y)
{
	cout << "input key = " << c << endl;
	switch (c)
	{
	case VK_ESCAPE:
		exit(0);
		break;

	case 's':
		flag = 0;
		break;
	case 't':
		flag = 1;
		break;
	case 'u':
		flag = 2;
		break;
	case 'v':
		flag = 3;
		break;
	case 'w':
		flag = 4;
		break;
	case 'x':
		flag = 5;
		break;
	}

	glutPostRedisplay();
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
	glutKeyboardFunc(keyboard);//Ű���� �̺�Ʈ �ݹ�


	init();
	SetupRC();
	glutMainLoop();
	//�⺻���� Ʋ������ �ܿ��ʿ���� ���迡�ȳ��´� ���躼�� �ش�.
}
