namespace tiny
{
  struct allocator
  {
    allocator(riku::typeinfo t);

    virtual riku::variant allocate();
    virtual bool free(riku::variant v);

    virtual unsigned capacity() const;
    virtual unsigned allocated() const;
    virtual unsigned available() const;

    virtual riku::array in_use() const;

    riku::typeinfo const type;

  protected:
    void add_page();
    void add_to_free_list(unsigned char* address);
    unsigned short blocks_per_page, block_size;

    std::list<unsigned char*> pages;
    unsigned char* free_list = NULL;
  };
}
