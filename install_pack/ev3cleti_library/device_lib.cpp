#include <device.h>

#ifndef FSTREAM_CACHE_SIZE
#define FSTREAM_CACHE_SIZE 16
#endif

// This class implements a small LRU cache. It assumes the number of elements
// is small, and so uses a simple linear search.
template <typename K, typename V>
class lru_cache
{
private:
  // typedef st::pair<K, V> item;
  // std::pair seems to be missing necessary move constructors :(
  struct item
  {
    K first;
    V second;

    item(const K &k) : first(k) {}
    item(item &&m) : first(std::move(m.first)), second(std::move(m.second)) {}
  };

public:
  lru_cache(size_t size = 3) : _size(size)
  {
  }

  V &operator[] (const K &k)
  {
    iterator i = find(k);
    if (i != _items.end())
    {
      // Found the key, bring the item to the front.
      _items.splice(_items.begin(), _items, i);
    } else {
      // If the cache is full, remove oldest items to make room.
      while (_items.size() + 1 > _size)
      {
        _items.pop_back();
      }
      // Insert a new default constructed value for this new key.
      _items.emplace_front(k);
    }
    // The new item is the most recently used.
    return _items.front().second;
  }

  void clear()
  {
    _items.clear();
  }

private:
  typedef typename std::list<item>::iterator iterator;

  iterator find(const K &k)
  {
    return std::find_if(_items.begin(), _items.end(),
                        [&](const item &i) { return i.first == k; });
  }

  size_t _size;
  std::list<item> _items;
};

// A global cache of files.
std::ifstream& ifstream_cache(const std::string &path) {
  static lru_cache<std::string, std::ifstream> cache(FSTREAM_CACHE_SIZE);
  static std::mutex mx;

  std::lock_guard<std::mutex> lock(mx);
  return cache[path];
}

std::ofstream& ofstream_cache(const std::string &path) {
  static lru_cache<std::string, std::ofstream> cache(FSTREAM_CACHE_SIZE);
  static std::mutex mx;

  std::lock_guard<std::mutex> lock(mx);
  return cache[path];
}

//-----------------------------------------------------------------------------

std::ofstream &ofstream_open(const std::string &path)
{
  std::ofstream &file = ofstream_cache(path);
  if (!file.is_open())
  {
    // Don't buffer writes to avoid latency. Also saves a bit of memory.
    file.rdbuf()->pubsetbuf(NULL, 0);
    file.open(path);
  }
  else
  {
    // Clear the error bits in case something happened.
    file.clear();
  }
  return file;
}

std::ifstream &ifstream_open(const std::string &path)
{
  std::ifstream &file = ifstream_cache(path);
  if (!file.is_open())
  {
    file.open(path);
  }
  else
  {
    // Clear the flags bits in case something happened (like reaching EOF).
    file.clear();
    file.seekg(0, std::ios::beg);
  }
  return file;
}

//-----------------------------------------------------------------------------

bool device::connect(
    const std::string &dir,
    const std::string &pattern,
    const std::map<std::string, std::set<std::string>> &match
    ) noexcept
{
    using namespace std;

    const size_t pattern_length = pattern.length();

    struct dirent *dp;
    DIR *dfd;

    if ((dfd = opendir(dir.c_str())) != nullptr) {

        while ((dp = readdir(dfd)) != nullptr) {

            if (strncmp(dp->d_name, pattern.c_str(), pattern_length)==0) {

                try {
                    _path = dir + dp->d_name + '/';

                    bool bMatch = true;

                    for (auto &m : match) {
                        const auto &attribute = m.first;
                        const auto &matches   = m.second;
                        const auto strValue   = get_attr_string(attribute);

                        if (   !matches.empty() 
                            && !matches.begin()->empty() 
                            && (matches.find(strValue) == matches.end())) {
                            bMatch = false;
                            break;
                        }
                    }

                    if (bMatch) {
                        closedir(dfd);
                        return true;
                    }
                }
                catch (...) { }

                _path.clear();
            }
        }

        closedir(dfd);
    }

    return false;
}

//-----------------------------------------------------------------------------

