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

void spi_slave_init() {
    // Init SPI (same frequency as master)
    spi_init(spi_default, 1 * 1000 * 1000);
    spi_set_slave(spi_default, true);

    // Assign pins for SPI (Pico default: SCK=18, MOSI=19, MISO=16, CS=17)
    gpio_set_function(PICO_DEFAULT_SPI_RX_PIN, GPIO_FUNC_SPI);   // MOSI
    gpio_set_function(PICO_DEFAULT_SPI_SCK_PIN, GPIO_FUNC_SPI);  // SCK
    gpio_set_function(PICO_DEFAULT_SPI_TX_PIN, GPIO_FUNC_SPI);   // MISO
    gpio_set_function(PICO_DEFAULT_SPI_CSN_PIN, GPIO_FUNC_SPI);  // CS
}

// ---------------- Core Function ----------------
// Send target angle to a slave and return its current position
int spi_exchange_angle(uint cs_pin, int target_angle) {
  uint8_t out_buf[BUF_LEN];
  uint8_t in_buf[BUF_LEN];

  // Pack int into bytes
  memcpy(out_buf, &target_angle, sizeof(int));

  // Activate slave
  select_slave(cs_pin);

  // Full-duplex transfer
  spi_write_read_blocking(spi_default, out_buf, in_buf, BUF_LEN);

  // Deactivate slave
  deselect_slave(cs_pin);

  // Unpack received int
  int current_position;
  memcpy(&current_position, in_buf, sizeof(int));

  return current_position;
}

int spi_slave_exchange(int current_position) {
    uint8_t out_buf[BUF_LEN];
    uint8_t in_buf[BUF_LEN];

    // Pack current position into buffer
    memcpy(out_buf, &current_position, sizeof(int));

    // Full-duplex transfer (blocking until master sends)
    spi_write_read_blocking(spi_default, out_buf, in_buf, BUF_LEN);

    // Unpack received target
    int target_angle;
    memcpy(&target_angle, in_buf, sizeof(int));

    return target_angle;
}