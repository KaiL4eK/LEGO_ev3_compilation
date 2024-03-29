#ifndef DEVICE_H_
#define DEVICE_H_

#include <set>
#include <string>
#include <map>
#include <system_error>
#include <list>
#include <algorithm>
#include <thread>
#include <mutex>
#include <iostream>
#include <fstream>
#include <dirent.h>
#include <string.h>

#include <ev3_ports.h>

#include <common.h>

#ifndef SYS_ROOT
#define SYS_ROOT "/sys/class"
#endif

typedef std::string         device_type;
typedef std::string         mode_type;
typedef std::set<mode_type> mode_set;
typedef std::string         address_type;

const address_type INPUT_AUTO;  //!< Automatic input selection
const address_type OUTPUT_AUTO; //!< Automatic output selection

std::ifstream &ifstream_open(const std::string &path);

//-----------------------------------------------------------------------------

class device
{
public:
    bool connect(const std::string &dir,
                 const std::string &pattern,
                 const std::map<std::string, std::set<std::string>> &match) noexcept;

    inline bool connected() const { return !_path.empty(); }

    int         device_index() const;

    int         get_attr_int   (const std::string &name) const;
    void        set_attr_int   (const std::string &name,
                                int value);
    std::string get_attr_string(const std::string &name) const;
    void        set_attr_string(const std::string &name,
                                const std::string &value);

    std::string get_attr_line  (const std::string &name) const;
    mode_set    get_attr_set   (const std::string &name,
                                std::string *pCur = nullptr) const;

    std::string get_attr_from_set(const std::string &name) const;

protected:
    std::string _path;
    mutable int _device_index = -1;
};

#endif // DEVICE_H_

