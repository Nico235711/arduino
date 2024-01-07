/****************************************************************************************
* Objeto para control SI-NO
* 
* Autor:	UNDAV
* Fecha:	2021
* Version:	1.0
* Archivo:	controlSiNo.cpp
* Comentario:	Librería elaborada en Sistemas de Control Automático, UNDAV, 2021.
*
****************************************************************************************/

#include "Arduino.h"
#include "controlSiNo.h"

/****************************************************************************************
* Funciones públicas del objeto SiNo  
****************************************************************************************/

SiNo::SiNo(float PRENDE, float APAGA)
{
  PinSalida = 0;
  EstadoSalida = false;
  Prende = PRENDE;
  Apaga = APAGA;
  Pendiente = 1;  // Normalmente, el actuador hace que suba la ENTRADA
  if (Apaga<Prende) Pendiente = -1;  // Esto puede ocurrir, por ejemplo, 
                                     // cuando la entrada es la señal de error.
                                     // Otro ejemplo: potencia de heladera, 
                                     // que hace que baje la temperatura. 
}

/***************************************************************************************/

void SiNo::AgregarSalida(byte PIN)
{
  PinSalida = PIN; 
  if (PinSalida>0) pinMode(PinSalida, OUTPUT);
}

/***************************************************************************************/

boolean SiNo::Controlar(float ENTRADA)       // Controla, teniendo en cuenta ENTRADA y Estado previo.
{
  if (((ENTRADA-Prende)*Pendiente)<0) {
    // Debo prender... 
    EstadoSalida = true;
  }
  if (((ENTRADA-Apaga)*Pendiente)>0) {
    // Debo apagar... 
    EstadoSalida = false;
  }
  // Sólo actúo si tengo definido un pin de salida
  if (PinSalida>0) digitalWrite(PinSalida,EstadoSalida);
  return EstadoSalida;
}

/***************************************************************************************/

void SiNo::Apagar()
{
  EstadoSalida = false;
  if (PinSalida>0) digitalWrite(PinSalida,EstadoSalida);
}

/***************************************************************************************/

void SiNo::Prender()
{
  EstadoSalida = true;
  if (PinSalida>0) digitalWrite(PinSalida,EstadoSalida);
}

/***************************************************************************************/

boolean SiNo::Estado()
{
  return EstadoSalida;
}

//***************************************************************************************