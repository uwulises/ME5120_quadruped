/**
 *
 * Position/angle motion control example
 * Steps:
 * 1) Configure the motor and magnetic sensor
 * 2) Run the code
 * 3) Set the target angle (in radians) from serial terminal
 *
 */
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

// Motor instance
BLDCMotor motor = BLDCMotor(20, 0.186, 90);
BLDCDriver3PWM driver = BLDCDriver3PWM(INH_A, INH_B, INH_C, EN_GATE);

// /**
//      * MagneticSensorI2C class constructor
//      * @param chip_address  I2C chip address
//      * @param bits number of bits of the sensor resolution 
//      * @param angle_register_msb  angle read register msb
//      * @param _bits_used_msb number of used bits in msb
//      */
//     MagneticSensorI2C(uint8_t _chip_address, int _bit_resolution, uint8_t _angle_register_msb, int _msb_bits_used);


MagneticSensorI2C sensor = MagneticSensorI2C(AS5600_I2C);

// angle set point variable
float target_angle = 0;
// instantiate the commander
Commander command = Commander(Serial);
void doTarget(char* cmd) { command.scalar(&target_angle, cmd); }


void setup() {
  
  // initialise magnetic sensor hardware
  sensor.init();
  delay(1000);
  // link the motor to the sensor
  motor.linkSensor(&sensor);

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
  driver.pwm_frequency = 20000;
  driver.init();
  // link the motor and the driver
  motor.linkDriver(&driver);

  // choose FOC modulation (optional)
  motor.foc_modulation = FOCModulationType::SpaceVectorPWM;

  // set motion control loop to be used
  motor.controller = MotionControlType::angle;

  // contoller configuration
  // default parameters in defaults.h

  // velocity PI controller parameters
  motor.PID_velocity.P = 5;
  motor.PID_velocity.I = 4;
  motor.PID_velocity.D = 0;
  // maximal voltage to be set to the motor
  motor.voltage_limit = 24;
  //Limits voltage (and therefore current) during motor alignment. Value in Volts.
  motor.voltage_sensor_align = 2;

  // velocity low pass filtering time constant
  // the lower the less filtered
  motor.LPF_velocity.Tf = 0.1f;

  // angle P controller
  motor.P_angle.P = 5;
  // maximal velocity of the position control
  motor.velocity_limit = 20;

  //encoder offset
  motor.sensor_offset=0;
  // use monitoring with serial
  Serial.begin(115200);
  // comment out if not needed
  motor.useMonitoring(Serial);


  // initialize motor
  motor.init();
  // align sensor and start FOC
  motor.initFOC();
  // add target command T
  command.add('T', doTarget, "target angle");

  Serial.println(F("Motor ready."));
  Serial.println(F("Set the target angle using serial terminal:"));

  _delay(1000);
}


void loop() {

  // main FOC algorithm function
  // the faster you run this function the better
  // Arduino UNO loop  ~1kHz
  // Bluepill loop ~10kHz
  motor.loopFOC();
  
  // Motion control function
  // velocity, position or voltage (defined in motor.controller)
  // this function can be run at much lower frequency than loopFOC() function
  // You can also use motor.move() and set the motor.target in the code
  motor.move(target_angle);
  // function intended to be used with serial plotter to monitor motor variables
  // significantly slowing the execution down!!!!
  //motor.monitor();
  // user communication
  command.run();

}