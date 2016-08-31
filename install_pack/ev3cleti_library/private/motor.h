#ifndef MOTOR_H_
#define MOTOR_H_

// The motor class provides a uniform interface for using motors with
// positional and directional feedback such as the EV3 and NXT motors.
// This feedback allows for precise control of the motors. This is the
// most common type of motor, so we just call it `motor`.
// 
// The way to configure a motor is to set the '_sp' attributes when
// calling a command or before. Only in 'run_direct' mode attribute
// changes are processed immediately, in the other modes they only
// take place when a new command is issued.

#include <device.h>

class motor : protected device
{
public:
  typedef device_type motor_type;

  motor(address_type);
  motor(address_type, const motor_type&);

  static constexpr char motor_large[]  = "lego-ev3-l-motor";
  static constexpr char motor_medium[] = "lego-ev3-m-motor";

  using device::connected;
  using device::device_index;

  // Run the motor until another command is sent.
  static constexpr char command_run_forever[] = "run-forever";

  // Run to an absolute position specified by `position_sp` and then
  // stop using the action specified in `stop_action`.
  static constexpr char command_run_to_abs_pos[] = "run-to-abs-pos";

  // Run to a position relative to the current `position` value.
  // The new position will be current `position` + `position_sp`.
  // When the new position is reached, the motor will stop using
  // the action specified by `stop_action`.
  static constexpr char command_run_to_rel_pos[] = "run-to-rel-pos";

  // Run the motor for the amount of time specified in `time_sp`
  // and then stop the motor using the action specified by `stop_action`.
  static constexpr char command_run_timed[] = "run-timed";

  // Run the motor at the duty cycle specified by `duty_cycle_sp`.
  // Unlike other run commands, changing `duty_cycle_sp` while running *will*
  // take effect immediately.
  static constexpr char command_run_direct[] = "run-direct";

  // Stop any of the run commands before they are complete using the
  // action specified by `stop_action`.
  static constexpr char command_stop[] = "stop";

  // Reset all of the motor parameter attributes to their default value.
  // This will also have the effect of stopping the motor.
  static constexpr char command_reset[] = "reset";

  // Sets the normal polarity of the rotary encoder.
  static constexpr char encoder_polarity_normal[] = "normal";

  // Sets the inversed polarity of the rotary encoder.
  static constexpr char encoder_polarity_inversed[] = "inversed";

  // With `normal` polarity, a positive duty cycle will
  // cause the motor to rotate clockwise.
  static constexpr char polarity_normal[] = "normal";

  // With `inversed` polarity, a positive duty cycle will
  // cause the motor to rotate counter-clockwise.
  static constexpr char polarity_inversed[] = "inversed";

  // Power is being sent to the motor.
  static constexpr char state_running[] = "running";

  // The motor is ramping up or down and has not yet reached a constant output level.
  static constexpr char state_ramping[] = "ramping";

  // The motor is not turning, but rather attempting to hold a fixed position.
  static constexpr char state_holding[] = "holding";

  // The motor is turning, but cannot reach its `speed_sp`.
  static constexpr char state_overloaded[] = "overloaded";

  // The motor is not turning when it should be.
  static constexpr char state_stalled[] = "stalled";

  // Power will be removed from the motor and it will freely coast to a stop.
  static constexpr char stop_action_coast[] = "coast";

  // Power will be removed from the motor and a passive electrical load will
  // be placed on the motor. This is usually done by shorting the motor terminals
  // together. This load will absorb the energy from the rotation of the motors and
  // cause the motor to stop more quickly than coasting.
  static constexpr char stop_action_brake[] = "brake";

  // Does not remove power from the motor. Instead it actively try to hold the motor
  // at the current position. If an external force tries to turn the motor, the motor
  // will `push back` to maintain its position.
  static constexpr char stop_action_hold[] = "hold";

