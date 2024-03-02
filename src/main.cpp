#include <Arduino.h>
#include <PS4Controller.h>
#include "motors.hpp"
#include "state.hpp"

// #define pino26 GPIO_NUM_4  // NÃO
// #define pino27 GPIO_NUM_16 // NÃO
// #define pino28 GPIO_NUM_17 // NÃO
// #define pino29 GPIO_NUM_5  // funcionaaa BINARIO KK
// #define pino30 GPIO_NUM_18 // NÃO
// #define pino31 GPIO_NUM_19 // NÃO

// #define pino23 GPIO_NUM_15 // 
// #define pino22 GPIO_NUM_8  // 
// #define pino21 GPIO_NUM_7  // 
// #define pino18 GPIO_NUM_10 // 
// #define pino17 GPIO_NUM_9  // 
// #define pino16 GPIO_NUM_13 // 

void setup() {
  motor_setup();
  Serial.begin(115200);
  PS4.begin("2c:33:7a:a5:9f:56");
  Serial.println("Ready.");
  set_state(Controller);
}

void loop() {
  if (PS4.isConnected()) {
    update_state();
    run_state();
    
    // Serial.print(get_motor_power(LEFT));
    // Serial.print("\t");
    // Serial.println(get_motor_power(RIGHT));
  }else{
    motor_control(RIGHT, 0);
    motor_control(LEFT, 0);
  }
}