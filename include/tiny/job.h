namespace tiny
{
  class job_manager;
  struct space;

  //! Abstract interface for discrete tasks done by the engine
  class job
  {
  public:
    virtual void start() = 0;          //!< Initialize the task
    virtual void update(float dt) = 0; //!< Update one tick
    virtual bool done() const = 0;     //!< Check if the task is done
    virtual void finish() = 0;         //!< Close the task

    virtual bool blocking() const;     //!< Check if this job prevents later jobs from executing, `false` by default
    virtual char const* lane() const;  //!< Get the "lane" of this task, none by default

    virtual riku::var<space> world() const; //!< Gets the space containing this task, if any

    riku::var<job_manager> boss;

    rkMetaHandle(job);
  };

  //! job which executes an arbitrary function until it returns true
  struct function_job : public job
  {
    function_job() : func() {}
    template<typename Functor> function_job(Functor f) : func(riku::function(f)) {}

    virtual void start();
    virtual void update(float dt);
    virtual bool done() const;
    virtual void finish();

    riku::var<riku::function> func;

    rkMetaHook(function_job);
  };

  //! Interface for the task of managing other tasks
  class job_manager : public job
  {
  public:
    //! Add another job for this to manage
    virtual void add(job::handle j, int priority = 0) = 0;
    //TODO: replace add() with push(), pop(), append(), and pop_back()

    // Inherited via job
    virtual void start() = 0;
    virtual void update(float dt) = 0;
    virtual bool done() const = 0;
    virtual void finish() = 0;

    typedef std::unordered_map<std::string, riku::variant> blackboard;
    blackboard board = blackboard();

    rkMetaHandle(job_manager);
  };

  //! Executes jobs in order until all are done
  struct job_list : public job_manager
  {
    virtual void add(job::handle j, int priority = 0);
    virtual void start();
    virtual void update(float dt);
    virtual bool done() const;
    virtual void finish();

    virtual bool deserialize(tiny::cref blob);

    std::vector<job::handle> jobs = std::vector<job::handle>();

    rkMetaHook(job_list);
  };
}
