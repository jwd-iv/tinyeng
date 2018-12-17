namespace tiny
{
  //! Manages an isolated set of game objects, like the active game environment or a menu
  struct space : public job_manager
  {
    //! Find the space with the given name and create it if it doesn't exist
    static space* named(char const* name);

    virtual void update(float dt); //!< Move one tick forward
    virtual void render(float a);  //!< Runs render jobs before the renderer is called

    /**
     * @brief      Allocates an object of the given type in this world
     *
     * @param[in]  t          The type to allocate
     * @param      archetype  What archetype to apply to the object
     *
     * @return     A new guid for the created object
     */
    virtual guid create(riku::typeinfo t, char const* archetype = NULL);
    virtual void destroy(guid ID); //!< Destroys the object if it's in this space

    //! Retrieves all objects of a given type, in memory order
    virtual riku::array get_all(riku::typeinfo t) const;

    virtual bool deserialize(tiny::cref blob);

    // Inherited via job_manager
    virtual void add(job::handle j, int priority);
    virtual void start();
    virtual bool done() const;
    virtual void finish();

    job_list update_jobs;
    job_list render_jobs;

    std::string name;
    bool active = true;

  protected:
    allocator objects = allocator(riku::find("tiny::entity"));
    std::unordered_map< std::string, riku::var<allocator> > factory;

    rkMetaHandle(space);
  };
}
