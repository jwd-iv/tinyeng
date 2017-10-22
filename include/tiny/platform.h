namespace tiny
{
  //! Interface for finding, reading, and writing files from disk
  class filesystem : public system
  {
  public:
    //! Interface for interacting with files directly
    class file
    {
    public:
      virtual std::string filename() const = 0; //!< Get the path and name, relative to the game's working directory
      virtual std::string name() const;         //!< Get just the file's name
      virtual std::string ext() const;          //!< Get the file's extension
      virtual std::string folder() const;       //!< Get the file's folder, relative to the working directory

      virtual unsigned size() const = 0; //!< Get the total length of the file in bytes
      virtual unsigned pos() const = 0;  //!< Get this handle's current position in the file
      virtual int seek(unsigned p) = 0;  //!< Set this handle to the specified position (starting at the front)
      
      /**
       * @brief      Reads the contents of the file into a specified location in memory
       * 
       * After this function, the new file::pos() should be at the end of whatever bytes were read.
       *
       * @param      mem   The address to write the file's contents to
       * @param[in]  len   How many bytes to read - if 0, the rest of the file will be read
       *
       * @return     How many bytes were successfully read from disk
       */
      virtual unsigned read(char* mem, unsigned len = 0) = 0;
      virtual std::string readline(unsigned len = 0) = 0; //!< Reads contents until the next newline or `len` bytes have been read
      virtual std::string contents();                     //!< Returns the remaining contents of the file

      rkMetaHook(file);
    };

    virtual bool exists(char const* filename) = 0;          //!< Check if a given file exists
    virtual riku::var<file> open(char const* filename) = 0; //!< Get a handle to the named file on disk, which will be null if it doesn't exist

    virtual void update(float);

    rkMetaHandle(filesystem);
  };

  typedef riku::var<filesystem::file> file;

  //! system interface for responding to user input
  class input : public system
  {
  public:
    //!< Generic interface for all input types - gamepads, joysticks, mice, etc.
    class device
    {
    public:
      virtual unsigned buttons() const = 0;          //!< How many buttons the device has
      virtual unsigned axes() const = 0;             //!< How many axes the device has
      virtual unsigned button(char const* name) = 0; //!< The ID of the named button, `0` if none exists
      virtual unsigned axis(char const* name) = 0;   //!< The ID of the named axis, `0` if none exists

      virtual bool down(unsigned b) const = 0;      //!< Is button `b` currently being held
      virtual bool pressed(unsigned b) const = 0;   //!< Was button `b` pressed since last frame
      virtual bool released(unsigned b) const = 0;  //!< Was button `b` released since last frame
      virtual float position(unsigned a) const = 0; //!< Get the current position (-1 to 1) of axis `a`

      rkMetaHandle(device);
    };

    //! Interface for getting the keyboard's status
    class keyboard : public device
    {
    public:
      // Inherited via device
      virtual unsigned axes() const;
      virtual unsigned axis(char const* name);
      virtual float position(unsigned a) const;

      rkMetaHandle(keyboard);
    };

    //! Interface for getting the mouse's status
    class mouse : public device
    {
    public:
      virtual unsigned x() const = 0; //!< The horizontal position of the mouse cursor
      virtual unsigned y() const = 0; //!< The vertical position of the mouse cursor
      virtual int dX() const = 0;     //!< How far the cursor has horizontally moved since last frame
      virtual int dY() const = 0;     //!< How far the cursor has vertically moved since last frame

      // Inherited via device
      virtual unsigned axes() const;
      virtual unsigned axis(char const* name);
      virtual float position(unsigned a) const;

      rkMetaHandle(mouse);
    };

    // Inherited via system
    virtual void initialize();
    virtual void update(float dt);
    virtual void close();

    rkMetaHandle(input);
  };

  //! system interface for a fixed step framerate controller
  class frc : public system
  {
  public:
    //TODO: Add begin_tick() and end_tick()
    virtual void begin_frame() = 0; //!< Called at the very beginning of the update frame
    virtual void tick() = 0;   //!< Called when one update frame is done

    virtual bool time_left() const = 0;
    virtual float dt() const = 0; //!< The duration of each update tick (in seconds)
    virtual float df() const = 0; //!< How long the previous render frame took
    virtual float alpha() const = 0; //!< Unused for now (see https://gafferongames.com/post/fix_your_timestep/)

    virtual void update(float) {}

    rkMetaHandle(frc);
  };

  //! system interface for the platform's window handle
  class window : public system
  {
  public:
    virtual bool is_active() const = 0; //!< Is the window currently visible
    virtual bool show() = 0;            //!< Bring the window to the foreground
    virtual bool hide() = 0;            //!< Minimize the window

    virtual unsigned width() const = 0;     //!< The horizontal size of the window (in pixels)
    virtual unsigned height() const = 0;    //!< The vertical size of the window (in pixels)
    virtual bool is_fullscreen() const = 0; //!< Has the window been made fullscreen

    virtual void render() = 0; //!< Present the window's framebuffer for the OS
    
    /**
     * @brief      Adjust window size
     *
     * @param[in]  w           The desired width (in pixels)
     * @param[in]  h           The desired height (in pixels)
     * @param[in]  fullscreen  If `true`, window will be changed to windowed fullscreen and
     *                         the monitor's resolution will be adjusted if possible
     *
     * @return     `true` if the change was successful
     */
    virtual bool set_size(unsigned w, unsigned h, bool fullscreen = false) = 0;

    rkMetaHandle(window);
  };

  //! system-level interface for the graphics engine
  class renderer : public system
  {
  public:
    virtual void render(float) = 0; //!< Make final draw calls and prepare the image for rendering to the window

    rkMetaHandle(renderer);
  };
}
