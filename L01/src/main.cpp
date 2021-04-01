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
   int r1;
   int g1;
   int b1;
   int r2;
   int g2;
   int b2;
   int r3;
   int g3;
   int b3;
};

struct BoundingBox {
   int xmin;
   int xmax;
   int ymin;
   int ymax;
};

// Triangle
struct Triangle t;
// Bounding Box
struct BoundingBox bb;

void setTriangle(struct Triangle *t, int ax, int ay, int bx, int by, int cx, int cy, int r1, int g1, int b1, int r2, int g2, int b2, int r3, int g3, int b3) {
   t->x1 = ax;
   t->y1 = ay;
   t->x2 = bx;
   t->y2 = by;
   t->x3 = cx;
   t->y3 = cy;
   t->r1 = r1;
   t->g1 = g1;
   t->b1 = b1;
   t->r2 = r2;
   t->g2 = g2;
   t->b2 = b2;
   t->r3 = r3;
   t->g3 = g3;
   t->b3 = b3;
}

void setBoundingBox(struct BoundingBox *bb, int xmin, int xmax, int ymin, int ymax) {
   bb->xmin = xmin;
   bb->xmax = xmax;
   bb->ymin = ymin;
   bb->ymax = ymax;
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
    int xmax = findMax(t.x1, t.x2, t.x3);
    int ymin = findMin(t.y1, t.y2, t.y3);
    int ymax = findMax(t.y1, t.y2, t.y3);
    setBoundingBox(bb, xmin, xmax, ymin, ymax);
}

bool inTriangle(float val) {
    return ((val >= 0) && (val <= 1));
}

void colorPixel(int x, int y, float alpha, float beta, float gamma, shared_ptr<Image> image) {
    if(inTriangle(alpha) && inTriangle(beta) && inTriangle(gamma)) {
        float r = (alpha*t.r1)+(beta*t.r2)+(gamma*t.r3);
        float g = (alpha*t.g1)+(beta*t.g2)+(gamma*t.g3);
        float b = (alpha*t.b1)+(beta*t.b2)+(gamma*t.b3);
        image->setPixel(x, y, r, g, b);
    }
}

void calcBary(int oldX, int oldY, shared_ptr<Image> image, float tArea) {
    int xaxc = t.x1 - t.x3;
    int yayc = t.y1 - t.y3;
    int xbxa = t.x2 - t.x1;
    int ybya = t.y2 - t.y1;
    float bArea = (xaxc*(oldY-t.y3)) - ((oldX-t.x3)*yayc);
    float gArea = (xbxa*(oldY-t.y1)) - ((oldX-t.x1)*ybya);
    float beta = bArea/tArea;
    float gamma = gArea/tArea;
    float alpha = 1 - beta - gamma;
    colorPixel(oldX, oldY, alpha, beta, gamma, image);
}

int main(int argc, char **argv)
{
	if(argc < 4) {
		cout << "Usage: Lab1 <out_image_name>.png width height x1 y1 r1 g1 b1 x2 y2 r2 g2 b2 x3 y3 r3 g3 b3" << endl;
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
    // Vertex 1 r value
    int r1 = atoi(argv[6]);
    // Vertex 1 g value
    int g1 = atoi(argv[7]);
    // Vertex 1 b value
    int b1 = atoi(argv[8]);
    // Vertex 2 x-coordinate
    int x2 = atoi(argv[9]);
    // Vertex 2 y-coordinate
    int y2 = atoi(argv[10]);
    // Vertex 2 r value
    int r2 = atoi(argv[11]);
    // Vertex 2 g value
    int g2 = atoi(argv[12]);
    // Vertex 2 b value
    int b2 = atoi(argv[13]);
    // Vertex 3 x-coordinate
    int x3 = atoi(argv[14]);
    // Vertex 3 y-coordinate
    int y3 = atoi(argv[15]);
    // Vertex 3 r value
    int r3 = atoi(argv[16]);
    // Vertex 3 g value
    int g3 = atoi(argv[17]);
    // Vertex 3 b value
    int b3 = atoi(argv[18]);

    setTriangle(&t, x1, y1, x2, y2, x3, y3, r1, g1, b1, r2, g2, b2, r3, g3, b3);
    calcBounds(t, &bb);

	// Create the image. We're using a `shared_ptr`, a C++11 feature.
	auto image = make_shared<Image>(width, height);

    // Calculate area of triangle
    float tArea = ((t.x2-t.x1)*(t.y3-t.y1)) - ((t.x3-t.x1)*(t.y2-t.y1));

	// Draw bounding box
	for(int y = bb.ymin; y < bb.ymax+1; ++y) {
		for(int x = bb.xmin; x < bb.xmax+1; ++x) {
            calcBary(x, y, image, tArea);
		}
	}

	// Write image to file
	image->writeToFile(filename);
	return 0;
}
