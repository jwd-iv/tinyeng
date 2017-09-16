namespace tiny
{
  //! Global singleton that controls the main game loop
  struct engine
  {
    static engine& get(); //!< Provides access to the engine

    void initialize(); //!< Starts systems, loads initial data
    void run();        //!< Runs the main game loop until the engine is told to stop() or has nothing to do
    void stop();       //!< Tells the engine to wrap up early
    void close();      //!< Clears out all data, closes all systems

    bool running = false;
    job_list start, update, render, finish;
    std::unordered_map< std::string, space::handle > spaces;
  };
}
