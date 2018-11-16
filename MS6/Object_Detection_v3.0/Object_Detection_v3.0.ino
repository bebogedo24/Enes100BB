#include <Enes100.h>
#include <DFRTank.h>

#define ECHO_PIN_LEFT 4 
#define TRIG_PIN_LEFT 5 
#define ECHO_PIN_RIGHT 2
#define TRIG_PIN_RIGHT 3
#define ECHO_PIN_CENTER 7
#define TRIG_PIN_CENTER 6
#define MARKER_ID  9
#define RX_PIN 8
#define TX_PIN 9
#define GOAL_X 2
#define GOAL_Y 1
#define TOLERANCE 0.01 // May need to Calibrate
#define CLOSEST_DISTANCE 10 //this is in cm, May need to Calibrate

Enes100 enes("Lost", BLACK_BOX, MARKER_ID, RX_PIN, TX_PIN);
DFRTank tank;

double cm_Center, cm_Left, cm_Right;
double TOO_CLOSE_TO_WALL = .5; // in meters, May need to Calibrate
boolean moving_Forward_Or_Not = false;

void setup() {
  Serial.begin(9600);
  
  tank.init();
  
  pinMode(TRIG_PIN_LEFT, OUTPUT);
  pinMode(ECHO_PIN_LEFT, INPUT);
  pinMode(TRIG_PIN_RIGHT, OUTPUT);
  pinMode(ECHO_PIN_RIGHT, INPUT);
  pinMode(TRIG_PIN_CENTER, OUTPUT);
  pinMode(ECHO_PIN_CENTER, INPUT);

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
  tank.setRightMotorPWM(123);
  tank.setLeftMotorPWM(-123);
  
}
void TurnCCW(){
  tank.setRightMotorPWM(-123);
  tank.setLeftMotorPWM(123);
  
}
void MoveForward(){
  tank.setRightMotorPWM(255);
  tank.setLeftMotorPWM(255);
  moving_Forward_Or_Not = true;
   
}
boolean MovingOrNot(){
  enes.updateLocation();
  distance1 = enes.location.y;
  delay(50); // May need to Calibrate
  enes.updateLocation();
  distance2 = enes.location.y;

  if((distance2-distance1) = 0){ // May need to Calibrate as if it's "pushing" the object
    return false; 
  } else {
    return true;
  }
  
}
void TurnLeftOrRight(double cm, String Direction){
  while(cm <= CLOSEST_DISTANCE){
    enes.updateLocation();
    if (Direction.equals("Left") && (enes.location.y - TOO_CLOSE_TO_WALL) > 0){ //only turns right if not close to right wall
      TurnCW(); 
      delay(250); // May need to Calibrate
      tank.turnOffMotors(); // stops turning

    } else if ((enes.location.y + TOO_CLOSE_TO_WALL) < 4){ // only turns left if not close to left wall
      TurnCW(); 
      delay(250); // May need to Calibrate
      tank.turnOffMotors(); 
         
    } else if(Direction.equals("Center"){ // turns toward center
      enes.updateLocation();
      if (enes.location.y > 2){
        TurnCW();
        delay(250); // May need to Calibrate
        turnOffMotors();
      } else {
        TurnCW();
        delay(250); // May need to Calibrate
        turnOffMotors();
      }  
         
    } else { 
      TurnCCW();
      delay(250); // May need to Calibrate
      tank.turnOffMotors();
      
    }   
    
    // Center has priority, because after the OSV is done sensing the center, it should sense the objects with the side sensors
    // May need to calibrate how far the OSV turns when something is sensed by the CENTER sensor, because of its blind spots
    if (Direction.equals("Center")){
      cm = DistanceMeasurement(TRIG_PIN_CENTER,ECHO_PIN_CENTER);
      
    } else if (Direction.equals("Left")) {
      cm = DistanceMeasurement(TRIG_PIN_LEFT,ECHO_PIN_LEFT);
    
    } else {
      cm = DistaneMeasurement(TRIG_PIN_RIGHT,ECHO_PIN_RIGHT);
    
    }
  }
}
void loop() {
  
  cm_Left = DistanceMeasurement(TRIG_PIN_LEFT,ECHO_PIN_LEFT); // Convert the time into a distance
  cm_Center = DistanceMeasurement(TRIG_PIN_CENTER, ECHO_PIN_CENTER); 
  cm_Right = DistanceMeasurement(TRIG_PIN_RIGHT,ECHO_PIN_RIGHT); 

  if (!(cm_Left <= CLOSEST_DISTANCE && cm_Right <= CLOSEST_DISTANCE) && cm_Center <= CLOSEST_DISTANCE) {
    Serial.println("move forward");
    MoveForward();
    
  } else if(moving_Forward_Or_Not && !MovingOrNot){
    moving_Forward_Or_Not = false;
    tank.setRightMotorPWM(-255); //back up
    tank.setLeftMotorPWM(-255);
    TurnCW();
    delay(100); // May need to Calibrate
      
  } else {
    //Check the Left US sensor
    if(cm_Center <= CLOSEST_DISTANCE){
      Serial.println("center");
      TurnLeftOrRight(cm_Center, "Center");
      
    } else if (cm_Left <= CLOSEST_DISTANCE){
      Serial.println("left");
      TurnLeftOrRight(cm_Left, "Left");
        
    } else if (cm_Right <= CLOSEST_DISTANCE){
      Serial.println("Right");
      TurnLeftOrRight(cm_Right, "Right");
      
    }
  }

  
}
