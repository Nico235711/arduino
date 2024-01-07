/***************************************************************************************
 *   CONTROL SI-NO DE UNA PAVA ELECTRICA   
 *   -----------------------------------
 *   
 *   Sistemas de Control Automático UNDAV - septiembre de 2022
 *      
 ***************************************************************************************/
 
#include <OneWire.h>
#include <DallasTemperature.h>
#include "controlSiNo.h"

const int sensorPin = 2;            //Pin del sensor de temperatura
const int relePin = 10;            //Pin del Rele
const int ledIntegradoPin = 13;     // Pin del led que trae integrada la placa arduino
const int botonParadaPin = 9;       //Pin para boton parada emergencia
const float TempObj = 85;           // temperatura objetivo
const float DeltaTempObj = 2;       // delta de Temperatura Objetivo
const int periodo = 2000;           // tiempo entre mediciones, en milisegundos

unsigned long TiempoAhora = 0;      // tiempo en milisegundos
unsigned long TiempoInicial = 0;    // tiempo en que comienza la mprimer medición
float UltTempMedida0;
float UltTempMedida1;
const float TempMin = TempObj-DeltaTempObj;                        
const float TempMax = TempObj+DeltaTempObj;
SiNo PavaElectrica(TempMin, TempMax);

OneWire ourWire(sensorPin);          //Se establece el pin 2  como bus OneWire
DallasTemperature sensors(&ourWire); //Se declara una variable u objeto para nuestro sensor

void setup(){
	    pinMode(botonParadaPin, INPUT);
	    pinMode(ledIntegradoPin,OUTPUT);  
      delay(1000);
      Serial.begin(9600);
      sensors.begin();                      // Se inicia los sensores
      PavaElectrica.AgregarSalida(relePin); // seteo pin de rele como salida
      muestroEncabezadoProyecto();
      TiempoInicial=millis();               // Este es el tiempo del primer muestreo
}

void loop(void) { 

     // *********************************************************
     // PASO 1 - MIDO
     // *********************************************************
     // Mido temperatura y actualizo variables 
     TiempoAhora = millis();
     sensors.requestTemperatures();
     UltTempMedida0 = sensors.getTempCByIndex(0);  
     UltTempMedida1 = sensors.getTempCByIndex(1);

     // *********************************************************
     // PASO 2 - DECIDO CONTROL SI-NO
     // *********************************************************
    PavaElectrica.Controlar(UltTempMedida1);      

     // *********************************************************
     // PASO 3 - MUESTRO VALORES
     // *********************************************************
     // Imprimo valores Sensados:
     imprimrValoresSensados(TiempoAhora-TiempoInicial,UltTempMedida0,UltTempMedida1,PavaElectrica.Estado());
     
     // *********************************************************
     // PASO 4 - ESPERO ANTES DE LA PRÓXIMA MEDICIÓN
     // *********************************************************
     // delay de 2s con funcion millis()
    do{
        // si se presionó el boton de emergencia --> paro el sistema
        if (digitalRead(botonParadaPin))
        {
			    pararSistema();
	      }
     } while(millis() < TiempoAhora+periodo);
    
}

void muestroEncabezadoProyecto() {
    // Encabezado del proyecto
      Serial.println(F("--------------------------------------------------------------"));
      Serial.println(F("   CONTROL DE TEMPERATURA DE PAVA ELECTRICA"));
      Serial.println(F("--------------------------------------------------------------"));
      Serial.print("Mantiene la T° del agua por arriba de ");
      Serial.print(TempMin);
      Serial.println(" ºC");
      Serial.print("Calienta como maximo la T° del Agua a ");
      Serial.print(TempMax);
      Serial.println(" ºC");
      Serial.println("");
      
      // Encabezado de la tabla
      Serial.print("Tiempo");
      Serial.print("\t");
      Serial.print("Amb");
      Serial.print("\t");
      Serial.print("Agua");
      Serial.print("\t");
      Serial.print("Estado Control [0/1]");
      Serial.print("\t");
      Serial.println("");
  }

 void imprimrValoresSensados(unsigned long Tiempo,float  Temp1, float  Temp2, bool Estado){
    // Imprimo valores Sensados:
      Serial.print(Tiempo);
      Serial.print("\t");
      Serial.print(Temp1);
      Serial.print("\t");
      Serial.print(Temp2);
      Serial.print("\t");
      Serial.print(Estado);
      Serial.print("\t");
      Serial.println("");
 }

void pararSistema()
{
            //Apagar pava
            PavaElectrica.Apagar();
            //Se comunica parada de emergencia
            //Encendiendo y apagando el led del arduino
            //infinitamente
            Serial.print("PARADA DE EMERGENCIA");
            do
            {
				        digitalWrite(ledIntegradoPin, HIGH);
                delay(1000);
                digitalWrite(ledIntegradoPin, LOW);
                delay(500);
            } while (true);
}   


