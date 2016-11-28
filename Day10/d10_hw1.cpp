#include<iostream>
#include<Windows.h> //이건 GUI 때문에 필요한건데.
#include<gl/GL.h>
#include<gl/GLU.h>
#include<gl/glut.h> //이건 유틸리티 툴킷인데, 사용자의 입력이나 화면 윈도우를 제어하기 위해서다.
#include <math.h> //삼각함수 쓰려고
#include<cstdio>

/*
* gl.h (OpenGL Core Library) - 렌더링 기능을 제공하는 라이브러리
* glu.h (OpenGL Utility Library) - GL 라이브러러의 도우미 역할
* glut.h (OpenGL Utility Toolkit) - 사용자 입력을 받아들이거나 화면 윈도우를 제어하기 위한 라이브러리
*/


/*
Ray Tracing은 GL 버전이 낮아서 어렵고 Pickling을 하는프로젝트이다.

*/

using namespace std;


struct Point
{
	float p[3];
};

struct Vector
{
	float p[3];
};

struct Matrix
{
	float m[9];
};

void SetPoint(Point *p, float a, float b, float c)
{
	p->p[0] = a;
	p->p[1] = b;
	p->p[2] = c;
}

void SetVector(Vector *v, float a, float b, float c)
{
	v->p[0] = a;
	v->p[1] = b;
	v->p[2] = c;

}

void SetMatrix(Matrix * m, float _00, float _01, float _02,
	float _10, float _11, float _12,
	float _20, float _21, float _22)
{
	m->m[0] = _00; m->m[1] = _01; m->m[2] = _02;
	m->m[3] = _10; m->m[4] = _11; m->m[5] = _12;
	m->m[6] = _20; m->m[7] = _21; m->m[8] = _22;
}


float DotProduct(Vector *v1, Vector *v2)
{
	float s = 0.0f;
	for (int i = 0; i < 3; i++)
		s += v1->p[i] * v2->p[i];

	return s;
}

Vector CrossProduct(Vector *v1, Vector *v2)
{
	Vector v3;

	v3.p[0] = (v1->p[1] * v2->p[2]) - (v1->p[2] * v2->p[1]);
	v3.p[1] = (v1->p[2] * v2->p[0]) - (v1->p[0] * v2->p[2]);
	v3.p[2] = (v1->p[0] * v2->p[1]) - (v1->p[1] * v2->p[0]);

	return v3;
}

float VecLength(Vector *v)
{
	return sqrt((v->p[0] * v->p[0]) + (v->p[1] * v->p[1]) + (v->p[2] * v->p[2]));
}

Vector Normalize(Vector *v)
{
	Vector v1;
	float length = VecLength(v);
	v1.p[0] = v->p[0] / length;
	v1.p[1] = v->p[1] / length;
	v1.p[2] = v->p[2] / length;

	return v1;
}

Vector InverseVector(Vector *v)
{
	Vector v1;
	for (int i = 0; i < 3; i++)
		v1.p[i] = -(v->p[i]);

	return v1;
}

Matrix MatrixTranspose(Matrix *m)
{
	Matrix c;
	c.m[0] = m->m[0];
	c.m[1] = m->m[3];
	c.m[2] = m->m[6];

	c.m[3] = m->m[1];
	c.m[4] = m->m[4];
	c.m[5] = m->m[7];

	c.m[6] = m->m[2];
	c.m[7] = m->m[5];
	c.m[8] = m->m[8];

	return c;

}

Point AddPointVec(Point *p, Vector *v)
{
	Point p1;
	for (int i = 0; i < 3; i++)
		p1.p[i] = p->p[i] + v->p[i];

	return p1;
}


Vector SubPoint(Point *p1, Point *p2)//end, ori
{
	Vector v;

	v.p[0] = p1->p[0] - p2->p[0];
	v.p[1] = p1->p[1] - p2->p[1];
	v.p[2] = p1->p[2] - p2->p[2];
	// ori->end로 가는 백터임
	return v;
}

Vector MultMatVec(Matrix *m, Vector *v)
{
	Vector v1;

	v1.p[0] = m->m[0] * v->p[0] + m->m[3] * v->p[1] + m->m[6] * v->p[2];
	v1.p[1] = m->m[1] * v->p[0] + m->m[4] * v->p[1] + m->m[7] * v->p[2];
	v1.p[2] = m->m[2] * v->p[0] + m->m[5] * v->p[1] + m->m[8] * v->p[2];

	return v1;
}

