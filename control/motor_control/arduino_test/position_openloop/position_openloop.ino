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
BLDCMotor motor = BLDCMotor(20, 0.186, 108);

BLDCDriver3PWM driver = BLDCDriver3PWM(INH_A, INH_B, INH_C, EN_GATE);
MagneticSensorI2C sensor = MagneticSensorI2C(0x36, 12, 0x0E, 4);

String inputString = "";
bool stringComplete = false;

// set point variable
float target = 0;

void serialEvent() {
  while (Serial.available()) {
    // get the new byte:
    char inChar = (char)Serial.read();
    // add it to the inputString:
    inputString += inChar;
    // if the incoming character is a newline, set a flag so the main loop can
    // do something about it:
    if (inChar == '\n') {
      stringComplete = true;
    }
  }
}



void setup() {

  //led onboard
  pinMode(25, OUTPUT);
  digitalWrite(25, HIGH);
  pinMode(DIR, OUTPUT);
  digitalWrite(DIR, LOW);
  // initialise magnetic sensor hardware
  sensor.init();
  // link the motor to the sensor
  motor.linkSensor(&sensor);

  // DRV8302 specific code
  //Alerts
  pinMode(N_FAULT, INPUT);
  pinMode(N_OCTW, INPUT);

  // M_OC  - enable overcurrent protection (LOW)
  pinMode(M_OC, OUTPUT);
  digitalWrite(M_OC, LOW);
  // M_PWM  - enable 3pwm mode
  pinMode(M_PWM, OUTPUT);
  digitalWrite(M_PWM, HIGH);
  // OD_ADJ - set the maximum overcurrent limit possible
  // Better option would be to use voltage divisor to set exact value
  pinMode(OC_ADJ, OUTPUT);
  digitalWrite(OC_ADJ, HIGH);

  // driver config
  // power supply voltage [V]
  driver.voltage_power_supply = 24;
  // pwm frequency to be used [Hz]
  driver.pwm_frequency = 50000;
  driver.init();
  // link the motor and the driver
  motor.linkDriver(&driver);

  // choose FOC modulation
  motor.foc_modulation = FOCModulationType::SpaceVectorPWM;

  // maximal voltage to be set to the motor
  motor.voltage_limit = 12;
  motor.velocity_limit = 10;
  motor.current_limit = 5;
  // use monitoring with serial
  Serial.begin(115200);
  motor.useMonitoring(Serial);

  // set motion control loop to be used
  motor.controller = MotionControlType::angle_openloop;
  
  // initialize motor
  motor.init();
  // align sensor and start FOC
  motor.initFOC();
  

  Serial.println(F("Motor ready."));
  _delay(1000);
  motor.move(0);
}


void loop() {

  // main FOC algorithm function
  motor.loopFOC();
  if (stringComplete) {
    if (inputString.substring(0, 1) == "T") {
      // take and split the next 6 characters of the string
      target = inputString.substring(1, 6).toInt();
      target = target/100;
      motor.target = target;
      Serial.print(target);
      Serial.print(" ");
      //Serial.print(sensor.getSensorAngle());
      //Serial.print(" ");
      //Serial.println(sensor.getVelocity());
      inputString = "";
      stringComplete = false;
    }
    inputString = "";
    stringComplete = false;
  }
  motor.move();

  // function intended to be used with serial plotter to monitor motor variables
  // significantly slowing the execution down!!!!
  //motor.monitor();
}