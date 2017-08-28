namespace tiny
{
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
      virtual std::string readline(unsigned len = 0) = 0;
      virtual std::string contents();

      rkMetaHook(handle);
    };

	virtual bool exists(char const* filename) = 0;
    virtual riku::var<handle> open(char const* filename) = 0;

    virtual void update(float);

    rkMetaHook(filesystem);
  };

  typedef riku::var<filesystem::handle> file;

  class input : public system
  {
  public:
    class device
    {
    public:
      virtual unsigned buttons() const = 0;
      virtual unsigned button(char const* name) = 0;
      virtual unsigned axes() const = 0;
      virtual unsigned axis(char const* name) = 0;

      virtual bool down(unsigned b) const = 0;
      virtual bool pressed(unsigned b) const = 0;
      virtual bool released(unsigned b) const = 0;

      virtual float position(unsigned a) const = 0;

      rkMetaHandle(device);
    };

    class keyboard : public device
    {
    public:
      virtual unsigned axes() const;
      virtual unsigned axis(char const* name);
      virtual float position(unsigned a) const;

      rkMetaHook(keyboard);
    };

    class mouse : public device
    {
    public:
      virtual unsigned x() const = 0;
      virtual unsigned y() const = 0;
      virtual int dX() const = 0;
      virtual int dY() const = 0;

      virtual unsigned axes() const;
      virtual unsigned axis(char const* name);
      virtual float position(unsigned a) const;

      rkMetaHook(mouse);
    };

    // Inherited via system
    virtual void initialize();
    virtual void update(float dt);
    virtual void close();

    rkMetaHook(input);
  };

  class frc : public system
  {
  public:
    virtual void begin_frame() = 0;
    virtual void end_frame() = 0;

    virtual bool time_left() const = 0;
    virtual float dt() const = 0; //the duration of each update tick
    virtual float df() const = 0; //how long the previous frame took
    virtual float alpha() const = 0;

    virtual void update(float) {}

    rkMetaHook(frc);
  };

  class window : public system
  {
  public:
    virtual bool is_active() const = 0;
    virtual bool show() = 0;
    virtual bool hide() = 0;

    virtual unsigned width() const = 0;
    virtual unsigned height() const = 0;

    virtual void render() = 0;
    
    virtual bool is_fullscreen() const = 0;
    virtual bool set_size(unsigned w, unsigned h, bool fullscreen = false) = 0;

    rkMetaHook(window);
  };

  class renderer : public system
  {
  public:
    virtual void render(float) = 0;

    rkMetaHook(renderer);
  };
}
