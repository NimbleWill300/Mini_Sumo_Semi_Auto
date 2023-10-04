#include <Arduino.h>
#include <PS4Controller.h>
#include "motors.hpp"
#include "state.hpp"

void setup() {
  motor_setup();
  Serial.begin(115200);
  PS4.begin("2c:dc:d7:39:25:74");
  Serial.println("Ready.");
  set_state(Controller);
}

void loop() {
  if (PS4.isConnected()) {
    update_state();
    run_state();
    
    Serial.print(get_motor_power(LEFT));
    Serial.print("\t");
    Serial.println(get_motor_power(RIGHT));
  }else{
    motor_control(RIGHT, 0);
    motor_control(LEFT, 0);
  }
}