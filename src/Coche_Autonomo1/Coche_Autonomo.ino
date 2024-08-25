#include <Servo.h>

Servo servo;

//M1 izquierda
#define P1 14
#define P2 15
#define ENA 6

//M2 derecha
#define P3 16
#define P4 17
#define ENB 5

//US frente
#define Trig1 4
#define Echo1 2

//US izquierda
#define Trig2 8
#define Echo2 7

//US derecha
#define Trig3 10
#define Echo3 9


void setup() {
  //Servo

  servo.attach(3);
  

  //Motor izquierda
  pinMode(P1, OUTPUT);
  pinMode(P2, OUTPUT);
  pinMode(ENA, OUTPUT);

  //Motor derecha
  pinMode(P3, OUTPUT);
  pinMode(P4, OUTPUT);
  pinMode(ENB, OUTPUT);

  //SU frente
  pinMode(Trig1, OUTPUT);
  pinMode(Echo1, INPUT);

  //SU izquierda
  pinMode(Trig2, OUTPUT);
  pinMode(Echo2, INPUT);

  //SU derecha
  pinMode(Trig3, OUTPUT);
  pinMode(Echo3, INPUT);

  servo.attach(3);
  servo.write(100);
}

long duracion, distancia, di, dd, df;

void loop() {
  servo.write(100);



  distancia = Distancia(Trig1, Echo1);
  if (distancia <= 15){
    analogWrite(ENA, 0);
    analogWrite(ENB, 0);   
    servo.attach(100); 
  }

  distancia = Distancia(Trig2, Echo2);
  if (distancia <= 10){
    servo.attach(60);
  }

  distancia = Distancia(Trig3, Echo3);
  if (distancia <=10){
    servo.attach(140);
  }

  df = Distancia(Trig1, Echo1);
  di = Distancia(Trig2, Echo2);
  dd = Distancia(Trig3, Echo3);

  if (df <= 10 && dd <= 10){
    servo.attach(140);
  }
  else if(df <= 10 && di <= 10){
    servo.attach(60);
  }
  else if (df > 10 && di > 10 && dd > 10 ){
    //Motor izquierda
    digitalWrite(P1, HIGH);
    digitalWrite(P2, LOW);
    analogWrite(ENA, 255);

    //Motor derecha
    digitalWrite(P3, HIGH);
    digitalWrite(P4, LOW);
    analogWrite(ENB, 255);
  }
  else{
    //Motor izquierda
    digitalWrite(P1, HIGH);
    digitalWrite(P2, LOW);
    analogWrite(ENA, 0);

    //Motor derecha
    digitalWrite(P3, HIGH);
    digitalWrite(P4, LOW);
    analogWrite(ENB, 0);
  }

}


long Distancia(int trig, int echo){
  digitalWrite(trig, LOW);
  delayMicroseconds(2);
  digitalWrite(trig, HIGH);
  delayMicroseconds(10);
  digitalWrite(trig, LOW);

  duracion = pulseIn(echo, HIGH);
  distancia = (duracion / 2) / 29.1;
  return distancia;
}
