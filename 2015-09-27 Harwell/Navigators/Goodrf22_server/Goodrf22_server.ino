// rf22_server.pde
// -*- mode: C++ -*-
// Example sketch showing how to create a simple messageing server
// with the RH_RF22 class. RH_RF22 class does not provide for addressing or
// reliability, so you should only use RH_RF22  if you do not need the higher
// level messaging abilities.
// It is designed to work with the other example rf22_client
// Tested on Duemilanove, Uno with Sparkfun RFM22 wireless shield
// Tested on Flymaple with sparkfun RFM22 wireless shield
// Tested on ChiKit Uno32 with sparkfun RFM22 wireless shield

#include <SPI.h>
#include <Wire.h>
#include "SoftwareSerial.h"
#include "SD.h"
#include <RH_RF22.h>
#include "Cube.h" 

Cube ubo;

// Singleton instance of the radio driver
// RH_RF22 rf22;
RH_RF22 rf22(10,7,hardware_spi);

void setup() 
{
  ubo.initialise(0);
//  Serial.begin(9600);
//  while(!Serial){};
  if (!rf22.init())
    Serial.println("init failed");
  rf22.setFrequency(437.0);
  // Defaults after init are 434.0MHz, 0.05MHz AFC pull-in, modulation FSK_Rb2_4Fd36
}

void loop()
{
  if (rf22.available())
  {
    // Should be a message for us now   
    uint8_t buf[RH_RF22_MAX_MESSAGE_LEN];
    uint8_t len = sizeof(buf);
    if (rf22.recv(buf, &len))
    {
      Serial.println((char*)buf);
      if(buf[0]=='1')
      {
        Serial.println("got request: Temperature");
        // Send a reply
        // uint8_t data[] = "And hello back to you";
//        Serial.print("Temperature: "); Serial.print(ubo.getTemperature()); Serial.print(" Celsius \n");
        uint8_t data[20];
        //sprintf((char*)data, "%f", ubo.getTemperature());
        dtostrf(ubo.getTemperature(), 5, 2, (char*)data);
//        Serial.println((char*)data);
        rf22.send(data, sizeof(data));
        rf22.waitPacketSent();
//        Serial.println("Sent a reply");
        
      }
      else if(buf[0]=='2')
      {
        Serial.println("got request: Light");
        // Send a reply
//        Serial.print("UV Index: "); Serial.print(ubo.getUV()); Serial.print("\n");
//        Serial.print("IR: "); Serial.print(ubo.getIR()); Serial.print("\n");
//        Serial.print("Light: "); Serial.print(ubo.getLight()); Serial.print(" Lux \n");
        uint8_t dataUV[20];
        uint8_t dataIR[20];
        uint8_t dataLight[20];
        dtostrf(ubo.getUV(), 5, 2, (char*)dataUV);
        dtostrf(ubo.getIR(), 5, 2, (char*)dataIR);
        dtostrf(ubo.getLight(), 5, 2, (char*)dataLight);
        rf22.send(dataUV, sizeof(dataUV));
        rf22.waitPacketSent();
//        Serial.println("Sent UV Data");
        rf22.send(dataIR, sizeof(dataIR));
        rf22.waitPacketSent();
//        Serial.println("Sent IR Data");
        rf22.send(dataLight, sizeof(dataLight));
        rf22.waitPacketSent();
//        Serial.println("Sent LUX Data");
      }
      else if(buf[0]=='3')
      {
//        Serial.println("got request: Accel");
        // Send a reply
//        Serial.print("Accel X: "); Serial.print(ubo.getaccelx()); Serial.print(" m/s^2 \n");
//        Serial.print("Accel Y: "); Serial.print(ubo.getaccely()); Serial.print(" m/s^2 \n");
//        Serial.print("Accel Z: "); Serial.print(ubo.getaccelz()); Serial.print(" m/s^2 \n"); 
        uint8_t dataAX[20];
        uint8_t dataAY[20];
        uint8_t dataAZ[20];
        dtostrf(ubo.getaccelx(), 5, 2, (char*)dataAX);
        dtostrf(ubo.getaccely(), 5, 2, (char*)dataAY);
        dtostrf(ubo.getaccelz(), 5, 2, (char*)dataAZ);
        rf22.send(dataAX, sizeof(dataAX));
        rf22.waitPacketSent();
//        Serial.println("Sent AX Data");
        rf22.send(dataAY, sizeof(dataAY));
        rf22.waitPacketSent();
//        Serial.println("Sent AY Data");
        rf22.send(dataAZ, sizeof(dataAZ));
        rf22.waitPacketSent();
//        Serial.println("Sent AZ Data");
      }
      else if(buf[0]=='4')
      {
//        Serial.println("got request: Mag");
        // Send a reply
//        Serial.print("Mag X: "); Serial.print(ubo.getmagx()); Serial.print(" Gauss \n");
//        Serial.print("Mag y: "); Serial.print(ubo.getmagy()); Serial.print(" Gauss \n");
//        Serial.print("Mag Z: "); Serial.print(ubo.getmagz()); Serial.print(" Gauss \n");
        uint8_t dataMX[20];
        uint8_t dataMY[20];
        uint8_t dataMZ[20];
        dtostrf(ubo.getmagx(), 5, 2, (char*)dataMX);
        dtostrf(ubo.getmagy(), 5, 2, (char*)dataMY);
        dtostrf(ubo.getmagz(), 5, 2, (char*)dataMZ);
        rf22.send(dataMX, sizeof(dataMX));
        rf22.waitPacketSent();
//        Serial.println("Sent MY Data");
        rf22.send(dataMY, sizeof(dataMY));
        rf22.waitPacketSent();
//        Serial.println("Sent MX Data");
        rf22.send(dataMZ, sizeof(dataMZ));
        rf22.waitPacketSent();
//        Serial.println("Sent MZ Data");
      }
      else if(buf[0]=='5')
      {
//        Serial.println("got request: Gyro");
        // Send a reply
//        Serial.print("Gyro X: "); Serial.print(ubo.getgyrox()); Serial.print(" dps \n");
//        Serial.print("Gyro Y: "); Serial.print(ubo.getgyroy()); Serial.print(" dps \n");
//        Serial.print("Gyro Z: "); Serial.print(ubo.getgyroz()); Serial.print(" dps \n");
        uint8_t dataGX[20];
        uint8_t dataGY[20];
        uint8_t dataGZ[20];
        dtostrf(ubo.getgyrox(), 5, 2, (char*)dataGX);
        dtostrf(ubo.getgyroy(), 5, 2, (char*)dataGY);
        dtostrf(ubo.getgyroz(), 5, 2, (char*)dataGZ);
        rf22.send(dataGX, sizeof(dataGX));
        rf22.waitPacketSent();
//        Serial.println("Sent GX Data");
        rf22.send(dataGY, sizeof(dataGY));
        rf22.waitPacketSent();
//        Serial.println("Sent GY Data");
        rf22.send(dataGZ, sizeof(dataGZ));
        rf22.waitPacketSent();
//        Serial.println("Sent GZ Data");
      }
      else if(buf[0]=='9')
      {
//        Serial.println("got request: Photo");
        uint8_t dataPhoto[] = "And hello back to you";
        rf22.send(dataPhoto, sizeof(dataPhoto));
        rf22.waitPacketSent();
      }  
    }
    else
    {
//      Serial.println("recv failed");
    }
  }
}

