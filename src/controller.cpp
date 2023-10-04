#include "controller.hpp"

LedColors colors;
Motors motor_power;
int first_change = 1;

int get_motor_power(side lado){
    if(lado == RIGHT) return motor_power.powerRight;
    else return motor_power.powerLeft;
}

void set_motor_power(side lado, int valor){
    if(lado == RIGHT) motor_power.powerRight = valor;
    else motor_power.powerLeft = valor;
}

void change_led_color(int r, int g, int b){
    if(r != colors.r || g != colors.g || b != colors.b || first_change){
      first_change = 0;
      colors.r = r;
      colors.g = g;
      colors.b = b;
      PS4.setLed(r, g, b);
      PS4.sendToController();
    }
}

int calculate_deadZone(int input){
  if(input > -MOTOR_DEADZONE && input < MOTOR_DEADZONE) return 0;
  else return input;
}

int update_controller(ControllerState state){
  if(state == NormalMode){
    int x = 0;
    int r2 = 0, l2 = 0;
    if (PS4.RStickX()) {
      x = PS4.RStickX();
    }
    if(PS4.R2Value()){
      r2 = PS4.R2Value();
    }else{
      r2 = 0;
    }
    if(PS4.L2Value()){
      l2 = PS4.L2Value();
    }else{
      l2 = 0;
    }
    // r2 = calculate_deadZone(r2);     //Como o valor nao varia q nem o joystick 
    // l2 = calculate_deadZone(l2);     //nao vi necessidade de colocar deadZone
    x = calculate_deadZone(x);
    int speed = r2 - l2;
    int mappedX = map(x, -127, 127, -MAX_MOTOR, MAX_MOTOR);
    
    motor_power.powerRight = speed - mappedX;
    motor_power.powerLeft  = speed + mappedX;
    
  }else{
    int right = 0;
    int left = 0;
    if (PS4.RStickY()) {
      right = PS4.RStickY();
    }
    if (PS4.LStickY()) {
      left = PS4.LStickY();
    }
    
    right = calculate_deadZone(right);
    left = calculate_deadZone(left);

    int mappedRight = map(right, -127, 127, -MAX_MOTOR, MAX_MOTOR);
    int mappedLeft  = map(left , -127, 127, -MAX_MOTOR, MAX_MOTOR);
    
    motor_power.powerRight = mappedRight;
    motor_power.powerLeft  = mappedLeft;
    
  }

  if(motor_power.powerRight == 0 && motor_power.powerLeft == 0){
    return 0;
  }else{
    return 1;
  }
}
