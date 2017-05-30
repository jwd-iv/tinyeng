namespace tiny
{
  class system
  {
  public:
    virtual void initialize() = 0;
    virtual void update(float dt) = 0;
    virtual void close() = 0;

    rkMetaHook(system);
  };

  namespace systems
  {
    void initialize();
    void update(float dt);
    void close();

    system* get(char const* name);

    template<typename T> T* get()
    {
      return dynamic_cast<T*>(tiny::systems::get(riku::get<T>()->name().c_str()));
    }
  }

  class filesystem : public system
  {
  public:
    class handle
    {
    public:
      virtual std::string filename() const = 0;
      virtual std::string name() const;
      virtual std::string ext() const;
      virtual std::string folder() const;

      virtual unsigned size() const = 0;
      virtual unsigned pos() const = 0;
      virtual int seek(unsigned p) = 0;
      
      virtual unsigned read(char* mem, unsigned len = 0) = 0;

      rkMetaHook(handle);
    };

    virtual riku::var<handle> open(char const* filename) = 0;

    virtual void update(float);

    rkMetaHook(filesystem);
  };

  class serializer : public system
  {
  public:
    class format
    {
    public:
      virtual std::string ext() const = 0;
      virtual riku::variant parse(char const* text) const = 0;
      virtual std::string serialize(riku::variant obj) const = 0;

      rkMetaHook(format);
    };

    virtual riku::variant parse(char const* text, char const* fmt) const;
    virtual riku::variant parse(char const* file) const;

    // Inherited via system
    virtual void initialize();
    virtual void update(float dt);
    virtual void close();

  private:
    std::unordered_map< std::string, riku::var<format> > formats;

    rkMetaHook(serializer);
  };

#define tinyDeserialize(FuncBody) rkInlineMethod( deserialize, \
  [](my_type& obj, riku::variant_type const& blob) -> bool { FuncBody } )

  class input : public system
  {
  public:
    class device
    {
    public:
      rkMetaHook(device);
    };

    rkMetaHook(input);
  };

  class frc : public system
  {
  public:
    virtual void begin_frame() = 0;
    virtual void update(float) = 0;
    virtual void end_frame() = 0;

    virtual bool time_left() const = 0;
    virtual float dt() const = 0; //the duration of each update tick
    virtual float df() const = 0; //how long the previous frame took

    rkMetaHook(frc);
  };

  class window : public system
  {
  public:
    virtual bool is_active() const = 0;
    virtual bool show() = 0;
    virtual bool hide() = 0;

    virtual void render() = 0;
    
    virtual bool is_fullscreen() const = 0;
    virtual bool set_size(unsigned w, unsigned h, bool fullscreen = false) = 0;

    rkMetaHook(window);
  };

  class renderer : public system
  {
  public:
    virtual void render() = 0;

    rkMetaHook(renderer);
  };
}
