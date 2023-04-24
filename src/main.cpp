#include <Arduino.h>
#include <Adafruit_Sensor.h>
#include "DHT.h"

#define DHTTYPE DHT11

#define pin_cocina 0
#define pin_dormitorio 2
#define pin_living 8

#define controlador 3

//variables globales
double error = 0;               // diferencis de temp entre la actual y la requerida
double error_ultimo = 0;        // ultimo error guardado
double error_acumulado = 0;     //valor del error acomulado
double kp = 3;
double ki = 5;
double H = 0;
double U = 0;
double ref = 24.90;             // valor temperatura requerida
long Tiempotranscurrido = 0;    // tiempo de muestreo en miliseg
unsigned long pasado = 0;       // tiempo pasado para asegurar el tiempo de muestreo
unsigned long ahora = 0;

DHT dht_cocina(pin_cocina, DHTTYPE);
DHT dht_dormitorio(pin_dormitorio, DHTTYPE);
DHT dht_living(pin_living, DHTTYPE);




/************************************************************************************************************/
double Cocina(double inp){

ahora = millis(); //tiempo actual

Tiempotranscurrido = (ahora - pasado);      //Tiempo actual menos el pasado
Serial.print("Tiempo transcurrido = ");
Serial.println(Tiempotranscurrido/1000);    // Tiempo transcurrido en segundos

error = inp -ref;                           // determina error
Serial.print("Error = ");                   // Error de temperatura con respecto a la requerida
if(error <=0){error= 0.00;}
Serial.println(error);

error_acumulado += (error * (Tiempotranscurrido/1000)); 
Serial.print("Error acumulado = ");
Serial.println(error_acumulado);

pasado = ahora;
if(error <= 0){
  U = 0.00;
}else{
 H = kp*error + ki*error_acumulado;  
 U = constrain(H,0, 255);
}
Serial.print("Variable PWM = ");
Serial.println(U);

error_ultimo = error; 
pasado = ahora; 

return U; //salida PWM
}






void setup()
{
  Serial.begin(9600); // Iniciar la comunicación serie
  dht_cocina.begin();
  dht_dormitorio.begin();
  dht_dormitorio.begin();
}

void loop()
{
  Serial.println("=============================================");
  double input = dht_cocina.readTemperature();
  Serial.print("Temperatura Cocina = ");
  Serial.println(input);
  //analogWrite(controlador,Cocina(input));
  Serial.println(Cocina(input));
  delay(4000);
}