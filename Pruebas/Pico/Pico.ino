#include <SPI.h>

#define CS_PIN 17 // Chip select pin for this Pico (change for each Pico)

volatile byte receivedData = 0; // Variable to store received data
volatile bool dataReceived = false;

void setup() {
  pinMode(CS_PIN, OUTPUT); // Set CS pin as output
  digitalWrite(CS_PIN, HIGH); // Set CS pin high (inactive)

  // Initialize SPI
  SPI.begin();
  SPI.setDataMode(SPI_MODE0);
  SPI.setClockDivider(SPI_CLOCK_DIV8); // Adjust clock speed as necessary

  // Enable SPI interrupt on receive
  SPCR |= _BV(SPE);
  SPCR |= _BV(SPIE);

  Serial.begin(9600); // Start serial communication for debugging
}

ISR(SPI_STC_vect) {
  receivedData = SPDR; // Read received data
  dataReceived = true; // Set flag to indicate data received
  SPDR = receivedData + 1; // Send response (for example, incrementing received value)
}

void loop() {
  if (dataReceived) {
    Serial.print("Received Data: ");
    Serial.println(receivedData);
    dataReceived = false;
  }
}
