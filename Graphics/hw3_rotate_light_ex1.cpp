#include<iostream>
#include<Windows.h> //�̰� GUI ������ �ʿ��Ѱǵ�.
#include<gl/GL.h>
#include<gl/GLU.h>
#include<gl/glut.h> //�̰� ��ƿ��Ƽ ��Ŷ�ε�, ������� �Է��̳� ȭ�� �����츦 �����ϱ� ���ؼ���.
#include<math.h> //�ﰢ�Լ� ������
#include<vector>

#define _CRT_SECURE_NO_WARNINGS
#pragma warning(disable:4996)
/*
* gl.h (OpenGL Core Library) - ������ ����� �����ϴ� ���̺귯��
* glu.h (OpenGL Utility Library) - GL ���̺귯���� ����� ����
* glut.h (OpenGL Utility Toolkit) - ����� �Է��� �޾Ƶ��̰ų� ȭ�� �����츦 �����ϱ� ���� ���̺귯��
*/

using namespace std;

typedef float point3[3];

typedef pair<float *, float *> P;
vector<P> v;

int cnt = 0;
vector<vector<float *>> vNormal;

int pCnt, iCnt; // position, indices 
float **positions;// [453][3];
int **indices;//[948][3];

int fFlag = 1;
int vFlag = 0;

int pFlag=1;
int dFlag=0;
int sFlag=0;

//�̰� �迭�� �ּ� ���� �� ���ϴ�.
int cFlagIdx = 0;
float cutOff[5] = { 5,10,15,20,25};
int nFlagIdx = 0;
float shininess[4] = { 0,30,60,100 };

static float theta;

GLfloat pX, pY, pZ;

float tx, ty,tz;

void timer(int value)
{
	theta += 90;
	if (theta >= 360.0) theta =0;
	
	nFlagIdx += 1;
	if (nFlagIdx == 4) nFlagIdx = 0;

	cFlagIdx += 1;
	if (cFlagIdx == 5) cFlagIdx = 0;

	glutPostRedisplay();//ȭ�� ������ �ʿ��� �� ���Լ��� �θ��� display �ݹ� �Լ��� �ٽ� �� �� ����
	glutTimerFunc(1000, timer, 1);//���̸� �ð� ���� ����,  ���ش� //30 frame ���´�.
									   //Ÿ�̸Ӱ� �۵��ϴµ� �ٽ� Ÿ�̸Ӹ� ���۽�Ŵ���ν� ��� �۵��ϰ� �Ǵ°���.
}

void Normalize(point3 p)
{
	double d = p[0] * p[0] + p[1] * p[1] + p[2] * p[2];

	if (d > 0)
	{
		float len = (float)(1.0 / sqrt(d));
		p[0] *= len;
		p[1] *= len;
		p[2] *= len;
	}
}

void CrossProduct(float* a, float* b, float* c, float* &r)
{
	r[0] = (b[1] - a[1]) * (c[2] - a[2]) - (b[2] - a[2]) * (c[1] - a[1]);
	r[1] = (b[2] - a[2]) * (c[0] - a[0]) - (b[0] - a[0]) * (c[2] - a[2]);
	r[2] = (b[0] - a[0]) * (c[1] - a[1]) - (b[1] - a[1]) * (c[0] - a[0]);
	Normalize(r);
}

void fDrawFill()
{
	glBegin(GL_TRIANGLES);
	{
		for (int i = 0; i < iCnt; i++)
		{
			float * n = new float[3];

			CrossProduct(positions[indices[i][0]], positions[indices[i][1]], positions[indices[i][2]], n);
			glNormal3fv(n);
			glVertex3fv(positions[indices[i][0]]);
			glVertex3fv(positions[indices[i][1]]);
			glVertex3fv(positions[indices[i][2]]);
		}
	}
	glEnd();
}

void vDrawFill()
{
	glBegin(GL_TRIANGLES);
	{
		for (int i = 0; i < iCnt; i++)
		{
			glNormal3fv(positions[indices[i][0]]);//�� �븻 �� �̿���ϳ�?
			glVertex3fv(positions[indices[i][0]]);

			glNormal3fv(positions[indices[i][1]]);
			glVertex3fv(positions[indices[i][1]]);

			glNormal3fv(positions[indices[i][2]]);
			glVertex3fv(positions[indices[i][2]]);
		}
	}
	glEnd();
}
void drawLines()
{
	glBegin(GL_TRIANGLES);
	{
		for (int i = 0; i < iCnt; i++)
		{
			float * n = new float[3];

			CrossProduct(positions[indices[i][0]], positions[indices[i][1]], positions[indices[i][2]], n);
			//glNormal3fv(n);

			float *loc = new float[3];

			for (int j = 0; j < 3; j++)
			{
				glVertex3fv(positions[indices[i][j]]);
				vNormal[indices[i][j]].push_back(n);//�� 3�� ���ٰ� �� �־�����Ѵ�.
				loc[j] = 0;//�ʱ�ȭ �������.
				for (int k = 0; k < 3; k++) loc[j] += positions[indices[i][k]][j];
				loc[j] /= 3;
			}
			v.push_back(P(loc, n)); //���⿡ ���� �׸��� �ٵ���.
		}
	}
	glEnd();
}

