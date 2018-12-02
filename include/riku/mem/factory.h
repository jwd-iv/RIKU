namespace riku
{
  struct factory
  {
    virtual void* create(void* mem = NULL) const;
    virtual void* copy(void* original, void* mem = NULL) const;
    virtual void  destroy(void* mem) const;
    virtual void  del(void* mem) const;

    typedef void* (*memfunc)(void* obj, void* arg);
    
    // will be where alternative constructors are stored, probably won't be necessary for a long while
    //std::map<typelist, memfunc> constructors;

    memfunc allocate = NULL, //!< throw a new one on the heap
      clone = NULL,          //!< throw a copy of obj on the heap
      free = NULL,           //!< remove obj from the heap
      construct = NULL,      //!< call the constructor at obj
      copy_construct = NULL, //!< construct a copy of args[0] at address obj
      destruct = NULL,       //!< call destructor at obj
      assign = NULL;         //!< call the assignment operator *obj = args[0]

    template<typename T> friend class internal::registrar;
  };

  namespace default_mem
  {
    template<typename T> struct heap_default_new;
    template<typename T> struct heap_copy;
    template<typename T> struct heap_delete;
    //template<typename T, typename... args> struct heap_new;

    template<typename T> struct placement_default_new;
    template<typename T> struct placement_copy;
    template<typename T> struct placement_delete;
    //template<typename T, typename... args> struct placement_new;

    template<typename T> struct assign;
  }
}
