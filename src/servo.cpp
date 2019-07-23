#include <iostream>
#include <wiringPi.h> 
#include <softPwm.h>
#include <stdio.h>
#include <signal.h>
#include <string>

#include "servo.h"

void angles::setAngles(int a1, int a2, int a3){
	angle1 = a1; 
	angle2 = a2;
       	angle3 = a3;
}

angles::angles():angle1(-1), angle2(-1), angle3(-1){};

angles::angles(int a1, int a2, int a3):angle1(a1), angle2(a2), angle3(a3){};	

void Hand::grabPiece(int timeMS){
	s->rotate(115, 75);
	delay(3000);
	delay(timeMS);
}

void Hand::releasePiece(){
	s->rotate(75, 75);
	delay(3000);
}

void Servo::rotateAngle(int angle){
	// 0 -> 70, 180 -> 240
	string command = "sudo echo ";
	command += to_string(pin);
	command += "=";
	int angleToMs = ( 70.0 + (170* ((double)angle/180.0) ) );
	command += to_string(angleToMs);
	command += " > /dev/servoblaster";
	
	char comm[command.length() + 1];
	for(unsigned i = 0; i < command.length()+1; i++){
		comm[i] = command[i];
	}
	system(comm);
}

void Servo::rotate(int angle, int init){
	if(init <= angle){
		for(int i = init; i <= angle; i = i+2){
			rotateAngle(i);
			delay(100);
		}
	}else{
		for(int i = init; i >= angle; i = i-2){
			rotateAngle(i);
			delay(100);
		}
	}
	rotateAngle(angle);
	delay(100);
}

Arm::Arm(int p1, int p2, int p3, int p4){
	s1 = new Servo(p1);
	s2 = new Servo(p2);
	s3 = new Servo(p3);
	h  = new Hand(p4);

	memo[0][1].setAngles(106, 16, 135);
	memo[0][3].setAngles(94, 18, 130);
	memo[0][5].setAngles(79, 14, 139);
	memo[0][7].setAngles(66, 10, 150);

	memo[1][0].setAngles(114, 23, 120);
	memo[1][2].setAngles(100, 25, 116);
	memo[1][4].setAngles(80, 37, 112);
	memo[1][6].setAngles(70, 28, 114);
	
	memo[2][1].setAngles(109, 35, 96);
	memo[2][3].setAngles(94, 37, 90);
	memo[2][5].setAngles(75, 34, 95);
	memo[2][7].setAngles(62, 31, 104);
	
	memo[3][0].setAngles(120, 37, 89);
	memo[3][2].setAngles(104, 41, 79);
	memo[3][4].setAngles(84, 46, 77);
	memo[3][6].setAngles(65, 41, 83);
	
	memo[4][1].setAngles(116, 46, 69);
	memo[4][3].setAngles(94, 47, 64);
	memo[4][5].setAngles(70, 43, 67);
	memo[4][7].setAngles(55, 40, 80);
	
	memo[5][0].setAngles(129, 46, 65);
	memo[5][2].setAngles(111, 51, 51);
	memo[5][4].setAngles(85,50, 53);
	memo[5][6].setAngles(57,50, 58);
	
	memo[6][1].setAngles(125, 50, 50);
	memo[6][3].setAngles(97, 53, 39);
	memo[6][5].setAngles(65, 53, 43);
	memo[6][7].setAngles(43, 50, 57);
	
	memo[7][0].setAngles(145, 52, 47);
	memo[7][2].setAngles(118, 53, 31);
	memo[7][4].setAngles(70, 52, 30);
	memo[7][6].setAngles(42, 53, 43);
}

void Arm::grabPiece(){
	h->grabPiece(1000);
}

void Arm::releasePiece(){
	h->releasePiece();
}
	
void Arm::move(int new1, int new2, int new3, int old1, int old2, int old3, string order){
	for(int i = 0; i < 3; i++){
		switch(order[i] - '0'){
			case 1: 
				s1->rotate(new1, old1);
				break;
			case 2:
				new2 = 25 + (140.0 * ((double)new2/180.0) );
				old2 = 25 + (140.0 * ((double)old2/180.0) );
				cout << new2 << endl;
				s2->rotate(180 - new2, 180 - old2);
				break;
			case 3:
				s3->rotate(new3, old3);
				break;
		}
		delay(3000);
	}
}

void Arm::move(angles dest, angles source, string order){
	move(dest.angle1, dest.angle2, dest.angle3, source.angle1, source.angle2, source.angle3, order);
}

void Arm::move(Move m){
	releasePiece();
	delay(3000);
	move(memo[7-m.source.y][7-m.source.x], angles(180, 90, 20), "132");
	grabPiece();
	move(angles(90, 90, 20), memo[7-m.source.y][7-m.source.x], "231");
	move(memo[7-m.destination.y][7-m.destination.x], angles(90, 90, 20), "132");
	releasePiece();
	move(angles(180,90, 20), memo[7-m.destination.y][7-m.destination.x], "231");
	 
	// add taken pieces
	return;
}

//EOF
