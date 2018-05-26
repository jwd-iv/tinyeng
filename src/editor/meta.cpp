#include "tiny.h"
#include "tiny\editor\editor.h"
#include "tiny\editor\inspector.h"
#include "tiny\editor\selector.h"
#include "tiny\editor\tool.h"
#include "tiny\editor\view.h"

rkExportLib(tinyedit)

rkType(tiny::inspector, rkParent(tiny::system) rkDefaultFactory
  rkMember(active)
)

rkType(tiny::inspector::view, rkStaticFactory)
