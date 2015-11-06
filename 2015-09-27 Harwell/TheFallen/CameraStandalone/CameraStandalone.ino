#include <SPI.h>
#include <SoftwareSerial.h> // completely unnessesary
#include <RH_RF22.h>
#include <Adafruit_VC0706.h>

/** Settings */
#define BUF_LEN 32 // packets longer than 50 cause 'error'
#define RF_FREQ 431.00
#define DWELL 5 // in 30 seconds

/** Pin Map */
#define SPI_SCK   15
#define SPI_MISO  14
#define SPI_MOSI  16
#define SPI_CS_RF 10
#define SPI_CS_SD 0 // GPIO0
#define I2C_SCL   3
#define I2C_SDA   2
#define INT_RF    7
#define GPIO3     20
#define WATCHDOG  18
#define I_SOL     A8
#define V_SOL     A7
#define I_BUS     A6
#define V_BUS     A9

/** Class initialisations */
Adafruit_VC0706 camera (&Serial1);
RH_RF22 radio (SPI_CS_RF, INT_RF, hardware_spi);


void resetWatchDog(void) {

  digitalWrite(WATCHDOG, LOW);
  delay(15); // pause for >10ms
  digitalWrite(WATCHDOG, HIGH);

  return;
}


void setup(void) {

  pinMode(WATCHDOG, OUTPUT);
  digitalWrite(WATCHDOG, HIGH);

  Serial.begin(115200);
  //while (!Serial);
  Serial.println("Camera & Radio testing!");

  // Try to locate the radio
  if (!radio.init()) {
    Serial.println("Radio found!");
  } else {
    Serial.println("Radio not found!");
  }

  radio.setFrequency(RF_FREQ);

  unsigned char mess[] = {"Starting:"};
  if (!radio.send(mess, sizeof(mess))) Serial.println("Failed to send!");;
  radio.waitPacketSent(500);

  Serial.println();

  // Try to locate the camera
  if (camera.begin()) {
    Serial.println("Camera Found:");
  } else {
    Serial.println("No camera found?");
    return;
  }

  // Print out the camera version information (optional)
  char *reply = camera.getVersion();
  if (reply == 0) {
    Serial.print("Failed to get version");
  } else {
    Serial.println("-----------------");
    Serial.print(reply);
    Serial.println("-----------------");
  }

  // Set the picture size - you can choose one of 640x480, 320x240 or 160x120
  // Remember that bigger pictures take longer to transmit!

  camera.setImageSize(VC0706_640x480);        // biggest
  //camera.setImageSize(VC0706_320x240);        // medium
  //camera.setImageSize(VC0706_160x120);          // small

  // You can read the size back from the camera (optional, but maybe useful?)
  unsigned char imgsize = camera.getImageSize();
  Serial.print("Image size: ");
  if (imgsize == VC0706_640x480) Serial.println("640x480");
  else if (imgsize == VC0706_320x240) Serial.println("320x240");
  else if (imgsize == VC0706_160x120) Serial.println("160x120");
  else Serial.println("???x???");
  Serial.println(imgsize, DEC);

  Serial.println("Set up complete!\r\n");
  resetWatchDog();
  delay(30000);
}

void loop(void) {

  resetWatchDog();
  unsigned char mid[] = {"005500"};
  if (!radio.send(mid, sizeof(mid))) Serial.println("Failed to send!");

  if (!camera.takePicture())
    Serial.println("Failed to snap!");
  else
    Serial.println("Picture taken!");

  // Get the size of the image (frame) taken
  unsigned short jpglen = camera.frameLength();
  Serial.print("\r\nSending a ");
  Serial.print(jpglen, DEC);
  Serial.print(" byte image in ");
  Serial.print(jpglen / BUF_LEN, DEC);
  Serial.println(" packets.\r\n");

  // Read all the data up to # bytes!
  unsigned char bytesToRead;
  unsigned int packetCount = 0;
  //unsigned char dummy[BUF_LEN];
  //for (int i = 0; i < BUF_LEN; i++) dummy[i] = 0x55;
  while (jpglen > 0) {
    resetWatchDog();
    // read BUF_LEN bytes at a time;
    unsigned char *imgBuffer;
    bytesToRead = min(BUF_LEN, jpglen); // change 32 to 64 for a speedup but may not work with all setups!
    imgBuffer = camera.readPicture(bytesToRead);

    /*for (int i = 0; i < bytesToRead; i++)
    {
      if (imgBuffer[i] < 0x10) Serial.print('0'); // add leading zero
      Serial.print(imgBuffer[i], HEX);
    }*/

    unsigned long delta = millis();

    for (int i = 0 ; i < 3; i++) // 3 re-tries
    {
      resetWatchDog();

      if (!radio.send(imgBuffer, bytesToRead))
      {
        Serial.println("Error!");
        i = 10; // quit
      }
      else
      {
        delay(5);
        if (!radio.waitPacketSent(500))
        {
          Serial.print("Timeout!");
          if (i < 2) Serial.print(" Trying again...");
          Serial.println();
          //resetWatchDog(); // just covering all the bases
        }
        else
        {
          delta = millis() - delta;
          //delay(100); // delay?
          Serial.print("Sent packet ");
          Serial.print(packetCount, DEC);
          Serial.print(" it took ");
          Serial.println(delta, DEC);
          i = 10; // quit
        }
      }
    }
    packetCount++;

    //Serial.println();
    jpglen -= bytesToRead;
    resetWatchDog();
  }

  Serial.println("\r\nDone!");

  delay(30000);
  resetWatchDog();
  delay(30000);

}
