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
      assert(get<filesystem>() != NULL);
      assert(get<serializer>() != NULL);
      assert(get<input>()      != NULL);
      assert(get<frc>()        != NULL);
      assert(get<window>()     != NULL);
      assert(get<renderer>()   != NULL);
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
        if ((*iter)->name().find("null") != riku::string::npos && children.size() > 1)
          continue;
        if (create(*iter) != NULL)
          return get(systype->name().c_str());
      }

      system* sys = reinterpret_cast<system*>(systype->mem_funcs.create());
      g_systems.push_back(riku::ptr(systype, sys));

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
        return sys;
      }
    }

    system* get(char const* name)
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
  struct null_files : public filesystem
  {
    virtual void initialize()     {}
    virtual void update(float dt) {}
    virtual void close()          {}

    virtual bool exists(char const* filename)     { return false; }
    virtual tiny::file open(char const* filename) { return tiny::file(); }

    rkMetaHook(null_files);
  };

  struct null_frc : public frc
  {
    virtual void initialize()     {}
    virtual void update(float dt) {}
    virtual void close()          {}

    virtual void begin_frame()     {}
    virtual void end_frame()       {}
    virtual bool time_left() const { return engine::get().running; }
    virtual float dt() const       { return .0f; }
    virtual float df() const       { return .0f; }
    virtual float alpha() const    { return .0f; }

    rkMetaHook(null_frc);
  };

  struct null_window : public window
  {
    virtual void initialize()     {}
    virtual void update(float dt) {}
    virtual void close()          {}

    virtual bool is_active() const  { return false; }
    virtual bool show()             { return false; }
    virtual bool hide()             { return false; }
    virtual unsigned width() const  { return 0; }
    virtual unsigned height() const { return 0; }
    virtual void render()           {}
    virtual bool is_fullscreen() const { return false; }
    virtual bool set_size(unsigned w, unsigned h, bool fullscreen = false) { return false; }

    rkMetaHook(null_window);
  };

  struct null_renderer : public renderer
  {
    virtual void initialize()     {}
    virtual void update(float dt) {}
    virtual void close()          {}
    virtual void render(float)      {}

    rkMetaHook(null_renderer);
  };
}

rkType(tiny::null_files,    rkParent(tiny::filesystem) rkDefaultFactory)
rkType(tiny::null_frc,      rkParent(tiny::frc)        rkDefaultFactory)
rkType(tiny::null_window,   rkParent(tiny::window)     rkDefaultFactory)
rkType(tiny::null_renderer, rkParent(tiny::renderer)   rkDefaultFactory)