int device::device_index() const
{
  using namespace std;

  if (_path.empty())
    error_process ( "device_index", "no device connected" );

  if (_device_index < 0)
  {
    unsigned f = 1;
    _device_index = 0;
    for (auto it=_path.rbegin(); it!=_path.rend(); ++it)
    {
      if ((*it < '0') || (*it > '9'))
        break;

      _device_index += (*it -'0') * f;
      f *= 10;
    }
  }

  return _device_index;
}

//-----------------------------------------------------------------------------

int device::get_attr_int(const std::string &name) const {
  using namespace std;

  if (_path.empty())
    error_process ( "get_attr_int", "no device connected" );
  for(int attempt = 0; attempt < 2; ++attempt) {
    ifstream &is = ifstream_open(_path + name);
    if (is.is_open())
    {
      int result = 0;
      try {
        is >> result;
        printf("%d\n", result);
        return result;
      } catch(...) {
        // This could mean the sysfs attribute was recreated and the
        // corresponding file handle got stale. Lets close the file and try
        // again (once):
        if (attempt != 0) throw;

        is.close();
        is.clear();
      }
    } else break;
  }
  error_process ( "get_attr_int", _path+name );
  return 0;
}

//-----------------------------------------------------------------------------

void device::set_attr_int(const std::string &name, int value) {
  using namespace std;

  if (_path.empty())
    error_process ( "set_attr_int", "no device connected" );
  for(int attempt = 0; attempt < 2; ++attempt) {
    ofstream &os = ofstream_open(_path + name);
    if (os.is_open())
    {
      if (os << value) return;

      // An error could mean that sysfs attribute was recreated and the cached
      // file handle is stale. Lets close the file and try again (once):
      if (attempt == 0 && errno == ENODEV) {
        os.close();
        os.clear();
      } else {
        throw system_error(std::error_code(errno, std::system_category()));
      }
    } else {
      error_process ( "set_attr_int", _path+name );
    }
  }
}

//-----------------------------------------------------------------------------

std::string device::get_attr_string(const std::string &name) const
{
  using namespace std;

  if (_path.empty())
    error_process ( "get_attr_string", "no device connected" );

  ifstream &is = ifstream_open(_path + name);
  if (is.is_open())
  {
    string result;
    is >> result;
    return result;
  }
  error_process ( "get_attr_string", _path+name );
}

//-----------------------------------------------------------------------------

void device::set_attr_string(const std::string &name, const std::string &value)
{
  using namespace std;

  if (_path.empty())
    error_process ( "set_attr_string", "no device connected" );

  ofstream &os = ofstream_open(_path + name);
  if (os.is_open())
  {
    if (!(os << value)) throw system_error(std::error_code(errno, std::system_category()));
    return;
  }
  error_process ( "set_attr_string", _path+name );
}

//-----------------------------------------------------------------------------

std::string device::get_attr_line(const std::string &name) const
{
  using namespace std;

  if (_path.empty())
    error_process ( "get_attr_line", "no device connected" );

  ifstream &is = ifstream_open(_path + name);
  if (is.is_open())
  {
    string result;
    getline(is, result);
    return result;
  }
  error_process ( "get_attr_line", _path+name );
}

//-----------------------------------------------------------------------------

mode_set device::get_attr_set(const std::string &name, std::string *pCur) const
{
  using namespace std;

  string s = get_attr_line(name);

  mode_set result;
  size_t pos, last_pos = 0;
  string t;
  do {
    pos = s.find(' ', last_pos);

    if (pos != string::npos)
    {
      t = s.substr(last_pos, pos-last_pos);
      last_pos = pos+1;
    }
    else
      t = s.substr(last_pos);

    if (!t.empty())
    {
      if (*t.begin()=='[')
      {
        t = t.substr(1, t.length()-2);
        if (pCur)
          *pCur = t;
      }
      result.insert(t);
    }
  } while (pos!=string::npos);

  return result;
}

//-----------------------------------------------------------------------------

std::string device::get_attr_from_set(const std::string &name) const
{
  using namespace std;

  string s = get_attr_line(name);

  size_t pos, last_pos = 0;
  string t;
  do {
    pos = s.find(' ', last_pos);

    if (pos != string::npos)
    {
      t = s.substr(last_pos, pos-last_pos);
      last_pos = pos+1;
    }
    else
      t = s.substr(last_pos);

    if (!t.empty())
    {
      if (*t.begin()=='[')
      {
        return t.substr(1, t.length()-2);
      }
    }
  } while (pos!=string::npos);

  return { "none" };
}
