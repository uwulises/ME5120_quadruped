#include "spi_msg.h"
#include "drv.h"
int target_angle = 0;
int current_position = 0;

void setup() {
  drv_setup();
  spi_slave_init();
}

void loop() {
  // main FOC algorithm function
  motor_loop();
  target_angle=spi_slave_exchange(current_position);
  update_target(target_angle);
  if (target_angle!=current_position){Serial.println(target_angle);}
  current_position = target_angle;
}
