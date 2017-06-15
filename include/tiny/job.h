namespace tiny
{
  class job_manager;
  struct space;

  class job
  {
  public:
    virtual void start() = 0;
    virtual void update(float dt) = 0;
    virtual bool done() const = 0;
    virtual void finish() = 0;

    virtual bool blocking() const;    //nonblocking by default
    virtual char const* lane() const; //no lane by default

    virtual riku::var<space> world() const;

    riku::var<job_manager> boss;

    rkMetaHandle(job);
  };

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

  class job_manager : public job
  {
  public:
    virtual void add(job::handle j, int priority = 0) = 0;
    //TODO: replace add() with push(), pop(), append(), and pop_back()

    virtual void start() = 0;
    virtual void update(float dt) = 0;
    virtual bool done() const = 0;
    virtual void finish() = 0;

    typedef std::unordered_map<std::string, riku::variant> blackboard;
    blackboard board = blackboard();

    rkMetaHandle(job_manager);
  };

  struct job_list : public job_manager
  {
    virtual void add(job::handle j, int priority = 0);
    virtual void start();
    virtual void update(float dt);
    virtual bool done() const;
    virtual void finish();

    virtual bool deserialize(riku::variant_type const& blob);

    std::vector<job::handle> jobs = std::vector<job::handle>();

    rkMetaHook(job_list);
  };
}
