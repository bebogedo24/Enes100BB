#include <Enes100.h>
#include <DFRTank.h>

#define MARKER_ID  13
#define RX_PIN 8
#define TX_PIN 9
#define GOAL_X 2
#define GOAL_Y 1

Enes100 enes("Lost", BLACK_BOX, MARKER_ID, RX_PIN, TX_PIN);
DFRTank tank;

void setup() {
  tank.init();

}

bool IR_Read() {
  int numReadings = 30;
  int i = 0;
  bool ret = false;
  
  while (i < numReadings) {
    if (analogRead(0) < 1000) {
      ret = true;
    }
    delay(10);
    i++;
  }
  return ret;
}

void loop() {
  tank.setRightMotorPWM(-255);
  tank.setLeftMotorPWM(255);
  delay(100);
  tank.turnOffMotors();
  while (IR_Read()) {
    tank.setRightMotorPWM(255);
    tank.setLeftMotorPWM(255);
  }
  
}
