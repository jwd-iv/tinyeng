namespace tiny
{
  struct resource
  {
    std::string filename;

    virtual std::string extensions() const = 0; //report which extensions fall under this resource
    virtual bool load(tiny::file f) = 0; //take the file and process its contents
    virtual bool unload() = 0; //free all allocated memory, etc.

    rkMetaHandle(resource);
  };

  class resourcemanager : public system
  {
  public:
    resource::handle load(char const* filename);
    bool unload(resource::handle res);

    // Inherited via system
    virtual void initialize();
    virtual void close();
    virtual void update(float dt);

  private:
    std::unordered_map<std::string, riku::typeinfo> resTypes;
    std::unordered_map<std::string, riku::var<resource> > loaded;

    rkMetaHook(resourcemanager);
  };
}
