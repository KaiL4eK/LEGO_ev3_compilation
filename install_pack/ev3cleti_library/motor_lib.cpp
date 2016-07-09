#include <motor.h>

motor::motor(address_type address)
{
    connect({{ "address", { address } }});
}

//-----------------------------------------------------------------------------

motor::motor(address_type address, const motor_type &t)
{
    connect({{ "address", { address } }, { "driver_name", { t }}});
}

//-----------------------------------------------------------------------------

bool motor::connect(const std::map<std::string, std::set<std::string>> &match) noexcept
{
    static const std::string _strClassDir { SYS_ROOT "/tacho-motor/" };
    static const std::string _strPattern  { "motor" };

    try
    {
        return device::connect(_strClassDir, _strPattern, match);
    }
    catch (...) { }

    _path.clear();

    return false;
}

//-----------------------------------------------------------------------------

dc_motor::dc_motor(address_type address)
{
    static const std::string _strClassDir { SYS_ROOT "/dc-motor/" };
    static const std::string _strPattern  { "motor" };

    connect(_strClassDir, _strPattern, {{ "address", { address }}});
}

//-----------------------------------------------------------------------------

servo_motor::servo_motor(address_type address)
{
    static const std::string _strClassDir { SYS_ROOT "/servo-motor/" };
    static const std::string _strPattern  { "motor" };

    connect(_strClassDir, _strPattern, {{ "address", { address }}});
}

//-----------------------------------------------------------------------------

std::map<std::string, motor *> motors_list;

motor *find_motor_in_list( std::string port )
{
    if ( motors_list.count( port ) ) {
        return( motors_list[port] );
    } else {
        motor *new_motor = new motor( port );

        if ( new_motor->connected() ) {
            motors_list[port] = new_motor;
            return new_motor;
        } else {
            delete new_motor;
            return NULL;
        }
    }
}

extern "C" {

bool Motor_isConnected ( const char *motor_port )
{
    motor *motor_ptr = find_motor_in_list( std::string(motor_port) );

    if ( motor_ptr ) {
        return motor_ptr->connected();
    }
    return( false );
}

void Motor_run_timed ( const char *motor_port, long time_ms, int16_t power )
{
    motor *motor_ptr = find_motor_in_list( std::string(motor_port) );

    if ( motor_ptr ) {
        motor_ptr->set_time_sp( time_ms );
        motor_ptr->set_duty_cycle_sp( power );
        motor_ptr->run_timed();
    }
}

void Motor_run_forever ( const char *motor_port, int16_t power )
{
    motor *motor_ptr = find_motor_in_list( std::string(motor_port) );

    if ( motor_ptr ) {
        motor_ptr->set_duty_cycle_sp( power );
        motor_ptr->run_forever();
    }
}

void Motor_stop ( const char *motor_port )
{
    motor *motor_ptr = find_motor_in_list( std::string(motor_port) );

    if ( motor_ptr ) {
        motor_ptr->stop();
    }
}

}
