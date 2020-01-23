/*
  http://www.instructables.com/member/Mohannad+Rawashdeh/
*/

#define IN1  3
#define IN2  4
#define IN3  5
#define IN4  6

int Steps = 0;
unsigned long lastTime;
int extraStap = 0;


char secStep = 64 * 64 / 60; // aantal stappen is 64, gear ration = 64, 60 seconden in een omwenteling
                             // let op er lijft nog 0.266666666 per stap over. is dus nog geen compleet rondje

void setup(){
  pinMode(IN1, OUTPUT); 
  pinMode(IN2, OUTPUT); 
  pinMode(IN3, OUTPUT); 
  pinMode(IN4, OUTPUT);
  lastTime = millis();
 Serial.begin(9600); 
}

void loop() {
  unsigned long currentMillis = millis();
  // millis loopt af en toe uit zijn geheugen en begint weer op 0
  if (millis() < lastTime) {
    lastTime = 0;
  }
  if (millis() - lastTime > 1000) {
      lastTime += 1000;
      stepper(secStep);
     // elke 14 stappen moeten er 4 stappen bij
     extraStap = (extraStap + 1) % 15;
     Serial.println(extraStap);
     if ( extraStap==0) {
        stepper(4);
        Serial.println("4 exrta stappen");
     } 
    }
}

void stepper(int xw){
  for (int x=0;x<xw;x++){

  unsigned long lastMicros = micros();
  while (micros() -  lastMicros < 900){
    // empty statement
  }
  switch(Steps){
     case 0:
       digitalWrite(IN1, LOW); 
       digitalWrite(IN2, LOW);
       digitalWrite(IN3, LOW);
       digitalWrite(IN4, HIGH);
       break; 
     case 1:
       digitalWrite(IN1, LOW); 
       digitalWrite(IN2, LOW);
       digitalWrite(IN3, HIGH);
       digitalWrite(IN4, HIGH);
       break; 
     case 2:
       digitalWrite(IN1, LOW); 
       digitalWrite(IN2, LOW);
       digitalWrite(IN3, HIGH);
       digitalWrite(IN4, LOW);
       break; 
     case 3:
       digitalWrite(IN1, LOW); 
       digitalWrite(IN2, HIGH);
       digitalWrite(IN3, HIGH);
       digitalWrite(IN4, LOW);
       break; 
     case 4:
       digitalWrite(IN1, LOW); 
       digitalWrite(IN2, HIGH);
       digitalWrite(IN3, LOW);
       digitalWrite(IN4, LOW);
       break; 
     case 5:
       digitalWrite(IN1, HIGH); 
       digitalWrite(IN2, HIGH);
       digitalWrite(IN3, LOW);
       digitalWrite(IN4, LOW);
       break; 
     case 6:
       digitalWrite(IN1, HIGH); 
       digitalWrite(IN2, LOW);
       digitalWrite(IN3, LOW);
       digitalWrite(IN4, LOW);
       break; 
     case 7:
       digitalWrite(IN1, HIGH); 
       digitalWrite(IN2, LOW);
       digitalWrite(IN3, LOW);
       digitalWrite(IN4, HIGH);
       break; 
     default:
       digitalWrite(IN1, LOW); 
       digitalWrite(IN2, LOW);
       digitalWrite(IN3, LOW);
       digitalWrite(IN4, LOW);
       break; 
    }
    SetDirection();
  }
} 

void SetDirection(){
  Steps++;
  if(Steps>7){Steps=0;}
  if(Steps<0){Steps=7; }
}
