#include "tiny.h"
#include <algorithm>

//TODO: manage one giant pool of memory here that is then broken up and eaten by all allocators

#define LEFT_PAD 2
#define ALIGN_ON 4

#define IN_USE '.'
#define BLANK  ' '

namespace tiny
{
  /**
   * @brief      Creates an allocator of the given type, which cannot be changed later.
   *
   * @param[in]  t     The type information of the objects to allocate.
   */
  allocator::allocator(riku::typeinfo t) : type(t)
  {
    blocks_per_page = 1024; //TODO: look this up when riku::attributes are a thing
    block_size = std::max(size_t(type->size() + LEFT_PAD), sizeof(unsigned char*));
    if (block_size % ALIGN_ON) //rudimentary alignment
      block_size += ALIGN_ON - (block_size % ALIGN_ON);
  }

  /**
   * @brief      Allocates another object, adding a new page if necessary.
   *
   * @return     A variant pointing at the new object.
   */
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
  
  /**
   * @brief      Deallocates the object (including destructor if reflected).
   *
   * @param[in]  v     A variant created by this allocator
   *
   * @return     true if deallocation was successful, false otherwise.
   */
  bool allocator::free(riku::variant v)
  {
    if (v.type() != type || v.data() == NULL)
      return false;

    unsigned char* block = ((unsigned char*)v.data()) - LEFT_PAD;
    if (*block == IN_USE)
    { //TODO: add extra pad bytes and use to store typeinfo for verification
      type->mem_funcs.destroy(v.data());
      add_to_free_list(block);
      return true;
    }

    return false;
  }

  /**
   * @brief      How many objects this allocator can hold at once before adding more pages.
   *
   * @return     Current number of pages * how many objects are on a page.
   */
  unsigned allocator::capacity() const
  {
    return pages.size() * blocks_per_page;
  }

  /**
   * @brief      How many objects are currently in use.
   *
   * @return     capacity() - length of the free list.
   */
  unsigned allocator::allocated() const
  {
    return capacity() - available();
  }

  /**
   * @brief      How many more objects can be created before allocating more memory.
   */
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

  /**
   * @brief      Generates a list of all objects currently allocated.
   *
   * @return     An array of the objects in memory order.
   */
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
