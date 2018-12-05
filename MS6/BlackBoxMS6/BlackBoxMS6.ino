#include <Enes100.h>
#include <DFRTank.h>


#define ECHO_PIN_LEFT 8 
#define TRIG_PIN_LEFT 9 
#define ECHO_PIN_RIGHT 2
#define TRIG_PIN_RIGHT 3
#define ECHO_PIN_CENTER 11
#define TRIG_PIN_CENTER 10
#define MARKER_ID  9
#define RX_PIN 13
#define TX_PIN 12
#define GOAL_X 2
#define GOAL_Y 1
#define TOLERANCE 0.07 // May need to Calibrate
#define CLOSEST_DISTANCE .45 //change back to 50 //this is in cm, May need to Calibrate
#define UP 1.570796
#define DOWN -1.570796

Enes100 enes("Lost", BLACK_BOX, MARKER_ID, RX_PIN, TX_PIN);
DFRTank tank;


double cm_Center, cm_Left, cm_Right;
double TOO_CLOSE_TO_WALL = .5; 
boolean arrivedAtCenter = false;
boolean baseObjectivesAchieved = false;
boolean pickedUpBlackBox = false;
int count = 0;
double averageTheta = 1;
double previousTheta = 0;
double currentTheta = 0;
boolean continueTurning = false;


int turn_cw(double newTheta);
int turn_ccw(double newTheta);

void setup() {
  tank.init();

  while (!enes.retrieveDestination());

  while (!enes.updateLocation());

  enes.println("Connected and Setup");
}

double DistanceMeasurement(int Trig_Pin,int Echo_Pin){
  double duration;
    // The sensor is triggered by a HIGH pulse of 10 or more microseconds.
  // Give a short LOW pulse beforehand to ensure a clean HIGH pulse:
  digitalWrite(Trig_Pin, LOW);
  delayMicroseconds(5);
  digitalWrite(Trig_Pin, HIGH);
  delayMicroseconds(10);
  digitalWrite(Trig_Pin, LOW);
 
  // Read the signal from the sensor: a HIGH pulse whose
  // duration is the time (in microseconds) from the sending
  // of the ping to the reception of its echo off of an object.
  duration = pulseIn(Echo_Pin, HIGH);
  return (duration/2) / 29.1;
  
}

void TurnCW(){
  tank.setRightMotorPWM(255);
  tank.setLeftMotorPWM(-255);
  delay(750); // May need to Calibrate
  tank.turnOffMotors();

}
void TurnCCW(){
  tank.setRightMotorPWM(-255);
  tank.setLeftMotorPWM(255);
  delay(750); // May need to Calibrate
  tank.turnOffMotors();
  
}
void MoveForward(){
  tank.setRightMotorPWM(255);
  tank.setLeftMotorPWM(255);
  delay(50);
   
}

void TurnLeftOrRight(double cm, String Direction){
  while(cm <= CLOSEST_DISTANCE){
    
    enes.updateLocation();
    /*
    if ((enes.location.y - TOO_CLOSE_TO_WALL) < 0){  
      while(!turn_ccw(UP)); 
      enes.println("turning left too close to wall");

    } else if ((enes.location.y + TOO_CLOSE_TO_WALL) > 4){ // only turns left if not close to left wall
      while(!turn_cw(DOWN));
      enes.println("turning right too close to wall");
       
    } else if (Direction.equals("Center")){ */// turns toward center
      if (enes.location.y > 1){
        while(!turn_cw(DOWN));       
      } else {
        while(!turn_ccw(UP));
      }
   /*   
    } else if (Direction.equals("Right")){
      while(!turn_ccw(UP));   
       
    } else {
      while(!turn_cw(DOWN));
      
    }
    */
    // Center has priority, because after the OSV is done sensing the center, it should sense the objects with the side sensors
    // May need to calibrate how far/long the OSV turns when something is sensed by the CENTER sensor, because of its blind spots

    if (Direction.equals("Center")){
      cm = DistanceMeasurement(TRIG_PIN_CENTER,ECHO_PIN_CENTER);    

      
    } else if (Direction.equals("Left")) {
      cm =  DistanceMeasurement(TRIG_PIN_LEFT,ECHO_PIN_LEFT);

    
    } else if (Direction.equals("Right")){
      cm = DistanceMeasurement(TRIG_PIN_RIGHT,ECHO_PIN_RIGHT);

    }
  }
}

