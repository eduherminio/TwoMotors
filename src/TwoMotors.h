/*  Library for controlling 2DC motors
*/

#ifndef TwoMotors_h
#define TwoMotors_h

#include "Arduino.h"

class TwoMotors
{
  public:
	TwoMotors();
	TwoMotors(
		int p_PWM_L, int p_PWM_R,
		int p_LE1, int p_LE2,
		int p_RE1, int p_RE2);

	void change_serial(HardwareSerial&);

	void emergency_stop();
	void set_speed();
	void correct_speed();
	void adapt_enables();

	void choose_speed(int speedL, int speedR);
	// void apply_trncoef(bool motor);
	bool lower_speed();
	bool extra_power();
	bool forward();
	bool backward();
	bool backward_extrapower();
	bool stop();

	bool left();
	bool right();
	bool left_fwd();
	bool right_fwd();

	bool left_bwd();
	bool right_bwd();

  private:
#pragma region MotorPins
	const uint8_t
		p_PWM_L,
		p_PWM_R,
		p_LE1, // Left motor
		p_LE2,
		p_RE1, // Right motor
		p_RE2;
#pragma endregion Motor pins

	// Current active pins
	uint8_t
		_pin_PWM,
		_pin_MI,
		_pin_MD;

	int speed[2] = {0};
	bool Lenable[2] = {false};
	bool Renable[2] = {false};

	int last_called = 999;

	HardwareSerial* ChosenSerial = &Serial;
};

#endif
