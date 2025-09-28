#include "drv.h"

// Motor instance (90KV, pero se recomienda un 50 a 70% más)
BLDCMotor motor = BLDCMotor(NUM_POLES, MOTOR_R, MOTOR_KV);

BLDCDriver3PWM driver = BLDCDriver3PWM(INH_A, INH_B, INH_C, EN_GATE);
MagneticSensorI2C sensor = MagneticSensorI2C(SENSOR_ADD, 12, 0x0E, 4);

void drv_setup(void) {
  pinMode(EN_LED, OUTPUT);
  digitalWrite(EN_LED, HIGH);
  pinMode(ALARM_OC_LED, OUTPUT);
  digitalWrite(ALARM_OC_LED, HIGH);
  pinMode(TEMP_LED, OUTPUT);
  digitalWrite(TEMP_LED, HIGH);
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
  //  DRV8302 specific code
  // Alerts
  pinMode(N_FAULT, INPUT);
  pinMode(N_OCTW, INPUT);
  // driver config
  // power supply voltage [V]
  driver.voltage_power_supply = POWER_SUPPLY_VOLTAGE;
  // pwm frequency to be used [Hz]
  driver.pwm_frequency = PWM_FREQUENCY;
  driver.init();
  // link the motor and the driver
  motor.linkDriver(&driver);
  // motor.velocity_index_search = 3; // rad/s
  // Limits voltage (and therefore current) during motor alignment. Value in Volts.
  motor.voltage_sensor_align = VOLTAGE_SENSOR_ALIGN;
  // choose FOC modulation
  motor.foc_modulation = FOC_MODULATION;
  // set motion control loop to be used
  motor.controller = MOTION_CONTROL;
  // velocity PI controller parameters
  motor.PID_velocity.P = PID_VEL_P;
  motor.PID_velocity.I = PID_VEL_I;
  motor.PID_velocity.D = PID_VEL_D;
  // motor.PID_velocity.output_ramp = PI_VEL_OUTPUT_RAMP;
  //  velocity low pass filtering time constant
  motor.LPF_velocity.Tf = LPF_VEL_TF;
  motor.LPF_angle.Tf = LPF_ANGLE_TF;
  // angle PID controller
  // default P=20
  motor.P_angle.P = ANGLE_PID_P;
  motor.P_angle.I = ANGLE_PID_I;  // usually only P controller is enough
  // acceleration control using output ramp
  // this variable is in rad/s^2 and sets the limit of acceleration
  motor.P_angle.output_ramp = ANGLE_PID_P_OUTPUT_RAMP;  // default 1e6 rad/s^2
  // maximal voltage to be set to the motor
  motor.voltage_limit = VOLTAGE_LIMIT;
  // maximal velocity of the position control
  motor.velocity_limit = VELOCITY_LIMIT;
  // motor.PID_velocity.limit = 100;
  motor.current_limit = CURRENT_LIMIT;
  // encoder offset
  motor.sensor_offset = SENSOR_OFFSET;
  // initialize motor
  motor.init();
  // align sensor and start FOC
  motor.initFOC();
  Serial.println(F("Motor ready."));
  _delay(1000);
}

void motor_loop(void) {
  motor.loopFOC();
}

void motor_move(void) {
  motor.move();
}

void update_target(float target) {
  motor.target = target;
}