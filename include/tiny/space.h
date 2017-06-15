namespace tiny
{
  struct space : public job_manager
  {
    static space* named(char const* name);

    virtual void update(float dt);
    virtual void render(float a);

    virtual guid create(riku::typeinfo t, char const* archetype = NULL);
    virtual void destroy(guid ID);

    virtual riku::array get_all(riku::typeinfo t) const;

    virtual bool deserialize(riku::variant_type const& blob);

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
