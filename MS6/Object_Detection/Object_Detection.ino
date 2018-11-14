#include <Enes100.h>
#include <DFRTank.h>

#define ECHO_PIN_LEFT 4 
#define TRIG_PIN_LEFT 5 
#define ECHO_PIN_RIGHT 2
#define TRIG_PIN_RIGHT 3
#define MARKER_ID  9
#define RX_PIN 8
#define TX_PIN 9
#define GOAL_X 2
#define GOAL_Y 1
#define TOLERANCE 0.01
#define CLOSEST_DISTANCE 8 //this is in cm

Enes100 enes("Lost", BLACK_BOX, MARKER_ID, RX_PIN, TX_PIN);
DFRTank tank;

double duration_Left, duration_Right, cm_Left, cm_Right;
double TOO_CLOSE_TO_WALL = .5; // in meters

void setup() {
  tank.init();
  
  pinMode(TRIG_PIN_LEFT, OUTPUT);
  pinMode(ECHO_PIN_LEFT, INPUT);
  pinMode(TRIG_PIN_RIGHT, OUTPUT);
  pinMode(ECHO_PIN_RIGHT, INPUT);

}

//turns clockwise to theta, meant to be used in loop until 1 is returned
int turn_cw(double newTheta){
  enes.updateLocation();
  if(enes.location.theta > newTheta + TOLERANCE) {
    //turn right:
    tank.setRightMotorPWM(123);
    tank.setLeftMotorPWM(-123);
    return 0;
  } else if (enes.location.theta < newTheta - TOLERANCE) {
    //turn left:
    tank.setRightMotorPWM(-123);
    tank.setLeftMotorPWM(123);
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
    tank.setRightMotorPWM(-123);
    tank.setLeftMotorPWM(123);
    return 0;    

  } else if (enes.location.theta < newTheta - TOLERANCE) {
    //turn right:
    tank.setRightMotorPWM(123);
    tank.setLeftMotorPWM(-123);
    return 0;
    
  } else {
    return 1;
  }
  tank.turnOffMotors();
  delay(150);
}

double DurationLength(int Trig_Pin,int Echo_Pin){
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
  return pulseIn(Echo_Pin, HIGH);
}

double ConvertToCM(double duration){
  return (duration/2) / 29.1; // Divide by 29.1 or multiply by 0.0343
}

//This stops the car and gathers a clear distance
double CheckDistances (double cm,int Trig_Pin,int Echo_Pin){
    double duration;
    tank.turnOffMotors();
    delay(50);
    duration = DurationLength(Trig_Pin,Echo_Pin);
    cm = ConvertToCM(duration);
    return cm;
}

// this function takes "Left" or "Right"
void turnLeftOrRight(double cm, String Direction){
  while(cm < CLOSEST_DISTANCE || cm = CLOSEST_DISTANCE){
    if(Direction.equals("Left") /*&& enes.location.y + TOO_CLOSE_TO_WALL < 4.0 && enes.location.y - TOO_CLOSE_TO_WALL > 0*/){
      while(turn_cw(0.0174533)<1); //turn until the tolerance measurements are met
    } /*else {
      turn_ccw(0.0174533); 
    }*/
    
    if(Direction.equals("Right") /*&& enes.location.y + TOO_CLOSE_TO_WALL < 4.0 && enes.location.y - TOO_CLOSE_TO_WALL > 0*/){
      while(turn_ccw(0.0174533)<1); //turn until the tolerance measurements are met
    } /*else {
      turn_ccw(0.0174533); 
    }*/
    cm = CheckDistances;
  }
}




void loop() {
  //These are the "cleaner" distance measurements
  double cm_Left_adjusted;
  double cm_Right_adjusted;

  // If the initial (meaning before any error checking of the distance) distance isn't too close, then move forward
  if(!((cm_Left < CLOSEST_DISTANCE || cm_Left = CLOSEST_DISTANCE) && (cm_Right < CLOSEST_DISTANCE || cm_Right = CLOSEST_DISTANCE))){
    tank.setRightMotorPWM(255);
    tank.setLeftMotorPWM(255);
    
    duration_Left = DurationLength(TRIG_PIN_LEFT,ECHO_PIN_LEFT); //Find the distance of any object in front of the left US sensor
    cm_Left = ConvertToCM(duration_Left); // Convert the time into a distance
      
    duration_Right = DurationLength(TRIG_PIN_RIGHT,ECHO_PIN_RIGHT); //Find the distance of any object in front of the right US sensor
    cm_Right = ConvertToCM(duration_Right); // Convert the time into a distance
    
  } else {
    //Check the Left US sensor
    if(cm_Left < CLOSEST_DISTANCE || cm_Left = CLOSEST_DISTANCE){
      cm_Left_adjusted = CheckDistances(cm_Left); //Get a more accurate measurement
      
      if(cm_Left_adjusted < CLOSEST_DISTANCE || cm_Left_adjusted = CLOSEST_DISTANCE){
        turnLeftOrRight(cm_Left_adjusted, "Left");
      }
      
    }
    //Check the Right US sensor
    if (cm_Right < CLOSEST_DISTANCE || cm_Right = CLOSEST_DISTANCE) {
      cm_Right_adjusted = CheckDistances(cm_Right); //Get a more accurate measurement
      
      if (cm_Right_adjusted < CLOSEST_DISTANCE || cm_Right_adjusted = CLOSEST_DISTANCE){
        turnLeftOrRight(cm_Right_adjusted, "Right");
      }
      
    }
  }

  
  /*
  Serial.print(inches);
  Serial.print("in, ");
  Serial.print(cm);
  Serial.print("cm");
  Serial.println();
  */
  

}