  // Address: read-only
  // Returns the name of the port that this motor is connected to.
  std::string address() const { return get_attr_string("address"); }

  // Command: write-only
  // Sends a command to the motor controller. See `commands` for a list of
  // possible values.
  auto set_command(std::string v) -> decltype(*this) {
    set_attr_string("command", v);
    return *this;
  }

  // Commands: read-only
  // Returns a list of commands that are supported by the motor
  // controller. Possible values are `run-forever`, `run-to-abs-pos`, `run-to-rel-pos`,
  // `run-timed`, `run-direct`, `stop` and `reset`. Not all commands may be supported.
  // 
  // - `run-forever` will cause the motor to run until another command is sent.
  // - `run-to-abs-pos` will run to an absolute position specified by `position_sp`
  //   and then stop using the action specified in `stop_action`.
  // - `run-to-rel-pos` will run to a position relative to the current `position` value.
  //   The new position will be current `position` + `position_sp`. When the new
  //   position is reached, the motor will stop using the action specified by `stop_action`.
  // - `run-timed` will run the motor for the amount of time specified in `time_sp`
  //   and then stop the motor using the action specified by `stop_action`.
  // - `run-direct` will run the motor at the duty cycle specified by `duty_cycle_sp`.
  //   Unlike other run commands, changing `duty_cycle_sp` while running *will*
  //   take effect immediately.
  // - `stop` will stop any of the run commands before they are complete using the
  //   action specified by `stop_action`.
  // - `reset` will reset all of the motor parameter attributes to their default value.
  //   This will also have the effect of stopping the motor.
  mode_set commands() const { return get_attr_set("commands"); }

  // Count Per Rot: read-only
  // Returns the number of tacho counts in one rotation of the motor. Tacho counts
  // are used by the position and speed attributes, so you can use this value
  // to convert rotations or degrees to tacho counts. (rotation motors only)
  int count_per_rot() const { return get_attr_int("count_per_rot"); }

  // Count Per M: read-only
  // Returns the number of tacho counts in one meter of travel of the motor. Tacho
  // counts are used by the position and speed attributes, so you can use this
  // value to convert from distance to tacho counts. (linear motors only)
  int count_per_m() const { return get_attr_int("count_per_m"); }

  // Driver Name: read-only
  // Returns the name of the driver that provides this tacho motor device.
  std::string driver_name() const { return get_attr_string("driver_name"); }

  // Duty Cycle: read-only
  // Returns the current duty cycle of the motor. Units are percent. Values
  // are -100 to 100.
  int duty_cycle() const { return get_attr_int("duty_cycle"); }

  // Duty Cycle SP: read/write
  // Writing sets the duty cycle setpoint. Reading returns the current value.
  // Units are in percent. Valid values are -100 to 100. A negative value causes
  // the motor to rotate in reverse.
  int duty_cycle_sp() const { return get_attr_int("duty_cycle_sp"); }
  auto set_duty_cycle_sp(int v) -> decltype(*this) {
    clip_arg(v, -100, 100);
    set_attr_int("duty_cycle_sp", v);
    return *this;
  }

  // Full Travel Count: read-only
  // Returns the number of tacho counts in the full travel of the motor. When
  // combined with the `count_per_m` atribute, you can use this value to
  // calculate the maximum travel distance of the motor. (linear motors only)
  int full_travel_count() const { return get_attr_int("full_travel_count"); }

  // Polarity: read/write
  // Sets the polarity of the motor. With `normal` polarity, a positive duty
  // cycle will cause the motor to rotate clockwise. With `inversed` polarity,
  // a positive duty cycle will cause the motor to rotate counter-clockwise.
  // Valid values are `normal` and `inversed`.
  std::string polarity() const { return get_attr_string("polarity"); }
  auto set_polarity(std::string v) -> decltype(*this) {
    set_attr_string("polarity", v);
    return *this;
  }

