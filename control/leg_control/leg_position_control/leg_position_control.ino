#include "spi_msg.h"
#include "drv.h"


String inputString = "";
bool stringComplete = false;

int hombro_target = 0;
int hombro_pos = 0;
int sup_target = 0;
int sup_pos = 0;
int inf_target = 0;
int inf_pos = 0;

void serialEvent() {
  while (Serial.available()) {
    // get the new byte:
    char inChar = (char)Serial.read();
    // add it to the inputString:
    inputString += inChar;
    // if the incoming character is a newline, set a flag so the main loop can
    // do something about it:
    if (inChar == '\n') {
      stringComplete = true;
    }
  }
}


void setup() {
  // put your setup code here, to run once:
  Serial.begin(115200);
  sleep_ms(2000);
  drv_setup();
  Serial.println("SPI Master with 2 slaves\n");
  spi_master_init();  // setup once
}

void loop() {
  // main FOC algorithm function
  motor_loop();
  serialEvent();
  // put your main code here, to run repeatedly:

  if (stringComplete) {
    if (inputString.substring(0, 1) == "T") {
      hombro_target = inputString.substring(1, 4).toInt();
      sup_target = inputString.substring(5, 8).toInt();
      inf_target = inputString.substring(9, 12).toInt();
      sup_pos=spi_exchange_angle(CS_SLAVE1, sup_motor);
      inf_pos=spi_exchange_angle(CS_SLAVE2, inf_motor);
      update_target(hombro_target);
      inputString = "";
      stringComplete = false;
    }
    inputString = "";
    stringComplete = false;
  }
}
