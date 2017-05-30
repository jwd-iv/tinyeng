#include "tiny.h"

#define VFS_LARGEFILE_SUPPORT
#define VFS_IGNORE_CASE
#include "VFSDirInternal.h"
//#include "VFSArchiveLoader.h"
#include "ttvfs_zip.h"
#include "ttvfs.h"

struct ttvfs_system : public tiny::filesystem
{
  // Inherited via filesystem
  virtual void initialize();
  virtual void update(float);
  virtual void close();
  virtual riku::var<handle> open(char const* filename);

protected:
  ttvfs::Root filesys;

  rkMetaHook(ttvfs_system);
};

struct ttvfs_handle : public tiny::filesystem::handle
{
  // Inherited via handle
  virtual std::string filename() const;
  virtual unsigned size() const;
  virtual unsigned pos() const;
  virtual int seek(unsigned p);
  virtual unsigned read(char* mem, unsigned len);

protected:
  friend struct ttvfs_system;
  ttvfs::File* fp = NULL;

  rkMetaHook(ttvfs_handle);
};
