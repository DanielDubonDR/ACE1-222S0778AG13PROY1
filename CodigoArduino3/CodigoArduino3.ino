#include <Stepper.h>


//-----------MOTOR 1-----------
#define MO1A 25
#define MO1B 24
#define MO1C 23
#define MO1D 22
//-----------MOTOR 2----------
#define MO2A 26
#define MO2B 27
#define MO2C 28
#define MO2D 29
//----------MOTOR 3-----------
#define MO3A 30
#define MO3B 31
#define MO3C 32
#define MO3D 33
//----------MOTOR 4-----------
#define MO4A 37
#define MO4B 36
#define MO4C 35
#define MO4D 34
//----------MOTOR 5-----------
#define MO5A 41
#define MO5B 40
#define MO5C 39
#define MO5D 38
//----------MOTOR 6-----------
#define MO6A 42
#define MO6B 43
#define MO6C 44
#define MO6D 45
//----------MOTOR 7-----------
#define MO7A 49
#define MO7B 48
#define MO7C 47
#define MO7D 46
//----------MOTOR 8-----------
#define MO8A 50
#define MO8B 51
#define MO8C 52
#define MO8D 53
//----------BUZZERS-----------
#define BUZZ1 7
#define BUZZ2 6
#define BUZZ3 5

const int v = 40;
Stepper stepper1(v,MO2A,MO2B,MO2C,MO2D);
Stepper stepper2(v,MO4A,MO4B,MO4C,MO4D);
Stepper stepper3(v,MO6A,MO6B,MO6C,MO6D);
Stepper stepper4(v,MO8A,MO8B,MO8C,MO8D);
//-----------Estados----------
int motor1 = 0;
int motor2 = 0;
int motor3 = 0;
int motor4 = 0;

//----------------------------
String msg;
int led = 8;
unsigned long tiempoAhora = 0;

int durac = 250;
void setup() 
{
  Serial.begin(9600);
  stepper1.setSpeed(5000);
  stepper2.setSpeed(5000);
  stepper3.setSpeed(5000);
  stepper4.setSpeed(5000);
  //pinMode(led, OUTPUT);
  for(int i = 22; i <=53;i++){
    pinMode(i,OUTPUT);
    }

    for(int j = 2; j <=7;j++){
    pinMode(j,OUTPUT);
    }


}

void loop() 
{
  
  if(Serial.available()>0)
  {
    msg=Serial.readStringUntil('\n');
    if(msg == "enc1")
    {
      motor1 = 1;
    }
    else if(msg == "apa1")
    {
      motor1 = 0;
    }    
    
    if(msg == "enc2")
    {
      motor2 = 1;
    }else if(msg == "apa2")
    {
      motor2 = 0;
    }
    
    if(msg == "enc3")
    {
      motor3 = 1;
    }else if(msg == "apa3")
    {
      motor3 = 0;
    }
    
    if(msg == "enc4")
    {
      motor4 = 1;
    }else if(msg == "apa4")
    {
      motor4 = 0;
      
    }

    if(msg == "buzz1"){
      buzz1();
      }
    if(msg == "buzz2"){
      buzz2();
      }
    if(msg == "buzz3"){
      buzz3();
      }
  }
  

  if(motor1 == 1){
    motorCorrer(MO1A,MO1B,MO1C,MO1D);
    stepper1.step(5);
    }


  if(motor2 == 1){
    motorCorrer(MO3A,MO3B,MO3C,MO3D);
    stepper2.step(5);
    }

  if(motor3 == 1){
    motorCorrer(MO5A,MO5B,MO5C,MO5D);
    stepper3.step(5);
    }

  if(motor4 == 1){
    motorCorrer(MO7A,MO7B,MO7C,MO7D);
    stepper4.step(5);
    }
}

void motorCorrer(int a, int b, int c, int d){
    step1(a,b,c,d);
    //step2
    step2(a,b,c,d);
    //step3
    step3(a,b,c,d);
    ///step4
    step4(a,b,c,d);
  
  }

void step1(int a, int b, int c, int d){
    //A+,B+
    digitalWrite(a, 1);
    digitalWrite(b, 0);
    digitalWrite(c, 1);
    digitalWrite(d, 0);
    tiempoAhora = millis();
    while (millis() < tiempoAhora + 100) {
   }
}

void step2(int a, int b, int c, int d){
    //A+,B-

    digitalWrite(a, 1);
    digitalWrite(b, 0);
    digitalWrite(c, 0);
    digitalWrite(d, 1);
    tiempoAhora = millis();
    while (millis() < tiempoAhora + 100) {
   }
    
    //delay(500);
}

void step3(int a, int b, int c, int d){
    //A-,B-
    digitalWrite(a, 0);
    digitalWrite(b, 1);
    digitalWrite(c, 0);
    digitalWrite(d, 1);
    tiempoAhora = millis();
    while (millis() < tiempoAhora + 100) {
   }
    //delay(500);
}

void step4(int a, int b, int c, int d){
    //A-,B+
    digitalWrite(a, 0);
    digitalWrite(b, 1);
    digitalWrite(c, 1);
    digitalWrite(d, 0);
    tiempoAhora = millis();
    while (millis() < tiempoAhora + 100) {
   }
    //delay(500);
}

void buzz1(){
    tone(BUZZ1,329);             
    delay(200);    
    noTone(BUZZ1); 
}
  
void buzz2(){
    tone(BUZZ2,293);             
    delay(200);    
    noTone(BUZZ2); 
  }

void buzz3(){
    tone(BUZZ3,392);             
    delay(200);    
    noTone(BUZZ3); 
  }
