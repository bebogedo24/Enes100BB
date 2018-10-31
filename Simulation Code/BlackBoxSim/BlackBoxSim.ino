#include <Enes100.h>
#include <Enes100Simulation.h>
#include <DFRTankSimulation.h>

#define MARKER_ID 1
#define RX_PIN 8
#define TX_PIN 9
#define GOAL_X 2
#define GOAL_Y 1
#define TOLERANCE 0.05
#define UP 1.570796
#define DOWN -1.570796

//To transfer to actual OSV, comment out "Enes100Simulation enes;" and uncoment:
//Enes100 enes("Lost", BLACK_BOX, MARKER_ID, RX_PIN, TX_PIN);
Enes100Simulation enes;
DFRTankSimulation tank;

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
  delay(100);
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
  delay(100);
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
      enes.println("done");
      tank.turnOffMotors();
      //IMPORATNT: uncomment when transferring to real OSV:
      //enes.navigated();
      //Coordinate ret(GOAL_X, GOAL_Y);
      //enes.baseObjective(ret);
      enes.println("Before UP");
      while(!turn_cw(UP));
      tank.turnOffMotors();
      delay(500);
      enes.println("UP");
      while(!turn_cw(0));
      tank.turnOffMotors();
      delay(500);
      enes.println("0");
      while(!turn_cw(DOWN));
      tank.turnOffMotors();
      delay(500);
      enes.println("DOWN");
      while(!turn_cw(0));
      tank.turnOffMotors();
      enes.println("0");
      
      
    }
    
  }

}
