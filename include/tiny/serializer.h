namespace tiny
{
  //! system for translating between game data and files loaded from disk
  struct serializer : public system
  {
  public:
    //! Represents a method of data storage, e.g. JSON, XML, binary, etc.
    class format
    {
    public:
      virtual std::string ext() const = 0; //!< Get the extension for files of this type

      /**
       * @brief      Take a block of memory and translate it into game data
       *
       * @param      text  The start byte of the memory to read
       *
       * @return     Some implementation of variant holding the translated data
       */
      virtual riku::variant parse(char const* text) const = 0;

      /**
       * @brief      Take game data and translate it into the given format
       *
       * @param[in]  obj   The data to write
       *
       * @return     The flattened block of text
       */
      virtual std::string serialize(riku::variant obj) const = 0; 

      rkMetaHook(format);
    };

    /**
     * @brief      Translate a given block of memory as the specified format
     *
     * @param      text  The memory to read
     * @param      fmt   Which format to read it as
     *
     * @return     The results of format::parse
     */
    virtual riku::variant parse(char const* text, char const* fmt) const;

    /**
     * @brief      Translate a given file
     *
     * @param      file  The filename (relative to the working directory) to
     *                   translate. The extension of this file dictates the
     *                   format.
     *
     * @return     The results of format::parse
     */
    virtual riku::variant parse(char const* file) const;

    // Inherited via system
    virtual void initialize();
    virtual void update(float dt);
    virtual void close();

  private:
    std::unordered_map< std::string, riku::var<format> > formats;

    rkMetaHandle(serializer);
  };
}

/**
 * @brief      Helper macro for adding a custom deserialization step when documenting a type
 * 
 * This creates a lamba named "deserialize" in the type's metadata, that takes two
 * arguments - a reference to the object being deserialized (`obj`), and a variant
 * holding the data to load in (`blob`). The lambda must return a boolean, if `true`
 * then the serializer will consider deserialization done, otherwise it will 
 *
 * @param      FuncBody  The contents of the function (no need for wrapping curly braces)
 */
#define tinyDeserialize(FuncBody) rkInlineMethod( deserialize, \
  [](my_type& obj, riku::variant_type const& blob) -> bool { FuncBody } )
