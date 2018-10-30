#include <Enes100.h>
#include <Enes100Simulation.h>
#include <DFRTankSimulation.h>

#define MARKER_ID 1
#define RX_PIN 8
#define TX_PIN 9
#define GOAL_X 3
#define GOAL_Y 2
#define TOLERANCE 0.3

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
        tank.setRightMotorPWM(255);
        tank.setLeftMotorPWM(255);
      }
    } else {
      tank.turnOffMotors();
    }
  }
  delay(500);

}