  // Position: read/write
  // Returns the current position of the motor in pulses of the rotary
  // encoder. When the motor rotates clockwise, the position will increase.
  // Likewise, rotating counter-clockwise causes the position to decrease.
  // Writing will set the position to that value.
  int position() const { return get_attr_int("position"); }
  auto set_position(int v) -> decltype(*this) {
    set_attr_int("position", v);
    return *this;
  }

  // Position P: read/write
  // The proportional constant for the position PID.
  int position_p() const { return get_attr_int("hold_pid/Kp"); }
  auto set_position_p(int v) -> decltype(*this) {
    set_attr_int("hold_pid/Kp", v);
    return *this;
  }

  // Position I: read/write
  // The integral constant for the position PID.
  int position_i() const { return get_attr_int("hold_pid/Ki"); }
  auto set_position_i(int v) -> decltype(*this) {
    set_attr_int("hold_pid/Ki", v);
    return *this;
  }

  // Position D: read/write
  // The derivative constant for the position PID.
  int position_d() const { return get_attr_int("hold_pid/Kd"); }
  auto set_position_d(int v) -> decltype(*this) {
    set_attr_int("hold_pid/Kd", v);
    return *this;
  }

  // Position SP: read/write
  // Writing specifies the target position for the `run-to-abs-pos` and `run-to-rel-pos`
  // commands. Reading returns the current value. Units are in tacho counts. You
  // can use the value returned by `counts_per_rot` to convert tacho counts to/from
  // rotations or degrees.
  int position_sp() const { return get_attr_int("position_sp"); }
  auto set_position_sp(int v) -> decltype(*this) {
    set_attr_int("position_sp", v);
    return *this;
  }

  // Max Speed: read-only
  // Returns the maximum value that is accepted by the `speed_sp` attribute. This
  // may be slightly different than the maximum speed that a particular motor can
  // reach - it's the maximum theoretical speed.
  int max_speed() const { return get_attr_int("max_speed"); }

  // Speed: read-only
  // Returns the current motor speed in tacho counts per second. Note, this is
  // not necessarily degrees (although it is for LEGO motors). Use the `count_per_rot`
  // attribute to convert this value to RPM or deg/sec.
  int speed() const { return get_attr_int("speed"); }

  // Speed SP: read/write
  // Writing sets the target speed in tacho counts per second used for all `run-*`
  // commands except `run-direct`. Reading returns the current value. A negative
  // value causes the motor to rotate in reverse with the exception of `run-to-*-pos`
  // commands where the sign is ignored. Use the `count_per_rot` attribute to convert
  // RPM or deg/sec to tacho counts per second. Use the `count_per_m` attribute to
  // convert m/s to tacho counts per second.
  int speed_sp() const { return get_attr_int("speed_sp"); }
  auto set_speed_sp(int v) -> decltype(*this) {
    set_attr_int("speed_sp", v);
    return *this;
  }

  // Ramp Up SP: read/write
  // Writing sets the ramp up setpoint. Reading returns the current value. Units
  // are in milliseconds and must be positive. When set to a non-zero value, the
  // motor speed will increase from 0 to 100% of `max_speed` over the span of this
  // setpoint. The actual ramp time is the ratio of the difference between the
  // `speed_sp` and the current `speed` and max_speed multiplied by `ramp_up_sp`.
  int ramp_up_sp() const { return get_attr_int("ramp_up_sp"); }
  auto set_ramp_up_sp(int v) -> decltype(*this) {
    set_attr_int("ramp_up_sp", v);
    return *this;
  }

  // Ramp Down SP: read/write
  // Writing sets the ramp down setpoint. Reading returns the current value. Units
  // are in milliseconds and must be positive. When set to a non-zero value, the
  // motor speed will decrease from 0 to 100% of `max_speed` over the span of this
  // setpoint. The actual ramp time is the ratio of the difference between the
  // `speed_sp` and the current `speed` and max_speed multiplied by `ramp_down_sp`.
  int ramp_down_sp() const { return get_attr_int("ramp_down_sp"); }
  auto set_ramp_down_sp(int v) -> decltype(*this) {
    set_attr_int("ramp_down_sp", v);
    return *this;
  }

