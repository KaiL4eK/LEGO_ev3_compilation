#include <device.h>

class lego_port : protected device
{
public:
  lego_port(address_type);

  using device::connected;
  using device::device_index;

//~autogen generic-get-set classes.legoPort>currentClass

  // Address: read-only
  // Returns the name of the port. See individual driver documentation for
  // the name that will be returned.
  std::string address() const { return get_attr_string("address"); }

  // Driver Name: read-only
  // Returns the name of the driver that loaded this device. You can find the
  // complete list of drivers in the [list of port drivers].
  std::string driver_name() const { return get_attr_string("driver_name"); }

  // Modes: read-only
  // Returns a list of the available modes of the port.
  mode_set modes() const { return get_attr_set("modes"); }

  // Mode: read/write
  // Reading returns the currently selected mode. Writing sets the mode.
  // Generally speaking when the mode changes any sensor or motor devices
  // associated with the port will be removed new ones loaded, however this
  // this will depend on the individual driver implementing this class.
  std::string mode() const { return get_attr_string("mode"); }
  auto set_mode(std::string v) -> decltype(*this) {
    set_attr_string("mode", v);
    return *this;
  }

  // Set Device: write-only
  // For modes that support it, writing the name of a driver will cause a new
  // device to be registered for that driver and attached to this port. For
  // example, since NXT/Analog sensors cannot be auto-detected, you must use
  // this attribute to load the correct driver. Returns -EOPNOTSUPP if setting a
  // device is not supported.
  auto set_set_device(std::string v) -> decltype(*this) {
    set_attr_string("set_device", v);
    return *this;
  }

  // Status: read-only
  // In most cases, reading status will return the same value as `mode`. In
  // cases where there is an `auto` mode additional values may be returned,
  // such as `no-device` or `error`. See individual port driver documentation
  // for the full list of possible values.
  std::string status() const { return get_attr_string("status"); }


//~autogen

protected:
  lego_port() {}

  bool connect(const std::map<std::string, std::set<std::string>>&) noexcept;
};
