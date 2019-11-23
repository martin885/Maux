#include "Maux.h"

Maux::Maux()
{
}

void Maux::loop()
{
  click();
  move();
}

void Maux::click()
{

  valor_sensor_presion = valor_sensor_presion + hx711.read();

  valor_sensor_presion = (valor_sensor_presion - tara) / reduccion_sensado_presion;

  if (!click_sostenido)
  {

    if (valor_sensor_presion - valor_anterior_sensor_presion > accionamiento_click_derecho && click_derecho == false && millis() > tiempo_arranque_click )
    {

      Mouse.click(MOUSE_RIGHT);
      clickeado = true;
      click_derecho = true;
      valor_anterior_sensor_presion = valor_sensor_presion;
    }
    else if (valor_sensor_presion - valor_anterior_sensor_presion > accionamiento_doble_click && doble_click == false && millis() > tiempo_arranque_click )
    {

      Mouse.click();
      Mouse.click();
      clickeado = true;
      doble_click = true;
      valor_anterior_sensor_presion = valor_sensor_presion;
    }
    else if (valor_sensor_presion - valor_anterior_sensor_presion > accionamiento_click && clickeado == false && millis() > tiempo_arranque_click)
    {

      clickeado = true;
      Mouse.press();

      if (millis() - tiempo_sin_click > tiempo_clickeado_umbral)
      {
        Mouse.press();
        click_sostenido = true;
      }
    }
    else if (valor_sensor_presion - valor_anterior_sensor_presion < accionamiento_click && clickeado == true)
    {

      clickeado = false;
      if (doble_click == true || click_derecho == true)
      {
        doble_click = false;
        click_derecho = false;
      }

      Mouse.release();
    }
    if (clickeado == true)
    {
      valor_anterior_sensor_presion = valor_anterior_sensor_presion;
      Serial.print("valor clickeado true = ");
      Serial.println(valor_sensor_presion - valor_anterior_sensor_presion);
    }
    else
    {
      valor_anterior_sensor_presion = valor_sensor_presion;
      Serial.print("valor clickeado false = ");
      Serial.println(valor_sensor_presion - valor_anterior_sensor_presion);
      tiempo_sin_click = millis();
    }
  }
  else
  {
    Serial.print("click sostenido ");
    if (valor_sensor_presion - valor_anterior_sensor_presion > accionamiento_click)
    {
      Mouse.release();
      clickeado = false;
      click_sostenido = false;
    }

    if (millis() - tiempo_sin_click > 120000) {
      Serial.println("Reseteando...");
      resetFunc();
    }

    valor_anterior_sensor_presion = valor_sensor_presion;    
    Serial.println(valor_sensor_presion - valor_anterior_sensor_presion);
  }

  valor_sensor_presion = 0;
}

void Maux::move()
{
  mpu.getRotation(&gx, &gy, &gz);
  vx += -amplificador_movimiento_x * offset_x(gx);
  vy += amplificador_movimiento_y * offset_y(gz);
  count++;

  Serial.print(gx);
  Serial.print(',');
  Serial.println(gz);

  if (millis() - tiempo_envio > umbral_tiempo_enviar)
  {

    tiempo_envio = millis();
    Mouse.move(promedio(vx, count), promedio(vy, count));
    count = 0;
    vx = 0;
    vy = 0;
  }
}

int16_t Maux::offset_x(int16_t gx)
{
  return ((gx - 450) / 150);
}

int16_t Maux::offset_y(int16_t gz)
{
  return ((gz + 150) / 150);
}

int Maux::promedio(int valor_movimiento, long contador)
{
  return (valor_movimiento / contador);
}

void Maux::setup_maux()
{

  Serial.begin(115200);
  Wire.begin();
  mpu.initialize();
  if (!mpu.testConnection())
  {
    while (1);
  }

  for (byte y = 0; y < cantidad_mediciones_tara; y++)
  {
    valor_sensor_presion = valor_sensor_presion + hx711.read();
  }
  tara = valor_sensor_presion / cantidad_mediciones_tara;
  valor_anterior_sensor_presion = tara;
  valor_sensor_presion = -1000;
  Mouse.begin();
}
