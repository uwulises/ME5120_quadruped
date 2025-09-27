#ifndef SPI_MSG_H
#define SPI_MSG_H
#include <stdint.h>
#include <stdio.h>
#include <cstring>
#include "pico/stdlib.h"
#include "hardware/spi.h"

#define BUF_LEN 4

// Define CS pins for each slave
#define CS_SLAVE1 20
#define CS_SLAVE2 21

void spi_master_init();

float spi_exchange_angle(uint cs_pin, float target_angle);

void select_slave(uint cs_pin);
void deselect_slave(uint cs_pin);

#endif