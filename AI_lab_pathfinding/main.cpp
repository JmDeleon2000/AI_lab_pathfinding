#include <iostream>
#include "BMP.h"





int main() 
{
	using namespace std;


	BMP_image* x = new BMP_image("testb.bmp");
	Discrete_image y = BMP_image::discretize(x);
	x = new(x) BMP_image(&y);
	BMP_image::output_BMP(x, "test_discretizationb.bmp");
	delete x;

	////0 00000000 00100010111100000000000
	//int y = 0b00000000000100010111100000000000;
	//float z;
	//z = *(float*)&y;
	//cout << z << endl;
	return 0;
}