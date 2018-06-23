#pragma once

#define GLM_FORCE_SWIZZLE
#include "glm/glm.hpp"

namespace tiny
{
  class transform : public component
  {
  public:
    virtual glm::vec3 pos() const = 0;
    virtual glm::vec3 scl() const = 0;
    virtual glm::float_t rot() const = 0;

    virtual bool set_pos(glm::vec3) = 0;
    virtual bool set_scl(glm::vec3) = 0;
    virtual bool set_rot(glm::float_t) = 0;

    rkMetaHandle(transform);
  };
}