/*
 HC-SR04 Ping distance sensor]
 VCC to arduino 5v GND to arduino GND
 Echo to Arduino pin 13 Trig to Arduino pin 12
 Red POS to Arduino pin 11
 Green POS to Arduino pin 10
 560 ohm resistor to both LED NEG and GRD power rail
 More info at: http://goo.gl/kJ8Gl
 Original code improvements to the Ping sketch sourced from Trollmaker.com
 Some code and wiring inspired by http://en.wikiversity.org/wiki/User:Dstaub/robotcar
 */

#define trigPin 13
#define echoPin 12
#define led 11
#define led2 10

#define ARRAY_SIZE 10
unsigned long distances[10];
int arrayPos = 0;
int counter = 0;

void setup() {
  Serial.begin (9600);
  pinMode(trigPin, OUTPUT);
  pinMode(echoPin, INPUT);
  pinMode(led, OUTPUT);
  pinMode(led2, OUTPUT);
}

void loop() {
  long duration, distance;
  digitalWrite(trigPin, LOW);
  delayMicroseconds(2);
  digitalWrite(trigPin, HIGH);
  delayMicroseconds(15);
  digitalWrite(trigPin, LOW);
  duration = pulseIn(echoPin, HIGH);
  distance = duration / 57;
  distances[arrayPos] = distance;
  arrayPos = ++arrayPos % ARRAY_SIZE;
  
  counter = ++counter % 10;
  if (counter==0) {
    Serial.print(duration);
    Serial.print(" ms, ");
    Serial.print(distance);
    Serial.println(" cm");
  }
  
  delay(50);
}

unsigned long getDistance() {
  unsigned long highest = 0;
  unsigned long lowest = 0xFFFFFFFF;
  unsigned long sum = 0;
  
  for (int i = 0; i < ARRAY_SIZE; i++) {
    highest=max(highest,distances[i]);
    lowest=min(highest,distances[i]);
    sum += distances[i];
  } 
  sum -= (highest + lowest);
  
  return sum/(ARRAY_SIZE - 2);
}