//turns clockwise to theta, meant to be used in loop until 1 is returned
int turn_cw(double newTheta){
  enes.updateLocation();
  
  if(count == 1){
    previousTheta = enes.location.theta;

  } else if (count > 15){
    currentTheta = enes.location.theta;
    averageTheta = currentTheta - previousTheta;
    count = 0;
    
  } count ++;
  
  if (averageTheta == 0){
    tank.setRightMotorPWM(-123);
    tank.setLeftMotorPWM(-123);
    delay(750);
    averageTheta = 1;
    
  }
  if(enes.location.theta > newTheta + TOLERANCE) {
    //turn right:
    tank.setRightMotorPWM(-123); //change back, multiply by -1
    tank.setLeftMotorPWM(123); //change back, multiply by -1
    //enes.println("CW -- TRUE");
    return 0;
    
  } else if (enes.location.theta < newTheta - TOLERANCE) {
    //turn left:
    tank.setRightMotorPWM(123); //change back, multiply by -1
    tank.setLeftMotorPWM(-123); //change back, multiply by -1
    //enes.println("CW -- FALSE");
    return 0;
    
  } else {
    return 1;
  }
}


//turns couner clockwise to theta, meant to be used in loop until 1 is returned
int turn_ccw(double newTheta){
  enes.updateLocation();
  
  if(count == 1){
    previousTheta = enes.location.theta;

  } else if (count > 20){
    currentTheta = enes.location.theta;
    averageTheta = currentTheta - previousTheta;
    count = 0;
    
  } count ++;
  
  if (averageTheta == 0){
    tank.setRightMotorPWM(-123);
    tank.setLeftMotorPWM(-123);
    delay(500);
    averageTheta = 1;
    
  }
  if(enes.location.theta < newTheta - TOLERANCE) {
    //turn left:
    tank.setRightMotorPWM(123); //change back, multiply by -1
    tank.setLeftMotorPWM(-123); //change back, multiply by -1
    //enes.println("CCW -- TRUE");
    return 0;    

  } else if (enes.location.theta > newTheta + TOLERANCE) {
    //turn right:
    tank.setRightMotorPWM(-123); //change back, multiply by -1
    tank.setLeftMotorPWM(123); //change back, multiply by -1
    //enes.println("CCW -- FALSE");
    return 0;
    
  } else {
    return 1;
  }
}


void ObjectDetection() {
  int i = 0;
  while(true){  
    for(int i = 0; i < 6; i++){
      cm_Left = DistanceMeasurement(TRIG_PIN_LEFT,ECHO_PIN_LEFT); // Convert the time into a distance
      cm_Center = DistanceMeasurement(TRIG_PIN_CENTER, ECHO_PIN_CENTER); 
      cm_Right = DistanceMeasurement(TRIG_PIN_RIGHT,ECHO_PIN_RIGHT);

      if(cm_Center <= CLOSEST_DISTANCE && i == 5){
        enes.println("Object Detected");
        TurnLeftOrRight(cm_Center, "Center");
        // MoveForward();
        
      } else if (cm_Left <= CLOSEST_DISTANCE && i == 5){
        TurnLeftOrRight(cm_Left, "Left");
        enes.println("Object Detected");    
        //MoveForward();
        
      } else if (cm_Right <= CLOSEST_DISTANCE && i == 5){
        TurnLeftOrRight(cm_Right, "Right");
        enes.println("Object Detected");  
        //MoveForward();
        
      }
    }      
    return;
  }
}

void loop() {
  if(pickedUpBlackBox){
    //returning to LZ
    // turn to theta = pi then go straight
    //if object detected, turn away until not sensed and then move forward a SET distance 
    //turn back toward LZ and continue forward
    
  } else if(baseObjectivesAchieved){
    //picking up Black Box
    pickedUpBlackBox = true; 
    
  } else if(arrivedAtCenter){
    //IR sensor Code
    baseObjectivesAchieved = true; //when arrived at BB and transmitted coordinates
    
  }
  
  else if (enes.updateLocation()) {
    enes.print("x: ");
    enes.print(enes.location.x);
    enes.print("y: ");
    enes.println(enes.location.y);
    
    if(enes.location.x < (GOAL_X + TOLERANCE)){
      while(!turn_cw(0));
      ObjectDetection();
      for(int a = 0; a<20; a++){
        MoveForward();
      }
 
    } else if(enes.location.y > (GOAL_Y + TOLERANCE)){
      while(!turn_cw(DOWN));
      ObjectDetection();
      MoveForward();

      
    } else if(enes.location.y < (GOAL_Y - TOLERANCE)){
      while(!turn_ccw(UP));
      ObjectDetection();
      MoveForward();
 
      
    }else {
      enes.println("arrived at center.");
      tank.turnOffMotors();
      
      enes.navigated();
      Coordinate ret(enes.location.x, enes.location.y);
      enes.baseObjective(ret);
      arrivedAtCenter = true;
      
    } 
  }

}
