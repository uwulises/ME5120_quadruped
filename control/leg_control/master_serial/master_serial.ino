#include <SoftwareSerial.h>

String inputString = "";      // a String to hold incoming data
bool stringComplete = false;  // whether the string is complete

SoftwareSerial FR(6, 7); // RX, TX
SoftwareSerial FL(8, 9); // RX, TX
SoftwareSerial RR(10,11); // RX, TX
SoftwareSerial RL(12, 13); // RX, TX

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
  // initialize serial:
  Serial.begin(9600);
  // reserve 200 bytes for the inputString:
  inputString.reserve(200);
  // set the data rate for the SoftwareSerial port
  FR.begin(4800);
  FL.begin(4800);
  RR.begin(4800);
  RL.begin(4800);
}

void loop() { // run over and over
// print the string when a newline arrives:
  if (stringComplete) {
    if (inputString.substring(0, 4) == "HOME") {
      FR.print("T000000000\n");
      FL.print("T000000000\n");
      RR.print("T000000000\n");
      RL.print("T000000000\n");
    }
    // clear the string:
    inputString = "";
    stringComplete = false;
  }
}