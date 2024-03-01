/*
 Name:		Programa_de_jornada.ino
 Created:	17/02/2024 12:05:47
 Author:	Luis Mi CONTROLLIUM
*/

// the setup function runs once when you press reset or power the board
#include <Wire.h>
#include <BluetoothSerial.h>
#include <BTAddress.h>
#include <BTAdvertisedDevice.h>
#include <BTScan.h>
// SDA  21
// SCl  22
TaskHandle_t loops;
BluetoothSerial SerialBT;
float NTC=0;
float LDR=0;
String serialData;
int CantBytes;
char datoCmd;
#define Actuador  4

void ReceiveEvent(int CantBytes){

    if (CantBytes == 10)
    {
        byte Load[10];
        Wire.readBytes(Load, CantBytes);

        // leer los datos de temperatura y humead de el arreglo de bytes recibido
        memcpy(&NTC, Load, sizeof(float));
        memcpy(&LDR, Load + sizeof(float), sizeof(float));

    }

}


void setup()
{
    Serial.begin(9600);
    SerialBT.begin("SensiGraph");
    Wire.begin(8);// poner un valor en el begin, implica slave, el # tiene que ser el mismo que el de trasmision
    Wire.onReceive(ReceiveEvent); // funcion para recibir datos
    xTaskCreatePinnedToCore(loop2, "loop_2", 1000, NULL, 2, &loops, 0);
    pinMode(Actuador, OUTPUT);
                 
}

void loop() {};

void loop2(void* parameter) // aqui estara algo inalambrico si es que lo pongo
{
    for (;;)
    {   
        
        datoCmd = (char)Serial.read();

        if (datoCmd == 'D')
        {
            while (Serial.available())
            {
                datoCmd = (char)Serial.read();
                serialData += datoCmd;
                int dato = serialData.toInt();
                digitalWrite(Actuador, dato);
            }
            serialData = "";
        }

        Serial.println("$" + String(NTC * 1.0) + ";" + String(LDR * 1.0)); //$353.0;353.4

        String DtaNTC = String(NTC);
        String DtaLDR = String(LDR);

        SerialBT.print("NTC:");
        SerialBT.print(DtaNTC);
        SerialBT.print("----");
        SerialBT.print("LDR:");
        SerialBT.println(DtaLDR);

        delay(150);
        
    }
}

