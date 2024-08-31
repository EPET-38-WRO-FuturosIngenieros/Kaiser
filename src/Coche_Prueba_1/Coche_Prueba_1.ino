#include <Servo.h>

Servo servoMotor; // Servo motor

Servo servoDire; 
const int trigPin = 10;
const int echoPin = 12;
const int servoPin = 9;

// Conexiones del L298N
const int IN1 = 4;
const int IN2 = 5;
const int IN3 = 6;
const int IN4 = 7;

long duracion;
int distancia;
unsigned long tiempoUltimoAcomodo = 0; // Tiempo del último acomodo
unsigned long intervaloAcomodo = 2800; // Intervalo inicial de 2.7 segundos

int contadorGiros = 0; // Contador de giros
int contadorVueltas = 0; // Contador de vueltas

void setup() {

  
  // Configuración de pines del motor
  pinMode(IN1, OUTPUT);
  pinMode(IN2, OUTPUT);
  pinMode(IN3, OUTPUT);
  pinMode(IN4, OUTPUT);
  
  // Configuración de pines del sensor ultrasónico
  pinMode(trigPin, OUTPUT);
  pinMode(echoPin, INPUT);



  // Conectar el servo al pin especificado
  servoMotor.attach(servoPin);
  servoDire.attach(3);
  servoDire.write(45);
  servoMotor.write(90);
  // Iniciar comunicación serial para depuración
  Serial.begin(9600);
}

void loop() {
  avanzar();
  distancia = obtenerDistancia();
  Serial.println(distancia);

  if (distancia < 60){
    detenerCoche();
    servoMotor.write(0);
    delay(500);
    int distanciaIzquierda = obtenerDistancia();
    
    // Revisar lado derecho
    servoMotor.write(180);
    delay(500);
    int distanciaDerecha = obtenerDistancia();
    servoMotor.write(90);
    
    // Decidir hacia qué lado girar
    if (distanciaDerecha > distanciaIzquierda) {
      girarDerecha();
    } else {
      girarIzquierda();
    }
  }
 
}
void avanzar(){
  digitalWrite(IN3, HIGH);
  digitalWrite(IN4, LOW);

}

void detenerCoche() {
  digitalWrite(IN1, LOW);
  digitalWrite(IN2, LOW);
  digitalWrite(IN3, LOW);
  digitalWrite(IN4, LOW);
}

void girarDerecha() {
  servoDire.write(0);
   avanzar();
  delay(1000);
  servoDire.write(45);
  
}

void girarIzquierda() {
  servoDire.write(90);
  avanzar();
  delay(1000);
  servoDire.write(45);
  
}

int obtenerDistancia() {
  digitalWrite(trigPin, LOW);
  delayMicroseconds(2);
  
  digitalWrite(trigPin, HIGH);
  delayMicroseconds(10);
  digitalWrite(trigPin, LOW);

  duracion = pulseIn(echoPin, HIGH);
  
  // Calcular la distancia
  int distancia = duracion * 0.034 / 2;
  
  return distancia;
}
