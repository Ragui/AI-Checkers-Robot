#include "mv.h"

using namespace cv;
using namespace std;

void BoardMV::setSquare(int i, int j, unsigned value){
		grid[i][j] = value;
}

void BoardMV::print(){
		for(int i = 0; i < rows; i++){
			for(int j = 0; j < cols; j++){
				cout << grid[i][j] << "   ";
			}
			cout << endl;
		}
	}

BoardMV::~BoardMV (){
		for(int c = 0; c < rows; c++){
			delete grid[c];
		}
		delete grid;
	}
	
BoardMV::BoardMV(int x, int y) : rows(x) , cols(y){
			grid = new unsigned*[x];
			for(int c = 0; c < x; c++){
				grid[c] = new unsigned[y];
			}
			
			for(int i = 0; i < x; i++){
				for(int j = 0; j < y; j++){
					grid[i][j] = 0;
				}
			} 
	}
	
double angle( Point pt1, Point pt2, Point pt0 )
{
    double dx1 = pt1.x - pt0.x;
    double dy1 = pt1.y - pt0.y;
    double dx2 = pt2.x - pt0.x;
    double dy2 = pt2.y - pt0.y;
    return (dx1*dx2 + dy1*dy2)/sqrt((dx1*dx1 + dy1*dy1)*(dx2*dx2 + dy2*dy2) + 1e-10);
}

//sort by X
bool compa(Point pt1, Point pt2){
	return pt1.x < pt2.x; 
}

bool compa2(Point pt1, Point pt2){
	return pt1.y < pt2.y; 
}

int area(int x1, int y1, int x2, int y2, int x3, int y3){
	return abs(( x1*(y2-y3) + x2*(y3 - y1)  + x3*(y1-y2)) / 2);
}

unsigned squareValue(const Mat &inputImage, int xATymax, int ymax, int xmax, int yATxmax, int ymin, int xmin, int xATymin, int yATxmin){
	// the [0] means blue values and 100, 100 is the position in the array
	// cout << image.at<Vec3b>(100, 100)[0] << endl;
	
	int countBlack = 0, countRed = 0;
	int squareArea = area(xmin, yATxmin, xATymin, ymin, xmax, yATxmax) + area( xmin, yATxmin, xATymax, ymax , xmax, yATxmax);
		
	for(int i = xmin; i < xmax; i++){
			bool rowDone = false;
			for(int j = ymin; j < ymax; j++){
				int trianglesArea = area(i, j, xATymin, ymin, xmin, yATxmin) + area(i, j, xATymax, ymax, xmin, yATxmin) + 
										area(i, j, xATymin, ymin, xmax, yATxmax) + area(i, j, xmax, yATxmax, xATymax, ymax);
				if(trianglesArea <= squareArea){
					rowDone = true;
					unsigned b = inputImage.at<Vec3b>(j, i)[0];
					unsigned g = inputImage.at<Vec3b>(j, i)[1];
					unsigned r = inputImage.at<Vec3b>(j, i)[2];
					if( b < 50 && g < 50 && r < 50){	//black
						countBlack++; 
					}
					if(r > 50 && r <130 && g < 40 && b < 50){	//red
						countRed++;
					}
				}else if(rowDone){
					break;
				}
			}
	}
	int percentageB = (countBlack * 100) / squareArea;
	int percentageR = (countRed   * 100) / squareArea;
	if(percentageR > 5)
		return 3;
	if(percentageB > 30)
		return 1;
	return 0;
}

