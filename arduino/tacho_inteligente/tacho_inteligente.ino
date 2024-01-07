#include <Servo.h>

Servo myservo;  // create servo object to control a servo, twelve servo objects can be created on most boards

int pos = 0;              // variable to store the servo position

const int servo = 2;

const int triggerSensorServo = 4;   // pin digital 4 para el Trigger del sensor
const int echoSensorServo = 3;     // pin digital 3 para el Echo del sensor
const int triggerSensorLed = 5;   //Pin digital 5 para el Trigger del sensor
const int echoSensorLed = 6;     //Pin digital 6 para el Echo del sensor

const int ledVerde = 8;
const int ledRojo = 9;
const int ledAmarillo = 10;

long tiempoSensorLed; //tiempo que demora en llegar el echo
long distanciaSensorLedCM; //distancia en centimetros

long tiempoSensorServo;       // tiempo que demora en llegar el echo
long distanciaSensorServoCM; // distancia en centimetros

void setup() {
  Serial.begin(9600);        // inicializamos la comunicación
    
  ConfigurarSensorHCSR04();
  ConfigurarLed();
  
  myservo.attach(servo);  // attaches the servo on pinServo to the servo object
  myservo.write(0);
}

void loop() {
  digitalWrite(triggerSensorServo, HIGH);
  delayMicroseconds(10);          // enviamos un pulso de 10us
  digitalWrite(triggerSensorServo, LOW);

  digitalWrite(triggerSensorLed, HIGH);
  delayMicroseconds(10);          // enviamos un pulso de 10us
  digitalWrite(triggerSensorLed, LOW);
  
  tiempoSensorLed = pulseIn(echoSensorLed, HIGH); // obtenemos el ancho del pulso
  distanciaSensorLedCM = tiempoSensorLed/58.2;   // escalamos el tiempo a una distancia en cm

  tiempoSensorServo = pulseIn(echoSensorServo, HIGH); // obtenemos el ancho del pulso
  distanciaSensorServoCM = tiempoSensorServo/58.2;   // escalamos el tiempo a una distancia en cm

  if (distanciaSensorServoCM <= 10){
      for (pos = 0; pos <= 130; pos += 1) { // goes from 0 degrees to 130 degrees in steps of 1 degree
        myservo.write(pos);                // tell servo to go to position in variable 'pos'
        delay(15);                        // waits 15ms for the servo to reach the position
    }
    delay(2000);
    for (pos = 130; pos >= 0; pos -= 1) { // goes from 130 degrees to 0 degrees
      myservo.write(pos);                // tell servo to go to position in variable 'pos'
      delay(15);                        // waits 15ms for the servo to reach the position
    }
  }
  
  if (distanciaSensorLedCM > 11 && distanciaSensorLedCM <= 22){
    PrenderLedVerde();
  }
  else if (distanciaSensorLedCM > 5 && distanciaSensorLedCM <= 11){
    PrenderLedAmarillo();
  }
  else if (distanciaSensorLedCM <= 5){
    PrenderLedRojo(); 
  }
  
  Serial.print("Distancia del sensor del servo: ");
  Serial.print(distanciaSensorServoCM);      // Enviamos serialmente el valor de la distancia
  Serial.print("cm");
  Serial.println();
  delay(100);          // Hacemos una pausa de 100ms
}

void PrenderLedVerde(){
  digitalWrite(ledVerde, HIGH);
  digitalWrite(ledAmarillo, LOW);
  digitalWrite(ledRojo, LOW);
}

void PrenderLedAmarillo(){
  digitalWrite(ledVerde, LOW);
  digitalWrite(ledAmarillo, HIGH);
  digitalWrite(ledRojo, LOW);
}

void PrenderLedRojo(){
  digitalWrite(ledVerde, LOW);
  digitalWrite(ledAmarillo, LOW);
  digitalWrite(ledRojo, HIGH);
}

void ConfigurarSensorHCSR04(){
  pinMode(triggerSensorServo, OUTPUT); // pin como salida
  pinMode(echoSensorServo, INPUT);    // pin como entrada
  digitalWrite(triggerSensorServo, LOW); // inicializamos el pin con 0
  pinMode(tiempoSensorLed, OUTPUT); // pin como salida
  pinMode(echoSensorLed, INPUT);    // pin como entrada
  digitalWrite(tiempoSensorLed, LOW); // inicializamos el pin con 0
}

void ConfigurarLed(){
  pinMode(ledVerde, OUTPUT);
  pinMode(ledRojo, OUTPUT);
  pinMode(ledAmarillo, OUTPUT);
}
