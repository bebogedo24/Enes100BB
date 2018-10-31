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

void loop() {
  
  if(enes.location.x < 3.5) {
    enes.updateLocation();
    
    tank.setLeftMotorPWM(255);
    tank.setRightMotorPWM(255);

    enes.println("Location updated!");
    enes.print("My x,y coordinate is ");
    enes.println(enes.location.x);
    enes.print(" ");
    enes.print(enes.location.y);

    enes.print("My theta is ");
    enes.println(enes.location.theta);
  }
}
