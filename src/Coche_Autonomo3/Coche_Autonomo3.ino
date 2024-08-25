#include <Servo.h>
#include <Wire.h>

// M1 Izquierda
#define P1 14
#define P2 15
#define ENA 6

// M2 derecha
#define P3 16
#define P4 17
#define ENB 5

// Sensor Ultrasonico Frente
#define Trig1 4
#define Echo1 2

// Sensor Ultrasonico Izquierda
#define Trig2 8
#define Echo2 7

// Sensor Ultrasonico Derecha
#define Trig3 10
#define Echo3 9

#define servoPin 3
#define posicioninicialservo 90

// Configuración del servo
Servo myServo;

// Variables para sensores ultrasónicos
long duration;
int distanciaF, distanciaIZ, distanciaDER;

// Variables de control
const int velocidad1 = 180;
const int velocidad0 = 0;
const int distanciaobs = 80; // Distancia en cm para considerar un obstáculo
int contadorVueltas = 0;  // Contador de vueltas
int girosRealizados = 0;  // Contador de giros para completar una vuelta

void setup() {
  Serial.begin(9600);

  // Inicialización de motores
  pinMode(P1, OUTPUT);
  pinMode(P2, OUTPUT);
  pinMode(ENA, OUTPUT);
  pinMode(P3, OUTPUT);
  pinMode(P4, OUTPUT);
  pinMode(ENB, OUTPUT);

  // Inicialización de sensores ultrasónicos
  pinMode(Trig1, OUTPUT);
  pinMode(Echo1, INPUT);
  pinMode(Trig2, OUTPUT);
  pinMode(Echo2, INPUT);
  pinMode(Trig3, OUTPUT);
  pinMode(Echo3, INPUT);

  // Configuración inicial del servo
  myServo.attach(servoPin);
  myServo.write(posicioninicialservo);

  Serial.println("¡Robot iniciado!");
}

void loop() {
  // Leer sensores ultrasónicos
  distanciaF = lectura_ultrasonico(Trig1, Echo1);
  distanciaIZ = lectura_ultrasonico(Trig2, Echo2);
  distanciaDER = lectura_ultrasonico(Trig3, Echo3);

  // Imprimir las distancias de los sensores
  Serial.print("Distancia Frente: ");
  Serial.print(distanciaF);
  Serial.print(" cm\t");

  Serial.print("Distancia Izquierda: ");
  Serial.print(distanciaIZ);
  Serial.print(" cm\t");

  Serial.print("Distancia Derecha: ");
  Serial.print(distanciaDER);
  Serial.println(" cm");

  if (contadorVueltas >= 3) {
    stop();  // Detener el coche después de 3 vueltas completas
    Serial.println("¡Tres vueltas completas realizadas!");
    while(1);  // Detener el programa
  }

  if (distanciaF < distanciaobs) {
    retroceder();
    delay(500); 
    stop();
    
    distanciaIZ = lectura_ultrasonico(Trig2, Echo2);
    distanciaDER = lectura_ultrasonico(Trig3, Echo3);

    if (distanciaIZ > distanciaDER) {
      girar_IZ();
    } else {
      girar_derecha();
    }

    girosRealizados++;  // Incrementar el contador de giros

    if (girosRealizados >= 4) {
      girosRealizados = 0;  // Reiniciar contador de giros
      contadorVueltas++;  // Incrementar el contador de vueltas después de 4 giros
    }

  } else {
    avanzar();

    if (distanciaIZ < 35) {
      acomodardirecionDER();
    } else if (distanciaDER < 35) {
      acomodardirecionIZ();
    }
  }

  if (distanciaF < 20) {
    retroceder();
    delay(1150);
    if (distanciaIZ > distanciaDER) {
      girar_IZ();
    } else {
      girar_derecha();
    }

    girosRealizados++;  // Incrementar el contador de giros

    if (girosRealizados >= 4) {
      girosRealizados = 0;  // Reiniciar contador de giros
      contadorVueltas++;  // Incrementar el contador de vueltas después de 4 giros
    }
  }

  adjustDireccion();

  delay(100);
}

// Funciones auxiliares
void avanzar() {
  digitalWrite(P1, LOW);
  digitalWrite(P2, HIGH);
  analogWrite(ENA, velocidad1);
  digitalWrite(P3, LOW);
  digitalWrite(P4, HIGH);
  analogWrite(ENB, velocidad1);
}

void retroceder() {
  digitalWrite(P1, HIGH);
  digitalWrite(P2, LOW);
  analogWrite(ENA, velocidad1);
  digitalWrite(P3, HIGH);
  digitalWrite(P4, LOW);
  analogWrite(ENB, velocidad1);
}

void stop() {
  digitalWrite(P1, LOW);
  digitalWrite(P2, LOW);
  analogWrite(ENA, velocidad0);
  digitalWrite(P3, LOW);
  digitalWrite(P4, LOW);
  analogWrite(ENB, velocidad0);
}

void acomodardirecionIZ() {
  myServo.write(110);
  delay(500);
  myServo.write(100);
}

void acomodardirecionDER() {
  myServo.write(90);
  delay(500);
  myServo.write(100);
}

void girar_derecha() {
  myServo.write(80);
  avanzar();
  delay(1700);
  myServo.write(100);
}

void girar_IZ() {
  myServo.write(120);
  avanzar();
  delay(1700);
  myServo.write(100);
}

void adjustDireccion() {
  myServo.write(posicioninicialservo);
}

int lectura_ultrasonico(int trigPin, int echoPin) {
  digitalWrite(trigPin, LOW);
  delayMicroseconds(2);
  digitalWrite(trigPin, HIGH);
  delayMicroseconds(10);
  digitalWrite(trigPin, LOW);
  duration = pulseIn(echoPin, HIGH);
  int distance = (duration / 2) / 29.1;
  return distance;
}