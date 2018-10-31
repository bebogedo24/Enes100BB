#include <Enes100.h>
#include <DFRTank.h>

#define MARKER_ID 8
#define RX_PIN 8
#define TX_PIN 9
int completeTurn = 0;

Enes100 enes("Lost", BLACK_BOX, MARKER_ID, RX_PIN, TX_PIN);
DFRTank tank;

void setup() {
  tank.init();

  while (!enes.retrieveDestination());

  //while (!enes.updateLocation());

  enes.println("Connected and Setup");
}

//this sets up the initial angle of the OSV after the tank object is initialized
int initialAngle = enes.location.theta;

void TurnLeft(){
  tank.setLeftMotorPWM(255);
  tank.setRightMotorPWM(-255);
}

void TurnRight(){
  tank.setLeftMotorPWM(-255);
  tank.setRightMotorPWM(255);
}

//turns clockwise to 90 degrees
void Turn_CW_To_Theta(int newTheta){
  if(enes.location.theta < newTheta) {
    enes.updateLocation();
    
    TurnLeft();

    enes.print("My theta is ");
    enes.println(enes.location.theta);
  }
}

void Turn_CCW_To_Theta(int newTheta2){
  if(enes.location.theta > newTheta2) {
    enes.updateLocation();
    
    TurnRight();

    enes.print("My theta is ");
    enes.println(enes.location.theta);
  }
}

void Turn_To_Theta_with_Error_Correction(int initialTheta, int finalTheta, int completeTurnIndicator){
 
  
  //turns clockwise between initial and final theta
  while(completeTurn != completeTurnIndicator && enes.location.theta < finalTheta && enes.location.theta > initialTheta){
    Turn_CW_To_Theta(finalTheta);
    delay(200); //this allows the vision system to update while checking theta;
    if(enes.location.theta == finalTheta){
      completeTurn = completeTurnIndicator;
    }
  }

  
  //if the final turn degree has been overshot, then turn back to finalTheta
  while(completeTurn != completeTurnIndicator && enes.location.theta > finalTheta){
    Turn_CCW_To_Theta(finalTheta);
    delay(200);
    if(enes.location.theta == finalTheta){
      completeTurn = completeTurnIndicator;
    }
  }
}
void loop() {

  //this is so any angle from (initialAngle-5) to the (initial angle + the 90 degree turn) will turn toward the desired angle degrees
  //this allows for some leeway in the angles
  Turn_To_Theta_with_Error_Correction((initialAngle-5),(initialAngle+90),1);
  delay(500);
  Turn_To_Theta_with_Error_Correction(89,180,2);
  delay(500);
  Turn_To_Theta_with_Error_Correction(179,270,3);
  delay(500);
  Turn_To_Theta_with_Error_Correction(269,359,4);

  

}