Point MultMatPoint(Matrix *m, Point *p)
{
	Point p1;

	p1.p[0] = m->m[0] * p->p[0] + m->m[3] * p->p[1] + m->m[6] * p->p[2];
	p1.p[1] = m->m[1] * p->p[0] + m->m[4] * p->p[1] + m->m[7] * p->p[2];
	p1.p[2] = m->m[2] * p->p[0] + m->m[5] * p->p[1] + m->m[8] * p->p[2];
	return p1;
}

struct Object
{
	float colors[3];
	Point position;
	float radius;
};

Object obj[4];//컬러, position, 반지름을 가지고있다.
float f1[] = { -2.f,2.f,-2.f,2.f };
float f2[] = { 2.f,-2.f,-2.f,2.f };
float colors[4][3] = { { 0.6,0.9,0.7 },{ 0.8f,0.1f,0.2f },{ 0.1f,0.8f,0.2f },{ 0.1f,0.2f,0.8f } };
int objCnt = 4;
//각 객체 4개를 관리를 할 거이다.




bool isLeftMouseDown;
float fovy;
int win_width;
int win_height;
float zNear;
float zFar;


void setupLight0()
{
	GLfloat position[4] = { -1.0,1.0,1.0,0.0 };
	GLfloat ambient[4] = { 0.3,0.3,0.3,1 };
	GLfloat diffuse[4] = { 1.0,1.0,1.0,1.0 };
	GLfloat specular[4] = { 1.0,1.0,1.0,1.0 };

	glLightfv(GL_LIGHT0, GL_POSITION, position);
	glLightfv(GL_LIGHT0, GL_AMBIENT, ambient);
	glLightfv(GL_LIGHT0, GL_DIFFUSE, diffuse);
	glLightfv(GL_LIGHT0, GL_SPECULAR, specular);
}

void setMaterial(float r, float g, float b)
{
	float ambient_m[4] = { 0.3,0.3,0.3,1.0 };
	float diffuse_m[4] = { r,g,b,1.0 };
	float specular_m[4] = { 1.0, 1.0, 1.0, 1.0 };
	float shininess_m = 100.0;

	glMaterialfv(GL_FRONT_AND_BACK, GL_AMBIENT, ambient_m);
	glMaterialfv(GL_FRONT_AND_BACK, GL_DIFFUSE, diffuse_m);
	glMaterialfv(GL_FRONT_AND_BACK, GL_SPECULAR, specular_m);
	glMaterialf(GL_FRONT_AND_BACK, GL_SHININESS, shininess_m);


}

void drawObject()
{

	for (int i = 0; i < objCnt; i++)
	{
		//obj 정보를가지고 teapot을 그린다.
		glLoadIdentity();
		glTranslatef(0, 0, -10.f);
		setMaterial(obj[i].colors[0], obj[i].colors[1], obj[i].colors[2]);

		glPushMatrix();
		{
			glTranslatef(obj[i].position.p[0], obj[i].position.p[1], obj[i].position.p[2]);
			glutSolidTeapot(obj[i].radius);
		}
		glPopMatrix;

	}

	glLoadIdentity();
	glTranslatef(0, 0, -10.f);


}

