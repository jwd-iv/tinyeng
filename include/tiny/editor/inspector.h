namespace tiny
{
  struct inspector : public system
  {
  public:
    typedef std::tuple<riku::typeinfo, riku::var<riku::function>, riku::var<riku::function>, riku::variant> get_set;

    virtual bool receiveWindowEvent(void const* pEvent);

    // Inherited via system
    virtual void initialize();
    virtual void update(float dt);
    virtual void close();

    void inspect(riku::variant_type& obj);
    void inspect(riku::variant_type const& obj);

    struct view
    {
      virtual bool edit(std::string name, riku::variant_type& value);
      virtual bool read(std::string name, riku::variant_type const& value);

      virtual bool add(std::string name, riku::typeinfo type, riku::var<riku::function> get, riku::var<riku::function> set = riku::var<riku::function>(), riku::variant obj = riku::variant());

      virtual bool link(std::string name, riku::variant_type& target);

      std::unordered_map<std::string, get_set> values;
      std::unordered_map<std::string, riku::variant> links;
      void* window = NULL;

      rkMetaHook(view);
    };

  protected:
    virtual void render(float dt);

    tiny::inspector::view* create_view(char const* name);

  private:
    bool active = true;
    std::unordered_map<std::string, view*> views;

    rkMetaHook(inspector);
  };
}

#define tinyInspector (*tiny::systems::get<tiny::inspector>())
