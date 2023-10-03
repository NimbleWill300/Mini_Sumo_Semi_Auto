#include "util.hpp"
#include <Arduino.h>

static MotorSpeed motor_speed;

void motor_control(side side, int32_t speed) {
    if (speed > MAX_MOTOR) 
        speed = MAX_MOTOR;
    else if (speed < -MAX_MOTOR)
        speed = -MAX_MOTOR;

    switch (side) {
        case RIGHT:
            motor_speed.right = speed;
            if (speed > 0) {
                digitalWrite(MOTOR_IN_A1, HIGH);
                digitalWrite(MOTOR_IN_A2, LOW);
                analogWrite(RIGHT_MOTOR_PWM, speed);
            } else{
                digitalWrite(MOTOR_IN_A1, LOW);
                digitalWrite(MOTOR_IN_A2, HIGH);
                analogWrite(RIGHT_MOTOR_PWM, -speed);
            }
            break;
        case LEFT:
            motor_speed.left = speed;
            if (speed > 0) {
                digitalWrite(MOTOR_IN_B1, LOW);
                digitalWrite(MOTOR_IN_B2, HIGH);
                analogWrite(LEFT_MOTOR_PWM, speed);
            } else{
                digitalWrite(MOTOR_IN_B1, HIGH);
                digitalWrite(MOTOR_IN_B2, LOW);
                analogWrite(LEFT_MOTOR_PWM, -speed);
            }
            break;
        default:
            break;
    }
}