BoardMV findPieces(const Mat &inputImage, int xATymax, int ymax, int xmax, int yATxmax, int ymin, int xmin, int xATymin, int yATxmin){
	//Mat outTemp(inputImage);
	BoardMV b(8,8);
	
	int w2 = ((yATxmin - ymin)+(ymax - yATxmax))/2;
	int h2 = ((xATymin - xmin)+(xmax - xATymax))/2;
	int w1 = ymax - yATxmin;
	int h1 = xATymax - xmin;
	bool rotate = false;
	
	if( (ymax - yATxmin) < (yATxmin - ymin) ){
		rotate = true;
	}
	
		
	for(int i = xmin, j = yATxmin, c1 = 0; i < (xmin+(h1/8)*8); i += h1/8, j += w1/8, c1++){
			for(int i2 = i, j2 = j, c2 = 0; i2 < (i+(h2/8)*8); i2 += h2/8, j2 -= w2/8, c2++){
				//line(outTemp, Point(i2, j2), Point(i2+h2/8, j2-w2/8), Scalar(0,0,0), 8, LINE_AA);
				//line(outTemp, Point(i2, j2), Point(i2+h1/8, j2+w1/8), Scalar(0,0,0), 8, LINE_AA);
				//line(outTemp, Point(i2+h2/8, j2-w2/8), Point(i2+h2/8+h1/8, j2-w2/8+w1/8), Scalar(0,0,0), 8, LINE_AA);
				//line(outTemp,  Point(i2+h1/8, j2+w1/8), Point(i2+h2/8+h1/8, j2-w2/8+w1/8), Scalar(0,0,0), 8, LINE_AA); 
				b.setSquare( (!rotate) ? c1 : 7 - c2, (!rotate) ? c2 : c1, squareValue(inputImage, i2+h1/8, j2+w1/8, i2+h2/8+h1/8, j2-w2/8+w1/8, j2-w2/8, i2, i2+h2/8, j2) );
			}
	}
	return b;
}

vector<Point> *findBoard(const Mat &inputImage){
	unordered_map<int, pair<vector<Point>, int>> map;
	int maxCount = INT_MIN;
	int areaS = 0;
	
	Mat outTemp = inputImage.clone();
	Mat blured(inputImage);
	//medianBlur(inputImage, blured, 9);
	namedWindow("Out", WINDOW_NORMAL);
	
	//one Channel 8-bit unsigned  
	Mat gray(blured.size(), CV_8U);
	Mat grayTemp;
	
	for(int c = 0; c < 1; c++){  //check which channel will work better with our model 
		int ch[] = {c, 0};	
		mixChannels(&blured, 1, &gray, 1, ch, 1);
	
		int thr = 11;
		for(int t = 0; t  < thr; t++){		//check best one to avoid unnecessary calculations
			grayTemp= gray >= (t+1)*255/thr;
			//imshow("Out", grayTemp);
			vector<vector<Point> > contours;
			vector<Point> approx;
			findContours(grayTemp, contours, RETR_LIST, CHAIN_APPROX_SIMPLE);
			
			for( size_t i = 0; i < contours.size(); i++ ){
					// approximate contour with accuracy proportional to the contour perimeter
					approxPolyDP(Mat(contours[i]), approx, arcLength(Mat(contours[i]), true)*0.02, true);
                
					
					int contourA = fabs(contourArea(Mat(approx)));
					if( approx.size() == 4 && 
						contourA > 40000  && //change area to get desired squares
						 isContourConvex(Mat(approx)) )
					{
						double maxCosine = 0;

						for( int j = 2; j < 5; j++ ){
							// find the maximum cosine of the angle between joint edges
							double cosine = fabs(angle(approx[j%4], approx[j-2], approx[j-1]));
							maxCosine = MAX(maxCosine, cosine);
						}

						// if cosines of all angles are small it means the angle is almost equal 90
						if( maxCosine < 0.15 ){
							bool flag = true;
							for(auto m : map){
								if(m.first <= contourA + 2000 &&  m.first >= contourA - 2000){
									int newCount = m.second.second+1;
									int newKey = (m.first+contourA)/2;
									if(m.second.second > maxCount){
										maxCount = newCount;
										areaS = newKey;
									}
									map.erase(m.first);
									pair< vector<Point>, int> p1 (vector<Point>(approx.begin(), approx.end()), newCount);
									map[newKey] = p1;
									flag = false;
									break;
								}
							}
							if(flag){
								pair< vector<Point>, int> p1 (vector<Point>(approx.begin(), approx.end()), 1);
								map[contourA] = p1;
							}
						}
							
					}
			}
			}
		}
	
	
	for(auto m : map){
		if(m.first >= areaS*50 &&  m.first <= areaS*74){
			const Point* p = &(m.second.first)[0];
			int n = (int)(m.second.first).size();
			polylines(outTemp, &p, &n, 1, true, Scalar(0,0,255), 5, LINE_AA);
			imshow("Out", outTemp);
			cout << m.first << endl;
			vector<Point> *board = new vector<Point>(m.second.first.begin(), m.second.first.end());
			return board;
		}
	}
	cout << endl << areaS << " " << maxCount << endl;
	return 0;
}
