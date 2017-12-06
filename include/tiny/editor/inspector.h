namespace tiny
{
  struct inspector : public system
  {
  public:
    virtual bool receiveWindowEvent(void const* pEvent);

    // Inherited via system
    virtual void initialize();
    virtual void update(float dt);
    virtual void close();

  protected:
    virtual void render(float dt);

  private:
    ;

    rkMetaHook(inspector);
  };
}