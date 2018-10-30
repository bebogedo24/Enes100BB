#include <Enes100.h>
#include <Enes100Simulation.h>
#include <DFRTankSimulation.h>

#define MARKER_ID 1
#define RX_PIN 8
#define TX_PIN 9
#define GOAL_X 2.5
#define GOAL_Y 
#define TOLERANCE 0.05
#define UP 1.57
#define DOWN -1.57

//To transfer to actual OSV, comment out "Enes100Simulation enes;" and uncoment:
//Enes100 enes("Lost", BLACK_BOX, MARKER_ID, RX_PIN, TX_PIN);
Enes100Simulation enes;
DFRTankSimulation tank;

void setup() {
  tank.init();

  while (!enes.retrieveDestination());

  while (!enes.updateLocation());

  enes.println("Connected and Setup");
}
void loop() {
  if (enes.updateLocation()) {
    if(enes.location.x < (GOAL_X + TOLERANCE)){
      if(enes.location.theta > 0.1) {
        //turn right:
        tank.setRightMotorPWM(-255);
        tank.setLeftMotorPWM(255);
      } else if (enes.location.theta < -0.1) {
        //turn left:
        tank.setRightMotorPWM(255);
        tank.setLeftMotorPWM(-255);
      } else {
        //move forward:
        tank.setRightMotorPWM(255);
        tank.setLeftMotorPWM(255);
      }
    } else if(enes.location.y < (GOAL_Y + TOLERANCE)){
      //1.47 = pi/2 - 0.1
      if(enes.location.theta > (DOWN - TOLERANCE)) {
        //turn left:
        tank.setRightMotorPWM(255);
        tank.setLeftMotorPWM(-255);

        //1.67 = pi/2 + 0.1
      } else if (enes.location.theta < (DOWN + TOLERANCE)) {
        //turn right:
        tank.setRightMotorPWM(-255);
        tank.setLeftMotorPWM(255);
      } else {
        //move forward:
        tank.setRightMotorPWM(255);
        tank.setLeftMotorPWM(255);
      }
    } else if(enes.location.y > (GOAL_Y + TOLERANCE)){
      //1.47 = pi/2 - 0.1
      if(enes.location.theta > (UP - TOLERANCE)) {
        //turn left:
        tank.setRightMotorPWM(255);
        tank.setLeftMotorPWM(-255);

        //1.67 = pi/2 + 0.1
      } else if (enes.location.theta < (UP + TOLERANCE)) {
        //turn right:
        tank.setRightMotorPWM(-255);
        tank.setLeftMotorPWM(255);
      } else {
        //move forward:
        tank.setRightMotorPWM(255);
        tank.setLeftMotorPWM(255);
      } 
    }else {
      tank.turnOffMotors();
      //IMPORATNT: uncomment when transferring to real OSV:
      //enes.navigated();
      //Coordinate ret(GOAL_X, GOAL_Y);
      //enes.baseObjective(ret);
    }
    
  }

}
