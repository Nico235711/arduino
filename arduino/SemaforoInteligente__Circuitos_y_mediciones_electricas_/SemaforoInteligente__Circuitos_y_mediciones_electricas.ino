#include <NewPing.h>

#define Pulsador1 2 // El pin 2 corresponde al pulsador 1
#define Pulsador2 3 // El pin 3 corresponde al pulsador 2

#define Trigger1 4 // Pongo el trigger del 1er. sensor de ultrasonido HC-SR04 en el pin 4
#define Echo1 5 // Pongo el echo del 1er. sensor de ultrasonido HC-SR04 en el pin 3

#define Trigger2 6  // Pongo el trigger del 2do. sensor de ultrasonido HC-SR04 en el pin 6
#define Echo2 7 // Pongo el echo del 2do. sensor de ultrasonido HC-SR04 en el pin 7

#define LedRojo1 8 
#define LedAmarillo1 9 
#define LedVerde1 10 // Los pines [8, 9, 10] corresponden al 1er. semaforo

#define LedRojo2 11 
#define LedAmarillo2 12 
#define LedVerde2 13 // Los pines [11, 12, 13] corresponde al 2do. semaforo

const float sonido = 34300.0; // Velocidad del sonido en cm/s  

void setup() {
  pinMode(Pulsador1, INPUT); // Pongo el pin 2 como entrada
  pinMode(Pulsador2, INPUT); // Pongo el pin 3 como entrada

  pinMode(Trigger1, OUTPUT); // Pongo el pin 4 como salida
  pinMode(Echo1, INPUT); // Pongo el pin 5 como entrada
  
  pinMode(Trigger2, OUTPUT); // Pongo el pin 6 como salida
  pinMode(Echo2, INPUT); // Pongo el pin 7 como entrada
  
  pinMode(LedRojo1, OUTPUT); // Pongo el pin 8 como salida
  pinMode(LedAmarillo1, OUTPUT); // Pongo el pin 9 como salida
  pinMode(LedVerde1, OUTPUT); // Pongo el pin 10 como salida
  
  pinMode(LedRojo2, OUTPUT); // Pongo el pin 11 como salida
  pinMode(LedAmarillo2, OUTPUT); // Pongo el pin 12 como salida
  pinMode(LedVerde2, OUTPUT); // Pongo el pin 13 como salida
}

void loop() {
  iniciarTrigger(Trigger1); // Preparo el 1er. sensor de ultrasonido
  iniciarTrigger(Trigger2); // Preparo el 2do. sensor de ultrasonido

 float distancia1 = sonar.ping_cm() // calcularDistancia(Echo1); // Paso por parametro el echo1
 float distancia2 = sonar.ping_cm() // Uso la funcion para calcular la distancia en centrimetros con la libreria NewPing.h // calcularDistancia(Echo2); // Paso por parametro el echo2

 if (distancia1 <= distancia2){
  prenderLeds(LedRojo1, LedAmarillo1, LedVerde1);
 }
 else if (distancia2 <= distancia1){
  prenderLeds(LedRojo2, LedAmarillo2, LedVerde2);
 }

 if (digitalRead(Pulsador1) == HIGH && digitalRead(Pulsador2) == HIGH){ // Si ambos pulsadores estan apretados, le da prioridad a la de la derecha (Pulsador2)
  prenderLedRojo(LedRojo2, LedAmarillo2, LedVerd2);
 }
  
 if (digitalRead(Pulsador1) == HIGH){
  prenderLedRojo(LedRojo1, LedAmarillo1, LedVerde1);
 }
 else if (digitalRead(Pulsador2) == HIGH){
  prenderLedRojo(LedRojo2, LedAmarillo2, LedVerde2);
 }
}

void iniciarTrigger(int trigger){
  digitalWrite(trigger, LOW); // Pongo el trigger en estado bajo (0)
  delayMicroseconds(2); // Retraso de 2 miro-segundos

  digitalWrite(trigger, HIGH); // Pongo el trigger en estado alto (1)
  delayMicroseconds(10); // Retraso de 10 miro-segundos

  digitalWrite(trigger, LOW);
}

//float calcularDistancia(int echo){
//  unsigned long tiempo = pulseIn(echo, HIGH); // pulseIn obtiene el tiempo que tarda en cambiar de estado, en este caso a HIGH
//  
//  float distancia = tiempo * 0.000001 * sonido / 2.0; // Obtengo la distancia en cm, convierto @tiempo en segundos ya que esta en us, para eso lo multiplico por 0.000001
//  return distancia;
//}

void prenderLeds(int led1, int led2, int led3){
  digitalWrite(led1, HIGH); // Prendo el led1
  digitalWrite(led2, LOW); 
  digitalWrite(led3, LOW); // Apago los led 2 (Amarillo) y 3 (Verde)
  delay(4000); // Demora de 4s

  digitalWrite(led1, LOW); // Apago el led1 (Rojo)
  digitalWrite(led2, HIGH); // Prendo el led 2 
  delay(1500); // Demora de 1.5s

  digitalWrite(led2, LOW); // Apago el led 2 
  digitalWrite(led3, HIGH); // Prendo el led 3 
  delay(3000); // Demora de 3s
}

void prenderLedRojo(int led1, int led2, int led3){
  digitalWrite(led1, HIGH); // Prendo el led1
  digitalWrite(led2, LOW); 
  digitalWrite(led3, LOW); // Apago los led 2 y 3
}
