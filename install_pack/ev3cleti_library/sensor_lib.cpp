#include <sensor.h>
#include <lego_port.h>
#include <common.h>

std::map<std::string, sensor *> sensors_list;

template<class SensorClass>
SensorClass *get_sensor_ptr( std::string port )
{
    if ( sensors_list.count( port ) ) {
        return( (SensorClass *)sensors_list[port] );
    } else {
        SensorClass *new_sensor = new SensorClass( port );

        if ( new_sensor->connected() ) {
            sensors_list[port] = new_sensor;
            return new_sensor;
        } else {
            delete new_sensor;
            error_process( __FUNCTION__, "Incorrect sensor port "+port );
            return( nullptr );
        }
    }
}

extern "C" 
{

  #include <ev3_sensor.h>
  #include <ev3_ports.h>

  ////////////////////////////////
  //**UltraSonic
  ////////////////////////////////
  float UltraSonicSensor_read_cm ( const char *sensor_port )
  {
      ultrasonic_sensor *us_ptr = get_sensor_ptr<ultrasonic_sensor>( std::string( sensor_port ) );

      return( us_ptr->distance_centimeters() );
  }

  ////////////////////////////////
  //**LightSonic
  ////////////////////////////////
  float LightSensor_read_percent ( const char *sensor_port, Light_s_mode_t mode )
  {
      light_sensor *light_ptr = get_sensor_ptr<light_sensor>( std::string( sensor_port ) );

      if ( mode == REFLECT )
          return( light_ptr->reflected_light_intensity() );
      else if ( mode == AMBIENT )
          return( light_ptr->ambient_light_intensity() );
      else
          return( 0.0f );
  }

  ////////////////////////////////-------
  //**TouchSonic
  ////////////////////////////////
  bool TouchSensor_isPressed ( const char *sensor_port )
  {
      touch_sensor *touch_ptr = get_sensor_ptr<touch_sensor>( std::string( sensor_port ) );

      if(!touch_ptr)
        return 0;

      return( touch_ptr->is_pressed() );
  }

  bool TouchSensor_Clicked ( const char *sensor_port)
  {

      touch_sensor *touch_ptr = get_sensor_ptr<touch_sensor>(std::string(sensor_port ) );

      if(!touch_ptr)
        return 0;

      return (touch_ptr->Clicked());

  }

  ////////////////////////////////
  //**ColorSonic
  ////////////////////////////////
  Color_s_color_t ColorSensor_get_color ( const char *sensor_port )
  {
      color_sensor *color_ptr = get_sensor_ptr<color_sensor>( std::string( sensor_port ) );

      int color = color_ptr->color();
      return( (color > COLOR_BROWN || color <= COLOR_NO) ? COLOR_NO : (Color_s_color_t)color );
  }

  uint8_t ColorSensor_get_reflected_light( const char *sensor_port)
  {

    color_sensor *CSensorPtr = get_sensor_ptr<color_sensor>(std::string(sensor_port));

    if(CSensorPtr)
    {

      
      
    }

  }


}

//-----------------------------------------------------------------------------

sensor::sensor(address_type address)
{
  connect({{ "address", { address }}});
}

//-----------------------------------------------------------------------------

sensor::sensor(address_type address, const std::set<sensor_type> &types)
{
  connect({{ "address", { address }},
           { "driver_name", types }});
}

//-----------------------------------------------------------------------------

bool sensor::connect(const std::map<std::string, std::set<std::string>> &match) noexcept
{
  static const std::string _strClassDir { SYS_ROOT "/lego-sensor/" };
  static const std::string _strPattern  { "sensor" };

  try
  {
    if (device::connect(_strClassDir, _strPattern, match))
    {
      return true;
    }
  }
  catch (...) { }

  _path.clear();

  return false;
}

//-----------------------------------------------------------------------------

std::string sensor::type_name() const
{
  auto type = driver_name();
  if (type.empty())
  {
    static const std::string s("<none>");
    return s;
  }

  static const std::map<sensor_type, const std::string> lookup_table {
    { ev3_touch,       "EV3 touch" },
    { ev3_color,       "EV3 color" },
    { ev3_ultrasonic,  "EV3 ultrasonic" },
    { ev3_gyro,        "EV3 gyro" },
    { ev3_infrared,    "EV3 infrared" },
    { nxt_touch,       "NXT touch" },
    { nxt_light,       "NXT light" },
    { nxt_sound,       "NXT sound" },
    { nxt_ultrasonic,  "NXT ultrasonic" },
    { nxt_i2c_sensor,  "I2C sensor" },
  };

  auto s = lookup_table.find(type);
  if (s != lookup_table.end())
    return s->second;

  return type;
}

//-----------------------------------------------------------------------------

int sensor::value(unsigned index) const
{
  if (static_cast<int>(index) >= num_values())
    throw std::invalid_argument("index");

  char svalue[7] = "value0";
  svalue[5] += index;

  return get_attr_int(svalue);
}

//-----------------------------------------------------------------------------

float sensor::float_value(unsigned index) const
{
    // int val = value( index );
    // return( val * powf(10, -decimals()) );
    return( value( index ) / (10.0 * decimals()) );
}

