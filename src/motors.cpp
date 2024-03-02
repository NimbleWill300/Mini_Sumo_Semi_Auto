#include "motors.hpp"
#include <Arduino.h>

static MotorSpeed motor_speed;

MotorDirection motor_direction;

void change_motor_direction(side side, int valor){
    if(side == RIGHT){
        motor_direction.right = valor;
    }else{
        motor_direction.left = valor;
    }
}

void motor_setup(){
    motor_speed.left = 0;
    motor_speed.right = 0;
    motor_direction.right = 1;
    motor_direction.left = 1;
    pinMode(EN_B, OUTPUT);
    pinMode(BI1, OUTPUT);
    pinMode(BI2, OUTPUT);
    pinMode(AI2, OUTPUT);
    pinMode(AI1, OUTPUT);
    pinMode(EN_A, OUTPUT);
    pinMode(STANDBY, OUTPUT);
    digitalWrite(STANDBY, HIGH);
}

int limit_value(int speed){
    if (speed > MAX_MOTOR) 
        speed = MAX_MOTOR;
    else if (speed < -MAX_MOTOR)
        speed = -MAX_MOTOR;
    return speed;
}

float acceleration(float last_speed, int new_speed){
    if(new_speed > last_speed){
        last_speed = last_speed + ACCELERATION;
    }if(new_speed < last_speed){
        last_speed = last_speed - ACCELERATION;
    }if(new_speed = 0){
        last_speed = 0;
    }
    return last_speed;
}

void motor_control(side side, int32_t speed) {
    speed = limit_value(speed);

    switch (side) {
        case RIGHT:
            motor_speed.right = acceleration(motor_speed.right, speed);
            motor_speed.right = motor_speed.right * motor_direction.right;
            Serial.println(motor_speed.right);
            if (motor_speed.right > 0) {
                digitalWrite(AI1, HIGH);
                digitalWrite(AI2, LOW);
                analogWrite(EN_A, motor_speed.right);
            } else{
                digitalWrite(AI1, LOW);
                digitalWrite(AI2, HIGH);
                analogWrite(EN_A, -motor_speed.right);
            }
            break;
        case LEFT:
            motor_speed.left = acceleration(motor_speed.left, speed);
            motor_speed.left = motor_speed.left * motor_direction.left;
            Serial.print(motor_speed.left);
            Serial.print("\t");
            if (motor_speed.left > 0) {
                digitalWrite(BI1, LOW);
                digitalWrite(BI2, HIGH);
                analogWrite(EN_B, motor_speed.left);
            } else{
                digitalWrite(BI1, HIGH);
                digitalWrite(BI2, LOW);
                analogWrite(EN_B, -motor_speed.left);
            }
            break;
        default:
            break;
    }
}