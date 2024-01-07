/****************************************************************************************
* Objeto para control SI-NO
* 
* Autor:	UNDAV
* Fecha:	2021
* Version:	1.0
* Archivo:	controlSiNo.h
* Comentario:	Librería elaborada en Sistemas de Control Automático, UNDAV, 2021.
*
****************************************************************************************/

#ifndef CONTROL_SINO_H
#define CONTROL_SINO_H
#include "Arduino.h"

/***************************************************************************************/

class SiNo                           // Objeto para control SI-NO.
{
  private:
    boolean EstadoSalida;            // Estado de salida del control SI-NO.
    byte PinSalida;                  // Pin de salida. Si es 0, no actúa (se debe actuar de otra forma).
    float Prende;                    // Nuestro objetivo es que la ENTRADA esté entre Prende y Apaga.
    float Apaga;
    int Pendiente;                   // +1 si (APAGA>PRENDE): quiere decir que el actuador hará que SUBA la ENTRADA
                                     // -1 si (APAGA<PRENDE): quiere decir que el actuador hará que BAJE la ENTRADA
  public:
    SiNo(float PRENDE, float APAGA); // Constructor, incluye la definición básica del control SI-NO.
    boolean Controlar(float ENTRADA);  // Controla, teniendo en cuenta ENTRADA y Estado previo.
    void AgregarSalida(byte PIN);    // Configura un pin de salida para actuar directamente 
                                     // ante cada llamada de la accion Controlar(...)
    void Apagar();                   // Apaga el pin y pone Estado en false.
                                     // Puede volver a encenderse con Controlar en cualquier momento.
    void Prender();                  // Prende el pin y pone Estado en true.
    boolean Estado();                // Devuelve el Estado de salida.
};

/***************************************************************************************/

#endif

