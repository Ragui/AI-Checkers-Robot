#ifndef MV_H
#define MV_H

#include "opencv2/highgui/highgui.hpp"
#include "opencv2/imgproc/imgproc.hpp"
#include "opencv2/core/core.hpp"
#include <iostream>
#include <vector>
#include <math.h>
#include <bits/stdc++.h>
#include <utility> 


using namespace cv;
using namespace std;

class BoardMV{
public:
	int rows, cols;
	unsigned **grid;
	
	BoardMV(int x, int y);
	~BoardMV ();
	void print();
	void setSquare(int i, int j, unsigned value);
};

//returns the board vertices as a vector of points
vector<Point> *findBoard(const Mat &inputImage);  
BoardMV findPieces(const Mat &inputImage, int xATymax, int ymax, int xmax, int yATxmax, int ymin, int xmin, int xATymin, int yATxmin);
unsigned squareValue(const Mat &inputImage, int xATymax, int ymax, int xmax, int yATxmax, int ymin, int xmin, int xATymin, int yATxmin);
int area(int x1, int y1, int x2, int y2, int x3, int y3);
double angle( Point pt1, Point pt2, Point pt0 );

//sort by X
bool compa(Point pt1, Point pt2);
bool compa2(Point pt1, Point pt2);



#endif
