#ifndef EV3_MOTOR_H_
#define EV3_MOTOR_H_

bool Motor_isConnected ( const char *motor_port );
void Motor_run_timed ( const char *motor_port, long time_ms, int16_t power );
void Motor_run_forever ( const char *motor_port, int16_t power );
void Motor_stop ( const char *motor_port );
void Motor_stor_all ( void );

#endif //EV3_MOTOR_H_
