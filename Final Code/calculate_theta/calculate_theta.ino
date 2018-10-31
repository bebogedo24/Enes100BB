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

double calculate_theta(double desiredX, double desiredY){
  enes.updateLocation();
  double desiredTheta;
  double yDifference = desiredY-enes.location.y;
  double xDifference = desiredX-enes.location.x;

  desiredTheta = atan2(yDifference,xDifference);

  return desiredTheta
}

void loop() {
  

}
