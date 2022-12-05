
#include <Servo.h>
//Inializando Variables

Servo servoMotor;
int numeros[11][4] = {
  {0,0,0,0},
  {0,0,0,1},
  {0,0,1,0},
  {0,0,1,1},
  {0,1,0,0},
  {0,1,0,1},
  {0,1,1,0},
  {0,1,1,1},
  {1,0,0,0},
  {1,0,0,1},
  {1,0,1,0},
  };
//Contadores de Paquetes
int paquetesEnRuta = 0;
int paquetesSmall = 0;
int paquetesMiddle = 0;
int paquetesBig = 0;
//Contadotes de despachos
int reinicioSmall = 0;
int reinicioMiddle = 0;
int reinicioBig = 0;
//BANDA PRINCIPAL
int INITBANDA1 = 22;
int FINBANDA1 = 30;
int ACTYBANDA1 = 26;
int ACTBBANDA1 = 31;
//BANDA PARA PAQUETES PEQUEÑOS
int INITBANDA2 = 32;
int FINBANDA2 = 40;
int ACTYBANDA2 = 36;
int ACTBBANDA2 = 41;
//BANDA PARA PAQUETES MEDIANOS
int INITBANDA3 = 42;
int FINBANDA3 = 50;
int ACTYBANDA3 = 46;
int ACTBBANDA3 = 51;
//BANDA PARA PAQUETES GRANDES
int INITBANDA4 = 2;
int FINBANDA4 = 10;
int ACTYBANDA4 = 6;
int ACTBBANDA4 = 11;

///botones de seleccion
//int btnSmall = 52;
//int btnMiddle = 53;
//int btnBig = 13;

int DES1 = 13;
int DES2 = 52;
int DES3 = 53; 

int btnD1 = 0;
int btnD2 = 0;
int btnD3= 0;

/*int btnS = 0;
int btnM = 0;
int btnB = 0;*/
//Estados
int estadoBanda = 0;
int estadoAux = 0;

int estadoPrincipal = 0;
int estadoBanda1 = 0;
int estadoBanda2 = 0;
int estadoBanda3 = 0;
int estadoBanda4 = 0;

int contP = 0;
int contM = 0;
int contB = 0;

unsigned long tiempoAhora = 0;
int tiempoLED = 250;
String msg;

void setup() {
  Serial.begin(9600);    //iniciar puerto serie
  Serial1.begin(9600);
  for(int i = 22; i <= 53; i++){
    pinMode(i, OUTPUT);
    }
    for(int i = 2; i <= 11; i++){
    pinMode(i, OUTPUT);
    }
    pinMode(DES1, INPUT);
    pinMode(DES2, INPUT);
    pinMode(DES3, INPUT);
    
   servoMotor.attach(12);
   servoMotor.write(0);
    delay(1000);
}
 
