#include <iostream>
#include "tiny.h"

rkLink(tiny_core)

int main()
{
  tiny::engine::get().initialize();
  tiny::engine::get().run();
  tiny::engine::get().close();

  return 0;
}
