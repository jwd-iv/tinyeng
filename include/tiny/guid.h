namespace tiny
{
  //! Implementation of variant for translating an ID into an object transparently
  struct guid : public riku::variant_type
  {
    template<typename T> static guid create(T* obj); //!< Make a new guid for `obj`

    guid();
    guid(guid const& rhs);
    guid& operator=(guid const& rhs);
    ~guid();

    // Inherited via variant_type
    virtual riku::typeinfo type() const;
    virtual void* data();
    virtual void const* data() const;

  protected:
    union {
      unsigned long long full;
      struct {
        unsigned long index;
        unsigned long version;
      } split;
    } id;

    friend struct guid_database;
    rkMetaHook(guid);
  };

  //! system for managing all existing guids
  struct guid_database : public system
  {
    guid add_entry(riku::variant e);        //!< Create a new guid for an object
    riku::variant get_entry(guid id) const; //!< Retrieve the object based on guid
    void remove_entry(guid id);             //!< Erase an object's ID (invalidates that guid)

    // Inherited via system
    virtual void initialize();
    virtual void update(float dt);
    virtual void close();

  protected:
    unsigned const blocks_per_page = 4;
    struct mem_block {
      guid ID;
      riku::variant mem;
    };

    std::vector<mem_block*> pages;
    mem_block* free_list;

    mem_block* new_block();
    void clear_block(mem_block* obj);
    bool is_valid(guid ID) const;

    rkMetaHandle(guid_database);
  };

  template<typename T> guid guid::create(T* obj)
  {
    if(obj != NULL)
      return tiny::systems::get<guid_database>()->add_entry(riku::ptr(obj->meta(), obj));
    return guid();
  }
}
