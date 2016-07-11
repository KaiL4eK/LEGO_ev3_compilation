#include <power_info.h>

power_info power_info::battery { "" };

//-----------------------------------------------------------------------------

power_info::power_info(std::string name)
{
  static const std::string _strClassDir { SYS_ROOT "/power_info/" };

  if (name.empty())
    name = "legoev3-battery";

  connect(_strClassDir, name, std::map<std::string, std::set<std::string>>());
}