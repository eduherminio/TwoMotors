/*  Library for controlling 2DC motors
*/

#include "Arduino.h"
#include "TwoMotors.h"
#include "TwoMotors_heading.h"
#include <string.h>
#include <math.h> // abs(n), copysign(n_magnitude, n_sign), signbit(n)

TwoMotors::TwoMotors()
	: TwoMotors(
		  def_p_PWM_L, def_p_PWM_R,
		  def_p_LE1, def_p_LE2,
		  def_p_RE1, def_p_RE2)
{
	ChosenSerial->println("Using library default pin configuration and parameters");
}

TwoMotors::TwoMotors(
	int p_PWM_L, int p_PWM_R,
	int p_LE1, int p_LE2,
	int p_RE1, int p_RE2)
	: p_PWM_L(p_PWM_L), p_PWM_R(p_PWM_R),
	  p_LE1(p_LE1), p_LE2(p_LE2),
	  p_RE1(p_RE1), p_RE2(p_RE2)
{
	ChosenSerial->begin(9600);
	pinMode(p_LE1, OUTPUT);
	pinMode(p_LE2, OUTPUT);
	pinMode(p_RE1, OUTPUT);
	pinMode(p_RE2, OUTPUT);

	//pinMode(p_PWM_R, OUTPUT);		// Pines analógicos usados como PWM
	//pinMode(p_PWM_L, OUTPUT);
}

void TwoMotors::change_serial(HardwareSerial &hardwareSerial)
{
	ChosenSerial = &hardwareSerial;
}

void TwoMotors::emergency_stop()
{
	analogWrite(p_PWM_L, ZERO);
	analogWrite(p_PWM_R, ZERO);

	digitalWrite(p_LE1, LOW); // Both 00 or 11 activates breaks
	digitalWrite(p_LE2, LOW);
	digitalWrite(p_RE1, LOW);
	digitalWrite(p_RE1, LOW);
}

void TwoMotors::set_speed()
{
	correct_speed(); // speed_result= +- coef x [min, max]

	adapt_enables();

	analogWrite(abs(p_PWM_L), speed[L]); // Keeping the sign
	analogWrite(abs(p_PWM_R), speed[R]);
	// Keeping the sign helps to deduct motors movement faster
	// (without having to enter in enable logic)
}

void TwoMotors::correct_speed()
{
	if (abs(speed[L]) < MIN_SPEED)
	{
		speed[L] = copysign(MIN_SPEED, speed[L]);
	}
	else if (abs(speed[L]) > MAX_SPEED)
	{
		speed[L] = copysign(MAX_SPEED, speed[L]);
	}
	if (abs(speed[R]) < MIN_SPEED)
	{
		speed[R] = copysign(MIN_SPEED, speed[R]);
	}
	else if (abs(speed[R]) > MAX_SPEED)
	{
		speed[R] = copysign(MAX_SPEED, speed[R]);
	}

	speed[L] *= coef_L;
	speed[R] *= coef_R;

	if (abs(speed[L]) < MIN_SPEED)
	{
		speed[L] = copysign(MIN_SPEED, speed[L]);
	}
	else if (abs(speed[L]) > MAX_SPEED)
	{
		speed[L] = copysign(MAX_SPEED, speed[L]);
	}
	if (abs(speed[R]) < MIN_SPEED)
	{
		speed[R] = copysign(MIN_SPEED, speed[R]);
	}
	else if (abs(speed[R]) > MAX_SPEED)
	{
		speed[R] = copysign(MAX_SPEED, speed[R]);
	}
}

void TwoMotors::adapt_enables()
{
	if (signbit(speed[L]))	//speed[L]<0 -> 01
	{
		digitalWrite(p_LE1, LOW);
		digitalWrite(p_LE2, HIGH);
	}
	else	// speed[L]>0 -> 10 
	{
		digitalWrite(p_LE1, HIGH);
		digitalWrite(p_LE2, LOW);
	}

	if (signbit(speed[R]))	//	speed[R]<0 -> 01
	{
		digitalWrite(p_RE1, LOW);
		digitalWrite(p_RE2, HIGH);
	}
	else	// speed[R]>0 -> 10
	{
		digitalWrite(p_RE1, HIGH);
		digitalWrite(p_RE2, LOW);
	}
}

