#include "tiny.h"

namespace tiny
{
  engine& engine::get()
  {
    static engine e;
    return e;
  }

  void engine::initialize()
  {
    auto ini = systems::get<tiny::serializer>()->parse("game/tiny.json");

    //for (auto const& iter : ini["Systems"].properties());
    systems::initialize();

    auto jobs = ini["Jobs"];
    jobs["start"]  >> start;
    jobs["update"] >> update;
    jobs["render"] >> render;
    jobs["finish"] >> finish;

    update.add(riku::val(function_job( []()->bool {return engine::get().running;} )));

    start.start();
    render.start();
    
    auto spaces = ini["Spaces"];
    for (auto const& name : spaces.properties())
    {
      space* spc = space::named(name.c_str());
      if (spc == NULL)
        continue;

      spaces[name.c_str()] >> *spc;
      spc->start();
    }
  }

  void engine::run()
  {
    running = true;

    while (running)
    {
      frc* FRC = systems::get<frc>();
      FRC->begin_frame();

      while (running && FRC->time_left())
      {
        float dt = FRC->dt();

        systems::update(dt);

        if (!start.done())
        {
          start.update(dt);
          if (start.done())
          {
            start.finish();
            update.start();
          }
        }
        else if (!update.done())
        {
          update.update(dt);
          if (update.done())
          {
            update.finish();
            finish.start();
          }
        }
        else if (!finish.done())
          finish.update(dt);
        else
          running = false;

        for (auto& pair : spaces)
          if(pair.second->active)
            pair.second->update(dt);
      }

      if (!render.done())
        render.update(FRC->df());

      for (auto& pair : spaces)
        pair.second->render(FRC->df());

      systems::get<renderer>()->render(FRC->alpha());
      systems::get<window>()->render();
    }
  }

  void engine::close()
  {
    for (auto& pair : spaces)
    {
      pair.second->finish();
      //delete space
    }
    spaces.clear();

    render.finish();
    finish.finish();

    systems::close();
  }
}
