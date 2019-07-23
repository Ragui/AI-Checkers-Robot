#include "opencv2/highgui/highgui.hpp"
#include "opencv2/imgproc/imgproc.hpp"
#include "opencv2/core/core.hpp"
#include <iostream>
#include <vector>
#include <math.h>
#include <bits/stdc++.h>
#include <utility>
#include <wiringPi.h> 
#include <softPwm.h>
#include <stdio.h>
#include <signal.h>
#include <string>

#include "mv.h"
#include "rules.h"
#include "servo.h"

using namespace cv;
using namespace std;

int main(int argc, char** argv) {

	system("sudo ./../PiBits/ServoBlaster/user/servod");
//	signal(SIGINT, signal_callback_handler);
	if(wiringPiSetup() == -1){
		cout << "setup wiringPi failed !" << endl;
		return 1;
	}
	cout << "Program starting" << endl;
	delay(1000);
	Arm arm(2, 5, 6, 1);	//GPIO 18 23 24 17
	
	system("sudo raspistill -o ~/seniorProject/image.jpg");
   	Mat image = imread("image.jpg", 1);

	if(! image.data){
		cout << "Not found !" << endl;
		return -1;
	}

	namedWindow("input", WINDOW_NORMAL);	
	vector<Point> *board = findBoard(image);
	Board bAI;
	
	if(board != 0){
		int xATymax, ymax, xmax, yATxmax;
		int ymin, xmin, xATymin, yATxmin;
		sort(board->begin(), board->end(), compa);
		yATxmin = (*board)[0].y;
		yATxmax = (*board)[3].y;
		xmax = (*board)[3].x;
		xmin = (*board)[0].x;
		
		sort(board->begin(), board->end(), compa2);
		xATymin = (*board)[0].x;
		xATymax = (*board)[3].x;
		ymax = (*board)[3].y;
		ymin = (*board)[0].y;
		//cout << endl << "xmin: " << xmin << ",  ymin: " << ymin << ",  xmax: " << xmax << ",  ymax: " << ymax << endl; 
		//cout << endl << "yATxmin: " << yATxmin << ",  xATymin: " << xATymin << ",  yATxmax: " << yATxmax << ",  xATymax: " << xATymax << endl; 
		if(ymin == yATxmin){
			yATxmax = ymax;
			xATymax = xmax;
			xATymin = xmin;
		}
		if(xmax == xATymax){
			yATxmax = (*board)[1].y;
			
		}
			
	/*	for(int i = 0; i < 4; i++){
			cout << (*board)[i].x << ".." <<(*board)[i].y << endl;
		}
		//cout << endl << "xmin: " << xmin << ",  ymin: " << ymin << ",  xmax: " << xmax << ",  ymax: " << ymax << endl; 
		cout << endl << "yATxmin: " << yATxmin << ",  xATymin: " << xATymin << ",  yATxmax: " << yATxmax << ",  xATymax: " << xATymax << endl;
	*/	BoardMV bMV = findPieces(image, xATymax, ymax, xmax, yATxmax, ymin, xmin, xATymin, yATxmin);
		bMV.print();
		cout << endl << endl;
		bAI.setBoard(bMV);
	}

	imshow("input", image);
	bAI.printBoard();
//	waitKey(0);
	
//	while(!b.gameOver()){
		bAI.turnCount = true;
		if(bAI.turnCount == true){
			 Move m = bAI.bestMove();
			// b.applyMove(m);
			printMove(m);
			arm.move(m);
		}else{
			bAI.printBoard();
			cout << bAI.turnCount << endl;
			int x1, y1, x2, y2;
			cin >> x1 >> y1 >> x2 >> y2;
			Move m;
			m.source = Position(x1, y1, bAI.grid[y1][x1]);
			m.destination = Position(x2, y2, 0);

			int x = 0, y = 0;
			while(x != -1){
				cin >> x >> y;
				if(x != -1){
					m.takenPieces.push_back(Position(x, y, bAI.grid[y][x]));
				}
			}
			cout << bAI.turnCount << endl;
			bAI.applyMove(m);
			cout << bAI.turnCount << endl;
		}
//	}
	cout << "Game Over";	
	

/*
	//initial position
	int angle1 = 90, angle2 = 90, angle3 = 20;
//	arm.move(90, 90, 20, angle1, angle2, angle3, "231");
	arm.releasePiece();
	delay(10000);
	
	arm.move(111, 51, 51, angle1, angle2, angle3, "132");
	angle1 = 111; angle2 = 51; angle3 = 51;
	arm.grabPiece();
	arm.move(90, 90, 20, angle1, angle2, angle3, "231");
	angle1 = 90, angle2 = 90, angle3 = 20;
*/
	
	return 0;
}

//EOF
