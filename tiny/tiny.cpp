#include "tiny.h"

rkType(tiny::engine,
  rkDefaultFactory
)

rkType(tiny::allocator, )

rkType(tiny::system, )
rkType(tiny::filesystem, rkParent(tiny::system))
rkType(tiny::filesystem::handle, )
rkType(tiny::serializer, rkParent(tiny::system)
  rkStaticFactory
)
rkType(tiny::serializer::format, )
rkType(tiny::input,      rkParent(tiny::system))
rkType(tiny::input::device, )
rkType(tiny::frc,        rkParent(tiny::system))
rkType(tiny::window,     rkParent(tiny::system))
rkType(tiny::renderer,   rkParent(tiny::system))

rkType(tiny::guid_database, rkParent(tiny::system)
  rkStaticFactory
)
rkType(tiny::guid,
  rkDefaultFactory
  rkConvertsTo(unsigned long)
)

rkType(tiny::job, )
rkType(tiny::job_manager, rkParent(tiny::job))
rkType(tiny::job_list, rkParent(tiny::job_manager)
  rkDefaultFactory
  rkMethod(deserialize)
)
rkType(tiny::function_job, rkParent(tiny::job)
  rkDefaultFactory
)

rkType(tiny::space, rkParent(tiny::job_manager)
  rkStaticFactory
  rkMember(active)
  rkMethod(deserialize)
)

rkType(tiny::component,
  rkDefaultFactory
)
rkType(tiny::entity, rkParent(tiny::component)
  rkDefaultFactory
  rkMethod(deserialize)
  rkMethod(serialize)
)
rkType(transform, rkParent(tiny::component)
  rkDefaultFactory
)
