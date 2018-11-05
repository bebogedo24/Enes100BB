#include <Enes100.h>
#include <DFRTank.h>

#define MARKER_ID  13
#define RX_PIN 8
#define TX_PIN 9
#define GOAL_X 2
#define GOAL_Y 1
#define TOLERANCE 0.05
#define UP 1.570796
#define DOWN -1.570796

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
  enes.turnOffMotors();
  delay(150);
  enes.updateLocation();
  if(enes.location.theta > newTheta + TOLERANCE) {
    //turn right:
    tank.setRightMotorPWM(-255);
    tank.setLeftMotorPWM(255);
    return 0;
  } else if (enes.location.theta < newTheta - TOLERANCE) {
    //turn left:
    tank.setRightMotorPWM(255);
    tank.setLeftMotorPWM(-255);
    return 0;
  } else {
    return 1;
  }
}


//turns couner clockwise to theta, meant to be used in loop until 1 is returned
int turn_ccw(double newTheta){
  enes.turnOffMotors();
  delay(150);
  enes.updateLocation();
  if(enes.location.theta > newTheta + TOLERANCE) {
    //turn left:
    tank.setRightMotorPWM(255);
    tank.setLeftMotorPWM(-255);
    return 0;    

  } else if (enes.location.theta < newTheta - TOLERANCE) {
    //turn right:
    tank.setRightMotorPWM(-255);
    tank.setLeftMotorPWM(255);
    return 0;
    
  } else {
    return 1;
  }
}

void loop() {
  
  if (enes.updateLocation()) {
    if(enes.location.x < (GOAL_X + TOLERANCE)){
      while(!turn_cw(0));
      tank.setRightMotorPWM(255);
      tank.setLeftMotorPWM(255);
      
    } else if(enes.location.y > (GOAL_Y + TOLERANCE)){
      while(!turn_cw(DOWN));
      tank.setRightMotorPWM(255);
      tank.setLeftMotorPWM(255);      
      
    } else if(enes.location.y < (GOAL_Y - TOLERANCE)){
      while(!turn_cw(UP));
      tank.setRightMotorPWM(255);
      tank.setLeftMotorPWM(255);  
      
    }else {
      enes.println("arrived at center.");
      tank.turnOffMotors();
      
      enes.navigated();
      Coordinate ret(enes.location.x, enes.location.y);
      enes.baseObjective(ret);
      
      while(!turn_cw(UP));
      tank.turnOffMotors();
      delay(500);

      while(!turn_cw(0));
      tank.turnOffMotors();
      delay(500);

      while(!turn_cw(DOWN));
      tank.turnOffMotors();
      delay(500);

      while(!turn_cw(0));
      tank.turnOffMotors();    
    } 
  }
}
