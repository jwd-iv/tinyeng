namespace tiny
{
  //! Represents data loaded from disk that needs to be accessed by multiple objects.
  struct resource
  {
    std::string filename;
    guid me;

    virtual std::string extensions() const = 0; //!< Get which extensions apply to this resource
    virtual bool load(tiny::file f) = 0;        //!< Process the contents of file `f`
    virtual bool unload() = 0;                  //!< Free any allocated memory, prepare for deletion

    rkMetaHandle(resource);
  };

  //! system that manages all resources currently needed by the game
  class resourcemanager : public system
  {
  public:

    /**
     * @brief      Load a resource from disk
     *
     * @param      filename  Path to the file (relative to the working
     *                       directory). Which kind of resource is loaded is
     *                       determined by the file's extension.
     *
     * @return     A handle to the loaded resource
     */
    resource::handle load(char const* filename);

    /**
     * @brief      Destroy a resource
     *
     * @param[in]  res   A handle to the resource to destroy
     *
     * @return     `true` if the deallocation was successful
     */
    bool unload(resource::handle res);

    // Inherited via system
    virtual void initialize();
    virtual void close();
    virtual void update(float dt);

  private:
    std::unordered_map<std::string, riku::typeinfo> resTypes;
    std::unordered_map<std::string, resource::handle> loaded;

    rkMetaHandle(resourcemanager);
  };
}
