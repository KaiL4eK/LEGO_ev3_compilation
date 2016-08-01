#ifndef SENSOR_H_
#define SENSOR_H_

#include <device.h>

class sensor : protected device
{
public:
  typedef device_type sensor_type;

  static constexpr char ev3_touch[]      = "lego-ev3-touch";
  static constexpr char ev3_color[]      = "lego-ev3-color";
  static constexpr char ev3_ultrasonic[] = "lego-ev3-us";
  static constexpr char ev3_gyro[]       = "lego-ev3-gyro";
  static constexpr char ev3_infrared[]   = "lego-ev3-ir";

  static constexpr char nxt_touch[]      = "lego-nxt-touch";
  static constexpr char nxt_light[]      = "lego-nxt-light";
  static constexpr char nxt_sound[]      = "lego-nxt-sound";
  static constexpr char nxt_ultrasonic[] = "lego-nxt-us";
  static constexpr char nxt_i2c_sensor[] = "nxt-i2c-sensor";
  static constexpr char nxt_analog[]     = "nxt-analog";

  sensor(address_type);
  sensor(address_type, const std::set<sensor_type>&);

  using device::connected;
  using device::device_index;

  // Returns the value or values measured by the sensor. Check `num_values` to
  // see how many values there are. Values with index >= num_values will return
  // an error. The values are fixed point numbers, so check `decimals` to see
  // if you need to divide to get the actual value.
  int   value(unsigned index=0) const;

  // The value converted to float using `decimals`.
  float float_value(unsigned index=0) const;

  // Human-readable name of the connected sensor.
  std::string type_name() const;

  // Bin Data Format: read-only
  // Returns the format of the values in `bin_data` for the current mode.
  // Possible values are:
  //
  //    - `u8`: Unsigned 8-bit integer (byte)
  //    - `s8`: Signed 8-bit integer (sbyte)
  //    - `u16`: Unsigned 16-bit integer (ushort)
  //    - `s16`: Signed 16-bit integer (short)
  //    - `s16_be`: Signed 16-bit integer, big endian
  //    - `s32`: Signed 32-bit integer (int)
  //    - `float`: IEEE 754 32-bit floating point (float)
  std::string bin_data_format() const { return get_attr_string("bin_data_format"); };

  // Bin Data: read-only
  // Returns the unscaled raw values in the `value<N>` attributes as raw byte
  // array. Use `bin_data_format`, `num_values` and the individual sensor
  // documentation to determine how to interpret the data.
  const std::vector<char>& bin_data() const;

  // Bin Data: read-only
  // Writes the unscaled raw values in the `value<N>` attributes into the
  // user-provided struct/buffer.  Use `bin_data_format`, `num_values` and the
  // individual sensor documentation to determine how to interpret the data.
  template <class T>
  void bin_data(T *buf) const {
      bin_data(); // fills _bin_data
      std::copy_n(_bin_data.data(), _bin_data.size(), static_cast<char*>(buf));
  }

  // Address: read-only
  // Returns the name of the port that the sensor is connected to, e.g. `ev3:in1`.
  // I2C sensors also include the I2C address (decimal), e.g. `ev3:in1:i2c8`.
  std::string address() const { return get_attr_string("address"); }

  // Command: write-only
  // Sends a command to the sensor.
  auto set_command(std::string v) -> decltype(*this) {
    set_attr_string("command", v);
    return *this;
  }

  // Commands: read-only
  // Returns a list of the valid commands for the sensor.
  // Returns -EOPNOTSUPP if no commands are supported.
  mode_set commands() const { return get_attr_set("commands"); }

  // Decimals: read-only
  // Returns the number of decimal places for the values in the `value<N>`
  // attributes of the current mode.
  int decimals() const { return get_attr_int("decimals"); }

  // Driver Name: read-only
  // Returns the name of the sensor device/driver. See the list of [supported
  // sensors] for a complete list of drivers.
  std::string driver_name() const { return get_attr_string("driver_name"); }

  //
  bool check_mode(std::string mode_to_check)
  {
    if(!this->mode().compare(mode_to_check))
      return true;
    else
      return false;
  }

  // Mode: read/write
  // Returns the current mode. Writing one of the values returned by `modes`
  // sets the sensor to that mode.
  std::string mode() const { return get_attr_string("mode"); }
  auto set_mode(std::string v) -> decltype(*this) {
    set_attr_string("mode", v);
    return *this;
  }

  // Modes: read-only
  // Returns a list of the valid modes for the sensor.
  mode_set modes() const { return get_attr_set("modes"); }

  // Num Values: read-only
  // Returns the number of `value<N>` attributes that will return a valid value
  // for the current mode.
  int num_values() const { return get_attr_int("num_values"); }

  // Units: read-only
  // Returns the units of the measured value for the current mode. May return
  // empty string
  std::string units() const { return get_attr_string("units"); }

protected:
  sensor() {}

