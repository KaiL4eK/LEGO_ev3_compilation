
#include <ev3_core.h>
#include <sensor.h>
#include <lego_port.h>

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
  return value(index) * powf(10, -decimals());
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

//~autogen generic-define-property-value specialSensorTypes.touchSensor>currentClass

constexpr char touch_sensor::mode_touch[];

//~autogen

touch_sensor::touch_sensor(address_type address) :
  sensor(address, { ev3_touch, nxt_touch })
{
}

//-----------------------------------------------------------------------------

//~autogen generic-define-property-value specialSensorTypes.colorSensor>currentClass

constexpr char color_sensor::mode_col_reflect[];
constexpr char color_sensor::mode_col_ambient[];
constexpr char color_sensor::mode_col_color[];
constexpr char color_sensor::mode_ref_raw[];
constexpr char color_sensor::mode_rgb_raw[];

//~autogen

color_sensor::color_sensor(address_type address) :
  sensor(address, { ev3_color })
{
}

//-----------------------------------------------------------------------------

//~autogen generic-define-property-value specialSensorTypes.ultrasonicSensor>currentClass

constexpr char ultrasonic_sensor::mode_us_dist_cm[];
constexpr char ultrasonic_sensor::mode_us_dist_in[];
constexpr char ultrasonic_sensor::mode_us_listen[];
constexpr char ultrasonic_sensor::mode_us_si_cm[];
constexpr char ultrasonic_sensor::mode_us_si_in[];

//~autogen

ultrasonic_sensor::ultrasonic_sensor(address_type address) :
  sensor(address, { ev3_ultrasonic, nxt_ultrasonic })
{
}

//-----------------------------------------------------------------------------

//~autogen generic-define-property-value specialSensorTypes.gyroSensor>currentClass

constexpr char gyro_sensor::mode_gyro_ang[];
constexpr char gyro_sensor::mode_gyro_rate[];
constexpr char gyro_sensor::mode_gyro_fas[];
constexpr char gyro_sensor::mode_gyro_g_a[];
constexpr char gyro_sensor::mode_gyro_cal[];

//~autogen

gyro_sensor::gyro_sensor(address_type address) :
  sensor(address, { ev3_gyro })
{
}

//-----------------------------------------------------------------------------

//~autogen generic-define-property-value specialSensorTypes.infraredSensor>currentClass

constexpr char infrared_sensor::mode_ir_prox[];
constexpr char infrared_sensor::mode_ir_seek[];
constexpr char infrared_sensor::mode_ir_remote[];
constexpr char infrared_sensor::mode_ir_rem_a[];
constexpr char infrared_sensor::mode_ir_cal[];

//~autogen

infrared_sensor::infrared_sensor(address_type address) :
  sensor(address, { ev3_infrared })
{
}

//-----------------------------------------------------------------------------

//~autogen generic-define-property-value specialSensorTypes.soundSensor>currentClass

constexpr char sound_sensor::mode_db[];
constexpr char sound_sensor::mode_dba[];

//~autogen

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

//~autogen generic-define-property-value specialSensorTypes.lightSensor>currentClass

constexpr char light_sensor::mode_reflect[];
constexpr char light_sensor::mode_ambient[];

//~autogen

light_sensor::light_sensor(address_type address) :
  sensor(address, { nxt_light })
{
}

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
