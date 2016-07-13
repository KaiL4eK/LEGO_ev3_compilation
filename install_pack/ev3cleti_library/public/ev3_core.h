#ifndef EV3_CORE_H_
#define EV3_CORE_H_

#ifndef __cplusplus
typedef enum bool_ {
	false = 0,
	true = 1
}bool;
#endif

#include <ev3_button.h>
#include <ev3_lcd.h>
#include <ev3_motor.h>
#include <ev3_power_info.h>
#include <ev3_sound.h>
#include <ev3_sensor.h>
#include <ev3_led.h>

#include <ev3_ports.h>

#endif // EV3_CORE_H_