//-----------------------------------------------------------------------------
const std::vector<char>& sensor::bin_data() const
{
  using namespace std;

  if (_path.empty())
    throw system_error(make_error_code(errc::function_not_supported), "no device connected");

  if (_bin_data.empty()) {
    static const map<string, int> lookup_table {
      {"u8",     1},
      {"s8",     1},
      {"u16",    2},
      {"s16",    2},
      {"s16_be", 2},
      {"s32",    4},
      {"float",  4}
    };

    int value_size = 1;

    auto s = lookup_table.find(bin_data_format());
    if (s != lookup_table.end())
      value_size = s->second;

    _bin_data.resize(num_values() * value_size);
  }

  const string fname = _path + "bin_data";
  ifstream &is = ifstream_open(fname);
  if (is.is_open())
  {
    is.read(_bin_data.data(), _bin_data.size());
    return _bin_data;
  }

  throw system_error(make_error_code(errc::no_such_device), fname);
}

//-----------------------------------------------------------------------------

i2c_sensor::i2c_sensor(address_type address) :
  sensor(address, { nxt_i2c_sensor })
{
}

//-----------------------------------------------------------------------------

touch_sensor::touch_sensor(address_type address) :
  sensor(address, { ev3_touch, nxt_touch })
{
}

//-----------------------------------------------------------------------------

color_sensor::color_sensor(address_type address) :
  sensor(address, { ev3_color })
{
}

//-----------------------------------------------------------------------------

ultrasonic_sensor::ultrasonic_sensor(address_type address) :
  sensor(address, { ev3_ultrasonic, nxt_ultrasonic })
{
}

//-----------------------------------------------------------------------------

gyro_sensor::gyro_sensor(address_type address) :
  sensor(address, { ev3_gyro })
{
}

//-----------------------------------------------------------------------------

infrared_sensor::infrared_sensor(address_type address) :
  sensor(address, { ev3_infrared })
{
}

//-----------------------------------------------------------------------------

sound_sensor::sound_sensor(address_type address) :
  sensor(address, { nxt_sound, nxt_analog })
{
    if (connected() && driver_name() == nxt_analog) {
        lego_port port(address);

        if (port.connected()) {
            port.set_set_device(nxt_sound);

            if (port.status() != nxt_sound) {
                // Failed to load lego-nxt-sound friver. Wrong port?
                _path.clear();
            }
        } else {
            _path.clear();
        }
    }
}

//-----------------------------------------------------------------------------

light_sensor::light_sensor(address_type address) :
  sensor(address, { nxt_light })
{
}

//-----------------------------------------------------------------------------

lego_port::lego_port(address_type address)
{
  connect({{ "address", { address } }});
}

//-----------------------------------------------------------------------------

bool lego_port::connect(const std::map<std::string, std::set<std::string>> &match) noexcept
{
  static const std::string _strClassDir { SYS_ROOT "/lego-port/" };
  static const std::string _strPattern  { "port" };

  try
  {
    return device::connect(_strClassDir, _strPattern, match);
  }
  catch (...) { }

  _path.clear();

  return false;
}

constexpr char sensor::ev3_touch[];
constexpr char sensor::ev3_color[];
constexpr char sensor::ev3_ultrasonic[];
constexpr char sensor::ev3_gyro[];
constexpr char sensor::ev3_infrared[];
constexpr char sensor::nxt_touch[];
constexpr char sensor::nxt_light[];
constexpr char sensor::nxt_sound[];
constexpr char sensor::nxt_ultrasonic[];
constexpr char sensor::nxt_i2c_sensor[];
constexpr char sensor::nxt_analog[];     

constexpr char touch_sensor::mode_touch[];

constexpr char color_sensor::mode_col_reflect[];
constexpr char color_sensor::mode_col_ambient[];
constexpr char color_sensor::mode_col_color[];
constexpr char color_sensor::mode_ref_raw[];
constexpr char color_sensor::mode_rgb_raw[];

constexpr char ultrasonic_sensor::mode_us_dist_cm[];
constexpr char ultrasonic_sensor::mode_us_dist_in[];
constexpr char ultrasonic_sensor::mode_us_listen[];
constexpr char ultrasonic_sensor::mode_us_si_cm[];
constexpr char ultrasonic_sensor::mode_us_si_in[];

constexpr char gyro_sensor::mode_gyro_ang[];
constexpr char gyro_sensor::mode_gyro_rate[];
constexpr char gyro_sensor::mode_gyro_fas[];
constexpr char gyro_sensor::mode_gyro_g_a[];
constexpr char gyro_sensor::mode_gyro_cal[];

constexpr char infrared_sensor::mode_ir_prox[];
constexpr char infrared_sensor::mode_ir_seek[];
constexpr char infrared_sensor::mode_ir_remote[];
constexpr char infrared_sensor::mode_ir_rem_a[];
constexpr char infrared_sensor::mode_ir_cal[];

constexpr char sound_sensor::mode_db[];
constexpr char sound_sensor::mode_dba[];

constexpr char light_sensor::mode_reflect[];
constexpr char light_sensor::mode_ambient[];
