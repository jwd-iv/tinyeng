namespace tiny
{
  struct engine
  {
    static engine& get();

    void initialize();
    void run();
    void stop();
    void close();

    bool running = false;
    job_list start, update, render, finish;
    std::unordered_map< std::string, riku::var<space> > spaces;
  };
}
