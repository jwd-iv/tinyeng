#include "tiny.h"

namespace tiny
{
  bool job::blocking() const
  {
    return true;
  }

  char const* job::lane() const
  {
    return NULL;
  }
  
  void job_list::add(job::handle j, int priority)
  {
    if (jobs.size() == 0)
      priority = 0;

    while (priority < 0)
      priority += jobs.size();

    if (priority > jobs.size())
      priority = jobs.size();

    j->boss = this;
    jobs.insert(jobs.begin() + (jobs.size() - priority), j);
  }
  
  void job_list::start()
  {
    for (auto& iter : jobs)
    {
      iter->start();
      if (iter->blocking())
        break;
    }
  }
  
  void job_list::update(float dt)
  {
    bool unblock = false;
    for (auto iter = jobs.begin(); iter != jobs.end(); ++iter)
    {
      auto& j = *iter;

      if (unblock)
        j->start();
      j->update(dt);

      if (j->done())
      { // clean up this job
        j->finish();

        // start up the jobs that this one blocked
        unblock = j->blocking();
        iter = jobs.erase(iter);
        if (jobs.empty())
          return;
        continue;
      }
      else if (j->blocking())
        break;
    }
  }
  
  bool job_list::done() const
  {
    auto iter = jobs.begin();
    while (iter != jobs.end())
    {
      if (!(*iter)->done())
        return false;
      if ((*iter)->blocking())
        break;
      ++iter;
    }
    return iter == jobs.end() ? true : false;
  }
  
  void job_list::finish()
  {
    for (auto& j : jobs)
      j->finish();
    jobs.clear();
  }

  bool job_list::deserialize(riku::variant_type const& blob)
  { // grab the list of jobs from the blob
    auto list = blob["list"];
    if (list.is_array())
    { // iterate over each listed job
      for (auto const& iter : *list.as_array())
      { // see if it has type information
        if (iter["riku::type"].is<std::string>())
        {
          riku::typeinfo jobtype = riku::find(iter["riku::type"].as<std::string>().c_str());

          if (jobtype != NULL && jobtype->has_parent(riku::get<job>()))
          { // try to allocate the job
            riku::val newjob(jobtype);
            if (newjob.data() != NULL)
            {
              iter.assignto(newjob);
              jobs.push_back(newjob);
            }
          }
        }
      }
    }
    return true;
  }

  void function_job::start()
  {
    if (func.data() == NULL || func->arg_count > 1 || func->obj_type != NULL
      || func->ret_type != riku::get<bool>())
      finish();
  }

  void function_job::update(float dt)
  {
    if (func.data() != NULL)
    {
      if (func->arg_count == 1)
      {
        if ((*func)(riku::ptr(), dt).as<bool>())
          finish();
      }
      else if((*func)(riku::ptr()).as<bool>())
        finish();
    }
  }

  bool function_job::done() const
  {
    return func.data() == NULL;
  }

  void function_job::finish()
  {
    func = riku::var<riku::function>();
  }
}