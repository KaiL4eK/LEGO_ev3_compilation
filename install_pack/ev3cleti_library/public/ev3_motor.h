#ifndef EV3_MOTOR_H_
#define EV3_MOTOR_H_

#define OUTPUT_A 0x1
#define OUTPUT_B 0x2
#define OUTPUT_C 0x4
#define OUTPUT_D 0x8
#define OUTPUT_ALL 0xF

bool Motor_isConnected ( const uint8_t MOTOR_PORTS );
void Motor_run_timed ( const uint8_t MOTOR_PORTS, long time_ms, int16_t power );
void Motor_run_forever ( const uint8_t MOTOR_PORTS, int16_t power );
void Motor_stop ( const uint8_t MOTOR_PORTS );
void Motor_stop_break ( const uint8_t MOTOR_PORTS ) ;
void Motor_stop_hold ( const uint8_t MOTOR_PORTS ) ;
void Motor_stop_all ( void );
int32_t Motor_get_position ( const uint8_t MOTOR_PORT);
void Motor_reset_position(const uint8_t MOTOR_PORTS);
void Motor_run_to_rel_pos( const uint8_t MOTOR_PORTS, int32_t pos, int8_t power  );
void Motor_run_to_abs_pos( const uint8_t MOTOR_PORTS, int32_t pos, int8_t power  );
void Motor_reverse(const uint8_t MOTOR_PORTS);
float Motor_get_speed(const uint8_t MOTOR_PORT);
//void Motor_ramp(const char *motor_port, int up, int down);
//void SetPID(const char *motor_port, float p, float i, float d);
#endif //EV3_MOTOR_H_ 