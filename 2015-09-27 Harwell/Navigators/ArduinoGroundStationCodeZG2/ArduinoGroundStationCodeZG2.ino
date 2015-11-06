// rf22_client.pde
// -*- mode: C++ -*-
// Example sketch showing how to create a simple messageing client
// with the RH_RF22 class. RH_RF22 class does not provide for addressing or
// reliability, so you should only use RH_RF22 if you do not need the higher
// level messaging abilities.
// It is designed to work with the other example rf22_server
// Tested on Duemilanove, Uno with Sparkfun RFM22 wireless shield
// Tested on Flymaple with sparkfun RFM22 wireless shield
// Tested on ChiKit Uno32 with sparkfun RFM22 wireless shield

#include <SPI.h>
#include "Wire.h"
#include "SoftwareSerial.h"
//#include <SD.h>
#include <RH_RF22.h>

// Singleton instance of the radio driver
RH_RF22 rf22;

// Navigators variable declarations chunk ( 1 of 2 )
float magAbsoluteValue;
float magx;
float magy;
float magz;
float magUnitX;
float magUnitY;
float magUnitZ;
float v1, v2, v3;
float sine;
float cosine;
float v11, v12, v13;
float v21, v22, v23;
float v31, v32, v33;
float vsquare11, vsquare12, vsquare13;
float vsquare21, vsquare22, vsquare23;
float vsquare31, vsquare32, vsquare33;
float R11, R12, R13;
float R21, R22, R23;
float R31, R32, R33;
float psiRad, thetaRad, phiRad;
float psi, theta, phi;
// end Navigators chunk ( 1 of 2 )

void setup() 
{
  Serial.begin(9600);
  while(!Serial){};
  if (!rf22.init())
    Serial.println("init failed");
  rf22.setFrequency(437);
  // Defaults after init are 434.0MHz, 0.05MHz AFC pull-in, modulation FSK_Rb2_4Fd36
}

