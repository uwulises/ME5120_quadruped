#ifndef DRV_H
#define DRV_H
#include <SimpleFOC.h>
#include <Arduino.h>
// DRV8302 pins connections
// FOR RASPBERRY PI PICO
#define INH_A 6          // Out phase A PWM
#define INH_B 7          // Out phase B PWM
#define INH_C 8          // Out phase C PWM
#define EN_GATE 3        // Digital out (Enables the gate driver)
#define EN_LED 13        // Digital out (Copies the state of the enable pin)
#define N_FAULT 9        // Digital in (Indicates a fault condition)
#define N_OCTW 10        // Digital in (Indicates an overcurrent condition)
#define ALARM_OC_LED 14  // copies the state of the nOCTW pin
#define TEMP_LED 15      // LED indicator when thermistor is too hot
#define DIR 22           // Direction of AS5600 (change the state as mirror on the opposite side)

#define NUM_POLES 20   // Number of poles of the motor
#define MOTOR_R 0.186  // Phases resistance
#define MOTOR_KV 108   // KV rating
#define FOC_MODULATION FOCModulationType::SpaceVectorPWM
#define MOTION_CONTROL MotionControlType::angle

#define POWER_SUPPLY_VOLTAGE 24  // [V]
#define PWM_FREQUENCY 50000      // [Hz]
#define VOLTAGE_SENSOR_ALIGN 1   // [V]
#define VELOCITY_INDEX_SEARCH 3  // [rad/s]
#define PID_VEL_P 1              // PID for velocity control
#define PID_VEL_I 2
#define PID_VEL_D 0
#define LPF_VEL_TF 0.01f  // Low pass filter time constant for velocity control
#define LPF_ANGLE_TF 0    // Low pass filter time constant for angle control
#define ANGLE_PID_P 20    // PID for angle control
#define ANGLE_PID_I 0
#define ANGLE_PID_P_OUTPUT_RAMP 1000
#define VOLTAGE_LIMIT 12  // limit the RPM to Voltage limit x KV
#define VELOCITY_LIMIT 10
#define CURRENT_LIMIT 5  // Check the current limit
#define SENSOR_OFFSET 0
#define SENSOR_ADD 0x36

/// @brief Setup the motor driver
void drv_setup(void);

/// @brief Loop for the motor driver, keep the loop fast as possible
void motor_loop(void);

/// @brief Move the motor
void motor_move(void);

/// @brief Update the target
/// @param target The target to update on motor.target instance
void update_target(float target);

#endif