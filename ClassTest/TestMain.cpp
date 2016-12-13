#include<iostream>

using namespace std;


class Point {

public:
	float x, y, z;

	Point() { this->x = 0; this->y = 0; this->z = 0; }
	Point(float _x, float _y, float _z):x(_x),y(_y),z(_z){}
	
	friend Point operator+ (Point a, Point b)//���������� ����� �Ҽ����ְ� �ȴ� ��� �Լ��� ����� �����ʴ´�.
	{
		Point p(a.x + b.x, a.y + b.y, a.z + b.z);
		return p;
	}
	Point& operator+= (Point a)
	{
		Point ret(this->x += a.x,
			this->y += a.y,
			this->z += a.z);
		return ret;
	}
	void showPoint()
	{
		cout<<"( " << this->x << ", " << this->y << ", " << this->z <<" ) "<< endl;

	}
	friend void show(Point p);
};


void show(Point p)
{
	cout << "( " << p.x << ", " << p.y << ", " << p.z << " ) " << endl;
}

int main(void)
{

	Point p(1, 2, 3);

	Point d=p;

	Point p2 = d + p;
	p2.showPoint();

	Point p3(1,1,1);

	p2 += p3;

	p2.showPoint();
	show(p2);//��������ϳ� �������ϳ� ��������/

	return 0;
}