/*============================================================================
 * Autor:
 * Licencia:
 * Fecha:
 *===========================================================================*/
 
// Inclusiones
#include "app.h" // <= Su propia cabecera
#include "sapi.h" // <= Biblioteca sAPI

/* con #define "defino" una variable llamada SalidaGPIO y le asigno la funcion 
GPIO_OUTPUT, asi en la linea 30 - 33 si bien repito la variable
SalidaGPIO 4 veces, la funcion GPIO_OUTPUT la repito una vez */
#define SALIDAGPIO GPIO_OUTPUT
const int distanciaLimite = 15; 
delay_t delayLed; 

// FUNCION PRINCIPAL, PUNTO DE ENTRADA AL PROGRAMA LUEGO DE ENCENDIDO O RESET.
int main( void ) { 
     // ---------- CONFIGURACIONES ------------------------------
     // Inicializar y configurar la plataforma
     boardConfig(); // Empezamos a configurar la placa
     uartConfig( UART_USB, 115200 ); // Inicializar periferico UART_USB
     delayConfig(&delayLed, 500); 
     // Inicializar el sensor ultrasonico HC-SR04
     ultrasonicSensorConfig( ULTRASONIC_SENSOR_0, ULTRASONIC_SENSOR_ENABLE ); 
     
     //configurar PUENTE H */
     gpioConfig(GPIO1, SALIDAGPIO); 
     gpioConfig(GPIO4, SALIDAGPIO); 
     gpioConfig(GPIO5, SALIDAGPIO); 
     gpioConfig(GPIO7, SALIDAGPIO); 
     delay(100); // Retardo inicial de 100 ms
 
     // Variables enteras de 32 bits para guardar la distancia en pulgadas
     // y centimetros
     uint32_t distanceInCms; 

         
    // ---------- REPETIR POR SIEMPRE --------------------------
     while( TRUE ) { 
         if(delayRead(&delayLed)){ 
         gpioToggle(LEDR); 
     } 
     // Obtenemos la distancia actual medida por el sensor en centimetros
     distanceInCms = ultrasonicSensorGetDistance(ULTRASONIC_SENSOR_0, CM); 
     
     /*gpioWrite(GPIO4, false); //IN 2
     gpioWrite(GPIO1, true); //IN 1
     gpioWrite(GPIO5, true); //IN 3
     gpioWrite(GPIO7, false); //IN 4 */
     if (distanceInCms > distanciaLimite){ 
     // Avanzan las 4 ruedas si distanceInCms (cm) es mayor distanciaLimite (15)
         gpioWrite(GPIO4, false); //IN 2
         gpioWrite(GPIO1, true); //IN 1 - Rueda 1
         gpioWrite(GPIO5, true); //IN 3
         gpioWrite(GPIO7, false); //IN 4 - Rueda 2
     } 
     else{ 
         // Avanzan dos ruedas y dos retroceden
         gpioWrite(GPIO4, false); //IN 2
         gpioWrite(GPIO1, true); //IN 1 - Rueda 1
         gpioWrite(GPIO5, false); //IN 3
         gpioWrite(GPIO7, true); //IN 4 - Rueda 2
     } 
     
     // Reportamos las distancias medidas en centímetros y pulgadas
     printf( "Distance: %d cm\".\r\n", distanceInCms); 
     
     } 
     // NO DEBE LLEGAR NUNCA AQUI, debido a que a este programa se ejecuta
     // directamente no sobre un microcontroladore y no es llamado por ningún
     // Sistema Operativo, como en el caso de un programa para PC.
     return 0; 
}