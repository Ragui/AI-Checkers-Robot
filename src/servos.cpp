#include <iostream>
#include <wiringPi.h> 
#include <softPwm.h>
#include <stdio.h>
#include <signal.h>
#include <string>

using namespace std;

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

public:
	Arm(int p1, int p2, int p3, int p4);
	void grabPiece();
	void releasePiece();
	void move(int new1, int new2, int new3, int old1, int old2, int old3, string order);
};

void signal_callback_handler(int signum);

int main(){
	system("sudo ./PiBits/ServoBlaster/user/servod");
	signal(SIGINT, signal_callback_handler);
	if(wiringPiSetup() == -1){
		cout << "setup wiringPi failed !" << endl;
		return 1;
	}
	cout << "Program starting" << endl;
	
	Arm arm(2, 5, 6, 1);	//GPIO 18 23 24 17
	delay(10000);
	
	//initial position
	int angle1 = 90, angle2 = 90, angle3 = 20;
	arm.move(90, 90, 20, angle1, angle2, angle3, "123");
	arm.releasePiece();
	
	arm.move(120, 180 - 60, 120, angle1, angle2, angle3, "321");
	angle1 = 120;
	angle2 = 180 - 60;
	angle3 = 120;
	system("sudo killall servod");  
	return 0;
}

void Hand::grabPiece(int timeMS){
	s->rotate(50, 50);
	delay(3000);
	delay(timeMS);
}

void Hand::releasePiece(){
	s->rotate(25, 25);
	delay(3000);
}

void signal_callback_handler(int signum){
	   printf("Caught signal %d\n",signum);
	   system("sudo killall servod");
	   exit(signum);
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
		for(int i = init; i <= angle; i++){
			rotateAngle(i);
			delay(100);
		}
	}else{
		for(int i = init; i >= angle; i--){
			rotateAngle(i);
			delay(100);
		}
	}
}

Arm::Arm(int p1, int p2, int p3, int p4){
		s1 = new Servo(p1);
		s2 = new Servo(p2);
		s3 = new Servo(p3);
		h  = new Hand(p4);
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
				s2->rotate(new2, old2);
				break;
			case 3:
				s3->rotate(new3, old3);
				break;
		}
		delay(3000);
	}
}




