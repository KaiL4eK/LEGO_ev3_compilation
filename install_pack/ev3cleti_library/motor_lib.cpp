#include <motor.h>
#include <common.h>

std::map<std::string, motor *> motors_list;

motor *get_motor_ptr( std::string port, bool useErrorCatcher = true)
{
    if ( motors_list.count( port ) ) {
        return( (motor *)motors_list[port] );
    } else {
        motor *new_motor = new motor( port );

        if ( new_motor->connected() ) {
            motors_list[port] = new_motor;
            return new_motor;
        } else {
            delete new_motor;
			if(useErrorCatcher)
				error_process( __FUNCTION__, "Incorrect motor port "+port );
            return( nullptr );
        }
    }
}
std::string& get_motor_port(const uint8_t port)
{
	switch (port)
	{
		case 0x1:
		{
			return std::string("out1");
		{
		case 0x2:
		{
			return std::string("out2");
		{
		case 0x4:
		{
			return std::string("out3");
		{
		case 0x8:
		{
			return std::string("out4");
		{
	}
	return std::string("");
}
extern "C" 
{

#define MOTORS_CHECK_START_NO_PTR() \
	for(int i = 1; i < 16; i*= 2)	\
	{								\
		if(MOTOR_PORTS & i)			\
		{							\

#define MOTORS_CHECK_START()		\
	MOTORS_CHECK_START_NO_PTR()		\
		motor_ptr = get_motor_ptr(get_motor_port(i)); \

#define MOTORS_CHECK_END()			\
		}							\
	}								\


    bool Motor_isConnected ( const uint8_t MOTOR_PORTS )
    {

		motor *motor_ptr = NULL;
		bool IsConnected = true;

		MOTORS_CHECK_START()
			if(!motor_ptr)
			{
				IsConnected = false;
				break;
			}
		MOTORS_CHECK_END()

		if(IsConnected)
			return (true);
     
        return( false );
    }

    void Motor_run_timed ( const uint8_t MOTOR_PORTS, long time_ms, int16_t power )
    {
        motor *motor_ptr = NULL;

		MOTORS_CHECK_START()
        motor_ptr->set_time_sp( time_ms );
        motor_ptr->set_duty_cycle_sp( power );
        motor_ptr->run_timed();
		MOTORS_CHECK_END()
    }

    void Motor_run_to_rel_pos( const uint8_t MOTOR_PORTS, int32_t pos, int8_t power )
    {

        motor *motor_ptr = NULL;

		MOTORS_CHECK_START()
        motor_ptr->set_duty_cycle_sp( power );
        motor_ptr->set_position_sp(pos);
        motor_ptr->run_to_rel_pos();
		MOTORS_CHECK_END()

    }

    void Motor_run_to_abs_pos( const uint8_t MOTOR_PORTS, int32_t pos, int8_t power )
    {

        motor *motor_ptr = NULL;

		MOTORS_CHECK_START()
        motor_ptr->set_duty_cycle_sp( power );
        motor_ptr->set_position_sp(pos);
        motor_ptr->run_to_abs_pos();
		MOTORS_CHECK_END()

    }

    void Motor_run_forever ( const uint8_t MOTOR_PORTS, int16_t power )
    {
        motor *motor_ptr = NULL;

		MOTORS_CHECK_START()
        motor_ptr->set_duty_cycle_sp( power );
        motor_ptr->run_forever();
		MOTORS_CHECK_END()

    }

	void Stop( const uint8_t MOTOR_PORTS )
	{
		
		motor *motor_ptr = NULL;
		MOTORS_CHECK_START()
        motor_ptr->stop();
		MOTORS_CHECK_END()
	}

    void Motor_stop ( const uint8_t MOTOR_PORTS )
    {
        this->set_stop_action(stop_action_coast[]);
		Stop(MOTOR_PORTS);
    }

	void Motor_stop_break( const uint8_t MOTOR_PORTS )
	{
		this->set_stop_action(stop_action_brake[]);
		Stop(MOTOR_PORTS);
	}

	void Motor_stop_hold( const uint8_t MOTOR_PORTS )
	{
		this->set_stop_action(stop_action_hold[]);
		Stop(MOTOR_PORTS);
	}

    void Motor_stop_all ( void )
    {
		this->set_stop_action(stop_action_coast[]);
        for (auto m_ptr : motors_list) {
            m_ptr.second->reset();
        }
    }

    int32_t Motor_get_position ( const uint8_t MOTOR_PORT)
    {

        motor *motor_ptr = get_motor_ptr(MOTOR_PORT);
        return motor_ptr->position();

    }

    void Motor_reset_position(const uint8_t MOTOR_PORTS)
    {
        motor *motor_ptr = NULL;

		MOTORS_CHECK_START()
        motor_ptr->set_position(0);
		MOTORS_CHECK_END()
    }

    void Motor_reverse(const uint8_t MOTOR_PORTS)
    {
        motor *motor_ptr = NULL;

		MOTORS_CHECK_START()
        motor_ptr->set_duty_cycle_sp(-1 * motor_ptr->duty_cycle_sp());
        motor_ptr->run_forever();
		MOTORS_CHECK_END()
    }

    float Motor_get_speed(const uint8_t MOTOR_PORT)
    {

        motor *motor_ptr = get_motor_ptr(MOTOR_PORT);

        return 360 * (float)motor_ptr->speed() / (float)motor_ptr->count_per_rot();

    }

	uint8_t Motor_get_is_overloaded( const uint8_t MOTOR_PORT )
	{
		motor *motor_ptr = get_motor_ptr(MOTOR_PORT);
		return NULL;
	}

    void SetPID(const char *motor_port, int p, int i, int d)
    {

        motor *motor_ptr = get_motor_ptr( std::string(motor_port) );
        if(!motor_ptr)
            return;
        printf("P\n");
        motor_ptr->set_position_p(p);
        printf("I\n");
        motor_ptr->set_position_i(i);
        printf("D\n");
        motor_ptr->set_position_d(d);
        printf("End\n");

    }

    void Motor_ramp(const char *motor_port, int up, int down)
    {

        motor *motor_ptr = get_motor_ptr( std::string(motor_port) );
        if(!motor_ptr)
            return;

        motor_ptr->set_ramp_up_sp(up);
        motor_ptr->set_ramp_down_sp(down);        

    }


}

//-----------------------------------------------------------------------------

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

std::map<std::string, servo_motor *> servo_motors_list;

servo_motor *get_servo_motor_ptr( std::string port )
{
    if ( servo_motors_list.count( port ) ) {
        return( (servo_motor *)servo_motors_list[port] );
    } else {
        servo_motor *new_servo_motor = new servo_motor( port );

        if ( new_servo_motor->connected() ) {
            servo_motors_list[port] = new_servo_motor;
            return new_servo_motor;
        } else {
            delete new_servo_motor;
            error_process( __FUNCTION__, "Incorrect motor port "+port );
            return( nullptr );
        }
    }
}

//-----------------------------------------------------------------------------

extern "C"
{


    bool ServoMotor_isConnected(const char *motor_port)
    {

        servo_motor* SMotorPtr = get_servo_motor_ptr(std::string(motor_port));

        if(SMotorPtr)
        {
            return SMotorPtr->connected();
        }

        return 0;

    }

    void ServoMotor_run(const char *motor_port, int rate)    
    {

        servo_motor* SMotorPtr = get_servo_motor_ptr(std::string(motor_port));

        if(SMotorPtr)
        {

            SMotorPtr->set_rate_sp(rate);
            SMotorPtr->run();

        }

    }

    void ServoMotor_stop(const char *motor_port)
    {

        servo_motor* SMotorPtr = get_servo_motor_ptr(std::string(motor_port));

        if(SMotorPtr)
        {

            SMotorPtr->float_();

        }

    }

}