  bool connect(const std::map<std::string, std::set<std::string>>&) noexcept;

  mutable std::vector<char> _bin_data;
};

//-----------------------------------------------------------------------------

// A generic interface to control I2C-type EV3 sensors.

//~autogen
class i2c_sensor : public sensor
{
public:
  i2c_sensor(address_type address = INPUT_AUTO);

//~autogen generic-get-set classes.i2cSensor>currentClass

  // FW Version: read-only
  // Returns the firmware version of the sensor if available. Currently only
  // I2C/NXT sensors support this.
  std::string fw_version() const { return get_attr_string("fw_version"); }

  // Poll MS: read/write
  // Returns the polling period of the sensor in milliseconds. Writing sets the
  // polling period. Setting to 0 disables polling. Minimum value is hard
  // coded as 50 msec. Returns -EOPNOTSUPP if changing polling is not supported.
  // Currently only I2C/NXT sensors support changing the polling period.
  int poll_ms() const { return get_attr_int("poll_ms"); }
  auto set_poll_ms(int v) -> decltype(*this) {
    set_attr_int("poll_ms", v);
    return *this;
  }
};

//-----------------------------------------------------------------------------

#define TS_DEFAULT 0
#define TS_PRESSED 1

// Touch Sensor
class touch_sensor : public sensor
{
public:
  touch_sensor(address_type address = INPUT_AUTO);

  // Button state
  static constexpr char mode_touch[] = "TOUCH";


  // A boolean indicating whether the current touch sensor is being
  // pressed.
  bool is_pressed() 
  {
    set_mode(mode_touch);
    return value(0);
  }

  // Return true, if sensor was pressed and released
  bool Clicked()
  {

    if(this->Status == TS_DEFAULT && this->is_pressed())
    {
      this->Status = TS_PRESSED;
      return 0;
    }
    if(this->Status == TS_PRESSED && !this->is_pressed())
    {
      this->Status = TS_DEFAULT;
      return 1;
    }

    return 0;

  }

private:

  char Status;

};

//-----------------------------------------------------------------------------

// LEGO EV3 color sensor.
class color_sensor : public sensor
{
public:
  color_sensor(address_type address = INPUT_AUTO);

  // Reflected light. Red LED on.
  static constexpr char mode_col_reflect[] = "COL-REFLECT";

  // Ambient light. Red LEDs off.
  static constexpr char mode_col_ambient[] = "COL-AMBIENT";

  // Color. All LEDs rapidly cycling, appears white.
  static constexpr char mode_col_color[] = "COL-COLOR";

  // Raw reflected. Red LED on
  static constexpr char mode_ref_raw[] = "REF-RAW";

  // Raw Color Components. All LEDs rapidly cycling, appears white.
  static constexpr char mode_rgb_raw[] = "RGB-RAW";

  // Reflected light intensity as a percentage. Light on sensor is red.
  int reflected_light_intensity() {
    set_mode(mode_col_reflect);
    return value(0);
  }

  // Ambient light intensity. Light on sensor is dimly lit blue.
  int ambient_light_intensity() {
    set_mode(mode_col_ambient);
    return value(0);
  }

  // Color detected by the sensor, categorized by overall value.
  //   - 0: No color
  //   - 1: Black
  //   - 2: Blue
  //   - 3: Green
  //   - 4: Yellow
  //   - 5: Red
  //   - 6: White
  //   - 7: Brown
  int color() {
    set_mode(mode_col_color);
    return value(0);
  }

  // Returns ReflectedLight
  uint8_t reflect()
  {

    set_mode(mode_col_reflect);
    this->l_reflect_light = value(0);
    return this->l_reflect_light;

  }

  // Returns Ambient light
  uint8_t ambient()
  {

    set_mode(mode_col_ambient);
    return this->value(0);

  }

  // Red component of the detected color, in the range 0-1020.
  int red() {
    set_mode(mode_rgb_raw);
    return value(0);
  }

  // Green component of the detected color, in the range 0-1020.
  int green() {
    set_mode(mode_rgb_raw);
    return value(1);
  }

  // Blue component of the detected color, in the range 0-1020.
  int blue() {
    set_mode(mode_rgb_raw);
    return value(2);
  }

private:

  //Contains last detected light reflect
  uint8_t l_reflect_light;

};

//-----------------------------------------------------------------------------

// LEGO EV3 ultrasonic sensor.
class ultrasonic_sensor : public sensor
{
public:
  ultrasonic_sensor(address_type address = INPUT_AUTO);

  // Continuous measurement in centimeters.
  static constexpr char mode_us_dist_cm[] = "US-DIST-CM";

  // Continuous measurement in inches.
  static constexpr char mode_us_dist_in[] = "US-DIST-IN";

  // Listen.
  static constexpr char mode_us_listen[] = "US-LISTEN";

  // Single measurement in centimeters.
  static constexpr char mode_us_si_cm[] = "US-SI-CM";

