#pragma once

//defines the interface for managing BMP images (task 1.1)

// defines the pixel size as 3 bytes per pixel
#define pixel_size  3 

//struct used to store pixels
struct BGR_pixel;

// class that holds information for a dicretized .bmp image
struct Discrete_image 
{
	int width = 0;
	int height = 0;
	int discrete_pixel_size = 0;
	char** data = nullptr;
};

// MACROS that define the accepted values a discrete bmp data value may take
// The value is green, representing a goal. 71 for G in ASCII.
#define DISCRETE_BMP_GOAL	71
// The value is black, representing a wall. 87 for W in ASCII.
#define DISCRETE_BMP_WALL	87
// The value is white, representing a walkable path. 80 for P in ASCII.
#define DISCRETE_BMP_PATH	80
// The value is red, representing the start of the path. 83 for S in ASCII.
#define DISCRETE_BMP_START	83



//class that opens and holds bmp image information
class BMP_image 
{
public:
	int width = 0;
	int height = 0;
	BGR_pixel** image = nullptr;

public:
	BMP_image();
	BMP_image(const char* filename);
	BMP_image(Discrete_image* discrete_image);
	static void output_BMP(BMP_image* img, const char* filename);
	static Discrete_image discretize(BMP_image* img);
};