void drawNormalVectors()
{
	for (P p : v)
	{
		glBegin(GL_LINES);
		{
			glVertex3f(p.first[0], p.first[1], p.first[2]);//�븻 ���Ϳ� ���� �ֵ��̴�.
			glVertex3f(p.second[0], p.second[1], p.second[2]);//��� ���� ������ �ǹ̸� 
		}
		glEnd();
	}
}

void drawNormalVertexVectors()
{
	for (int idx = 0; idx < vNormal.size(); idx++)
	{//idx�� �ε�����. ���� ���� �ε�����
		float x, y, z, m;
		m = x = y = z = 0;
		//Gouraud Shading�� �Ѵ�.
		//���� �ε��� IDX���� �����ߴ� �븻 ���͵��� ���ͷ����� ���鼭 ��罦�̵��� ���ش�.
		for (float * f : vNormal[idx])
		{
			x += f[0];
			y += f[1];
			z += f[2];
			m += abs(f[0]) + abs(f[1]) + abs(f[2]);//���� ���ͷ� ������ֱ� ����.
		}

		glBegin(GL_LINES);
		{
			glVertex3f(x / m*1.5, y / m*1.5, z / m*1.5);
			glVertex3fv(positions[idx]);
		}
		glEnd();

	}
}


void lightSettings()
{
	//������ ��ġ ������ ���ڰ� 0�̸� directional ��
	//���谡 �������� �� �����̳� ���� ���� ��� ������
	glDisable(GL_LIGHT1);
	GLfloat position0[4] = { pX,pY,pZ,pFlag };
	glLightfv(GL_LIGHT0, GL_POSITION, position0);

	//������ ���� �����Ѵ�. �갡 �־�� �����ִ°Ű�...
	GLfloat ambient0[4] = { 0.1, 0.1, 0.1, 1.0f };
	GLfloat diffuse0[4] = { 0.4, 0.4, 0.4, 1 };
	GLfloat specular0[4] = { 0.2 ,0.2,0.2, 1.0 };

	glLightfv(GL_LIGHT0, GL_AMBIENT, ambient0);
	glLightfv(GL_LIGHT0, GL_DIFFUSE, diffuse0);
	glLightfv(GL_LIGHT0, GL_SPECULAR, specular0);

	//������ ����
	glLightf(GL_LIGHT0, GL_CONSTANT_ATTENUATION, 0.2);
	glLightf(GL_LIGHT0, GL_LINEAR_ATTENUATION, 0.1);
	glLightf(GL_LIGHT0, GL_QUADRATIC_ATTENUATION, 0.05);

	GLfloat mat_ambient[4] = { 0.3,0.0,0.0,1.0 };
	GLfloat mat_diffuse[4] = { 1,0.5,0.5,1.0 };
	GLfloat mat_specular[4] = { 1,1,1,1 };
	GLfloat mat_shininess = shininess[nFlagIdx];


	//Material ��Ƽ���󿡴� GL_FORNT, GL_BACK ,GL_FRONT_AND_BACK�� �����Ѵ�. ����
	//��Ƽ������ specifying�Ѵ�.
	glMaterialfv(GL_FRONT, GL_AMBIENT, mat_ambient);
	glMaterialfv(GL_FRONT, GL_DIFFUSE, mat_diffuse);
	glMaterialfv(GL_FRONT, GL_SPECULAR, mat_specular);
	glMaterialf(GL_FRONT, GL_SHININESS, mat_shininess);
}