void loop()
{
  byte byteVar;
  Serial.flush();
  while(!Serial.available());
  byteVar = Serial.read();
  //Serial.print("byteVar = ");
  //Serial.println(byteVar);
  uint8_t data[RH_RF22_MAX_MESSAGE_LEN];
  data[1]='\n';
  if((byteVar == 49) || (byteVar == 50) || (byteVar == 51) || (byteVar == 52) || (byteVar == 53) || (byteVar == 57))
  {
    if(byteVar == 49)
    {
      Serial.println("Sending request for Temperature");
      data[0]='1';
    }
    else if(byteVar == 50)
    {
      Serial.println("UV Index, IR and Light");
      data[0]='2';      
    }
    else if(byteVar == 51)
    {
      Serial.println("Accelerometer");
      data[0]='3';      
    }
    else if(byteVar == 52)
    {
      Serial.println("Magnetic field");
      data[0]='4';      
    }
    else if(byteVar == 53)
    {
      Serial.println("Gyroscope");
      data[0]='5';      
    }
    else if(byteVar == 57)
    {
      Serial.println("Take a picture");
      data[0]='9';      
    }
    rf22.send(data, sizeof(data));
    rf22.waitPacketSent();
    
    // Now wait for a reply
    uint8_t buf[RH_RF22_MAX_MESSAGE_LEN];
    uint8_t len = sizeof(buf);
  
    if (rf22.waitAvailableTimeout(6000))
    { 
      // Should be a reply message for us now   
      if (rf22.recv(buf, &len))
      {
        Serial.print("Reply from NanoSat: ");
        if(byteVar == 49)
        {
          Serial.println((char*)buf);
        }
        else if(byteVar == 50)
        {
          Serial.println("");
          Serial.print("UV : ");
          Serial.println((char*)buf);
          if (rf22.waitAvailableTimeout(1000))
          {
            if (rf22.recv(buf, &len))
            {
              Serial.print("IR : ");
              Serial.println((char*)buf);            
            }
          }
          else
          {
            Serial.println("IR Timed-out");
          }
          if (rf22.waitAvailableTimeout(1000))
          {
            if (rf22.recv(buf, &len))
            {
              Serial.print("Light : ");
              Serial.println((char*)buf);            
            }
          }
          else
          {
            Serial.println("Light Timed-out");
          }
        }
        else if(byteVar == 51)
        {
          Serial.println("");
          Serial.print("Accel-X : ");
          Serial.println((char*)buf);
          if (rf22.waitAvailableTimeout(1000))
          {
            if (rf22.recv(buf, &len))
            {
              Serial.print("Accel-Y : ");
              Serial.println((char*)buf);            
            }
          }
          else
          {
            Serial.println("Accel-Y Timed-out");
          }
          if (rf22.waitAvailableTimeout(1000))
          {
            if (rf22.recv(buf, &len))
            {
              Serial.print("Accel-Z : ");
              Serial.println((char*)buf);            
            }
          }
          else
          {
            Serial.println("Accel-Z Timed-out");
          }
        }
        else if(byteVar == 52)
        {
          Serial.println("");
          Serial.print("Mag-X : ");
          Serial.println((char*)buf);
          magx = atof((char*)buf);
          if (rf22.waitAvailableTimeout(1000))
          {
            if (rf22.recv(buf, &len))
            {
              Serial.print("Mag-Y : ");
              Serial.println((char*)buf);
              magy = atof((char*)buf);
              //Serial.print("Test MagY");
              //Serial.println(magy);
            }
          }
          else
          {
            Serial.println("Mag-Y Timed-out");
          }
          if (rf22.waitAvailableTimeout(1000))
          {
            if (rf22.recv(buf, &len))
            {
              Serial.print("Mag-Z : ");
              Serial.println((char*)buf);
              magz = atof((char*)buf);        
            }
          }
          else
          {
            Serial.println("Mag-Z Timed-out");
          }
          magAbsoluteValue=sqrt(magx*magx+magy*magy+magz*magz);
          magUnitX=magx/magAbsoluteValue;
          magUnitY=magy/magAbsoluteValue;
          magUnitZ=magz/magAbsoluteValue;
          v1=0;
          v2=-magUnitZ;
          v3=magUnitY;
          sine=sqrt(v2*v2+v3*v3);
          cosine=magUnitX;
          v11=0;
          v12=-v3;
          v13=v2;
          v21=v3;
          v22=0;
          v23=-v1;
          v31=-v2;
          v32=v1;
          v33=0;
          vsquare11=v11*v11+v12*v21+v13*v13;
          vsquare12=v11*v21+v12*v22+v13*v23;
          vsquare13=v11*v31+v12*v32+v13*v33;
          vsquare21=v21*v11+v22*v21+v23*v13;
          vsquare22=v21*v21+v22*v22+v23*v23;
          vsquare23=v21*v31+v22*v32+v23*v33;
          vsquare31=v31*v11+v32*v21+v33*v13;
          vsquare32=v31*v21+v32*v22+v33*v23;
          vsquare33=v31*v21+v32*v32+v33*v33;
          R11=1+v11+vsquare11*(1-cosine)/(sine*sine);
          R12=0+v12+vsquare12*(1-cosine)/(sine*sine);
          R13=0+v13+vsquare13*(1-cosine)/(sine*sine);
          R21=0+v21+vsquare21*(1-cosine)/(sine*sine);
          R22=1+v22+vsquare22*(1-cosine)/(sine*sine);
          R23=0+v23+vsquare23*(1-cosine)/(sine*sine);
          R31=0+v31+vsquare31*(1-cosine)/(sine*sine);
          R32=0+v32+vsquare32*(1-cosine)/(sine*sine);
          R33=1+v33+vsquare33*(1-cosine)/(sine*sine);
          thetaRad=-asin(R31);
          psiRad=atan2(R32,R33);
          phiRad=atan2((R21/cos(theta)),R11/cos(theta));
          theta=thetaRad*180/3.14;
          psi=psiRad*180/3.14;
          phi=phiRad*180/3.14;
          Serial.print("(should be less than 20 degress for photo) psi="); Serial.print(psi); Serial.print(" \n");
          Serial.print("(should be less than 20 degress for photo) theta="); Serial.print(theta); Serial.print(" \n");
          Serial.print("phi="); Serial.print(phi); Serial.print(" \n");          
        }
        else if(byteVar == 53)
        {
          Serial.println("");
          Serial.print("Gyro-X : ");
          Serial.println((char*)buf);
          if (rf22.waitAvailableTimeout(1000))
          {
            if (rf22.recv(buf, &len))
            {
              Serial.print("Gyro-Y : ");
              Serial.println((char*)buf);            
            }
          }
          else
          {
            Serial.println("Gyro-Y Timed-out");
          }
          if (rf22.waitAvailableTimeout(1000))
          {
            if (rf22.recv(buf, &len))
            {
              Serial.print("Gyro-Z : ");
              Serial.println((char*)buf);            
            }
          }
          else
          {
            Serial.println("Gyro-Z Timed-out");
          }
        }
        else if(byteVar == 57)
        {
          Serial.println("Picture has been taken and stored on SD card");
        }
      }
      else
      {
        Serial.println("Receive from NanoSat failed :-(");
      }
    }
    else
    {
      Serial.println("No reply from NanoSat :-(");
    }
    delay(400);
  }
  else
  {
    Serial.println("************************************");
    Serial.println("Command not defined yet. Options are");
    Serial.println("1 - Temperature");
    Serial.println("2 - UV Index, IR and Light");
    Serial.println("3 - Accelerometer");
    Serial.println("4 - Magnetic Field");
    Serial.println("5 - Gyroscope");
    Serial.println("9 - Take a picture");
    Serial.println("************************************");
    delay(400);
  }
}

