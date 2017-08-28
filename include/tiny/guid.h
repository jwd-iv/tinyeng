namespace tiny
{
  struct guid : public riku::variant_type
  {
    template<typename T> static guid create(T* obj);

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

  struct guid_database : public system
  {
    guid add_entry(riku::variant e);
    riku::variant get_entry(guid id) const;
    void remove_entry(guid id);

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

    rkMetaHook(guid_database);
  };

  //put obj in the database and make a guid of its id
  template<typename T> guid guid::create(T* obj)
  {
    if(obj != NULL)
      return tiny::systems::get<guid_database>()->add_entry(riku::ptr(obj->meta(), obj));
    return guid();
  }
}
