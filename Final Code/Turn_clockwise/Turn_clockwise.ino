#include <Enes100.h>
#include <DFRTank.h>

#define MARKER_ID 8
#define RX_PIN 8
#define TX_PIN 9

Enes100 enes("Lost", BLACK_BOX, MARKER_ID, RX_PIN, TX_PIN);
DFRTank tank;

void setup() {
  tank.init();

  while (!enes.retrieveDestination());

  //while (!enes.updateLocation());

  enes.println("Connected and Setup");
}

void TurnLeft(){
  tank.setLeftMotorPWM(-255);
  tank.setRightMotorPWM(255);
}

void TurnRight(){
  tank.setLeftMotorPWM(255);
  tank.setRightMotorPWM(-255);
}

//turns clockwise to theta
void Turn_CW_To_Theta(int newTheta){
  if(enes.location.theta < newTheta) {
    enes.updateLocation();
    
    TurnLeft();

    enes.print("My theta is ");
    enes.println(enes.location.theta);
  }
}

void loop() {
  //the delay is to allow the vision system to catch up
  Turn_CW_To_Theta(90);
  delay(500);
  Turn_CW_To_Theta(180);
  delay(500);
  Turn_CW_To_Theta(270);
  delay(500);
  Turn_CW_To_Theta(359);
}
