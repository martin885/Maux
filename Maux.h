
#include <Arduino.h>
#include <Q2HX711.h>
#include <MPU6050.h>
#include <Wire.h>
#include <I2Cdev.h>
#include <Mouse.h>


#define hx711_data_pin  4
#define  hx711_clock_pin  5

class Maux{
 private:
MPU6050 mpu;
Q2HX711 hx711=Q2HX711(hx711_data_pin,hx711_clock_pin);
float valor_sensor_presion;
float valor_anterior_sensor_presion;
float tara;
int16_t ax, ay, az, gx, gy, gz;
int vx=0, vy=0;
unsigned long tiempo_envio=0;
long count=0;
bool clickeado=false;
bool doble_click = false;
bool click_derecho = false;
byte cantidad_mediciones_tara = 50;
int reduccion_sensado_presion = 223;
int accionamiento_doble_click = 30;
int accionamiento_click_derecho = 50;
int accionamiento_click = 5;
int amplificador_movimiento_x = 3;
int amplificador_movimiento_y = 3;
unsigned long umbral_tiempo_enviar = 1;
int promedio (int valor_movimiento, long contador);
void click();
void move();
int16_t offset_x(int16_t gx);
int16_t offset_y(int16_t gz);
unsigned long tiempo_sin_click;
unsigned long tiempo_clickeado_umbral=1900; 
unsigned long tiempo_arranque_click = 3000;
bool click_sostenido=false;
void(* resetFunc) (void) = 0;
public:
Maux();
void setup_maux();
void loop();
};
