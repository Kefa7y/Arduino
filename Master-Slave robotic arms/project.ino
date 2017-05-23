
#define encoder0PinA  2
#define encoder0PinB  4
#define encoder1PinA  3
#define encoder1PinB  8
#define servo0        9
#define servo1        10
#define encoder0Push  11
#define encoder1Push  7
#define LEDDone       12
#define LEDMode       13
#include <Servo.h>

Servo myservo1;Servo myservo0;  // create servo object to control a servo

volatile unsigned char encoder0Pos = 0;
volatile unsigned char encoder1Pos = 0;
unsigned char saved[10][2];
unsigned char current;  //the empty space to be saved into (does not wrap)
unsigned char follow;   //the index of the next position to be executed
boolean track,execute;
boolean past0, past1;

void setup() { 


  pinMode(encoder0PinA, INPUT); 
  digitalWrite(encoder0PinA, HIGH);       // turn on pull-up resistor
  pinMode(encoder0PinB, INPUT); 
  digitalWrite(encoder0PinB, HIGH);       // turn on pull-up resistor

  
  pinMode(encoder1PinA, INPUT); 
  digitalWrite(encoder1PinA, HIGH);       // turn on pull-up resistor
  pinMode(encoder1PinB, INPUT); 
  digitalWrite(encoder1PinB, HIGH);       // turn on pull-up resistor

  pinMode(encoder0Push, INPUT); 
  digitalWrite(encoder0Push, HIGH);

  pinMode(encoder1Push, INPUT); 
  digitalWrite(encoder1Push, HIGH);

  pinMode(LEDDone, OUTPUT);
  pinMode(LEDMode, OUTPUT);

  digitalWrite(LEDDone,LOW);

  attachInterrupt(0, doEncoder0, CHANGE);  // encoder pin on interrupt 0 - pin 2

  attachInterrupt(digitalPinToInterrupt(encoder1PinA), doEncoder1, CHANGE);  // encoder pin on interrupt 0 - pin 2

  myservo0.attach(servo0); 
  myservo1.attach(servo1);

  track=1;
  execute=0;
  
  //Serial.begin (9600);

} 

void loop(){
  if(track & !execute){
    myservo0.write(encoder0Pos*360/48);
    myservo1.write(encoder1Pos*360/48);
  }
  if(track & execute){
    myservo0.write(saved[follow][0]*360/48);
    myservo1.write(saved[follow][1]*360/48);
    follow= (follow+1)%current;
    delay(200);
  }

  if(past0 && !digitalRead(encoder0Push)){    //button 0 pushed
    track=!track;
    execute=0;
    follow=0;
  }
  if(past1 && !digitalRead(encoder1Push)){    //button 1 pushed
    if(!track){
      if(current<10){
        saved[current][0]=encoder0Pos;
        saved[current][1]=encoder1Pos;
        current++;
        digitalWrite(LEDDone,HIGH);
      } 
    }else{
      execute =!execute;
      digitalWrite(LEDDone,execute);
    }
  }
  past0 = digitalRead(encoder0Push);
  past1 = digitalRead(encoder1Push);
  digitalWrite(LEDMode,track);
}

void doEncoder0() {
  digitalWrite(LEDDone,LOW);
  if (digitalRead(encoder0PinA) == digitalRead(encoder0PinB)) {
    if(encoder0Pos<24)
      encoder0Pos++;
  } else {
    if(encoder0Pos>0)
      encoder0Pos--;
  }
}

void doEncoder1() {
  digitalWrite(LEDDone,LOW);
  if (digitalRead(encoder1PinA) == digitalRead(encoder1PinB)) {
    if(encoder1Pos<24)
      encoder1Pos++;
  } else {
    if(encoder1Pos>0)
      encoder1Pos--;
  }
}

