#include <SPI.h>
#include <SPISlave.h>
#include "drv.h"

SPISettings spisettings(1000000, MSBFIRST, SPI_MODE0);

String inputString = "";
bool stringComplete = false;

float motor_pos = 0;
int transmits = 0;

void serialEvent()
{
  while (Serial.available())
  {
    // get the new byte:
    char inChar = (char)Serial.read();
    // add it to the inputString:
    inputString += inChar;
    // if the incoming character is a newline, set a flag so the main loop can
    // do something about it:
    if (inChar == '\n')
    {
      stringComplete = true;
    }
  }
}

void spi_setup()
{

  // device 1
  SPI.begin();
  SPI.setRX(16);
  SPI.setCS(17);
  SPI.setSCK(18);
  SPI.setTX(19);
  SPI.begin(true);
  delay(5000);
}

void msg_spi(int position, int device)
{
  char msg[42];
  memset(msg, 0, sizeof(msg));
  sprintf(msg, "What's up? This is transmission %d at position %d", transmits, position);
  Serial.printf("\n\nM-SEND: '%s'\n", msg);
  SPI.beginTransaction(spisettings);
  SPI.transfer(msg, sizeof(msg));
  SPI.endTransaction();
  Serial.printf("M-RECV: '%s'\n", msg);
  transmits++;
  delay(5000);
}

void setup()
{
  // use monitoring with serial
  Serial.begin(115200);
  drv_setup();
  spi_setup();
}

void loop()
{
  // main FOC algorithm function
  motor_loop();
  serialEvent();
  if (stringComplete)
  {
    if (inputString.substring(0, 1) == "T")
    {
      // take and split the next 6 characters of the string
      motor_pos = inputString.substring(1, 4).toInt();
      update_target(motor_pos);
      Serial.println(motor_pos);
      // Serial.print(" ");
      // Serial.print(motor.shaft_angle);
      // Serial.print(" ");
      // Serial.println(sensor.getSensorAngle());
      inputString = "";
      stringComplete = false;
    }
    if (inputString.substring(0, 3) == "LEG")
    {
      inputString = "";
      stringComplete = false;
    }
    if (inputString.substring(0, 6) == "HOMBRO")
    {
      inputString = "";
      stringComplete = false;
    }
    if (inputString.substring(0, 7) == "PATASUP")
    {
      inputString = "";
      stringComplete = false;
    }
    if (inputString.substring(0, 7) == "PATAINF")
    {
      inputString = "";
      stringComplete = false;
    }
    inputString = "";
    stringComplete = false;
  }
  motor_move();
}