  // Speed P: read/write
  // The proportional constant for the speed regulation PID.
  int speed_p() const { return get_attr_int("speed_pid/Kp"); }
  auto set_speed_p(int v) -> decltype(*this) {
    set_attr_int("speed_pid/Kp", v);
    return *this;
  }

  // Speed I: read/write
  // The integral constant for the speed regulation PID.
  int speed_i() const { return get_attr_int("speed_pid/Ki"); }
  auto set_speed_i(int v) -> decltype(*this) {
    set_attr_int("speed_pid/Ki", v);
    return *this;
  }

  // Speed D: read/write
  // The derivative constant for the speed regulation PID.
  int speed_d() const { return get_attr_int("speed_pid/Kd"); }
  auto set_speed_d(int v) -> decltype(*this) {
    set_attr_int("speed_pid/Kd", v);
    return *this;
  }

  // State: read-only
  // Reading returns a list of state flags. Possible flags are
  // `running`, `ramping`, `holding`, `overloaded` and `stalled`.
  mode_set state() const { return get_attr_set("state"); }

  // Stop Action: read/write
  // Reading returns the current stop action. Writing sets the stop action.
  // The value determines the motors behavior when `command` is set to `stop`.
  // Also, it determines the motors behavior when a run command completes. See
  // `stop_actions` for a list of possible values.
  std::string stop_action() const { return get_attr_string("stop_action"); }
  auto set_stop_action(std::string v) -> decltype(*this) {
    set_attr_string("stop_action", v);
    return *this;
  }

  // Stop Actions: read-only
  // Returns a list of stop actions supported by the motor controller.
  // Possible values are `coast`, `brake` and `hold`. `coast` means that power will
  // be removed from the motor and it will freely coast to a stop. `brake` means
  // that power will be removed from the motor and a passive electrical load will
  // be placed on the motor. This is usually done by shorting the motor terminals
  // together. This load will absorb the energy from the rotation of the motors and
  // cause the motor to stop more quickly than coasting. `hold` does not remove
  // power from the motor. Instead it actively tries to hold the motor at the current
  // position. If an external force tries to turn the motor, the motor will 'push
  // back' to maintain its position.
  mode_set stop_actions() const { return get_attr_set("stop_actions"); }

  // Time SP: read/write
  // Writing specifies the amount of time the motor will run when using the
  // `run-timed` command. Reading returns the current value. Units are in
  // milliseconds.
  int time_sp() const { return get_attr_int("time_sp"); }
  auto set_time_sp(int v) -> decltype(*this) {
    set_attr_int("time_sp", v);
    return *this;
  }

  // Run the motor until another command is sent.
  void run_forever() { set_command("run-forever"); }

  // Run to an absolute position specified by `position_sp` and then
  // stop using the action specified in `stop_action`.
  void run_to_abs_pos() { set_command("run-to-abs-pos"); }

  // Run to a position relative to the current `position` value.
  // The new position will be current `position` + `position_sp`.
  // When the new position is reached, the motor will stop using
  // the action specified by `stop_action`.
  void run_to_rel_pos() { set_command("run-to-rel-pos"); }

  // Run the motor for the amount of time specified in `time_sp`
  // and then stop the motor using the action specified by `stop_action`.
  void run_timed() { set_command("run-timed"); }

  // Run the motor at the duty cycle specified by `duty_cycle_sp`.
  // Unlike other run commands, changing `duty_cycle_sp` while running *will*
  // take effect immediately.
  void run_direct() { set_command("run-direct"); }

  // Stop any of the run commands before they are complete using the
  // action specified by `stop_action`.
  void stop() { set_command("stop"); }

