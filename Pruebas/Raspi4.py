import spidev
import lgpio  # Replacing RPi.GPIO with lgpio for Raspberry Pi 5 compatibility
import time

# Setup Chip Select (CS) pins for each Pico
cs_pins = [8, 7, 5]  # GPIO pins for CS
h = lgpio.gpiochip_open(0)  # Open GPIO chip 0

for pin in cs_pins:
    lgpio.gpio_claim_output(h, pin)  # Set CS pins as output
    lgpio.gpio_write(h, pin, 1)  # Set CS pins high

# SPI setup
spi = spidev.SpiDev()
spi.open(0, 0)  # SPI bus 0, device 0 (SPI0)
spi.max_speed_hz = 500000  # Set SPI speed (500kHz)

def select_device(device_num):
    for pin in cs_pins:
        lgpio.gpio_write(h, pin, 1)  # Deselect all devices (set CS high)
    lgpio.gpio_write(h, cs_pins[device_num], 0)  # Select the target device (set CS low)

def communicate_with_pico(device_num, data):
    select_device(device_num)
    response = spi.xfer2(data)  # Send and receive data via SPI
    lgpio.gpio_write(h, cs_pins[device_num], 1)  # Deselect the device (set CS high)
    return response

try:
    while True:
        # Send data to the first Pico
        response_1 = communicate_with_pico(0, [0x10])
        print("Response from Pico 1:", response_1)

        # Send data to the second Pico
        response_2 = communicate_with_pico(1, [0x20])
        print("Response from Pico 2:", response_2)

        # Send data to the third Pico
        response_3 = communicate_with_pico(2, [0x30])
        print("Response from Pico 3:", response_3)

        time.sleep(1)

finally:
    lgpio.gpiochip_close(h)  # Cleanup GPIO
    spi.close()
