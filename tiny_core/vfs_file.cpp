#include "vfs_file.h"
#include <iostream>

void ttvfs_system::initialize()
{
  if (!ttvfs::checkCompat())
    ::system("pause");

  filesys.AddLoader(new ttvfs::DiskLoader);
  //filesys.AddArchiveLoader(new ttvfs::VFSZipArchiveLoader);

  filesys.Mount("", "game");
  filesys.Mount(ttvfs::GetUserDir().c_str(), "user");
}

void ttvfs_system::update(float)
{
  filesys.ClearGarbage();
}

void ttvfs_system::close()
{
  filesys.Clear();
}

riku::var<tiny::filesystem::handle> ttvfs_system::open(char const* filename)
{
  std::string fname(filename);
  ttvfs::FixSlashes(fname);

  ttvfs_handle f;
  f.fp = filesys.GetFile(fname.c_str());
  if(f.fp != NULL)
    return riku::val(f);
  return riku::variant();
}

std::string ttvfs_handle::filename() const
{
  if(fp != NULL)
    return std::string(fp->fullname(), fp->fullnameLen());
  return std::string();
}

unsigned ttvfs_handle::size() const
{
  return fp == NULL ? 0 : fp->size();
}

unsigned ttvfs_handle::pos() const
{
  return fp == NULL ? 0 : fp->isopen() ? fp->getpos() : 0;
}

int ttvfs_handle::seek(unsigned p)
{
  unsigned old = pos();
  if (fp != NULL)
    fp->seek(p, 0);
  return p - old;
}

unsigned ttvfs_handle::read(char* mem, unsigned len)
{
  if (pos() > size())
    seek(0);
  if (len == 0)
    len = size();

  unsigned p = pos();

  if (fp != NULL)
  {
    fp->open("r");
    fp->seek(p, 0);

    return fp->read(mem, len);
  }

  return 0;
}
