namespace tiny
{
  struct component
  {
    guid me, parent;

    virtual bool notify(char const* message, riku::variant data);

    rkMetaHook(component);
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

    rkMetaHook(entity);
  };
}

struct transform : public tiny::component
{
  float pX, pY, pZ;
  float sX, sY, sZ;
  float rX, rY, rZ, theta;

  rkMetaHook(transform);
};
