namespace tiny
{
  /**
   * @brief      Page-based object allocator using RIKU's memory functions.
   */
  struct allocator
  {
    allocator(riku::typeinfo t);

    virtual tiny::var allocate();
    virtual bool free(tiny::var v);

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
