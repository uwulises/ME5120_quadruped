from serial_control import SerialControl
import time
# Create an instance of the SerialControl class
motor = SerialControl(port="COM9", baudrate=115200)

#create a list of radians to send to the motor
radians = [0, 1, 2, 0, 4, 5, 0]

# Open the serial port
motor.open_serial()

# Send the commands to the motor
for rad in radians:
    motor.send_command(rad)
    time.sleep(0.5)

# Close the serial port
motor.close_serial()