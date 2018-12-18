#include "tiny.h"

namespace tiny
{
  space* space::named(char const* name)
  {
    auto pair = engine::get().spaces.find(name);
    if (pair != engine::get().spaces.end())
      return pair->second;

    riku::var<space> spc((riku::ptr(riku::get<space>(), riku::get<space>()->mem_funcs.create())));
    if (spc.data() == NULL)
      return NULL;

    spc->name = name;
    engine::get().spaces[name] = spc;
    return spc;
  }

  void space::update(float dt)
  {
    update_jobs.update(dt);
    for (auto& iter : get_all(riku::get<component>()))
      iter("update", dt);
  }

  void space::render(float a)
  {
    render_jobs.update(a);
    for (auto& iter : get_all(riku::get<component>()))
      iter("render", a);
  }

  guid space::create(riku::typeinfo t, char const* archetype)
  {
    if (t == riku::get<entity>())
    {
      entity* ent = objects.allocate().to<entity>();
      if (ent != NULL)
      {
        ent->me = guid::create(ent);
        ent->world = this;
        return ent->me;
      }
    }
    else if (t->has_parent(riku::get<component>()))
    {
      auto pair = factory.find(t->name().c_str());
      if (pair == factory.end())
      {
        riku::var<allocator> newalloc = new allocator(t);
        if (newalloc.data() == NULL)
          return guid();

        factory[t->name().c_str()] = newalloc;
        pair = factory.find(t->name().c_str());
      }

      component* comp = pair->second->allocate().to<component>();
      if (comp != NULL)
      {
        comp->me = guid::create(comp);
        return comp->me;
      }
    }
    return guid();
  }

  void space::destroy(guid ID)
  {
    //TODO: find allocator and destroy ID if valid
  }

  riku::array space::get_all(riku::typeinfo t) const
  {
    if (t == riku::get<entity>())
      return objects.in_use();
    else if (t->has_parent(riku::get<component>()))
    {
      riku::array ret;

      for (auto& pair : factory)
        if (pair.second->type->has_parent(t))
          ret.push_back(pair.second->in_use());

      return ret;
    }

    return riku::array();
  }

  bool space::deserialize(riku::variant_type const & blob)
  {
    auto list = blob["entities"];
    if (list.is_array())
    {
      for (auto const& ent : *list.as_array())
      {
        ent.modify(create(riku::get<entity>()));
      }
    }
    blob["update"] >> update_jobs;
    blob["render"] >> render_jobs;
    return false;
  }

  void space::add(job::handle j, int priority)
  {
    update_jobs.add(j, priority);
  }

  void space::start()
  {
    //load initial level and job files if present

    update_jobs.boss = this;
    update_jobs.start();

    render_jobs.boss = this;
    render_jobs.start();
  }

  bool space::done() const
  {
    return update_jobs.done() && render_jobs.done();
  }

  void space::finish()
  {
    update_jobs.finish();
    render_jobs.finish();
  }
}
