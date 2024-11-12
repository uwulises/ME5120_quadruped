#include <SPI.h>

#define CS_PIN 17  // Chip select pin (adjust based on your wiring)

volatile byte receivedData = 0;
volatile bool dataReceived = false;

// Define the SPI settings (speed: 1 MHz, mode: 0)
SPISettings spiSettings(500000, MSBFIRST, SPI_MODE0);

void setup() {
  // Setup serial for debuggi500000
  Serial.begin(9600);

  // Set CS_PIN as output and set it high (inactive)
  pinMode(CS_PIN, OUTPUT);
  digitalWrite(CS_PIN, HIGH);

  // Begin SPI as a slave
  SPI.begin();

  // Attach interrupt on CS pin to detect when the master initiates communication
  attachInterrupt(digitalPinToInterrupt(CS_PIN), onSPIReceive, FALLING);
}

void onSPIReceive() {
  // Begin the SPI transaction with the defined settings
  SPI.beginTransaction(spiSettings);

  // Read data from master (dummy byte 0x00 sent to receive)
  receivedData = SPI.transfer(0x00);
  dataReceived = true;

  // End the SPI transaction
  SPI.endTransaction();
}

void loop() {
  if (dataReceived) {
    Serial.print("Data received: ");
    Serial.println(receivedData);

    // Send response back to the master (e.g., incremented data)
    byte response = receivedData + 1;

    // Begin the SPI transaction again to send data back
    SPI.beginTransaction(spiSettings);
    SPI.transfer(response);
    SPI.endTransaction();

    // Clear the dataReceived flag
    dataReceived = false;
  }
}
