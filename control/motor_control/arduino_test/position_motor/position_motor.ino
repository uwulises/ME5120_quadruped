#include <SimpleFOC.h>
// DRV8302 pins connections
// don't forget to connect the common ground pin

//FOR RASPBERRY PI PICO
#define INH_A 6
#define INH_B 7
#define INH_C 8

#define EN_GATE 3
#define M_PWM 11
#define M_OC 12
#define OC_ADJ 13
#define N_FAULT 9
#define N_OCTW 10

#define DIR 19

// Motor instance (90KV, pero se recomienda un 50 a 70% más)
BLDCMotor motor = BLDCMotor(20, 0.186, 90);

BLDCDriver3PWM driver = BLDCDriver3PWM(INH_A, INH_B, INH_C, EN_GATE);
MagneticSensorI2C sensor = MagneticSensorI2C(AS5600_I2C);

// set point variable
float target = 5;


void setup() {
  
  //led onboard
  pinMode(25, OUTPUT);
  digitalWrite(25,HIGH);
  pinMode(DIR, OUTPUT);
  digitalWrite(DIR, LOW);
  // initialise magnetic sensor hardware
  sensor.init();
  delay(1000);
  // link the motor to the sensor
  motor.linkSensor(&sensor);
  //motor.linkCurrentSense();

  // DRV8302 specific code
  //Alerts
  pinMode(N_FAULT,INPUT);
  pinMode(N_OCTW,INPUT);

  // M_OC  - enable overcurrent protection
  pinMode(M_OC,OUTPUT);
  digitalWrite(M_OC,HIGH);
  // M_PWM  - enable 3pwm mode
  pinMode(M_PWM,OUTPUT);
  digitalWrite(M_PWM,HIGH);
  // OD_ADJ - set the maximum overcurrent limit possible
  // Better option would be to use voltage divisor to set exact value
  pinMode(OC_ADJ,OUTPUT);
  digitalWrite(OC_ADJ,HIGH);

  // driver config
  // power supply voltage [V]
  driver.voltage_power_supply = 24;
   // pwm frequency to be used [Hz]
  driver.pwm_frequency = 40000;
  driver.init();
  // link the motor and the driver
  motor.linkDriver(&driver);

  // choose FOC modulation (recomendado para sensores de efecto hall)
  motor.foc_modulation = FOCModulationType::Trapezoid_120;
  //motor.foc_modulation = FOCModulationType::SpaceVectorPWM;
  // set motion control loop to be used
  //motor.controller = MotionControlType::angle;
  motor.controller = MotionControlType::velocity_openloop;

  // velocity PI controller parameters
  motor.PID_velocity.P = 2;
  motor.PID_velocity.I = 0;
  motor.PID_velocity.D = 1;
  // maximal voltage to be set to the motor
  motor.voltage_limit = 24;
  motor.PID_velocity.output_ramp = 300;
  //Limits voltage (and therefore current) during motor alignment. Value in Volts.
  motor.voltage_sensor_align = 1;

  // velocity low pass filtering time constant
  // the lower the less filtered
  motor.LPF_velocity.Tf = 0.1;

  // angle P controller
  motor.P_angle.P = 5;
  // maximal velocity of the position control
  motor.velocity_limit = 100;
  motor.PID_velocity.limit = 100;
  motor.current_limit = 5;

  //encoder offset
  motor.sensor_offset=0;
  // use monitoring with serial
  Serial.begin(115200);
  motor.useMonitoring(Serial);
  // initialize motor
  motor.init();
  // align sensor and start FOC
  motor.initFOC();

  Serial.println(F("Motor ready."));

  _delay(1000);
}


void loop() {

  // main FOC algorithm function
  motor.loopFOC();
  
  // Motion control function
  // velocity, position or voltage (defined in motor.controller)
  // this function can be run at much lower frequency than loopFOC() function
  // You can also use motor.move() and set the motor.target in the code
  motor.move(target);
  // function intended to be used with serial plotter to monitor motor variables
  // significantly slowing the execution down!!!!
  //motor.monitor();
}