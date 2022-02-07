#include "BMP.h"
#include <fstream>
#include <iostream>


// implements task 1.1

// represents a pixel as stored by the .bmp format
struct BGR_pixel
{
	unsigned char b;
	unsigned char g;
	unsigned char r;
};


// implementation for the BMP_image interface
// default contructor
BMP_image::BMP_image() {}
// opens a .bmp file and loads it to memory
BMP_image::BMP_image(const char* filename)
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
	image = new BGR_pixel* [height];

	int i, j = 0;
	while (j < height)
	{
		i = 0;
		image[j] = new BGR_pixel[width];
		while (i < width)
		{
			text.read((char*)&image[j][i], 3);
			i++;
		}
		j++;
	}
	text.close();
}

// outputs data from a BMP_image object to a .bmp file
void BMP_image::output_BMP(BMP_image* img, const char* filename)
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
			fwrite(&img->image[j][i], 1, pixel_size, dump);
			i++;
		}
		j++;
	}

	fclose(dump);
	for (int i = 0; i < img->width; i++)
		delete[] img->image[i];
	delete[] img->image;
}

// recreates a BMP image based on data from a previously discretized image. (data will be lost by design)
BMP_image::BMP_image(Discrete_image* discrete_image) 
{
	width = discrete_image->width * discrete_image->discrete_pixel_size;
	height = discrete_image->height * discrete_image->discrete_pixel_size;

	image = new BGR_pixel* [height];
	char discrete_pixel;
	int i, j = 0;
	while (j < height)
	{
		i = 0;
		image[j] = new BGR_pixel[width];
		while (i < width)
		{
			discrete_pixel = discrete_image->data[
				j / discrete_image->discrete_pixel_size][
					i / discrete_image->discrete_pixel_size];

			switch (discrete_pixel)
			{
			case DISCRETE_BMP_GOAL:
				image[j][i].b = 0;
				image[j][i].g = 255;
				image[j][i].r = 0;
				break;
			case DISCRETE_BMP_START:
				image[j][i].b = 0;
				image[j][i].g = 0;
				image[j][i].r = 255;
				break;
			case DISCRETE_BMP_WALL:
				image[j][i].b = 0;
				image[j][i].g = 0;
				image[j][i].r = 0;
				break;
			case DISCRETE_BMP_PATH:
				image[j][i].b = 255;
				image[j][i].g = 255;
				image[j][i].r = 255;
				break;
			default:
				break;
			}
			i++;
		}
		j++;
	}
}

// discretize the image for faster agent processing. Certain loss of data.
Discrete_image BMP_image::discretize(BMP_image* image_data)
{
	Discrete_image output;
	output.discrete_pixel_size = image_data->width / 32;
	output.width = image_data->width / output.discrete_pixel_size;
	output.height = image_data->height / output.discrete_pixel_size;
	int pixel_sampler_x;
	int pixel_sampler_y = output.discrete_pixel_size / 2;


	BGR_pixel sampled_pixel;
	output.data = new char* [output.height];

	int i, j = 0;
	while (j < output.height)
	{
		i = 0;
		output.data[j] = new char [output.width];

		while (i < output.width)
		{
			sampled_pixel = image_data->image[
				j * output.discrete_pixel_size][
					i * output.discrete_pixel_size];

				if (sampled_pixel.b < 150 &&
					sampled_pixel.g < 150 &&
					sampled_pixel.r < 150) // test for a black pixel
					output.data[j][i] = DISCRETE_BMP_WALL;

				if (sampled_pixel.b >= 150 &&
					sampled_pixel.g >= 150 &&
					sampled_pixel.r >= 150) // test for a white pixel
					output.data[j][i] = DISCRETE_BMP_PATH;

				if (sampled_pixel.b < 150 &&
					sampled_pixel.g >= 150 &&
					sampled_pixel.r < 150) // test for a green pixel
					output.data[j][i] = DISCRETE_BMP_GOAL;

				if (sampled_pixel.b < 150 &&
					sampled_pixel.g < 150 &&
					sampled_pixel.r >= 150) // test for a red pixel
					output.data[j][i] = DISCRETE_BMP_START;
			i++;
		}
		j++;
	}

	return output;
}