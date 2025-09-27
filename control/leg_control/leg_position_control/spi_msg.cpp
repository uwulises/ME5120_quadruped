#include "spi_msg.h"

// ---------------- Utility Functions ----------------
void spi_master_init() {
  // Init SPI
  spi_init(spi_default, 1 * 1000 * 1000);
  gpio_set_function(PICO_DEFAULT_SPI_RX_PIN, GPIO_FUNC_SPI);
  gpio_set_function(PICO_DEFAULT_SPI_SCK_PIN, GPIO_FUNC_SPI);
  gpio_set_function(PICO_DEFAULT_SPI_TX_PIN, GPIO_FUNC_SPI);

  // Configure CS pins
  gpio_init(CS_SLAVE1);
  gpio_set_dir(CS_SLAVE1, GPIO_OUT);
  gpio_put(CS_SLAVE1, 1);

  gpio_init(CS_SLAVE2);
  gpio_set_dir(CS_SLAVE2, GPIO_OUT);
  gpio_put(CS_SLAVE2, 1);
}

void select_slave(uint cs_pin) {
  gpio_put(cs_pin, 0);
}
void deselect_slave(uint cs_pin) {
  gpio_put(cs_pin, 1);
}

// ---------------- Core Function ----------------
// Send target angle to a slave and return its current position
float spi_exchange_angle(uint cs_pin, float target_angle) {
  uint8_t out_buf[BUF_LEN];
  uint8_t in_buf[BUF_LEN];

  // Pack float into bytes
  memcpy(out_buf, &target_angle, sizeof(float));

  // Activate slave
  select_slave(cs_pin);

  // Full-duplex transfer
  spi_write_read_blocking(spi_default, out_buf, in_buf, BUF_LEN);

  // Deactivate slave
  deselect_slave(cs_pin);

  // Unpack received float
  float current_position;
  memcpy(&current_position, in_buf, sizeof(float));

  return current_position;
}