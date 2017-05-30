#include "tiny.h"
#include "vfs_file.h"
#include "json_format.h"

rkExportLib(tiny_core)

rkType(ttvfs_system, rkParent(tiny::filesystem)
  rkStaticFactory
)

rkType(ttvfs_handle, rkParent(tiny::filesystem::handle)
  rkDefaultFactory
)

rkType(json_format, rkParent(tiny::serializer::format)
  rkStaticFactory
)

rkType(json_blob, rkParent(riku::variant_type)
  rkDefaultFactory
)