  // Reset all of the motor parameter attributes to their default value.
  // This will also have the effect of stopping the motor.
  void reset() { set_command("reset"); }

  //bool getIsState(char*

protected:
  motor() {}

  bool connect(const std::map<std::string, std::set<std::string>>&) noexcept;
};

//-----------------------------------------------------------------------------

// The DC motor class provides a uniform interface for using regular DC motors
// with no fancy controls or feedback. This includes LEGO MINDSTORMS RCX motors
// and LEGO Power Functions motors.

class dc_motor : protected device
{
public:
  dc_motor(address_type address = OUTPUT_AUTO);

  using device::connected;
  using device::device_index;

  // Run the motor until another command is sent.
  static constexpr char command_run_forever[] = "run-forever";

  // Run the motor for the amount of time specified in `time_sp`
  // and then stop the motor using the action specified by `stop_action`.
  static constexpr char command_run_timed[] = "run-timed";

  // Run the motor at the duty cycle specified by `duty_cycle_sp`.
  // Unlike other run commands, changing `duty_cycle_sp` while running *will*
  // take effect immediately.
  static constexpr char command_run_direct[] = "run-direct";

  // Stop any of the run commands before they are complete using the
  // action specified by `stop_action`.
  static constexpr char command_stop[] = "stop";

  // With `normal` polarity, a positive duty cycle will
  // cause the motor to rotate clockwise.
  static constexpr char polarity_normal[] = "normal";

  // With `inversed` polarity, a positive duty cycle will
  // cause the motor to rotate counter-clockwise.
  static constexpr char polarity_inversed[] = "inversed";

  // Power will be removed from the motor and it will freely coast to a stop.
  static constexpr char stop_action_coast[] = "coast";

  // Power will be removed from the motor and a passive electrical load will
  // be placed on the motor. This is usually done by shorting the motor terminals
  // together. This load will absorb the energy from the rotation of the motors and
  // cause the motor to stop more quickly than coasting.
  static constexpr char stop_action_brake[] = "brake";

  // Address: read-only
  // Returns the name of the port that this motor is connected to.
  std::string address() const { return get_attr_string("address"); }

  // Command: write-only
  // Sets the command for the motor. Possible values are `run-forever`, `run-timed` and
  // `stop`. Not all commands may be supported, so be sure to check the contents
  // of the `commands` attribute.
  auto set_command(std::string v) -> decltype(*this) {
    set_attr_string("command", v);
    return *this;
  }

  // Commands: read-only
  // Returns a list of commands supported by the motor
  // controller.
  mode_set commands() const { return get_attr_set("commands"); }

  // Driver Name: read-only
  // Returns the name of the motor driver that loaded this device. See the list
  // of [supported devices] for a list of drivers.
  std::string driver_name() const { return get_attr_string("driver_name"); }

  // Duty Cycle: read-only
  // Shows the current duty cycle of the PWM signal sent to the motor. Values
  // are -100 to 100 (-100% to 100%).
  int duty_cycle() const { return get_attr_int("duty_cycle"); }

  // Duty Cycle SP: read/write
  // Writing sets the duty cycle setpoint of the PWM signal sent to the motor.
  // Valid values are -100 to 100 (-100% to 100%). Reading returns the current
  // setpoint.
  int duty_cycle_sp() const { return get_attr_int("duty_cycle_sp"); }
  auto set_duty_cycle_sp(int v) -> decltype(*this) {
    set_attr_int("duty_cycle_sp", v);
    return *this;
  }

  // Polarity: read/write
  // Sets the polarity of the motor. Valid values are `normal` and `inversed`.
  std::string polarity() const { return get_attr_string("polarity"); }
  auto set_polarity(std::string v) -> decltype(*this) {
    set_attr_string("polarity", v);
    return *this;
  }

