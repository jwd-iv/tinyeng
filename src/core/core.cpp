#include "tiny.h"
#include "core/vfs_file.h"
#include "core/json_format.h"

rkLink(tiny)
rkExportLib(tiny_core)

rkType(ttvfs_system, rkParent(tiny::filesystem)
  rkStaticFactory
)

rkType(ttvfs_handle, rkParent(tiny::filesystem::file)
  rkDefaultFactory
)

rkType(json_format, rkParent(tiny::serializer::format)
  rkStaticFactory
)

rkType(json_blob, rkParent(riku::variant_type)
  rkDefaultFactory
)
