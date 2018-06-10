#include "tiny.h"
#include "tiny\editor\inspector.h"
#include "AntTweakBar.h"

TwType translate_type(riku::variant const& val);
void TW_CALL invoke_set(const void*, void*);
void TW_CALL invoke_get(void*, void*);
void get_from_tweakbar(riku::variant&, void const*);
void send_to_tweakbar(riku::variant const&, void*);

namespace tiny
{
  bool inspector::receiveWindowEvent(void const* pEvent)
  {
    if (active)
      return TwEventSFML(pEvent, 2, 4);
    return false;
  }

  void inspector::initialize()
  {
    TwInit(TW_OPENGL, NULL);

    create_view("inspector");
    views["inspector"]->link("engine", riku::ptr(engine::get()));
    views["inspector"]->edit("active", riku::ptr(active));

    engine::get().render.add(riku::val(function_job(
      [](float dt)->bool {
      systems::get<inspector>()->render(dt);
      return false;
    }
    )));
  }

  void inspector::update(float dt)
  {
    auto win = systems::get<window>();
    TwWindowSize(win->width(), win->height());
  }

  void inspector::render(float dt)
  {
    if (active)
      TwDraw();
  }

  inspector::view* inspector::create_view(char const* name)
  {
    auto bar = TwGetBarByName(name);

    if (bar != NULL)
    {
      TwDeleteBar(bar);
      bar = NULL;
    }

    bar = TwNewBar(name);

    auto view = new inspector::view();
    view->window = bar;
    views[name] = view;

    return view;
  }

  void inspector::close()
  {
    TwDeleteAllBars();

    TwTerminate();

    for (auto& pair : views)
      delete pair.second;

    views.clear();
  }

  void inspector::inspect(riku::variant_type& obj)
  {
    create_view(obj.type()->name().c_str())->edit("", obj);
  }

  void inspector::inspect(riku::variant_type const& obj)
  {
    create_view(obj.type()->name().c_str())->read("", obj);
  }

  bool inspector::view::edit(std::string name, riku::variant_type& value)
  {
    riku::variant target(value);

    if (target.data() != NULL)
    {
      if(translate_type(target) != TW_TYPE_UNDEF)
        return add(name, value.type(),
          riku::func_ptr(riku::function(send_to_tweakbar)),
          riku::func_ptr(riku::function(get_from_tweakbar)),
          target
        );
      else if (target.type()->has_parent(rk::get<rk::variant_type>()))
        return link(name, target);
      else
      {
        if (!name.empty())
          name += '.';

        bool successful = false;

        for (auto const& fname : target.type()->functions(false, true))
        {
          auto func = target.function(fname);
          if ((func->obj_type != NULL && target.type()->has_parent(func->obj_type))
             || (func->arg_count == 1 && target.type()->has_parent(func->arg_list[0].type))
          )
            successful |= add(name + fname, target.function(fname), target);
        }

        for (auto const& pname : target.type()->properties(true))
        {
          successful |= edit(name + pname, target.property(pname));
          //TODO: instead add() a get/set that calls the property get()/set() specifically
        }

        if(!name.empty())
          name.erase(--name.end());
        
        auto delim = name.find_last_of('.');
        if (delim != name.npos)
        {
          std::string parentgroup = name.substr(0, delim), bar = TwGetBarName((TwBar*) window);

          TwDefine((std::string() +
            "`" + bar + "`/`" + name + "` group='" + parentgroup + "'"
          ).c_str());
        }

        return successful;
      }
    }

    return false;
  }

  bool inspector::view::read(std::string name, riku::variant_type const& value)
  {
    riku::variant target(value);

    if (target.data() != NULL)
    {
      return add(name, value.type(),
        riku::func_ptr(riku::function(send_to_tweakbar)),
        riku::func_ptr(),
        target
        );
    }

    return false;
  }

  bool inspector::view::add(std::string name, riku::typeinfo type, riku::func_ptr get, riku::func_ptr set, riku::variant obj)
  {
    auto getset = values.find(name);

    if (getset != values.end())
      return false;

    values[name] = std::make_tuple(type, get, set, obj);

    //group parsing
    auto delimiter = name.find_last_of('.');
    std::string group, label;

    if (delimiter != name.npos)
    {
      group = name.substr(0, delimiter);
      label = name.substr(delimiter + 1);
    }

    return TwAddVarCB(
      (TwBar*) window,
      name.c_str(),
      translate_type(type),
      set.data() == NULL ? NULL : invoke_set,
      invoke_get,
      &values[name],
      group.empty()
        ? NULL
        : (std::string("group='") + group + "' label='" + label + "'").c_str()
    );
  }

  bool inspector::view::add(std::string name, riku::func_ptr button, riku::variant data)
  {
    //TODO: add a button to the bar that executes the given function with the given data
    return false;
  }

  bool inspector::view::link(std::string name, riku::variant_type& target)
  {
    //TODO: add(name, function that calls inspector::inspect(target), target)
    return false;
  }
}

TwType translate_type(riku::variant const& val)
{
  if      (val.type() == riku::get<bool>())        return TW_TYPE_BOOLCPP;
  else if (val.type() == riku::get<char>())        return TW_TYPE_CHAR;
  else if (val.type() == riku::get<int>())         return TW_TYPE_INT32;
  else if (val.type() == riku::get<unsigned>())    return TW_TYPE_UINT32;
  else if (val.type() == riku::get<float>())       return TW_TYPE_FLOAT;
  else if (val.type() == riku::get<double>())      return TW_TYPE_DOUBLE;
  else if (val.type() == riku::get<char*>())       return TW_TYPE_CDSTRING;
  else if (val.type() == riku::get<std::string>()) return TW_TYPE_STDSTRING;
  else if (val.type() == riku::find("glm::quat4")) return TW_TYPE_QUAT4F;
  else if (val.type() == riku::find("glm::vec3"))  return TW_TYPE_DIR3F;
  //else if (val.type() == riku::get<COLOR3F>()) return TW_TYPE_COLOR3F;
  //else if (val.type() == riku::get<COLOR4F>()) return TW_TYPE_COLOR4F;
  //else if (val.type() == riku::get<QUAT4D>()) return TW_TYPE_QUAT4D;
  //else if (val.type() == riku::get<DIR3D>()) return TW_TYPE_DIR3D;
  else return TW_TYPE_UNDEF;
}

void TW_CALL invoke_set(const void* value, void* tuple)
{
  auto& set = std::get<2>(*(tiny::inspector::get_set*) tuple);
  auto& obj = std::get<3>(*(tiny::inspector::get_set*) tuple);

  riku::array params;

  if (obj.data() != NULL)
    params.push_back(obj);
  params.push_back(riku::val(riku::get<void const*>(), &value));

  set->invoke(params);
}

void TW_CALL invoke_get(void* ret, void* tuple)
{
  using namespace riku;
  auto& fget = std::get<1>(*(tiny::inspector::get_set*) tuple);
  auto& obj = std::get<3>(*(tiny::inspector::get_set*) tuple);

  riku::array params;

  if (obj.data() != NULL)
    params.push_back(obj);
  params.push_back(val(get<void*>(), &ret));

  fget->invoke(params);
}

void get_from_tweakbar(riku::variant& obj, void const* twval)
{
  obj.assign(riku::ptr(obj.type(), twval));
}

void send_to_tweakbar(riku::variant const& obj, void* twval)
{
  riku::ptr(obj.type(), twval).assign(obj);
}