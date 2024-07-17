#include "drv.h"

// Motor instance (90KV, pero se recomienda un 50 a 70% más)
BLDCMotor motor = BLDCMotor(20, 0.186, 108);

BLDCDriver3PWM driver = BLDCDriver3PWM(INH_A, INH_B, INH_C, EN_GATE);
MagneticSensorI2C sensor = MagneticSensorI2C(0x36, 12, 0x0E, 4);

void drv_setup(void)
{
  pinMode(EN_LED,OUTPUT);
  digitalWrite(EN_LED,HIGH);
  pinMode(ALARM_OC_LED,OUTPUT);
  digitalWrite(ALARM_OC_LED,HIGH);
  pinMode(TEMP_LED,OUTPUT);
  digitalWrite(TEMP_LED,HIGH);
  pinMode(DIR, OUTPUT);
  digitalWrite(DIR, LOW);
  // initialise magnetic sensor hardware
  sensor.init();
  // configure i2C
  Wire.setClock(100000);
  // link the motor to the sensor
  motor.linkSensor(&sensor);
  // DRV8302 specific code
  // Alerts
  pinMode(N_FAULT, INPUT);
  pinMode(N_OCTW, INPUT);
  // providing
  sensor.init(&Wire);
  // link the motor to the sensor
  motor.linkSensor(&sensor);
  // motor.linkCurrentSense();
  //  DRV8302 specific code
  // Alerts
  pinMode(N_FAULT, INPUT);
  pinMode(N_OCTW, INPUT);
  // driver config
  // power supply voltage [V]
  driver.voltage_power_supply = 24;
  // pwm frequency to be used [Hz]
  driver.pwm_frequency = 50000;
  driver.init();
  // link the motor and the driver
  motor.linkDriver(&driver);
  // motor.velocity_index_search = 3; // rad/s
  // Limits voltage (and therefore current) during motor alignment. Value in Volts.
  motor.voltage_sensor_align = 1;
  // choose FOC modulation
  motor.foc_modulation = FOCModulationType::SpaceVectorPWM;
  // set motion control loop to be used
  motor.controller = MotionControlType::angle;
  // velocity PI controller parameters
  motor.PID_velocity.P = 1;
  motor.PID_velocity.I = 2;
  motor.PID_velocity.D = 0;
  // motor.PID_velocity.output_ramp = 1000;
  //  velocity low pass filtering time constant
  motor.LPF_velocity.Tf = 0.01;
  motor.LPF_angle.Tf = 0;
  // angle PID controller
  // default P=20
  motor.P_angle.P = 20;
  motor.P_angle.I = 0; // usually only P controller is enough
  motor.P_angle.D = 0; // usually only P controller is enough
  // acceleration control using output ramp
  // this variable is in rad/s^2 and sets the limit of acceleration
  motor.P_angle.output_ramp = 10000; // default 1e6 rad/s^2
  // maximal voltage to be set to the motor
  motor.voltage_limit = 12;
  // maximal velocity of the position control
  motor.velocity_limit = 10;
  // motor.PID_velocity.limit = 100;
  motor.current_limit = 5;
  // encoder offset
  motor.sensor_offset = 0;
  // initialize motor
  motor.init();
  // align sensor and start FOC
  motor.initFOC();
  Serial.println(F("Motor ready."));
  _delay(1000);
}

void motor_loop(void)
{
  motor.loopFOC();
}

void motor_move(void)
{
  motor.move();
}

void update_target(float target)
{
  motor.target = target;
}