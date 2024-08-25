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
#define posicioninicialservo 100

// Configuración del servo
Servo myServo;

// Variables para sensores ultrasónicos
long duration;
int distanciaF, distanciaIZ, distanciaDER;

// Variables de control
const int velocidad1 = 180;
const int velocidad0 = 0;
const int distanciaobs = 89; // Distancia en cm para considerar un obstáculo

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

  if (distanciaF < distanciaobs) {
    // Si hay un obstáculo en frente, retroceder
    retroceder();
    delay(30); // Tiempo para retroceder

    // Detener el coche antes de medir los espacios
    stop();
    delay(500); // Tiempo para detenerse

    // Re-evaluar distancias a la izquierda y derecha después del retroceso
    distanciaIZ = lectura_ultrasonico(Trig2, Echo2);
    distanciaDER = lectura_ultrasonico(Trig3, Echo3);

    // Imprimir las distancias después del retroceso
    Serial.print("Distancia Izquierda después del retroceso: ");
    Serial.print(distanciaIZ);
    Serial.print(" cm\t");

    Serial.print("Distancia Derecha después del retroceso: ");
    Serial.print(distanciaDER);
    Serial.println(" cm");

    if (distanciaIZ > distanciaDER) {
      // Si hay más espacio a la izquierda
      girar_IZ();
      delay(1000); // Tiempo para girar a la izquierda
    } else {
      // Si hay más espacio a la derecha
      girar_derecha();
      delay(1000); // Tiempo para girar a la derecha
    }

    
  } else {
 
    avanzar();
    if (distanciaIZ < 20) {
      acomodardirecionDER();
      delay(400);
    }
    if (distanciaDER < 20) {
      acomodardirecionIZ();
      delay(400);
    }
  }
  if (distanciaF < 10) {
    retroceder();
    delay(2000);
    if (distanciaIZ > distanciaDER) {
     
      girar_IZ();
      delay(1000); 
    } else {
    
      girar_derecha();
      delay(1000); 
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
  myServo.write(115);
  avanzar();
  delay(700); 
  myServo.write(115);
}

void acomodardirecionDER() {
  myServo.write(95);
  avanzar();
  delay(700); 
  myServo.write(115);
}

void girar_derecha() {
  // Aumentado para asegurar suficiente retroceso
  myServo.write(75);
  avanzar();
  delay(1400); // Aumentado para asegurar giro completo
  myServo.write(100);
}

void girar_IZ() {
   // Aumentado para asegurar suficiente retroceso
  myServo.write(125);
  avanzar();
  delay(1400); // Aumentado para asegurar giro completo
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
  int distance = duration * 0.0344 / 2;
  return distance;
}