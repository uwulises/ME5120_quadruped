import spidev
import RPi.GPIO as GPIO
import time

# Setup Chip Select (CS) pins for each Pico
cs_pins = [8, 7, 5]  # GPIO pins for CS
GPIO.setmode(GPIO.BCM)
for pin in cs_pins:
    GPIO.setup(pin, GPIO.OUT)
    GPIO.output(pin, GPIO.HIGH)

# SPI setup
spi = spidev.SpiDev()
spi.open(0, 0)  # SPI bus 0, device 0 (SPI0)
spi.max_speed_hz = 500000  # Set SPI speed (500kHz)

def select_device(device_num):
    for pin in cs_pins:
        GPIO.output(pin, GPIO.HIGH)
    GPIO.output(cs_pins[device_num], GPIO.LOW)

def communicate_with_pico(device_num, data):
    select_device(device_num)
    response = spi.xfer2(data)
    GPIO.output(cs_pins[device_num], GPIO.HIGH)
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
    GPIO.cleanup()
    spi.close()
