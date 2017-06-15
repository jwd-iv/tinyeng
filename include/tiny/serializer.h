namespace tiny
{
  struct serializer : public system
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
}

#define tinyDeserialize(FuncBody) rkInlineMethod( deserialize, \
  [](my_type& obj, riku::variant_type const& blob) -> bool { FuncBody } )
