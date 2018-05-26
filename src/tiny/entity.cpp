#include "tiny.h"

namespace tiny
{
  bool component::notify(char const* message, riku::variant data)
  {
    return false;
  }

  entity::~entity()
  {
    if (world.data() != NULL)
    {
      for (guid iter : components)
        world->destroy(iter);
      components.clear();
    }
  }

  guid entity::add(char const* type)
  {
    riku::typeinfo t = riku::find(type);
    if (t != NULL && t->has_parent(riku::get<component>()))
    {
      const guid existing = find(type);
      if (existing.data() != NULL)
        return existing;

      if (world.data() != NULL)
      { // make it and add it
        guid newcomp = world->create(t);
        components.push_back(newcomp);
        return newcomp;
      }
    }

    return guid();
  }

  guid entity::find(char const* type, bool useparent) const
  {
    riku::typeinfo t = riku::find(type);

    if (t != NULL && t->has_parent(riku::get<component>()))
      for (auto iter : components)
        if (iter.to<component>()->meta()->has_parent(t))
          return iter;

    if (useparent && parent.is<entity>())
      return parent.to<entity>()->find(type, useparent);// TODO: determine whether this should be 'useparent' or 'false'
    else
      return guid();
  }

  std::vector<guid> entity::children(bool grandchildren) const
  {
    std::vector<guid> youngins;

    for (auto iter : components)
      if (iter.is<entity>())
        youngins.push_back(iter);

    return youngins;
  }

  bool entity::notify(char const* message, riku::variant data)
  {
    bool processed = false;

    for (guid iter : components)
      processed |= iter.to<component>()->notify(message, data);

    return processed;
  }

  bool entity::deserialize(riku::variant_type const & blob)
  {
    blob["archetype"] >> archetype;

    if (!archetype.empty())
    {
      //TODO: grab a copy of the archetype, assign blob to it and use the result
    }

    auto comps = blob["components"];
    auto childs = blob["children"];

    for (auto const& compname : comps.properties())
      comps[compname.c_str()].assignto(add(compname.c_str()));

    if (childs.is_array())
      for (auto const& iter : *childs.as_array())
        iter.assignto(add("entity"));

    return true;
  }

  bool entity::serialize(riku::variant_type & blob) const
  {
    return true;
  }
}
