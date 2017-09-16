#include "tiny.h"
#include <assert.h>

namespace tiny
{
  namespace systems
  {
    static std::vector<system::handle> g_systems;

    void initialize()
    {
      //later on this will assert that they're not riku::null_system
      assert(get<filesystem>().data() != NULL);
      assert(get<serializer>().data() != NULL);
      assert(get<input>().data() != NULL);
      assert(get<frc>().data() != NULL);
      assert(get<window>().data() != NULL);
      assert(get<renderer>().data() != NULL);
    }

    void update(float dt)
    {
      for (system* sys : g_systems)
        sys->update(dt);
    }

    void close()
    {
      while (!g_systems.empty())
      {
        system* sys = *--g_systems.end();
        g_systems.pop_back();

        sys->close();
        sys->meta()->mem_funcs.del(sys);
      }
    }

    system* create(riku::typeinfo systype)
    {
      riku::typelist children = systype->children(true);
      for (auto iter = children.rbegin(); iter != children.rend(); ++iter)
      {
        if (*iter == riku::find("tiny::null_system"))
          continue;
        if (create(*iter) != NULL)
          return get(systype->name().c_str());
      }

      g_systems.push_back(riku::ptr(systype, systype->mem_funcs.create()));
      system* sys = *--g_systems.end();

      if (sys == NULL || !sys->meta()->has_parent(systype))
      {
        g_systems.pop_back();
        if (sys != NULL)
          systype->mem_funcs.del(sys);

        return NULL;
      }
      else
      {
        sys->initialize();
        return *--g_systems.end();
      }
    }

    system::handle get(char const* name)
    {
      riku::typeinfo systype = riku::find(name);
      if (systype == NULL)
        return nullptr;

      for (auto& iter : g_systems)
        if (iter.type()->has_parent(systype))
          return iter;

      return create(systype);
    }
  }
}

namespace tiny
{
  struct null_system : public filesystem, public input, public frc, public window, public renderer
  {
    virtual void initialize()     {}
    virtual void update(float dt) {}
    virtual void close()          {}

    virtual bool exists(char const* filename) { return false; }
    virtual file open(char const* filename)   { return file(); }

    virtual void begin_frame()     {}
    virtual void end_frame()       {}
    virtual bool time_left() const { return engine::get().running; }
    virtual float dt() const       { return .0f; }
    virtual float df() const       { return .0f; }
    virtual float alpha() const    { return .0f; }

    virtual bool is_active() const  { return false; }
    virtual bool show()             { return false; }
    virtual bool hide()             { return false; }
    virtual unsigned width() const  { return 0; }
    virtual unsigned height() const { return 0; }
    virtual void render()           {}
    virtual void render(float)      {}
    virtual bool is_fullscreen() const { return false; }
    virtual bool set_size(unsigned w, unsigned h, bool fullscreen = false) { return false; }

    rkMetaHook(null_system);
  };
}

rkType(tiny::null_system,
  rkParent(tiny::filesystem)
  rkParent(tiny::input)
  rkParent(tiny::frc)
  rkParent(tiny::window)
  rkParent(tiny::renderer)

  rkDefaultFactory
)
