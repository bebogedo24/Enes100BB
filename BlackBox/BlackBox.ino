#include <Enes100.h>
#include <Enes100Simulation.h>
#include <DFRTankSimulation.h>

#define MARKER_ID 1
#define APC220_OUT 2
#define APC220_IN 3

Enes100 enes("BlackBoxTeam", BLACK_BOX, MARKER_ID, APC220_IN, APC220_OUT);

void setup() {
  // put your setup code here, to run once:
}

void loop() {
  // put your main code here, to run repeatedly:
     // Update the OSV's current location
    if (enes.updateLocation()) {
        enes.println("Location updated!");
        enes.print("My x coordinate is ");
        enes.println(enes.location.x);
        enes.print("My y coordinate is ");
        enes.println(enes.location.y);
        enes.print("My theta is ");
        enes.println(enes.location.theta);
    } else {
        enes.println("Couldn't update my location");
    }

    enes.navigated();

    // Transmit the coordinate of the black box
    Coordinate blackBox(2.9, 1.4);
    enes.baseObjective(blackBox);
}
}

