#include "tiny.h"
#include <algorithm>

#define LEFT_PAD 2
#define ALIGN_ON 4

#define IN_USE '.'
#define BLANK  ' '

namespace tiny
{
  allocator::allocator(riku::typeinfo t) : type(t)
  {
    blocks_per_page = 1024; //look this up when riku::attributes are a thing
    block_size = std::max(size_t(type->size() + LEFT_PAD), sizeof(unsigned char*));
    if (block_size % ALIGN_ON) //rudimentary alignment
      block_size += ALIGN_ON - (block_size % ALIGN_ON);
  }

  riku::variant allocator::allocate()
  {
    if (free_list == NULL)
      add_page();

    unsigned char* block = free_list;
    free_list = *reinterpret_cast<unsigned char**>(free_list);

    // the first two bytes are reserved for indicating the block is in use
    memset(block, BLANK, block_size);
    *block = IN_USE;
    return riku::ptr(type, type->mem_funcs.create(block + LEFT_PAD));
  }
  
  bool allocator::free(riku::variant v)
  {
    if (v.type() != type || v.data() == NULL)
      return false;

    unsigned char* block = ((unsigned char*)v.data()) - LEFT_PAD;
    if (*block == IN_USE)
    {
      type->mem_funcs.destroy(v.data());
      add_to_free_list(block);
      return true;
    }

    return false;
  }

  unsigned allocator::capacity() const
  {
    return pages.size() * blocks_per_page;
  }

  unsigned allocator::allocated() const
  {
    return capacity() - available();
  }

  unsigned allocator::available() const
  {
    unsigned count = 0;
    unsigned char* walk = free_list;
    while (walk != NULL)
    {
      walk = *reinterpret_cast<unsigned char**>(walk);
      ++count;
    }
    return count;
  }

  riku::array allocator::in_use() const
  {
    riku::array ret;

    for (auto const& page : pages)
    {
      for (int i = 0; i < blocks_per_page*block_size; i += block_size)
      {
        if (page[i] == IN_USE)
          ret.insert(riku::ptr(type, page + i + LEFT_PAD), ret.length());
      }
    }

    return ret;
  }
  
  void allocator::add_page()
  {
    unsigned char* page = new unsigned char[blocks_per_page * block_size];
    pages.push_back(page);

    for (int i = (blocks_per_page - 1) * block_size; i >= 0; i -= block_size)
      add_to_free_list(page + i);
  }
  
  void allocator::add_to_free_list(unsigned char* address)
  {
    if (address != NULL)
    {
      memset(address, BLANK, block_size);
      *(reinterpret_cast<unsigned char**>(address)) = free_list;
      free_list = address;
    }
  }
}