  // Ramp Down SP: read/write
  // Sets the time in milliseconds that it take the motor to ramp down from 100%
  // to 0%. Valid values are 0 to 10000 (10 seconds). Default is 0.
  int ramp_down_sp() const { return get_attr_int("ramp_down_sp"); }
  auto set_ramp_down_sp(int v) -> decltype(*this) {
    set_attr_int("ramp_down_sp", v);
    return *this;
  }

  // Ramp Up SP: read/write
  // Sets the time in milliseconds that it take the motor to up ramp from 0% to
  // 100%. Valid values are 0 to 10000 (10 seconds). Default is 0.
  int ramp_up_sp() const { return get_attr_int("ramp_up_sp"); }
  auto set_ramp_up_sp(int v) -> decltype(*this) {
    set_attr_int("ramp_up_sp", v);
    return *this;
  }

  // State: read-only
  // Gets a list of flags indicating the motor status. Possible
  // flags are `running` and `ramping`. `running` indicates that the motor is
  // powered. `ramping` indicates that the motor has not yet reached the
  // `duty_cycle_sp`.
  mode_set state() const { return get_attr_set("state"); }

  // Stop Action: write-only
  // Sets the stop action that will be used when the motor stops. Read
  // `stop_actions` to get the list of valid values.
  auto set_stop_action(std::string v) -> decltype(*this) {
    set_attr_string("stop_action", v);
    return *this;
  }

  // Stop Actions: read-only
  // Gets a list of stop actions. Valid values are `coast`
  // and `brake`.
  mode_set stop_actions() const { return get_attr_set("stop_actions"); }

  // Time SP: read/write
  // Writing specifies the amount of time the motor will run when using the
  // `run-timed` command. Reading returns the current value. Units are in
  // milliseconds.
  int time_sp() const { return get_attr_int("time_sp"); }
  auto set_time_sp(int v) -> decltype(*this) {
    set_attr_int("time_sp", v);
    return *this;
  }

  // Run the motor until another command is sent.
  void run_forever() { set_command("run-forever"); }

  // Run the motor for the amount of time specified in `time_sp`
  // and then stop the motor using the action specified by `stop_action`.
  void run_timed() { set_command("run-timed"); }

  // Run the motor at the duty cycle specified by `duty_cycle_sp`.
  // Unlike other run commands, changing `duty_cycle_sp` while running *will*
  // take effect immediately.
  void run_direct() { set_command("run-direct"); }

  // Stop any of the run commands before they are complete using the
  // action specified by `stop_action`.
  void stop() { set_command("stop"); }

protected:
  std::string _port_name;
};

//-----------------------------------------------------------------------------

// The servo motor class provides a uniform interface for using hobby type
// servo motors.

class servo_motor : protected device
{
public:
  servo_motor(address_type address = OUTPUT_AUTO);

  using device::connected;
  using device::device_index;

  // Drive servo to the position set in the `position_sp` attribute.
  static constexpr char command_run[] = "run";

  // Remove power from the motor.
  static constexpr char command_float[] = "float";

  // With `normal` polarity, a positive duty cycle will
  // cause the motor to rotate clockwise.
  static constexpr char polarity_normal[] = "normal";

  // With `inversed` polarity, a positive duty cycle will
  // cause the motor to rotate counter-clockwise.
  static constexpr char polarity_inversed[] = "inversed";

  // Address: read-only
  // Returns the name of the port that this motor is connected to.
  std::string address() const { return get_attr_string("address"); }

  // Command: write-only
  // Sets the command for the servo. Valid values are `run` and `float`. Setting
  // to `run` will cause the servo to be driven to the position_sp set in the
  // `position_sp` attribute. Setting to `float` will remove power from the motor.
  auto set_command(std::string v) -> decltype(*this) {
    set_attr_string("command", v);
    return *this;
  }

  // Driver Name: read-only
  // Returns the name of the motor driver that loaded this device. See the list
  // of [supported devices] for a list of drivers.
  std::string driver_name() const { return get_attr_string("driver_name"); }

