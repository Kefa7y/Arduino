       

#define servo0        9
#define irout         2
#define irin          4
#define button        7
#define LED           13

#include <Servo.h>
Servo myservo0;  // create servo object to control a servo
int occupied;
int lastbut;
int exiting;

void setup() { 
  
  pinMode(servo0,OUTPUT);
  pinMode(button,INPUT);
  myservo0.attach(servo0);
  pinMode(irout,INPUT);
  pinMode(irin,INPUT);
  pinMode(LED,OUTPUT);
  occupied=0; 
  myservo0.write(90);

} 

void loop(){
  int out = digitalRead(irout); 
  int in = digitalRead(irin);
  int but = digitalRead(button);
  if(out == 0 && occupied ==0){
    myservo0.write(0);
  }
  if(in == 0 && occupied==0){
    myservo0.write(90);
    occupied=1;
    digitalWrite(LED,HIGH);
  }

  if(but ==0 && lastbut == 1 && occupied==1){
    myservo0.write(0);
    exiting=1;
  }

  if(out ==0 && occupied==1 && exiting ==1){
    myservo0.write(90);
    occupied=0;
    digitalWrite(LED,LOW);
    exiting=0;
    delay(1000);
  }

  lastbut=but;
}


