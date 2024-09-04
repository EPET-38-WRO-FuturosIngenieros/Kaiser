#include <Servo.h>

Servo SU;
Servo SD;
Servo SA;

const int botonPin = 4;   
int estadoBoton;       

// Servos
#define trig 12
#define echo 13

#define trig2 8
#define echo2 9

// Motores
#define IN3 6
#define IN4 7

long distancia, duracion, dF, dD, dI, dA;
int direccion = 0; // -1 Izquierda | +1 Derecha | 0 Centro
int TiempoDeGiro = 3300;
int Frente = 20;

void setup() {
  Serial.begin(9600);

  // Configuración del botón
  pinMode(botonPin, INPUT_PULLUP); 

  // Sensor Ultrasónico
  pinMode(trig, OUTPUT);
  pinMode(echo, INPUT);
  pinMode(trig2, OUTPUT);
  pinMode(echo2, INPUT);

  // Pin Motores
  pinMode(IN3, OUTPUT);
  pinMode(IN4, OUTPUT);

  // Servomotores
  SU.attach(3);
  SD.attach(11);
  SA.attach(10);

  // Posiciones Iniciales
  SU.write(90);
  SD.write(130);
  SA.write(70);

  delay(1000);

  Serial.println("Iniciando");
}

void loop() {
  estadoBoton = digitalRead(botonPin); 

  if (estadoBoton == LOW) {
    Serial.println("Botón presionado...");
    iniciarProgramacion();
  }

  delay(100); 
}

void iniciarProgramacion() {
  SU.write(90);

  digitalWrite(IN3, LOW);
  digitalWrite(IN4, HIGH);

  dF = lectura_ultrasonico(trig, echo);

  if (direccion == 0) {
    dF = lectura_ultrasonico(trig, echo);
    if (dF <= 40) {
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

      digitalWrite(IN3, HIGH);
      digitalWrite(IN4, LOW);

      delay(1800);

      if (dI >= dD) {
        Serial.print("Girando a la Izquierda \n");
        SD.write(70);
        SA.write(170);

        digitalWrite(IN3, LOW);
        digitalWrite(IN4, HIGH);

        direccion = -1;

        delay(TiempoDeGiro);

        SD.write(130);

      } else if (dI <= dD) {
        Serial.print("Girando a la Derecha \n");
        SD.write(170);
        SA.write(0);

        digitalWrite(IN3, LOW);
        digitalWrite(IN4, HIGH);

        direccion = 1;

        delay(TiempoDeGiro);

        SD.write(130);

      } else {
        Serial.print("Recto \n ");

        digitalWrite(IN3, HIGH);
        digitalWrite(IN4, LOW);

        delay(1000);
      }
    }
  } else if (direccion == 1) {
    dF = lectura_ultrasonico(trig, echo);
    if (dF <= Frente) {
      Serial.println("Dentro de Loop Derecha");

      digitalWrite(IN3, LOW);
      digitalWrite(IN4, LOW);

      delay(500);

      digitalWrite(IN3, HIGH);
      digitalWrite(IN4, LOW);

      delay(1500);

      Serial.print("Girando a la Derecha \n");
      SD.write(170);

      digitalWrite(IN3, LOW);
      digitalWrite(IN4, HIGH);

      delay(TiempoDeGiro);

      SD.write(130);
    }

    dA = lectura_ultrasonico(trig2, echo2);

    if (dA >= 20) {
      SD.write(160);

      delay(500);

      SD.write(130);

    } else if (dA <= 20) {
      SD.write(100);

      delay(500);

      SD.write(130);
    }

  } else if (direccion == -1) {
    dF = lectura_ultrasonico(trig, echo);
    if (dF <= Frente) {
      Serial.println("Dentro de Loop Izquierda");

      digitalWrite(IN3, LOW);
      digitalWrite(IN4, LOW);

      delay(500);

      digitalWrite(IN3, HIGH);
      digitalWrite(IN4, LOW);

      delay(1800);

      Serial.print("Girando a la Izquierda \n");
      SD.write(30);

      digitalWrite(IN3, LOW);
      digitalWrite(IN4, HIGH);

      delay(3000);

      SD.write(130);
    }

    dA = lectura_ultrasonico(trig2, echo2);

    if (dA >= 30) {
      SD.write(160);

      delay(200);

    } else if (dA <= 20) {
      SD.write(100);

      delay(200);
    }

    SD.write(130);
    Serial.println(dA);

  } else {
    dF = lectura_ultrasonico(trig, echo);
    if (dF <= Frente) {
      direccion = 0;
    }
  }
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