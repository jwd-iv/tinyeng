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

      riku::val res(rtype, rtype->mem_funcs.create());
      if (res.data() != NULL)
      {
        for (auto const& ext : ParseExt(res.to<resource>()->extensions()))
          resTypes[ext] = rtype;
      }
    }
  }

  void resourcemanager::close()
  {
    while (!loaded.empty())
      unload(loaded.begin()->second);
  }

  resource::handle resourcemanager::load(char const* filename)
  {
    if (loaded.find(filename) != loaded.end())
      return loaded[filename];

    tiny::file f = systems::get<filesystem>()->open(filename);

    if(f.data() == NULL)
      return resource::handle();

    std::string ext(filename);

    ext = ext.substr(ext.find_last_of('.') + 1);
    if (resTypes.find(ext) == resTypes.end())
      return resource::handle();

    auto type = resTypes[ext];
    if (type == NULL || !type->has_parent(riku::get<resource>()))
      return resource::handle();

    resource::handle res = riku::val(type, type->mem_funcs.create());
    auto pRes = res.to<resource>();
    if (pRes == NULL)
      return resource::handle();

    if (!pRes->load(f))
      return resource::handle();

    loaded[filename] = res;
    pRes->me = guid::create(pRes);
    pRes->filename = filename;
    return res->me;
  }

  bool resourcemanager::unload(resource::handle res)
  {
    if (res.data() == NULL)
      return true;

    auto name = res->filename;

    if (loaded.find(name) != loaded.end())
    {
      auto r = loaded[name];
      loaded.erase(name);
      return r->unload();
    }

    return false;
  }

  void resourcemanager::update(float dt)
  {
  }
}
