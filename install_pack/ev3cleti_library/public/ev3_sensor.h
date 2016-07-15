#ifndef EV3_SENSOR_H_
#define EV3_SENSOR_H_

typedef enum light_s_mode {
	REFLECT,
	AMBIENT
}Light_s_mode_t;

typedef enum color_s_color_ {
   COLOR_NO = 0,
   COLOR_BLACK = 1,
   COLOR_BLUE = 2,
   COLOR_GREEN = 3,
   COLOR_YELLOW = 4,
   COLOR_RED = 5,
   COLOR_WHITE = 6,
   COLOR_BROWN = 7
}Color_s_color_t;

////////////////////////////////
//**UltraSonic
////////////////////////////////
float UltraSonicSensor_read_cm ( const char *sensor_port );
////////////////////////////////
//**LightSonic
////////////////////////////////
float LightSensor_read_percent ( const char *sensor_port, Light_s_mode_t mode );
////////////////////////////////
//**TouchSonic
////////////////////////////////
bool TouchSensor_isPressed ( const char *sensor_port );
bool TouchSensor_Clicked ( const char *sensor_port);
////////////////////////////////
//**ColorSonic
////////////////////////////////
Color_s_color_t ColorSensor_get_color ( const char *sensor_port );

#endif //EV3_SENSOR_H_
