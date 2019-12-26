#include "tiny.h"
#include "core/vfs_file.h"
#include "core/json_format.h"
#include "core/transform.h"

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

rkType(tiny::transform, rkParent(tiny::component)
)

rkType(glm::vec3, rkDefaultFactory
  rkMember(x) rkMember(y) rkMember(z)
)

rkType(glm::vec4, rkDefaultFactory
  rkMember(r) rkMember(g) rkMember(b) rkMember(a)
)
