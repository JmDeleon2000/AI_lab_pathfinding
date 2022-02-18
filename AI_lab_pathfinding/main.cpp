#include <iostream>
#include "BMP.h"
#include "problem.h"


int main() 
{
	using namespace std;
	
	BMP_image* x = new BMP_image("test.bmp");
	Discrete_image y = BMP_image::discretize(x);
	problem_definition* agente = new A_Star_v1_Agent(y);
	x = new(x) BMP_image(&agente->ambient);
	//x = new(x) BMP_image(&y);
	BMP_image::output_BMP(x, "output.bmp");
	delete x;
	
	
	return 0;
}

