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

#define COLOR_COMPONENT_RED 0
#define COLOR_COMPONENT_GREEN 1
#define COLOR_COMPONENT_BLUE 2

////////////////////////////////
//**UltraSonicSensor
////////////////////////////////
float UltraSonicSensor_read_cm ( const char *sensor_port );
float UltraSonicSensor_read_in (const char* sensor_port);
uint8_t  UltraSonicSensor_is_noise_here(const char* sensor_port);
float UltraSonicSensor_read_cm_sm(const char* sensor_port);
float UltraSonicSensor_read_in_sm(const char* sensor_port);
////////////////////////////////
//**LightSensor
////////////////////////////////
float LightSensor_read_percent ( const char *sensor_port, Light_s_mode_t mode );
////////////////////////////////
//**TouchSensor
////////////////////////////////
bool TouchSensor_isPressed ( const char *sensor_port );
bool TouchSensor_Clicked ( const char *sensor_port);
////////////////////////////////
//**ColorSensor
////////////////////////////////
Color_s_color_t ColorSensor_get_color ( const char *sensor_port );
uint8_t ColorSensor_get_reflected_light(const char *sensor_port);
uint8_t ColorSensor_get_ambient_light(const char* sensor_port);
uint16_t ColorSensor_get_color_component(const char* sensor_port, uint8_t COLOR_COMPONENT_ );
uint8_t ColorSensor_detect_color(const char* sensor_port, Color_s_color_t color);
uint8_t ColorSensor_detect_color_RGB(const char* sensor_port, uint16_t red, uint16_t green, uint16_t blue, uint16_t error);
////////////////////////////////
//**GyroSensor
////////////////////////////////
int16_t GyroSensor_angle(const char* sensor_port);
int16_t GyroSensor_rate(const char* sensor_port);
int16_t GyroSensor_acceleration(const char* sensor_port);
//uint16_t gyro_sensor_angle_bearing(const char* sensor_port);
void GyroSensor_calibrate(const char* sensor_port);

#endif //EV3_SENSOR_H_
