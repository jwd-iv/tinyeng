#include "tiny.h"

namespace tiny
{
  std::vector<std::string> ParseExt(std::string extensions)
  {
    std::vector<std::string> exts;

    unsigned b = 0;
    unsigned e = 0;

    while (b < extensions.size())
    {
      e = extensions.find_first_of(",./+; ", b);
      if (e > extensions.size())
        e = extensions.size();

      exts.push_back(extensions.substr(b, e - b));
      if ((--exts.end())->empty())
        exts.pop_back();
      b = e + 1;
    }

    return exts;
  }

  void resourcemanager::initialize()
  {
    std::list<riku::typeinfo> resources;

    for (auto const& iter : riku::get<resource>()->children())
      resources.push_back(iter);

    while (!resources.empty())
    {
      riku::typeinfo rtype = resources.front();
      resources.pop_front();

      for (auto const& iter : rtype->children())
        resources.push_back(iter);

      riku::ptr res(rtype, rtype->mem_funcs.create());
      if (res.data() != NULL)
      {
        for (auto const& ext : ParseExt(res.to<resource>()->extensions()))
          resTypes[ext] = rtype;
      }
    }
  }

  void resourcemanager::close()
  {
    //while (!loaded.empty())
    //  Unload(loaded.begin()->second);
  }

  resource::handle resourcemanager::load(char const * filename)
  {
    return resource::handle();
  }

  bool resourcemanager::unload(resource::handle res)
  {
    return false;
  }

  void resourcemanager::update(float dt)
  {
  }
}