//Compute the picking ray
//뭔가 ... 스크린에서 Viewport로 가져오는 부분
//실제로 투영행렬은 다른 시점을 시뮬레이트하기 위해 투영창의 포인트 배율율을 변경한다.
void calcRay(int _x, int _y, Point * _rayOri, Vector *_rayDir)
{

	//rayOri<p> 와 RayDir<v>이 참조되는 구조이다.
	float aspectRatio = win_width / win_height;
	float fovyRadius = fovy * 3.14159f / 180.f; //fovy =45; //eye 에서 보는게 fovy인가?
	float rayDx = tanf(fovyRadius *0.5f) * aspectRatio;
	float rayDy = tanf(fovyRadius *0.5f);

	float windowHalfWidth = (float)win_width*0.5f;
	float windowHalfHeight = (float)win_height*0.5f;

	float dx = rayDx *((float)_x / windowHalfWidth - 1.0f);
	float dy = rayDy *(1.f - (float)_y / windowHalfHeight);

	//결국에 여기서의 목적은 dx,dy를 구하는 것이였던것같애 fovy를 가지고!


	/*
	fovy = 45;
	zNear = 1.0; //프로젝션 윈도우와 ,, Far Plan을 말하는거 같기도하다.
	zFar = 1000.0;
	//그림에서 보aus COP	에서 보고 점P가 near Plane 인것같음.
	*/


	Point ori;
	SetPoint(&ori, dx*zNear, dy*zNear, -zNear);
	/*
	p->p[0] = a;
	p->p[1] = b;
	p->p[2] = c;
	*/

	Point end;
	SetPoint(&end, dx*zFar, dy*zFar, -zFar);

	Vector dir = SubPoint(&end, &ori); //점과 점을 빼면 백터가 나오지 당연히 무튼 그래서 방향 백터를 얻어
	dir = Normalize(&dir);

	//ori와 end와 dir(vector)를 얻었다 일단.

	//Transforming rays

	double mv[16];//모델 뷰 매트릭스 얻어온다.

	glGetDoublev(GL_MODELVIEW_MATRIX, mv);

	Matrix mat;
	SetMatrix(&mat, mv[0], mv[4], mv[8], mv[1], mv[5], mv[9], mv[2], mv[6], mv[10]);

	Vector transVec;//translate 이건 왜구해놓지???
	SetVector(&transVec, mv[12], mv[13], mv[14]);

	Matrix invMat = MatrixTranspose(&mat);//메트릭스 실제 배열과 다르니까.. transpose 해줘야함
	Vector invTransVec = InverseVector(&transVec);//벡터의 방향이 ori->end로 가는거니까 반대로 방향바꿈

	ori = AddPointVec(&ori, &invTransVec); //point + vector = point임 왜 더하냐?
	ori = MultMatPoint(&invMat, &ori);//Matrix에 point 곱함 Point 나온다.

	end = AddPointVec(&end, &invTransVec);//점에 벡터를 더하는 행위 점을 그방향만큼이동?
	end = MultMatPoint(&invMat, &end);//점에 메트릭스를 더하는 행위 변환!?

	dir = MultMatVec(&invMat, &dir);//dir은 벡터 어쨋든 메트릭스 x 벡터는 벡터

									//end는 뭐하러 구한겨???

									//결국 ori와 dir를 구해서 넘겨주는거다 ... marix를 곱해서 변환을해서 ...translate인것같음
	*(_rayOri) = ori;
	*(_rayDir) = dir;
}


/*
Ray-Object Intersection

구체의 중심C와 반지름 r이 있을 때 아래의 암시적 방정식을 통해서 점 p를 테스트 할 수 있다.

||p-c|| -r <= 0

*/


bool interSection(Point &_rayOri, Vector &_rayDir, Point &_objPos, float _radius, float *t)
{
	//bool isCollision = interSection(rayOri, rayDir, obj.position, obj.radius, &t);
	/*
	Intersection 교차되는가 아닌가를 확인하는 것이다.

	(a) : 광선이 구체를 빗나간다.
	(b) : 광선이 구체의 중심을 교차하나 t0과 t1의 값이 모두 음수
	(c) : 광선이 구체의 내부에 있다. 하나는 음수이고 하나는 양수이나 양수가 단일 교차점을 제공
	(d) : 광선이 구체를 교차한다. t0, t1모두 양수.
	(e) : 광선이 구체와 접촉해 있다. 이때 해는 양수 이며 t0=t1이다.

	이건 오히려 그래픽스 수업을 들어야겠는데??
	*/


	Vector vo = SubPoint(&_rayOri, &_objPos);
	//subPoint ,, 그래서 vo는 무엇인가?


	//DotProduct 하는 이유를 알아야겠고...
	float a = (float)DotProduct(&_rayDir, &_rayDir);
	float b = (float)(2.0f * DotProduct(&_rayDir, &vo));//2.0f는 왜 곱해주는가?
	float c = (float)DotProduct(&vo, &vo) - (float)(_radius *_radius);
	float disc = b*b - 4.0f *a*c;

	if (disc < 0)
		return false;

	float distSqrt = sqrtf(disc);
	float q;

	if (b < 0)
		q = (-b - distSqrt) / 2.0f;
	else
		q = (-b + distSqrt) / 2.0f;

	float t0 = q / a;
	float t1 = c / q;

	if (t0 > t1) {
		float temp = t0;
		t0 = t1;
		t1 = temp;
	}

	if (t1 < 0)
		return false;
	if (t0 < 0) {
		*t = (float)t1;
		return true;
	}
	else {
		*t = (float)t0;
		return true;
	}


}



void pickingObject(int _x, int _y) {


	float t;// 굳이왜 참조형으로 보내서 어따 쓰는거지??
	Point rayOri;
	Vector rayDir;
	calcRay(_x, _y, &rayOri, &rayDir);


	//레이로 ori와 dir(vec)를 가져왔다.

	//여기에서 obj를 여러개 체크하면된다. 그래서 그게 맞으면 바뀌면 됨... 간단함.
	for (int i = 0; i < objCnt; i++)
	{
		bool isCollision = interSection(rayOri, rayDir, obj[i].position, obj[i].radius, &t);
		//intersection 구체가있을때 ... 구체 안에 들어 오는지 판단을한다.

		//충돌 발생한 부분 색을 파란색으로 바꿔준다.
		if (isCollision)
		{
			obj[i].colors[0] = 0.3f;
			obj[i].colors[1] = 0.3f;
			obj[i].colors[2] = 0.3f;

			//break;하면 성능 좋앟질ㄷ스?
			glutPostRedisplay();
		}
	}

}
void mouse(int button, int state, int x, int y)
{
	if (button == GLUT_LEFT_BUTTON)// && state == GLUT_DOWN)
	{
		pickingObject(x, y);
	}

	glutPostRedisplay();
}




