#include <iostream>
#include <fstream>

#define pixel_size  3 // as in 3 bytes
struct BGR_pixel
{
	float b;
	float g;
	float r;
};

class BMP_image
{
public:
	int width = 0;
	int height = 0;
	BGR_pixel** image = nullptr;

public:
	BMP_image()  {}
	BMP_image(const char* filename)
	{
		using namespace std;

		int headerSize;
		ifstream text;
		text.open(filename, ios::binary);

		text.seekg(10);
		text.read((char*)&headerSize, 4);
		text.seekg(18);
		text.read((char*)&width, 4);
		text.read((char*)&height, 4);
		text.seekg(headerSize);
		image = new BGR_pixel * [height];

		int i = 0, j;
		while (i < height)
		{
			j = 0;
			image[i] = new BGR_pixel[width];
			while (j < width)
			{
				text.read((char*)&image[i][j], 3);
				j++;
			}
			i++;
		}
		text.close();
	}


	static void output_BMP(BMP_image* img, const char* filename) 
	{
		unsigned char header[14] = { 'B', 'M', 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 };
		unsigned char headerinfo[40] = { 40, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 0, 24, 0 };
		const int filesize = img->width * img->height * pixel_size;


		*(int*)(header + 2) = filesize;

		*(int*)(headerinfo + 4) = img->width;

		*(int*)(headerinfo + 8) = img->height;

		FILE* dump;
		if (fopen_s(&dump, filename, "wb")) 
		{
			std::cerr << "error outputing bmp" << std::endl;
			return;
		}

		fwrite(header, 1, 14, dump);
		fwrite(headerinfo, 1, 40, dump);

		int i, j = 0;
		while (j < img->height)
		{
			i = 0;
			while (i < img->width)
			{
				fwrite(&img->image[j][i], 1, 3, dump);
				i++;
			}
			j++;
		}

		fclose(dump);
		for (int i = 0; i < img->width; i++)
			delete[] img->image[i];
		delete[] img->image;
	}
};



int main() 
{
	using namespace std;
	cout << "Works!" << endl;

	BMP_image* x = new BMP_image("test.bmp");
	BMP_image::output_BMP(x, "test1.bmp");
	delete x;
	return 0;
}