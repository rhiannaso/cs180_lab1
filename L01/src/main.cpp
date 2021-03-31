#include <iostream>
#include <string>
#include <vector>
#include <memory>
#include "Image.h"

// This allows you to skip the `std::` in front of C++ standard library
// functions. You can also say `using std::cout` to be more selective.
// You should never do this in a header file.
using namespace std;

struct Triangle {
   int x1;
   int y1;
   int x2;
   int y2;
   int x3;
   int y3;
};

struct BoundingBox {
   int xmin;
   int xmax;
   int ymin;
   int ymax;
};

void setTriangle(struct Triangle *t, int ax, int ay, int bx, int by, int cx, int cy) {
   t->x1 = ax;
   t->y1 = ay;
   t->x2 = bx;
   t->y2 = by;
   t->x3 = cx;
   t->y3 = cy;
}

void setBoundingBox(struct BoundingBox *bb, int xmin, int xmax, int ymin, int ymax) {
   bb->xmin = xmin;
   bb->xmax = xmax;
   bb->ymin = ymin;
   bb->ymax = ymax;
}

void printT( struct Triangle t ) {
    cout << "X1 : " << t.x1 << endl;
    cout << "Y1 : " << t.y1 << endl;
    cout << "X2 : " << t.x2 << endl;
    cout << "Y2 : " << t.y2 << endl;
    cout << "X3 : " << t.x3 << endl;
    cout << "Y3 : " << t.y3 << endl;
}

int findMin(int c1, int c2, int c3) {
    int minimum = min(c1, min(c2, c3));
    return minimum;
}

int findMax(int c1, int c2, int c3) {
    int maximum = max(c1, max(c2, c3));
    return maximum;
}

void calcBounds( struct Triangle t, struct BoundingBox *bb ) {
    int xmin = findMin(t.x1, t.x2, t.x3);
    cout << "X MIN: " << xmin << endl;
    int xmax = findMax(t.x1, t.x2, t.x3);
    cout << "X MAX: " << xmax << endl;
    int ymin = findMin(t.y1, t.y2, t.y3);
    cout << "Y MIN: " << ymin << endl;
    int ymax = findMax(t.y1, t.y2, t.y3);
    cout << "Y MAX: " << ymax << endl;
    setBoundingBox(bb, xmin, xmax, ymin, ymax);
}

int main(int argc, char **argv)
{
	if(argc < 4) {
		cout << "Usage: Lab1 <out_image_name>.png width height vax vay vbx vby vcx vcy" << endl;
		return 0;
	}
	// Output filename
	string filename(argv[1]);
	// Width of image
	int width = atoi(argv[2]);
	// Height of image
	int height = atoi(argv[3]);
    // Vertex 1 x-coordinate
    int x1 = atoi(argv[4]);
    // Vertex 1 y-coordinate
    int y1 = atoi(argv[5]);
    // Vertex 2 x-coordinate
    int x2 = atoi(argv[6]);
    // Vertex 2 y-coordinate
    int y2 = atoi(argv[7]);
    // Vertex 3 x-coordinate
    int x3 = atoi(argv[8]);
    // Vertex 3 y-coordinate
    int y3 = atoi(argv[9]);

    // Triangle
    struct Triangle t;
    setTriangle(&t, x1, y1, x2, y2, x3, y3);
    //printT(t);
    // Bounding Box
    struct BoundingBox bb;
    calcBounds(t, &bb);

	// Create the image. We're using a `shared_ptr`, a C++11 feature.
	auto image = make_shared<Image>(width, height);

	// Draw bounding box
	for(int y = bb.ymin; y < bb.ymax+1; ++y) {
		for(int x = bb.xmin; x < bb.xmax+1; ++x) {
            unsigned char r;
            unsigned char g;
            if (x % 2 == 0) {
                r = 255;
                g = 0;
            } else {
                r = 0;
                g = 255;
            }
			// unsigned char r = 255;
			// unsigned char g = 0;
			unsigned char b = 0;
			image->setPixel(x, y, r, g, b);
		}
	}

    // Draw vertices
    image->setPixel(x1, y1, 255, 255, 255);
    image->setPixel(x2, y2, 255, 255, 255);
    image->setPixel(x3, y3, 255, 255, 255);

	// Write image to file
	image->writeToFile(filename);
	return 0;
}
