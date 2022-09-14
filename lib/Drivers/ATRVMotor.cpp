#include "ATRVMotor.h"
#include <Arduino.h>
//#include "BTS7960.h"

using namespace lattice;

//ATRVMotor::ATRVMotor(const int LeftPin, const int RightPin):LeftM(LeftPin), RightM(RightPin) { }
//EN?
ATRVMotor::ATRVMotor(uint8_t L_EN, uint8_t R_EN, uint8_t L_PWM, uint8_t R_PWM){
	_R_PWM = R_PWM;
	_L_PWM = L_PWM;
	_L_EN = L_EN;
	_R_EN = R_EN;
	pinMode(_R_PWM, OUTPUT);
	pinMode(_L_PWM, OUTPUT);
	pinMode(_L_EN, OUTPUT);
	pinMode(_R_EN, OUTPUT);
}

void ATRVMotor::SetPWM(uint8_t LeftPWM, uint8_t RightPWM){
    analogWrite(_L_PWM, LeftPWM);
    analogWrite(_R_PWM, RightPWM);
}

// void ATRVMotor::TurnRight(uint8_t pwm){
//      analogWrite(_L_PWM, 0);
// 	 delayMicroseconds(100);
//      analogWrite(_R_PWM, pwm);
// }

// void ATRVMotor::TurnLeft(uint8_t pwm){
//      analogWrite(_R_PWM, 0);
// 	 delayMicroseconds(100);
//      analogWrite(_L_PWM, pwm);
// }

void ATRVMotor::Enable(){
	digitalWrite(_L_EN,1);
	if(_R_EN != 0) digitalWrite(_R_EN, HIGH);  
}

void ATRVMotor::Disable(){
	digitalWrite(_L_EN,0);
	if(_R_EN != 0) digitalWrite(_R_EN, LOW);
}

void ATRVMotor::Stop(){
  analogWrite(_L_PWM, LOW);
  analogWrite(_R_PWM, LOW);
}
