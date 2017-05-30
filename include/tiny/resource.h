namespace tiny
{
  /*struct resource
  {
    std::string filename;

    virtual std::string extensions() const = 0;
    virtual bool Load(FileHandle file) = 0; //take the file and process its contents
    virtual bool Unload() = 0; //free all allocated memory, etc.

    PUPA_DataHook(Resource);
  };

  class ResourceManager : public Service
  {
  public:
    bool IsResource(char const* filename) const;

    Data::Handle Load(char const* filename);
    bool Unload(Data::Handle resource);

    // Inherited via Service
    virtual void Initialize();
    virtual void Close();
    virtual void Update(float dt);

  private:
    std::unordered_map<std::string, Meta::TypeInfo> resTypes;
    std::unordered_map<std::string, Data::Handle> loaded;

    META_Hook(ResourceManager);
  };*/
}
