#include<iostream>
#include<vector>
using namespace std;


typedef pair<int *, int *> P;
vector<vector<float>> vNormal;
vector<P> v;

int main(void)
{
	int idx = 1;

	
	vNormal.resize(10);

	float a = 0.1f;
	for (int i = 0; i < 10; i++) {
		vNormal[9].push_back(a++);
	}

	for (float f : vNormal[9])
		cout << f << endl;


	return 0;
}