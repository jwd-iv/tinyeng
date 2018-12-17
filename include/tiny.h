#pragma once
#include "RIKU.h"

namespace tiny
{
  typedef riku::variant_type&       ref;
  typedef riku::variant_type const& cref;

  typedef riku::variant var;
}

#include "tiny/memory.h"
#include "tiny/system.h"
#include "tiny/guid.h"

#include "tiny/platform.h"
#include "tiny/serializer.h"
#include "tiny/resource.h"

#include "tiny/job.h"
#include "tiny/space.h"
#include "tiny/entity.h"
#include "tiny/event.h"

#include "tiny/engine.h"