void spotLightSettings()
{

	glEnable(GL_LIGHT1);
	
	GLfloat position0[4] = { pX,pY,pZ,1 };
	glLightfv(GL_LIGHT1, GL_POSITION, position0);

	//������ ���� �����Ѵ�. �갡 �־�� �����ִ°Ű�...
	GLfloat ambient0[4] = { 0.1, 0.1, 0.1, 1.0f };
	GLfloat diffuse0[4] = { 0.5, 0.5, 0.5, 1 };
	GLfloat specular0[4] = { 0.2 ,0.2,0.2, 1.0 };

	glLightfv(GL_LIGHT1, GL_AMBIENT, ambient0);
	glLightfv(GL_LIGHT1, GL_DIFFUSE, diffuse0);
	glLightfv(GL_LIGHT1, GL_SPECULAR, specular0);

	//������ ����
	glLightf(GL_LIGHT1, GL_CONSTANT_ATTENUATION, 0.2);
	glLightf(GL_LIGHT1, GL_LINEAR_ATTENUATION, 0.1);
	glLightf(GL_LIGHT1, GL_QUADRATIC_ATTENUATION, 0.05);

	GLfloat spot_direction[3] = { -1,-1 ,-1 };
	glLightfv(GL_LIGHT1, GL_SPOT_DIRECTION, spot_direction);
	glLightf(GL_LIGHT1, GL_SPOT_CUTOFF, cutOff[cFlagIdx]);
	glLightf(GL_LIGHT1, GL_SPOT_EXPONENT, 30);//�̷��� �θ�..//ũ�� �ѻ��·��ϸ� ����

	GLfloat mat_ambient[4] = { 0.3,0.0,0.0,1.0 };
	GLfloat mat_diffuse[4] = { 1,0.5,0.5,1.0 };
	GLfloat mat_specular[4] = { 1,1,1,1 };
	GLfloat mat_shininess = shininess[nFlagIdx];


	//Material ��Ƽ���󿡴� GL_FORNT, GL_BACK ,GL_FRONT_AND_BACK�� �����Ѵ�. ����
	//��Ƽ������ specifying�Ѵ�.
	glMaterialfv(GL_FRONT, GL_AMBIENT, mat_ambient);
	glMaterialfv(GL_FRONT, GL_DIFFUSE, mat_diffuse);
	glMaterialfv(GL_FRONT, GL_SPECULAR, mat_specular);
	glMaterialf(GL_FRONT, GL_SHININESS, mat_shininess);

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

	gluLookAt(0.2, 0.2, 0.2, 0, 0, 0, 0, 1, 0); //���� �����̴�.


	float cos_th = cos(theta * 3.14159 / 180);
	float sin_th = sin(theta * 3.14159 / 180);

	float rotateMatrix[16] = {
		cos_th,	0,	-sin_th,	0,
		0,		1,	0,		0,
		sin_th,0,	cos_th,	0,
		0,		0,	0,		1
	};

	float translateMatrixPre[16] = {
		1,	0,	0,	0,
		0,	1,	0,	0,
		0,	0,	1,	0,
		tx,ty,tz,1,
	};

	float translateMatrixPost[16] = {
		1,	0,	0,	0,
		0,	1,	0,	0,
		0,	0,	1,	0,
		-tx,-ty,-tz,1,
	};

	glPushMatrix();
	{
		glMultMatrixf(translateMatrixPre);// 1,1,1 ������ �̵�
		glMultMatrixf(rotateMatrix);// theta ��ŭ ȸ��
		glMultMatrixf(translateMatrixPost);//-1,-1,-1 �������� �̵�

		if (sFlag)
		{
			spotLightSettings();
		}
		else {
			lightSettings();
		}
	}
	glPopMatrix();
	


	glEnable(GL_POLYGON_OFFSET_FILL);// offset ������ enable �������
	glPolygonOffset(10, 0);//factor, units �̴�.
						   //factor > create a variable depth offset for each polygon. ���� �������� �ټ� �ִ°Ŵ� �� �����￡
						   //factor > Specifies a scale factor that is used to create a variable depth offset for each polygon. The initial value is zero.
						   //units > Specifies a value that is multiplied by an implementation-specific value to create a constant depth offset. The initial value is 0.

	glPolygonMode(GL_FRONT, GL_FILL);
	glColor3f(0.5, 0.5, 0.5);

	if (fFlag) fDrawFill(); //draw
	else vDrawFill();

	glDisable(GL_POLYGON_OFFSET_FILL);

		//���� ������ �� �׸��� �κ�
	glColor3f(0, 0, 0);
	glPolygonMode(GL_FRONT, GL_LINE);
	drawLines();//draw

	glutSwapBuffers(); //GLUT_DOUBLE ���۸� ������ؼ� �����ؼ������ ���̴�.
}



