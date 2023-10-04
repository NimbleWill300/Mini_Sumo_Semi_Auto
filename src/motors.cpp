#include "motors.hpp"
#include <Arduino.h>

static MotorSpeed motor_speed;

void motor_setup(){
    pinMode(EN_B, OUTPUT);
    pinMode(BI1, OUTPUT);
    pinMode(BI2, OUTPUT);
    pinMode(AI2, OUTPUT);
    pinMode(AI1, OUTPUT);
    pinMode(EN_A, OUTPUT);
    pinMode(STANDBY, OUTPUT);
    digitalWrite(STANDBY, HIGH);
}

void motor_control(side side, int32_t speed) {
    if (speed > MAX_MOTOR) 
        speed = MAX_MOTOR;
    else if (speed < -MAX_MOTOR)
        speed = -MAX_MOTOR;

    switch (side) {
        case RIGHT:
            motor_speed.right = speed;
            if (speed > 0) {
                digitalWrite(AI1, HIGH);
                digitalWrite(AI2, LOW);
                analogWrite(EN_A, speed);
            } else{
                digitalWrite(AI1, LOW);
                digitalWrite(AI2, HIGH);
                analogWrite(EN_A, -speed);
            }
            break;
        case LEFT:
            motor_speed.left = speed;
            if (speed > 0) {
                digitalWrite(BI1, LOW);
                digitalWrite(BI2, HIGH);
                analogWrite(EN_B, speed);
            } else{
                digitalWrite(BI1, HIGH);
                digitalWrite(BI2, LOW);
                analogWrite(EN_B, -speed);
            }
            break;
        default:
            break;
    }
}