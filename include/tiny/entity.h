namespace tiny
{
  struct entity;

  struct component
  {
    guid me;
    riku::var<entity> parent;

    virtual bool notify(char const* message, riku::variant data);

    rkMetaHandle(component);
  };

  struct entity : public component
  {
    std::string archetype;
    riku::var<space> world;
    std::vector<guid> components;

    ~entity();

    virtual guid add(char const* type);
    virtual guid find(char const* type, bool useparent = false) const;

    virtual std::vector<guid> children(bool grandchildren = false) const;

    virtual bool notify(char const* message, riku::variant data);

    virtual bool deserialize(riku::variant_type const& blob);
    virtual bool serialize(riku::variant_type& blob) const;

    template<typename T> inline T* find(bool useparent = false) const
    {
      return find(riku::get<T>()->name().c_str(), useparent).to<T>();
    }

    rkMetaHandle(entity);
  };
}

#include "event.h"