void SetupRC(void) {
	glClearColor(1.0f, 1.0f, 1.0f, 1.0f);
	glEnable(GL_DEPTH_TEST);

	//BACK�� CULL �ϰڴ� �ȱ׸��ž�.
	glEnable(GL_CULL_FACE);
	glEnable(GL_NORMALIZE);
	glCullFace(GL_BACK);


	//���� Ȱ��ȭ
	glEnable(GL_LIGHTING);
	glEnable(GL_LIGHT0);
	glShadeModel(GL_SMOOTH);

	lightSettings();
	//spotLightSettings();
}

void init(void)
{
	theta=0;
	pX = pY = pZ = 1;
	tx = ty = tz = 0;
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
					 //�ٵ� �̰� ���� �ƴµ� ,�̷��� ���� ��ǥ�踦 �ʱ�ȭ����� �ϰ� �̷��� ���ȳ��ٵ�...
					 //��Ǳٵ� �Ƚᵵ ������°ű��� ....��ǥ�踦 �������� �����̻�????
					 //���⿡�� �ʱ�ȭ ���ִ� ������ ����?? � ��ǥ�踦 �ʱ�ȭ �����شٴ� ������??

	if (w <= h)
		glOrtho(-1.0, 1.0, -1.0*(float)h / (float)w,
			1.0*(float)h / (float)w, -1.0, 1.0);
	else
		glOrtho(-1.0*(float)w / (float)h, 1.0*(float)w / (float)h,
			-1.0, 1.0, -1.0, 1.0);
}


void readFile()
{
	freopen("bunny_origin.txt", "r", stdin);

	cin >> pCnt >> iCnt;

	positions = new float*[pCnt]; ///453
	indices = new int *[iCnt]; ///948
	vNormal.resize(pCnt);
	for (int i = 0; i < pCnt; i++) positions[i] = new float[3];
	for (int i = 0; i < iCnt; i++) indices[i] = new int[3];
	for (int i = 0; i < pCnt; i++) cin >> positions[i][0] >> positions[i][1] >> positions[i][2];

	for (int i = 0; i < iCnt; i++)
	{
		cin >> indices[i][0] >> indices[i][1] >> indices[i][2];
		indices[i][0]--; indices[i][1]--; indices[i][2]--;
	}
}

void keyboard(unsigned char c, int x, int y)
{
	cout << "input key = " << c << endl;
	switch (c)
	{
	case VK_ESCAPE:
		exit(0);
		break;

	case 'f'://Draw face normal vectors. ��f�� key to toggle. (a)
		fFlag = 1;
		break;

	case 'v'://Draw vertex normal vectors.��v�� key to toggle. (b)
		fFlag = 0;
		break;
	case 'p': // toggle point
		sFlag = 0; //�̷��� �Ǹ� �ƿ��� ������ ��Ȳ �߻�
		pFlag = 1;
		break;

	case 'd'://toggle directional
		pFlag = sFlag = 0;
		pFlag = 0;
		break;

	case 's'://toggle spot ���� �������� ��� ����������.
		pFlag = 1;
		if (sFlag) sFlag = 0;
		else sFlag = 1;

		break;

	case 'c'://����� �ȴ�. Change the cut-off angle of the spot light
		//5 10 15 20 25 1�ʿ� 5�� ������ ��ȭ�ϸ� �ɵ��ϴ�.
		cFlagIdx += 1;
		if (cFlagIdx == 5) cFlagIdx = 0;

		break;

	case 'n'://Change the shininess coefficient of the spot light 
		//n Ű�� ����� �����ϸ鼭 
		//4�ʴ� �ϳ��� ����Ŭ 4�� �ؼ� �ϸ� �ɵ��ϰ�
		nFlagIdx += 1;
		if (nFlagIdx == 4) nFlagIdx = 0;

	case 'z':
		if (tx == 1) tx = ty = tz = 0;
		else tx = ty = tz = 1;

		cout << tx << ", " << ty << ", " << tz << endl;

		break;
	}

	glutPostRedisplay();
}


void main(int argc, char * argv[])
{
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB);
	glutInitWindowSize(800, 800);
	glutCreateWindow("Simple");

	readFile();

	glutDisplayFunc(RenderScene);//�������Ǵ� �κ�
	glutReshapeFunc(ChangeSize);//SIZE�� �ٲ𸶴� ȣ���� �ȴ�.
								//	glutSpecialFunc(specialKeyboard);//����� Ű���� �̺�Ʈ�� �ݹ�
	glutKeyboardFunc(keyboard);//Ű���� �̺�Ʈ �ݹ�

	init();
	SetupRC();
	glutMainLoop();
	//�⺻���� Ʋ������ �ܿ��ʿ���� ���迡�ȳ��´� ���躼�� �ش�.
}
