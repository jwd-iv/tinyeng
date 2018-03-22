#include "tiny.h"

namespace tiny
{
  std::string filesystem::file::name() const
  {
    auto fname = filename();

    if (fname.find_last_of('\\') < fname.length())
      fname = fname.substr(fname.find_last_of('\\'));

    return fname.substr(0, fname.find_last_of('.'));
  }

  std::string filesystem::file::ext() const
  {
    return filename().substr(filename().find_last_of('.') + 1);
  }

  std::string filesystem::file::folder() const
  {
    return filename().substr(0, filename().find_last_of('\\'));
  }

  std::string filesystem::file::contents()
  {
    __debugbreak(); /* NOT CURRENTLY WORKING
    char* buf = new char[size()];
    read(buf);
    std::string ret(buf);
    delete[] buf; */
    return std::string();
  }

  void filesystem::update(float) {}

  unsigned input::keyboard::axes() const
  {
    return 0;
  }

  unsigned input::keyboard::axis(char const * name)
  {
    return 0;
  }

  float input::keyboard::position(unsigned a) const
  {
    return 0.0f;
  }

  unsigned input::mouse::axes() const
  {
    return 0;
  }

  unsigned input::mouse::axis(char const * name)
  {
    return 0;
  }

  float input::mouse::position(unsigned a) const
  {
    return 0.0f;
  }

  void input::initialize()
  {
  }

  void input::update(float dt)
  {
  }

  void input::close()
  {
  }
}

