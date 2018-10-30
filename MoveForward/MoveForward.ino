#include <Enes100.h>
#include <Enes100Simulation.h>
#include <DFRTankSimulation.h>

#define MARKER_ID 1
#define RX_PIN 8
#define TX_PIN 9

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
  
  //tweak the ending value, 4, so that it stops right before the arena end
  if(enes.location.x < 4) {
    enes.updateLocation();
    
    //Replace these with whatever commands make the motors move.
    tank.setLeftMotorPWM(255);
    tank.setRightMotorPWM(255);
  }

}