  // Max Pulse SP: read/write
  // Used to set the pulse size in milliseconds for the signal that tells the
  // servo to drive to the maximum (clockwise) position_sp. Default value is 2400.
  // Valid values are 2300 to 2700. You must write to the position_sp attribute for
  // changes to this attribute to take effect.
  int max_pulse_sp() const { return get_attr_int("max_pulse_sp"); }
  auto set_max_pulse_sp(int v) -> decltype(*this) {
    set_attr_int("max_pulse_sp", v);
    return *this;
  }

  // Mid Pulse SP: read/write
  // Used to set the pulse size in milliseconds for the signal that tells the
  // servo to drive to the mid position_sp. Default value is 1500. Valid
  // values are 1300 to 1700. For example, on a 180 degree servo, this would be
  // 90 degrees. On continuous rotation servo, this is the 'neutral' position_sp
  // where the motor does not turn. You must write to the position_sp attribute for
  // changes to this attribute to take effect.
  int mid_pulse_sp() const { return get_attr_int("mid_pulse_sp"); }
  auto set_mid_pulse_sp(int v) -> decltype(*this) {
    set_attr_int("mid_pulse_sp", v);
    return *this;
  }

  // Min Pulse SP: read/write
  // Used to set the pulse size in milliseconds for the signal that tells the
  // servo to drive to the miniumum (counter-clockwise) position_sp. Default value
  // is 600. Valid values are 300 to 700. You must write to the position_sp
  // attribute for changes to this attribute to take effect.
  int min_pulse_sp() const { return get_attr_int("min_pulse_sp"); }
  auto set_min_pulse_sp(int v) -> decltype(*this) {
    set_attr_int("min_pulse_sp", v);
    return *this;
  }

  // Polarity: read/write
  // Sets the polarity of the servo. Valid values are `normal` and `inversed`.
  // Setting the value to `inversed` will cause the position_sp value to be
  // inversed. i.e `-100` will correspond to `max_pulse_sp`, and `100` will
  // correspond to `min_pulse_sp`.
  std::string polarity() const { return get_attr_string("polarity"); }
  auto set_polarity(std::string v) -> decltype(*this) {
    set_attr_string("polarity", v);
    return *this;
  }

  // Position SP: read/write
  // Reading returns the current position_sp of the servo. Writing instructs the
  // servo to move to the specified position_sp. Units are percent. Valid values
  // are -100 to 100 (-100% to 100%) where `-100` corresponds to `min_pulse_sp`,
  // `0` corresponds to `mid_pulse_sp` and `100` corresponds to `max_pulse_sp`.
  int position_sp() const { return get_attr_int("position_sp"); }
  auto set_position_sp(int v) -> decltype(*this) {
    set_attr_int("position_sp", v);
    return *this;
  }

  // Rate SP: read/write
  // Sets the rate_sp at which the servo travels from 0 to 100.0% (half of the full
  // range of the servo). Units are in milliseconds. Example: Setting the rate_sp
  // to 1000 means that it will take a 180 degree servo 2 second to move from 0
  // to 180 degrees. Note: Some servo controllers may not support this in which
  // case reading and writing will fail with `-EOPNOTSUPP`. In continuous rotation
  // servos, this value will affect the rate_sp at which the speed ramps up or down.
  int rate_sp() const { return get_attr_int("rate_sp"); }
  auto set_rate_sp(int v) -> decltype(*this) {
    set_attr_int("rate_sp", v);
    return *this;
  }

  // State: read-only
  // Returns a list of flags indicating the state of the servo.
  // Possible values are:
  // * `running`: Indicates that the motor is powered.
  mode_set state() const { return get_attr_set("state"); }

  // Drive servo to the position set in the `position_sp` attribute.
  void run() { set_command("run"); }

  // Remove power from the motor.
  void float_() { set_command("float"); }
};

#endif //MOTOR_H_
