#include <iostream>
#include "BMP.h"
#include "problem.h"


int main() 
{
	using namespace std;

	BMP_image* x = new BMP_image("test.bmp");
	Discrete_image y = BMP_image::discretize(x);
	BFS_Agent* agente = new BFS_Agent(y);
	x = new(x) BMP_image(&agente->ambient);
	//x = new(x) BMP_image(&y);
	BMP_image::output_BMP(x, "test_discretizationb.bmp");
	delete x;

	
	return 0;
}

