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

	
	return 0;
}

