#include <Enes100.h>
#include <DFRTank.h>

#define MARKER_ID  9
#define RX_PIN 8
#define TX_PIN 9
#define GOAL_X 2
#define GOAL_Y 1
#define TOLERANCE 0.1
#define UP 1.570796
#define DOWN -1.570796
double leftTheta;
double rightTheta;
boolean arrivedLeftTheta = false;
boolean arrivedRightTheta = false;

Enes100 enes("Lost", BLACK_BOX, MARKER_ID, RX_PIN, TX_PIN);
DFRTank tank;

int turn_cw(double newTheta);
int turn_ccw(double newTheta);

void setup() {
  tank.init();

  while (!enes.retrieveDestination());

  while (!enes.updateLocation());

  enes.println("Connected and Setup");
}

//turns clockwise to theta, meant to be used in loop until 1 is returned
int turn_cw(double newTheta){
  enes.updateLocation();
  if(enes.location.theta > newTheta + TOLERANCE) {
    //turn right:
    tank.setRightMotorPWM(255);
    tank.setLeftMotorPWM(-255);
    return 0;
  } else if (enes.location.theta < newTheta - TOLERANCE) {
    //turn left:
    tank.setRightMotorPWM(-255);
    tank.setLeftMotorPWM(255);
    return 0;
  } else {
    return 1;
  }
  tank.turnOffMotors();
  delay(150);
}


//turns couner clockwise to theta, meant to be used in loop until 1 is returned
int turn_ccw(double newTheta){
  enes.updateLocation();
  if(enes.location.theta > newTheta + TOLERANCE) {
    //turn left:
    tank.setRightMotorPWM(-255);
    tank.setLeftMotorPWM(255);
    return 0;    

  } else if (enes.location.theta < newTheta - TOLERANCE) {
    //turn right:
    tank.setRightMotorPWM(255);
    tank.setLeftMotorPWM(-255);
    return 0;
    
  } else {
    return 1;
  }
  tank.turnOffMotors();
  delay(150);
}

void loop{
  if(enes.updateLocation){
    while(turning_CW_to_sense_BB){
      if(sensor_value>0 && !arrivedLeftTheta){
        tank.turnOffMotors();
        leftTheta = enes.location.theta;
        arrivedLeftTheta = true;
      }
      if(sensor_value<0 && !arrivedRightTheta){
        tank.turnOffMotors();
        rightTheta = enes.location.theta;
        arrivedRightTheta = true;    
      }
      
      
    }
  turn_ccw((leftTheta+rightTheta)/2);

  
    
  }
}
}
