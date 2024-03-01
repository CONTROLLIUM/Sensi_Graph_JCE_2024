/*
 Name:		Comunicacion_I2C_Maestro.ino
 Created:	17/02/2024 13:23:50
 Author:	Luis Mi CONTROLLIUM
*/
//SDA   2
//SCL   3
// the setup function runs once when you press reset or power the board

#include <LiquidCrystal.h>
#include <Wire.h>
float NTC, LDR,ntc,ldr = 0.0;
#define LDRpin A2
#define NTCpin A0
const float Beta_const = 4300.0;
const float Nominal_Temp = 25.0;
const float Nomimal_Res = 10000.0;
const float LuxCurve = 189.55;
const float R0 = 10000.0;//resistencia en la oscuridad
byte Celsius[8] = {
  B11000,
  B11000,
  B00000,
  B11111,
  B10000,
  B10000,
  B10000,
  B11111
};
byte Lumen[8] = {
  B10101,
  B01110,
  B11111,
  B01110,
  B10101,
  B00100,
  B00100,
  B00100
};
LiquidCrystal lcd(7, 9, 10, 16, 14, 15);
//LiquidCrystal lcd(rs, en, d4, d5, d6, d7);
void setup() 
{   
    Wire.begin();//iniciar la comunicacion i2c
    lcd.begin(16, 2);
    lcd.setCursor(0, 0);
    lcd.print("Temp:");
    lcd.setCursor(0, 1);
    lcd.print("Lum:");

    lcd.createChar(0, Celsius);
    lcd.createChar(1, Lumen);

    lcd.setCursor(15, 0); // Fila 0, columna 15
    lcd.write(byte(0));
    lcd.setCursor(15, 1); // Fila 1, columna 15
    lcd.write(byte(1));
}

// the loop function runs over and over again until power down or reset
void loop() 
{    
        
    ldr = analogRead(LDRpin);
    ntc = analogRead(NTCpin);

    float voltage = ntc * (5.0 / 1023.0);
    float resistance = (5.0 * Nomimal_Res / voltage) - Nomimal_Res;
    NTC = (1.0 / ((1.0 / (Nominal_Temp + 273.15)) + (1.0 / Beta_const) * log(resistance / Nomimal_Res)) - 273.15);
    
    float resist = (1023.0 / ldr - 1) * R0;  
    LDR = LuxCurve / resist;  

    //lo veo por lcd
    lcd.setCursor(5, 0);
    lcd.print(NTC);
    lcd.setCursor(4, 1);
    lcd.print(LDR);
    
    //envio 12c
    //formar un arreglo de bytes con los datos a enviar
    byte Load[10];
    memcpy(Load, &NTC, sizeof(float));
    memcpy(Load + sizeof(float), &LDR, sizeof(float));
    //enviar los datos al esclavo
    Wire.beginTransmission(8);//el 8 es la direccion
    Wire.write(Load, 10);
    Wire.endTransmission();
    NTC = 0.0;
    LDR = 0.0;
    delay(250);
}
