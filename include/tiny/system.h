namespace tiny
{
  class system
  {
  public:
    virtual void initialize() = 0;
    virtual void update(float dt) = 0;
    virtual void close() = 0;

    rkMetaHook(system);
  };

  namespace systems
  {
    void initialize();
    void update(float dt);
    void close();

    system* get(char const* name);

    template<typename T> T* get()
    {
      return dynamic_cast<T*>(tiny::systems::get(riku::get<T>()->name().c_str()));
    }
  }
}

#include "platform.h"
#include "serializer.h"
#include "resource.h"
