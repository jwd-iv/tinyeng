#include "tiny.h"

namespace tiny
{
  guid::guid() { id.full = 0; }
  
  guid::guid(guid const & rhs) { id.full = rhs.id.full; }

  guid& guid::operator=(guid const& rhs)
  {
    id.full = rhs.id.full;
    return *this;
  }
  
  guid::~guid() {}

  riku::typeinfo guid::type() const
  {
    return systems::get<guid_database>()->get_entry(*this).type();
  }

  void* guid::data()
  {
    //TODO: thread safety here
    return systems::get<guid_database>()->get_entry(*this).data();
  }

  void const* guid::data() const
  {
    return systems::get<guid_database>()->get_entry(*this).data();
  }

  guid guid_database::add_entry(tiny::var e)
  {
    mem_block* entry = new_block();
    if (entry != NULL)
    {
      entry->mem = e;
      return entry->ID;
    }

    return guid();
  }

  tiny::var guid_database::get_entry(guid id) const
  {
    if (is_valid(id))
    {
      unsigned long page = id.id.split.index / blocks_per_page;
      unsigned long offset = id.id.split.index % blocks_per_page;

      return pages[page][offset].mem;
    }
    return riku::variant();
  }

  void guid_database::remove_entry(guid id)
  {
    if (is_valid(id))
    {
      unsigned long page = id.id.split.index / blocks_per_page;
      unsigned long offset = id.id.split.index % blocks_per_page;

      clear_block(pages[page] + offset);
    }
  }

  void guid_database::initialize()
  {
  }

  void guid_database::update(float dt)
  {
  }

  void guid_database::close()
  {
  }

  guid_database::mem_block* guid_database::new_block()
  {
    if (free_list == NULL)
    { // add a new page to the free list
      mem_block* newPage = new mem_block[blocks_per_page];
      pages.push_back(newPage);

      unsigned long startIndex = (pages.size() - 1)*blocks_per_page;

      for (unsigned long i = blocks_per_page; i > 0; --i)
      {
        mem_block* iter = newPage + i - 1;
        iter->ID.id.split.index = startIndex + i - 1;
        iter->ID.id.split.version = 1;

        iter->mem = riku::val((void*)free_list);
        free_list = iter;
      }
    }

    unsigned long index = free_list->ID.id.split.index;
    free_list = (mem_block*)free_list->mem.as<void*>();

    unsigned long page = index / blocks_per_page;
    unsigned long offset = index % blocks_per_page;
    mem_block& obj = pages[page][offset];

    obj.mem = riku::variant();
    obj.ID.id.split.index = index;

    return &obj;
  }
  
  void guid_database::clear_block(mem_block* obj)
  {
    ++obj->ID.id.split.version;
    obj->mem = riku::val((void*)free_list);
    free_list = obj;
  }
  
  bool guid_database::is_valid(guid ID) const
  {
    unsigned long page = ID.id.split.index / blocks_per_page;
    unsigned long offset = ID.id.split.index % blocks_per_page;

    return (page < pages.size() && offset < blocks_per_page
      && pages[page][offset].ID.id.split.version == ID.id.split.version);
  }
}
