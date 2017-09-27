#include "tiny.h"

namespace tiny
{
  void serializer::initialize()
  {
    for (auto const& fmt : riku::get<format>()->children())
    {
      riku::var<format> interpreter(riku::val(fmt, fmt->mem_funcs.create()));
      if (interpreter.data() != NULL)
        formats[interpreter->ext()] = interpreter;
    }
  }

  riku::variant serializer::parse(char const* text, char const* fmt) const
  {
    auto pair = formats.find(fmt);
    if(pair == formats.end())
      return riku::variant();

    return pair->second->parse(text);
  }

  riku::variant serializer::parse(char const* file) const
  {
    auto f = systems::get<filesystem>()->open(file);
    if (f.data() == NULL)
      return riku::variant();

    f->seek(0);
    char* contents = new char[f->size() + 1];
    contents[f->read(contents)] = NULL;

    auto ret = parse(contents, f->ext().c_str());
    delete[] contents;

    return ret;
  }

  void serializer::update(float dt)
  {
  }

  void serializer::close()
  {
  }
}
