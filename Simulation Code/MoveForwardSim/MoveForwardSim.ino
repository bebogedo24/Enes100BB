#include <Enes100.h>
#include <Enes100Simulation.h>
#include <DFRTankSimulation.h>

#define MARKER_ID 1
#define AP
#define APC220_IN 3

//To transfer to actual OSV, comment out "Enes100Simulation enes;" and uncoment:
//Enes100 enes("Lost", BLACK_BOX, MARKER_ID, APC220_IN, APC220_OUT);
Enes100Simulation enes;
DFRTankSimulation tank;

void setup() {
  tank.init();

  while (!enes.retrieveDestination());

  while (!enes.updateLocation());

  enes.println("Connected and Setup");
}

void loop() {
  //tweak the ending value, 4, so that it stops right before the arena end
  while(enes.location.x < 4) {
    //Replace these with whatever commands make the motors move.
    tank.setLeftMotorPWM(255);
    tank.setRightMotorPWM(255);
  }

}
