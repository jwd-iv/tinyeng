#include "tiny.h"
#include <assert.h>

namespace tiny
{
  namespace systems
  {
    static std::vector< riku::var<system> > g_systems;

    void initialize()
    {
      //later on this will assert that they're not riku::null_system
      assert(get<filesystem>() != NULL);
      assert(get<serializer>() != NULL);
      assert(get<input>() != NULL);
      assert(get<frc>() != NULL);
      assert(get<window>() != NULL);
      assert(get<renderer>() != NULL);
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
        if (*iter == riku::find("tiny::null_system") && children.size() > 1)
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
  std::string filesystem::handle::name() const
  {
    auto fname = filename();

    if (fname.find_last_of('\\') < fname.length())
      fname = fname.substr(fname.find_last_of('\\'));

    return fname.substr(0, fname.find_last_of('.'));
  }

  std::string filesystem::handle::ext() const
  {
    return filename().substr(filename().find_last_of('.') + 1);
  }

  std::string filesystem::handle::folder() const
  {
    return filename().substr(0, filename().find_last_of('\\'));
  }

  void filesystem::update(float) {}
}

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

namespace tiny
{
  struct null_system : public filesystem, public input, public frc, public window, public renderer
  {
    virtual void initialize() {}
    virtual void update(float dt) {}
    virtual void close() {}

    virtual riku::var<handle> open(char const* filename) { return riku::var<handle>(); }

    virtual void begin_frame() {}
    virtual void end_frame() {}
    virtual bool time_left() const { return engine::get().running; }
    virtual float dt() const { return .0f; }
    virtual float df() const { return .0f; }

    virtual bool is_active() const { return false; }
    virtual bool show() { return false; }
    virtual bool hide() { return false; }
    virtual void render() {}
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
