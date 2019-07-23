#ifndef SERVO_H
#define SERVO_H

#include "rules.h"

class angles{
public:
	int angle1, angle2, angle3;
	angles();	
	angles(int a1, int a2, int a3);	
	void setAngles(int a1, int a2, int a3);	
};

class Servo{
private:
	int pin;
	void rotateAngle(int angle);
				
public:
	Servo(int p):pin(p){};
	void rotate(int angle, int init);
						
};

class Hand{
private:
	Servo *s;
	int pin;
				
public:
	Hand(int p):pin(p){
		s = new Servo(p);
	}
	void grabPiece(int timeMS);
	void releasePiece();
};

class Arm{
private:
	Servo *s1, *s2, *s3;
	Hand *h;
	angles memo[8][8]; 

public:
	Arm(int p1, int p2, int p3, int p4);
	void grabPiece();
	void releasePiece();
	void move(int new1, int new2, int new3, int old1, int old2, int old3, string order);
	void move(angles dest, angles source, string order);
	void move(Move m);		
};

#endif
