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
#define TOLERANCE 0.01
#define CLOSEST_DISTANCE 10 //this is in cm

Enes100 enes("Lost", BLACK_BOX, MARKER_ID, RX_PIN, TX_PIN);
DFRTank tank;

double cm_Center, cm_Left, cm_Right;
double TOO_CLOSE_TO_WALL = .5; // in meters

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

int i = 0;

void loop() {
  
  cm_Left = DistanceMeasurement(TRIG_PIN_LEFT,ECHO_PIN_LEFT); // Convert the time into a distance
  if(i == 10){
    Serial.print(cm_Left);
  }

  cm_Center = DistanceMeasurement(TRIG_PIN_CENTER, ECHO_PIN_CENTER); // Convert the time into a distance
  if(i == 10){
    Serial.print("          ");
    Serial.print(cm_Center);
  }

  
  cm_Right = DistanceMeasurement(TRIG_PIN_RIGHT,ECHO_PIN_RIGHT); // Convert the time into a distance
  if(i == 10){
    Serial.print("          ");
    Serial.print(cm_Right);
    Serial.println("");
    i = 0;
  }
  i++;
  

  
}