void loop(){
  btnD1 = digitalRead(DES1);
  btnD2 = digitalRead(DES2);
  btnD3 = digitalRead(DES3);
      
  if(Serial.available()>0)
  {
    msg=Serial.readStringUntil('\n');
    Serial.println(msg);
    if (msg == "pequeno" && paquetesEnRuta <2 && estadoBanda == 0)
    {
      estadoBanda = 1;
      paquetesEnRuta++;
      estadoBanda1 = 1;
      Serial1.print("enc1");
      Serial1.print('\n');
      paqueteSmall(INITBANDA1,ACTYBANDA1,ACTBBANDA1);
    }
    
    if (msg == "mediano" && paquetesEnRuta <2 && estadoBanda == 0)
    {
        estadoBanda = 1;
        paquetesEnRuta++;
        estadoBanda1 = 2;
        Serial1.print("enc1");
        Serial1.print('\n');
        paqueteMiddle(FINBANDA1,ACTYBANDA1,ACTBBANDA1);
    }
    if (msg == "grande" && paquetesEnRuta <2 && estadoBanda == 0)
    {
       estadoBanda = 1;    
       paquetesEnRuta++;
       estadoBanda1 = 3;
       Serial1.print("enc1");
       Serial1.print('\n');
       paqueteBig(INITBANDA1,FINBANDA1,ACTYBANDA1,ACTBBANDA1);
    }

    if(msg=="b1")
    {
      Serial1.print("buzz1");
      Serial1.print('\n');
      paquetesSmall = 0;
    }

    if(msg=="b2")
    {
      Serial1.print("buzz2");
      Serial1.print('\n');
      paquetesMiddle = 0;
    }

    if(msg=="b3")
    {
      Serial1.print("buzz3");
      Serial1.print('\n');
      paquetesBig = 0;
    }
  }


  servoMotor.write(0);
  //delay(250);
    tiempoAhora = millis();
    while (millis() < tiempoAhora + 250) {
   }
//************************BANDA PRINCIPAL************************************
   if(estadoBanda1 == 1 && contP <10)
   {
      contP++;
      paqueteSmall(INITBANDA1,ACTYBANDA1,ACTBBANDA1);
   }else if(estadoBanda1 == 1 && contP >=10){
    contP =0;
    estadoBanda1 = 0;
    estadoBanda = 0;
    estadoBanda2 = 1;
    Serial1.print("apa1");
    Serial1.print('\n');

    digitalWrite(ACTYBANDA1, HIGH);
    servoMotor.write(63);
    Serial.print("ok");
    Serial.print('\n');
    //delay(1000);
    tiempoAhora = millis();
    while (millis() < tiempoAhora + 1000) {
   }
    Serial1.print("enc2");
    Serial1.print('\n');
   }
   else if(estadoBanda1 == 2 && contM <10)
   {
      contM++;
      paqueteMiddle(FINBANDA1,ACTYBANDA1,ACTBBANDA1);
   }else if(estadoBanda1 == 2 && contM >=10){
    contM = 0;
    estadoBanda1 = 0;
    estadoBanda = 0;
    estadoBanda3 = 1;
    Serial1.print("apa1");
    Serial1.print('\n');  

    digitalWrite(ACTBBANDA1, HIGH);
    servoMotor.write(90);
    Serial.print("ok");
    Serial.print('\n');
    //delay(1000);
    tiempoAhora = millis();
    while (millis() < tiempoAhora + 1000) {
   }
    Serial1.print("enc3");
    Serial1.print('\n');
    }
    else if(estadoBanda1 == 3 && contB <10)
   {
      contB++;
      paqueteBig(INITBANDA1,FINBANDA1,ACTYBANDA1,ACTBBANDA1);
   }else if(estadoBanda1 == 3 && contB >=10){
    contB = 0;
    estadoBanda1 = 0;
    estadoBanda = 0;
    estadoBanda4 = 1;
    Serial1.print("apa1");
    Serial1.print('\n');
    
    digitalWrite(ACTBBANDA1, HIGH);
    servoMotor.write(135);
    Serial.print("ok");
    Serial.print('\n');
    //delay(1000);
    tiempoAhora = millis();
    while (millis() < tiempoAhora + 1000) {
   }
    Serial1.print("enc4");
    Serial1.print('\n');
    }
    
//******************BANDA PAQUETES PEQUEÑOS*****************************
    if(estadoBanda2 == 1 && contP < 10){
      
      paqueteSmall(INITBANDA2,ACTYBANDA2,ACTBBANDA2);
      contP++;
    }else if(estadoBanda2 == 1 && contP >=10){
      contP = 0;
      estadoBanda2 = 0;
      paquetesEnRuta--;
      paquetesSmall++;
      digitalWrite(ACTYBANDA2, HIGH);
      Serial1.print("apa2");
      Serial1.print('\n');
      Serial.print("pp");
      Serial.print('\n');
   }
   
//*********************BANDA PAQUETES MEDIANOS*****************************   
   if(estadoBanda3 == 1 && contM < 10){
      
      paqueteMiddle(FINBANDA3,ACTYBANDA3,ACTBBANDA3);
      contM++;
    }else if(estadoBanda3 == 1 && contM >=10){
      contM = 0;
      estadoBanda3 = 0;
      paquetesEnRuta--;
      paquetesMiddle++;
      digitalWrite(ACTBBANDA3, HIGH);
      Serial1.print("apa3");
      Serial1.print('\n');
      Serial.print("pm");
      Serial.print('\n');
   }
//************************BANDA PAQUETES GRANDES**********************
    if(estadoBanda4 == 1 && contB < 10){
      
      paqueteBig(INITBANDA4,FINBANDA4,ACTYBANDA4,ACTBBANDA4);
      contB++;
    }else if(estadoBanda4 == 1 && contB >=10){
      contB = 0;
      estadoBanda4 = 0;
      paquetesEnRuta--;
      paquetesBig++;
      
      digitalWrite(ACTBBANDA4, HIGH);
      digitalWrite(ACTYBANDA4, HIGH);
      Serial1.print("apa4");
      Serial1.print('\n');
      Serial.print("pg");
      Serial.print('\n');
   }      




  if(btnD1 == HIGH){
    Serial1.print("buzz1");
    Serial1.print('\n');
    paquetesSmall = 0;
  }
  
  if (btnD2 == HIGH){
    Serial1.print("buzz2");
    Serial1.print('\n');
    paquetesMiddle = 0;
  }

  if(btnD3 ==  HIGH){
    Serial1.print("buzz3");
    Serial1.print('\n');
    paquetesBig = 0;
  }

}


