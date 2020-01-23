// Sweep
// by BARRAGAN <http://barraganstudio.com>
// This example code is in the public domain.
 
 
#include <Servo.h>

#define MAX_GRADEN 170
#define MIN_GRADEN 10

#define FRONT                 0
#define REAR                  1

#define LEFT_WHEEL_INTERRUPT  2
#define RIGHT_WHEEL_INTERRUPT 3
#define FRONT_SERVO_PIN       9
#define REAR_SERVO_PIN        8
#define FRONT_TRIGGER_PIN     10
#define FRONT_ECHO_PIN        11
#define REAR_TRIGGER_PIN      12
#define REAR_ECHO_PIN         13


// groffe afstands indicatie
#define FAR                   100
#define CLOSE                 5  
#define MEDIUM                20



Servo servoFront;   // 
Servo servoRear;    // 

 
int pos = 0;    // variable to store the servo position

long leftTicks = 0;
void leftWheelCounter() {
  leftTicks++;
  Serial.print("left interrupt nr " );
  Serial.println(leftTicks);
}

long rightTicks = 0;
void rightWheelCounter() {
   rightTicks++;
  Serial.print("right interrupt nr " );
  Serial.println(rightTicks);
}
 
void setup()
{
  servoFront.attach(FRONT_SERVO_PIN);    // 
  servoRear.attach(REAR_SERVO_PIN);  // 
  setServo(servoFront, 90, 100);
  setServo(servoRear, 90, 100);

  // HC-RS04 echo module
  pinMode(FRONT_TRIGGER_PIN, OUTPUT);
  pinMode(FRONT_ECHO_PIN,    INPUT );
  pinMode(REAR_TRIGGER_PIN,  OUTPUT);
  pinMode(REAR_ECHO_PIN,     INPUT );
  
  Serial.begin(250000);

  attachInterrupt(digitalPinToInterrupt(LEFT_WHEEL_INTERRUPT),  leftWheelCounter, RISING);
  attachInterrupt(digitalPinToInterrupt(RIGHT_WHEEL_INTERRUPT), rightWheelCounter, RISING);
  
}
 
void loop()
{
  check(FRONT); 
  check(REAR);
  delay(2000); 
}

void check(int position) {
  String msg;
  Servo servo;
  switch(position) {
  case FRONT:
     msg = "front";
     servo = servoFront;
     break;
  case REAR:
     msg = "rear";
     servo = servoRear;
     break;
   default:
     Serial.println("positie niet gedefinieerd");
     return;
  }

  // bepaal huidige positie
  int dist = getDistance(position);
  Serial.print("Afstand " + msg + " 90 graden: ");
  Serial.println(dist);
  if (dist > CLOSE) {
    // we mogen doorrijden
    Serial.println("We kunnen door");
    return; 
  }
  
  int _delay = 300;
  setServo(servo,  10,  _delay);
  Serial.print("Afstand " + msg + " 10 graden: ");
  Serial.println(dist);
  if (dist > CLOSE) {
    // we mogen doorrijden
    Serial.println("We kunnen 10 graden pakken");
    setServo(servo,  90, 0);
    return; 
  }
  
  setServo(servo,  170,  _delay);
  dist = getDistance(position);
  Serial.print("Afstand " + msg + " 170 graden: ");
  Serial.println(dist);
  if (dist > CLOSE) {
    // we mogen doorrijden
    Serial.println("We kunnen 170 graden pakken");
    setServo(servo,  90, 0);
    return; 
  }

  // ok, we moeten dus de andere kant op
  Serial.println("Versperd, we moeten terug!");
}

int getDistance(int position) {
  char triggerPin, echoPin;
  switch (position) {
  case FRONT:
     triggerPin = FRONT_TRIGGER_PIN;
     echoPin = FRONT_ECHO_PIN;
     break;
  case REAR:
     triggerPin = REAR_TRIGGER_PIN;
     echoPin = REAR_ECHO_PIN;
     break;
   default:
     Serial.println("positie niet gedefinieerd");
     return -1;
  }
 digitalWrite(triggerPin, LOW); 
 delayMicroseconds(2); 

 digitalWrite(triggerPin, HIGH);
 delayMicroseconds(10); 
 
 digitalWrite(triggerPin, LOW);
 int duration = pulseIn(echoPin, HIGH);
 
 //Calculate the distance (in cm) based on the speed of sound.
 return duration / 58.2;
}

void setServo(Servo s, int hoek, int _delay) {
    s.write(hoek);              // tell servo to go to position in variable 'pos'
    delay(_delay);              // waits 15ms for the servo to reach the position
}

