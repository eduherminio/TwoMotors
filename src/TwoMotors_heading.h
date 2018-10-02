/*  Library for controlling 2DC motors
*/

#ifndef TwoMotors_heading_h
#define TwoMotors_heading_h

#define ZERO 0 // Shortcuts
#define L 0
#define MLEFT 0
#define R 1
#define MRIGHT 1
// Raw speeds (before coef_L/R)
#define MIN_SPEED 0   // Min speed the engine is able to go - must be 0
#define MAX_SPEED 255 // Max speed limit
#define FWD_SPEED 120
#define BWD_SPEED 75
#define TRN_SPEED 100 // When just turning
#define TRN_SPEED_ TRN_COEF *TRN_SPEED
//#define LOW_SPEED		25

#define coef_L 1.07 // speed= coef * raw_speed
#define coef_R 0.85
#define LOW_COEF 0.75 // LOW_SPEED= LOW_COEF x current speed
#define TRN_COEF 0.60

#define def_p_PWM_L 12
#define def_p_PWM_R 11

// Left motor
#define def_p_LE1 23 					
#define def_p_LE2 22

// Right motor
#define def_p_RE1 25
#define def_p_RE2 24

#endif
