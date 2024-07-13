#ifndef DRV_H
#define DRV_H
#include <SimpleFOC.h>
// DRV8302 pins connections
#include <Arduino.h>
// FOR RASPBERRY PI PICO
#define INH_A 6
#define INH_B 7
#define INH_C 8
#define EN_GATE 3
#define EN_LED 13
#define N_FAULT 9
#define N_OCTW 10
#define DIR 22

void drv_setup(void);
void motor_loop(void);

void motor_move(void);

void update_target(float target);

#endif