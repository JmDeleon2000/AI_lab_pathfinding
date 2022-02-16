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
#define DISCRETE_BMP_GOAL	0x40
// The value is black, representing a wall. 87 for W in ASCII.
#define DISCRETE_BMP_WALL	0x10
// The value is white, representing a walkable path. 80 for P in ASCII.
#define DISCRETE_BMP_PATH	0b0000
// The value is red, representing the start of the path. 83 for S in ASCII.
#define DISCRETE_BMP_START	0x20
// The value is purple, representing a step of the suggested path. 65 for A in ASCII.
#define DISCRETE_BMP_AWNSER	0x30

// Masks used to mark and test nodes as visited or explored
#define VISITED_RIGHT	0b0001
#define VISITED_LEFT	0b0010
#define VISITED_DOWN	0b0100
#define VISITED_UP		0b1000

#define DISCRETE_BOUND 64


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