void TwoMotors::choose_speed(int speedL, int speedR)
{
	speed[L] = speedL;
	speed[R] = speedR;
	set_speed();
}

bool TwoMotors::lower_speed()
{
	speed[L] = copysign(LOW_COEF * speed[L], speed[L]);
	speed[R] = copysign(LOW_COEF * speed[R], speed[R]);

	set_speed();

	// if (last_called != 1)
	// 	ChosenSerial->println("motors.lower_speed()");
	// last_called = 1;

	return false;
}

bool TwoMotors::forward()
{
	speed[L] = FWD_SPEED;
	speed[R] = FWD_SPEED;

	set_speed();

	// if (last_called != 2)
	// 	ChosenSerial->println("motors.forward()");
	// last_called = 2;

	return false;
}

bool TwoMotors::backward()
{
	speed[L] = -BWD_SPEED;
	speed[R] = -BWD_SPEED;

	set_speed();

	// if (last_called != 3)
	// 	ChosenSerial->println("motors.backward()");
	// last_called = 3;

	return false;
}

bool TwoMotors::stop()
{
	speed[L] = MIN_SPEED;
	speed[R] = MIN_SPEED;

	set_speed();

	// ChosenSerial->println("motors.stop()");

	return false;
}

bool TwoMotors::right()
{
	speed[L] = +TRN_SPEED;
	speed[R] = +MIN_SPEED;

	set_speed();

	// if (last_called != 4)
	// 	ChosenSerial->println("motors.right()");
	// last_called = 4;

	return false;
}

bool TwoMotors::left()
{
	speed[L] = +MIN_SPEED;
	speed[R] = +TRN_SPEED;

	set_speed();

	// if (last_called != 5)
	// 	ChosenSerial->println("motors.left()");
	// last_called = 5;

	return false;
}

bool TwoMotors::left_fwd()
{
	speed[L] = +TRN_SPEED_;
	speed[R] = +TRN_SPEED;

	set_speed();

	// if (last_called != 6)
	// {
	// 	ChosenSerial->print("motors.left_fwd()");
	// 	ChosenSerial->print(speed[L]);
	// 	ChosenSerial->print(" ");
	// 	ChosenSerial->println(speed[R]);
	// }
	// last_called = 6;

	return false;
}

bool TwoMotors::right_fwd()
{
	speed[L] = +TRN_SPEED;
	speed[R] = +TRN_SPEED_;

	set_speed();

	// if (last_called != 7)
	// {
	// 	ChosenSerial->print("motors.right_fwd()");
	// 	ChosenSerial->print(speed[L]);
	// 	ChosenSerial->print(" ");
	// 	ChosenSerial->println(speed[R]);
	// }
	// last_called = 7;

	return false;
}

bool TwoMotors::right_bwd()
{ // turns right while going backwards
	speed[L] = -MIN_SPEED;
	speed[R] = -TRN_SPEED;

	set_speed();

	// if (last_called != 8)
	// 	ChosenSerial->println("motors.right_bwd()");
	// last_called = 8;

	return false;
}

bool TwoMotors::left_bwd()
{ // turns left while going backwards
	speed[L] = -TRN_SPEED;
	speed[R] = -MIN_SPEED;

	set_speed();

	// if (last_called != 9)
	// 	ChosenSerial->println("motors.left_bwd()");
	// last_called = 9;

	return false;
}

bool TwoMotors::extra_power()
{
	speed[L] = copysign(MAX_SPEED, speed[L]);
	speed[R] = copysign(MAX_SPEED, speed[R]);

	set_speed();

	// if (last_called != 10)
	// 	ChosenSerial->println("motors.extra_power()");
	// last_called = 10;

	return true;
}

bool TwoMotors::backward_extrapower()
{ // same as backward, but with -MAX_SPEED instead of -BWD_SPEED
	speed[L] = -MAX_SPEED;
	speed[R] = -MAX_SPEED;

	set_speed();

	// if (last_called != 11)
	// 	ChosenSerial->println("motors.backward()");
	// last_called = 11;

	return false;
}

/*
void TwoMotors::apply_trncoef(bool motor)	{
	if(motor= MLEFT)
		choose_speed(TRN_COEF * speed[R], speed[R]);
	else
		choose_speed(speed[L], TRN_COEF * speed[L]);
}
*/