void paqueteSmall(int inicio,int actY, int actB){
  digitalWrite(actY, LOW);
  digitalWrite(actB, HIGH);
    pintarAzul(inicio,contP);
    
    tiempoAhora = millis();
    while (millis() < tiempoAhora + tiempoLED ) {
   }
  }

void paqueteMiddle(int fin,int actY, int actB){
    digitalWrite(actY, HIGH);
    digitalWrite(actB, LOW);
    pintarAmarillo(fin,contM);
    tiempoAhora = millis();
    while (millis() < tiempoAhora + tiempoLED ) {
   }
  }

void paqueteBig(int inicio, int fin,int actY, int actB){
    digitalWrite(actY, LOW);
    digitalWrite(actB, HIGH);
    pintarAzul(inicio,contB);
    tiempoAhora = millis();
    while (millis() < tiempoAhora + tiempoLED ) {
   }
    digitalWrite(actY, HIGH);
    digitalWrite(actB, LOW);
    pintarAmarillo(fin,contB);
    tiempoAhora = millis();
    while (millis() < tiempoAhora + tiempoLED ) {
   }

    digitalWrite(actY, LOW);
    digitalWrite(actB, HIGH);
    pintarAzul(inicio,contB);
    tiempoAhora = millis();
    while (millis() < tiempoAhora + tiempoLED ) {
   }
   
    digitalWrite(actY, HIGH);
    digitalWrite(actB, LOW);
    pintarAmarillo(fin,contB);
    tiempoAhora = millis();
    while (millis() < tiempoAhora + tiempoLED ) {
   }
  
  }

void pintarAzul(int inicio, int pos){
    digitalWrite(inicio+3, numeros[pos][0]);
    digitalWrite(inicio+2, numeros[pos][1]);
    digitalWrite(inicio+1, numeros[pos][2]);
    digitalWrite(inicio, numeros[pos][3]);
  }

void pintarAmarillo(int fin, int pos){
    digitalWrite(fin, numeros[pos][0]);
    digitalWrite(fin-1, numeros[pos][1]);
    digitalWrite(fin-2, numeros[pos][2]);
    digitalWrite(fin-3, numeros[pos][3]);
  }
