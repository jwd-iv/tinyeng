namespace tiny
{
  struct entity;

  //! The building block of every game object. Represents a chunk of game data
  struct component
  {
    guid me; //!< This object's ID
    riku::var<entity> parent; //!< A handle to the parent entity

    /**
     * @brief      Notify this component of a game event
     *
     * @param      message  The name of the event
     * @param[in]  data     Any accompanying data
     *
     * @return     `true` if this component responded in any way
     */
    virtual bool notify(char const* message, riku::variant data);

    rkMetaHandle(component);
  };

  //! Used to manage game objects as hierarchies of components
  struct entity : public component
  {
    std::string archetype;        //!< The archetype used to create this
    riku::var<space> world;       //!< A handle to the containing space
    std::vector<guid> components; //!< Handles to all the component parts of this object

    ~entity();
 
    //! Creates a component of the given type name and adds it to the game object
    virtual guid add(char const* type);
    
    /**
     * @brief      Gets the first component of this object of the specified type
     *
     * @param      type       The name of the type to look for
     * @param[in]  useparent  If `true`, the entity's parent object will be
     *                        searched for the component if none is found.
     *
     * @return     The ID of the found component
     */
    virtual guid find(char const* type, bool useparent = false) const;

    //! Get a list of all child entities to this object
    virtual std::vector<guid> children(bool grandchildren = false) const;

    //! Passes the event to all components and children
    virtual bool notify(char const* message, riku::variant data);

    virtual bool deserialize(riku::variant_type const& blob);
    virtual bool serialize(riku::variant_type& blob) const;

    //! Template shorthand to streamline getting a component
    template<typename T> inline T* find(bool useparent = false) const
    {
      return find(riku::get<T>()->name().c_str(), useparent).to<T>();
    }

    rkMetaHandle(entity);
  };
}
