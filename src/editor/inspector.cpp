#include "tiny.h"
#include "tiny\editor\inspector.h"
#include "AntTweakBar.h"

TwBar* test_bar = NULL;

bool tiny::inspector::receiveWindowEvent(void const* pEvent)
{
  return TwEventSFML(pEvent, 2, 4);
}

void tiny::inspector::initialize()
{
  TwInit(TW_OPENGL, NULL);
  test_bar = TwNewBar("This is a test.");

  tiny::engine::get().render.add(riku::val(function_job(
    [](float dt)->bool {
      tiny::systems::get<tiny::inspector>()->render(dt);
      return false;
    }
  )));
}

void tiny::inspector::update(float dt)
{
  auto win = systems::get<window>();
  TwWindowSize(win->width(), win->height());
}

void tiny::inspector::render(float dt)
{
  TwDraw();
}

void tiny::inspector::close()
{
  TwDeleteAllBars();
  test_bar = NULL;

  TwTerminate();
}