void init(void)
{
	//어차피 판단은 해주는거고 ,,, obj에 각각 색을 입히고 각각의 포지션을 가지고 있으면 되는거다 ...
	//근데 그럼 색을 바꿔 줘야하잖아 그때 걔가 걔인지 어케알어???
	//그래서 다시원래색으로 바꾸는건 어렵다.. 그말이구나.

	for (int i = 0; i < objCnt; i++)
	{
		obj[i].colors[0] = colors[i][0];
		obj[i].colors[1] = colors[i][1];
		obj[i].colors[2] = colors[i][2];

		SetPoint(&obj[i].position, f1[i], f2[i], 0.0f);

		obj[i].radius = 1.0;
	}


	isLeftMouseDown = false;

	fovy = 45;
	zNear = 1.0;
	zFar = 1000.0;
}


void timer(int value)
{
	glutPostRedisplay();//화면 갱신이 필요할 때 이함수를 부르면 display 콜백 함수를 다시 한 번 실행
	glutTimerFunc(1000 / 30, timer, 1);//다이머 시간 마다 갱신,  해준다 //30 frame 나온다.
									   //타이머가 작동하는데 다시 타이머를 동작시킴으로써 계쏙 작동하게 되는거지.
}

void RenderScene(void)
{
	//Clear the window with current clearing color
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT); //버퍼의 조합을 지운다.
														/*  DEPTH_BUFFER를 두어서 픽셀별로 깊이 값을 저장하도록 하였다. 이를 이용해서 어떤 물체를 그리고자 할때 그앞에 물체가 있으면 있을때 깊이를
														비교하여 DEPTH가 작은 삼각형쪽은 그려지고, DEPTH가 큰 사각형쪽은 그리지 않게 할 수 있게되는 것이다.
														(동일한 좌표의 픽셀이 있을때 깊이에 따라 그려야 할지 말아야 할지 선택하게 되는 것이다.) */
	setupLight0();
	glMatrixMode(GL_MODELVIEW);//MODELVEIW는 그리는거고 projection은 투영하는거다.
	glLoadIdentity();

	glTranslatef(0, 0, -10.f);
	drawObject();

	glutSwapBuffers(); //GLUT_DOUBLE 버퍼를 더블로해서 스왑해서사용할 것이다.
}

void SetupRC(void) {
	glClearColor(1.0f, 1.0f, 1.0f, 1.0f);
	glEnable(GL_DEPTH_TEST);

	glEnable(GL_LIGHTING);
	glEnable(GL_LIGHT0);
}


void ChangeSize(int w, int h)
{
	if (h == 0)// 0으로 나누어지는거 막기위해서
		h = 1;

	float aspectRatio = (float)w / (float)h;

	win_width = w;
	win_height = h;

	glViewport(0, 0, w, h); // 실제로 보여지는 부분을 뷰포트라 명시한다.

	glMatrixMode(GL_PROJECTION); //이부분이 PROJECTION인 이유는 3D를 2D로 투영시켜야한다.
								 //glOrtho를 통해서
	glLoadIdentity();//좌표계 초기화
					 //근데 이게 보면 아는데 ,이러고 나서 좌표계를 초기화해줘야 하고 이런게 기억안날텐데...
					 //사실근데 안써도 상관없는거구나 ....좌표계를 조작하지 않은이상????
					 //여기에서 초기화 해주는 이유는 뭘까?? 어떤 좌표계를 초기화 시켜준다는 말이지??

	gluPerspective(fovy, aspectRatio, zNear, zFar);
}




void main(int argc, char * argv[])
{
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB);
	glutInitWindowSize(600, 600);
	glutCreateWindow("Simple");

	glutDisplayFunc(RenderScene);//렌더링되는 부분
	glutReshapeFunc(ChangeSize);//SIZE가 바뀔마다 호출이 된다.
								//	glutSpecialFunc(specialKeyboard);//스페셜 키보드 이벤트를 콜백
								//	glutKeyboardFunc(keyboard);//키보드 이벤트 콜백
	glutMouseFunc(mouse);

	init();
	SetupRC();
	glutMainLoop();
	//기본적인 틀같은거 외우필요없고 시험에안나온다 시험볼떄 준다.
}
