#ifndef EV3_MOTOR_H_
#define EV3_MOTOR_H_

bool Motor_isConnected ( const char *motor_port );
void Motor_run_timed ( const char *motor_port, long time_ms, int16_t power );
void Motor_run_forever ( const char *motor_port, int16_t power );
void Motor_stop ( const char *motor_port );
void Motor_stor_all ( void );
int32_t Motor_get_position ( const char *motor_port);
void Motor_reset_position(const char *motor_port);
void Motor_run_to_rel_pos( const char* motor_port, int32_t pos, int8_t power  );
void Motor_run_to_abs_pos( const char* motor_port, int32_t pos, int8_t power  );
//void SetPID(const char *motor_port, int p, int i, int d);
#endif //EV3_MOTOR_H_
