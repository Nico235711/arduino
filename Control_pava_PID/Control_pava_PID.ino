/***************************************************************************************
 *   CONTROL PI DE UNA PAVA ELECTRICA   
 *   -----------------------------------
 *   
 *   Sistemas de Control Automático UNDAV - septiembre de 2022
 *      
 ***************************************************************************************/
 
#include <OneWire.h>
#include <DallasTemperature.h>
#include "pid2.h"

const int sensorPin = 2;            //Pin del sensor de temperatura
const int relePin = 7;            //Pin del Rele
const int ledIntegradoPin = 13;     // Pin del led que trae integrada la placa arduino
const int botonParadaPin = 9;       //Pin para boton parada emergencia
const float TempObj = 85;           // temperatura objetivo



unsigned long TiempoAhora = 0;      // tiempo en milisegundos
unsigned long TiempoInicial = 0;    // tiempo en que comienza la mprimer medición
float UltTempMedida0;
float UltTempMedida1;

//float Kp = 55; //Proporcional
//float Ti = 0; // Proporcional
float Kp = 40; //proporcional-integral
float Ti = 3000; // 3000 segundos son 50 minutos - proporcional-integral
float Td = 0; 
float SalidaMin= 0;
float SalidaMax= 2200;
float SalidaSaturada=0;
const bool limitarSalidaTrue = true; //
const bool CompensaIntegralTrue = true; //denota si se esta compensando la integral
controlPID PavaElectrica(Kp, Ti, Td);
float accionControl = 0;
float accionInt = 0;
float accionProp = 0;
float accionPropInt = 0;
float compensacion = 0;

const int periodoPWM =  5000; // 5 segundos
unsigned long TH; //TH es le tiempo de encendido en el que se manda potencia a la pava
unsigned long TL;

OneWire ourWire(sensorPin);          //Se establece el pin 2  como bus OneWire
DallasTemperature sensors(&ourWire); //Se declara una variable u objeto para nuestro sensor

void setup(){
	    pinMode(botonParadaPin, INPUT);
	    pinMode(ledIntegradoPin,OUTPUT);  
      pinMode(relePin,OUTPUT); 
      delay(1000);
      Serial.begin(9600);
      sensors.begin();                      // Se inicia los sensores
      //Se calcula salida saturada teniendo en cuenta el delay del sensor de temperatura
      SalidaSaturada = ( (float) (periodoPWM - 600) / periodoPWM ) * SalidaMax;
      //Limito la salida del actuador y seteo sus valores min y maximos
      PavaElectrica.LimitarSalida(limitarSalidaTrue, SalidaMin, SalidaSaturada);
      PavaElectrica.CompensarIntegral(CompensaIntegralTrue);
      muestroEncabezadoProyecto();
      TiempoInicial = millis();               // Este es el tiempo del primer muestreo
      TiempoAhora = TiempoInicial;
}

void loop(void) { 

     // *********************************************************
     // PASO 1 - MIDO
     // *********************************************************
     // Mido temperatura y actualizo variables 
     sensors.requestTemperatures();
     UltTempMedida0 = sensors.getTempCByIndex(0);  
     UltTempMedida1 = sensors.getTempCByIndex(1);

     // *********************************************************
     // PASO 2 - CONTROLAR PID
     // *********************************************************
     // Ajusta la tensión de la pava en base a la temperatura
    accionControl = PavaElectrica.Controlar(TempObj - UltTempMedida1); 
    accionInt = PavaElectrica.ObtenerIntegral();
    accionProp = PavaElectrica.ObtenerProporcional();
    compensacion = PavaElectrica.ObtenerCompensacion();



     // *********************************************************
     // PASO 3 - MUESTRO VALORES
     // *********************************************************
     // Imprimo valores Sensados:
     imprimrValoresSensados(TiempoAhora-TiempoInicial,UltTempMedida0,UltTempMedida1,accionControl,accionProp,accionInt,compensacion); 
     

    //Logica PWM
    TH = (unsigned long) (accionControl / SalidaMax * periodoPWM);
    TL = millis() + TH;


    digitalWrite (relePin, HIGH);


     // *********************************************************
     // PASO 4 - ESPERO ANTES DE LA PRÓXIMA MEDICIÓN
     // *********************************************************
     // delay de 5s con funcion millis()
    do{
        // si se presionó el boton de emergencia --> paro el sistema
        if (digitalRead(botonParadaPin))
        {
			    pararSistema();
	      }
        if(millis() >= TL){
          digitalWrite (relePin, LOW);
        }
        
     } while(millis() < TiempoAhora+periodoPWM);

    TiempoAhora += periodoPWM;
    digitalWrite (relePin, LOW);
}

void muestroEncabezadoProyecto() {
    // Encabezado del proyecto
      Serial.println(F("--------------------------------------------------------------"));
      Serial.println(F("   CONTROL DE TEMPERATURA DE PAVA ELECTRICA"));
      Serial.println(F("--------------------------------------------------------------"));

      Serial.print("Constante Kp: ");
      Serial.print(Kp);
      Serial.println(" W/ºC");
      Serial.print("Constante Ti: ");
      Serial.print(Ti);
      Serial.println(" ms");
      Serial.print("Periodo PWM: ");
      Serial.print(periodoPWM);
      Serial.println(" ms");
      Serial.println("");

      // Encabezado de la tabla
      Serial.print("Tiempo");
      Serial.print("\t");
      Serial.print("Amb");
      Serial.print("\t");
      Serial.print("Agua");
      Serial.print("\t");
      Serial.print("PI");
      Serial.print("\t");
      Serial.print("Prop");
      Serial.print("\t");
      Serial.print("Inte");
      Serial.print("\t");
      Serial.print("Comp");
      Serial.print("\t");
      Serial.println("");
  }

 void imprimrValoresSensados(unsigned long Tiempo,float  Temp1, float  Temp2, float accionControl, float accionProp, float accionInt, float compensacion){
    // Imprimo valores Sensados:
      Serial.print(Tiempo);
      Serial.print("\t");
      Serial.print(Temp1);
      Serial.print("\t");
      Serial.print(Temp2);
      Serial.print("\t");
      Serial.print(accionControl);
      Serial.print("\t");  
      Serial.print(accionProp);
      Serial.print("\t");    
      Serial.print(accionInt);
      Serial.print("\t");   
      Serial.print(compensacion);
      Serial.print("\t");   
      Serial.println("");
 }

void pararSistema()
{
            //Apagar pava
            PavaElectrica.Apagar();
            //Se apaga rele
            digitalWrite (relePin, LOW);
            
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


