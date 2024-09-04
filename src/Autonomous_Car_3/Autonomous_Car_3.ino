#include <Servo.h>

Servo SU;
Servo SD;
Servo SA;


//  Servo de Frente
      //SU.write(90);
      //SD.write(130);
      //SA.write(70);

//  Servos a la Derecha

      //SU.write(0);
      //SD.write(170);
      //SA.write(170);

//  Servos a la Izquierda

      //SU.write(0);
      //SD.write(70);
      //SA.write(0);


//US
#define trig 12
#define echo 13

#define trig2 4
#define echo2 5

//Mototes
#define IN3 6
#define IN4 7

long distancia, duracion, dF, dD, dI, dA;
int direccion = 0; //-1 Izquierda | +1 Derecha | 0 Centro
int TiempoDeGiro = 3000;
int Frente = 50;

void setup(){

  Serial.begin(9600);

  //Sensor Ultrasónico
  pinMode(trig, OUTPUT);
  pinMode(echo, INPUT);
  pinMode(trig2, OUTPUT);
  pinMode(echo2, INPUT);

  //Pin Motores
  pinMode(IN3, OUTPUT);
  pinMode(IN4, OUTPUT);  

  //Servomotores
  SU.attach(3);
  SD.attach(11);
  SA.attach(10);

  //Posiciones Iniciales
  SU.write(90);
  SD.write(130);
  SA.write(70);

  delay(1000);

  Serial.println("Iniciando");

}

void loop(){
  
  SU.write(90);

  digitalWrite(IN3, LOW);
  digitalWrite(IN4, HIGH);

  dF = lectura_ultrasonico(trig, echo);


  if(direccion == 0){
    dF = lectura_ultrasonico(trig, echo);
    if(dF <= Frente){
      digitalWrite(IN3, LOW);
      digitalWrite(IN4, LOW);

      delay(500);

      SU.write(200);
      delay(700);
      dI = lectura_ultrasonico(trig, echo);

      delay(1000);

      SU.write(0);
      delay(1200);
      dD = lectura_ultrasonico(trig, echo);

      delay(1000);

      SU.write(90);

      if (dI >= dD){
        Serial.print("Girando a la Izquierda \n");
        SD.write(70);
        SA.write(170);

        digitalWrite(IN3, LOW);
        digitalWrite(IN4, HIGH);

        direccion = -1;

        delay(TiempoDeGiro);

        SD.write(130);

      }else if (dI <= dD){
        Serial.print("Girando a la Derecha \n");
        SD.write(170);
        SA.write(0);

        digitalWrite(IN3, LOW);
        digitalWrite(IN4, HIGH);

        direccion = 1;

        delay(TiempoDeGiro);

        SD.write(130);


      }else{
        Serial.print("Recto \n ");

        digitalWrite(IN3, HIGH);
        digitalWrite(IN4, LOW);

        delay(1000);
      }

    }
  } else if(direccion == 1){
    dF = lectura_ultrasonico(trig, echo);
    if(dF <= Frente){
      Serial.println("Dentro de Loop Derecha");

      Serial.print("Girando a la Derecha \n");
      SD.write(170);

      digitalWrite(IN3, LOW);
      digitalWrite(IN4, HIGH);

      delay(TiempoDeGiro);

      SD.write(130);

    }
  }else if (direccion == -1){
    dF = lectura_ultrasonico(trig, echo);
    if(dF <= Frente){
      Serial.println("Dentro de Loop Izquierda");

      Serial.print("Girando a la Izquierda \n");
      SD.write(30);

      digitalWrite(IN3, LOW);
      digitalWrite(IN4, HIGH);

      delay(TiempoDeGiro);

      SD.write(130);

    }
  }else{
    dF = lectura_ultrasonico(trig, echo);
    if(dF <= Frente){
      direccion = 0;
    }
  }

  Serial.println(dF);
}


int lectura_ultrasonico(int trigPin, int echoPin) {
  digitalWrite(trigPin, LOW);
  delayMicroseconds(2);
  digitalWrite(trigPin, HIGH);
  delayMicroseconds(10);
  digitalWrite(trigPin, LOW);
  duracion = pulseIn(echoPin, HIGH);
  int distancia = duracion / 29 / 2;
  return distancia;
}