  // Single measurement in inches.
  static constexpr char mode_us_si_in[] = "US-SI-IN";


  // Measurement of the distance detected by the sensor,
  // in centimeters.
  float distance_centimeters() {
    if(!this->check_mode(mode_us_dist_cm))
        set_mode(mode_us_dist_cm);
    return float_value(0);
  }

  // Measurement of the distance detected by the sensor,
  // in inches.
  float distance_inches() {
    if(!this->check_mode(mode_us_dist_in))
        set_mode(mode_us_dist_in);
    return float_value(0);
  }

  // Value indicating whether another ultrasonic sensor could
  // be heard nearby.
  bool other_sensor_present() {
    if(!this->check_mode(mode_us_listen))
        set_mode(mode_us_listen);
    return value(0);
  }

  //Reterns measurement of distence detected by the sensor,
  //in centimeters
  //Use Single measurement mode
  float distance_centimeters_sm()
  {
    set_mode(mode_us_si_cm);
    return float_value(0);
  }

  //Reterns measurement of distence detected by the sensor, in inches
  //Use Single measurement mode
  float distance_inches_sm()
  {
    set_mode(mode_us_si_in);
    return float_value(0);
  }

};

//-----------------------------------------------------------------------------

// LEGO EV3 gyro sensor.
class gyro_sensor : public sensor
{
public:
  gyro_sensor(address_type address = INPUT_AUTO);

  // Angle
  static constexpr char mode_gyro_ang[] = "GYRO-ANG";

  // Rotational speed
  static constexpr char mode_gyro_rate[] = "GYRO-RATE";

  // Raw sensor value
  static constexpr char mode_gyro_fas[] = "GYRO-FAS";

  // Angle and rotational speed
  static constexpr char mode_gyro_g_a[] = "GYRO-G&A";

  // Calibration ???
  static constexpr char mode_gyro_cal[] = "GYRO-CAL";


  // The number of degrees that the sensor has been rotated
  // since it was put into this mode.
  int angle() {
    if(!check_mode(mode_gyro_ang))
      set_mode(mode_gyro_ang);
    return value(0);
  }

  // The rate at which the sensor is rotating, in degrees/second.
  int rate() {
    if(!check_mode(mode_gyro_rate))
      set_mode(mode_gyro_rate);
    return value(0);
  }

  void calibrate()
  {
    set_mode(mode_gyro_cal);
  }

  int16_t acceleration()
  {
    if(!check_mode(mode_gyro_fas))
      set_mode(mode_gyro_fas);

    return value(0);

  }

};

//-----------------------------------------------------------------------------

// LEGO EV3 infrared sensor.
class infrared_sensor : public sensor
{
public:
  infrared_sensor(address_type address = INPUT_AUTO);

  // Proximity
  static constexpr char mode_ir_prox[] = "IR-PROX";

  // IR Seeker
  static constexpr char mode_ir_seek[] = "IR-SEEK";

  // IR Remote Control
  static constexpr char mode_ir_remote[] = "IR-REMOTE";

  // IR Remote Control. State of the buttons is coded in binary
  static constexpr char mode_ir_rem_a[] = "IR-REM-A";

  // Calibration ???
  static constexpr char mode_ir_cal[] = "IR-CAL";


  // A measurement of the distance between the sensor and the remote,
  // as a percentage. 100% is approximately 70cm/27in.
  int proximity() {
    set_mode(mode_ir_prox);
    return value(0);
  }

};

//-----------------------------------------------------------------------------

// LEGO NXT Sound Sensor
class sound_sensor : public sensor
{
public:
  sound_sensor(address_type address = INPUT_AUTO);

  // Sound pressure level. Flat weighting
  static constexpr char mode_db[] = "DB";

  // Sound pressure level. A weighting
  static constexpr char mode_dba[] = "DBA";


  // A measurement of the measured sound pressure level, as a
  // percent. Uses a flat weighting.
  float sound_pressure() {
    set_mode(mode_db);
    return float_value(0);
  }

  // A measurement of the measured sound pressure level, as a
  // percent. Uses A-weighting, which focuses on levels up to 55 dB.
  float sound_pressure_low() {
    set_mode(mode_dba);
    return float_value(0);
  }

};

//-----------------------------------------------------------------------------

// LEGO NXT Light Sensor
class light_sensor : public sensor
{
public:
  light_sensor(address_type address = INPUT_AUTO);

  // Reflected light. LED on
  static constexpr char mode_reflect[] = "REFLECT";

  // Ambient light. LED off
  static constexpr char mode_ambient[] = "AMBIENT";


  // A measurement of the reflected light intensity, as a percentage.
  float reflected_light_intensity() {
    set_mode(mode_reflect);
    return float_value(0);
  }

  // A measurement of the ambient light intensity, as a percentage.
  float ambient_light_intensity() {
    set_mode(mode_ambient);
    return float_value(0);
  }

};

#endif // SENSOR_H_
