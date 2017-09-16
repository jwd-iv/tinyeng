namespace tiny
{
  /**
   * @brief      The interface for global level services.
   * 
   * Every system is a globally accessible singleton, that is created and initialized
   * before the game loop begins, updates every tick, and is then closed and destroyed
   * when the engine is done.
   */
  class system
  {
  public:
    virtual void initialize() = 0;     //!< Perform the initial setup for this service
    virtual void update(float dt) = 0; //!< Update according to the engine's tick length (dt)
    virtual void close() = 0;          //!< Clean up and prepare to be destroyed

    rkMetaHandle(system);
  };

  namespace systems
  {
    void initialize();     //!< Confirm the presence of each required system and initializes all systems in update order
    void update(float dt); //!< Updates all existing systems at once, in order
    void close();          //!< Closes and destroys each system in reverse update order

    /**
     * @brief      Finds a running system by string name
     *
     * @param      name  Null-terminated name of the system or type of system desired
     *
     * @return     NULL if a system implementing the requested type is not found, the system pointer otherwise
     */
    system::handle get(char const* name);

    /**
     * @brief      Templated shortcut for finding a system and getting the appropriate pointer
     *
     * @tparam     T     The type of system to find
     *
     * @return     A handle to the first system in order that is or inherits the requested type, NULL if nothing was found
     */
    template<typename T> inline T::handle get()
    {
      return dynamic_cast<T*>(tiny::systems::get(riku::get<T>()->name().c_str()));
    }
  }
}
