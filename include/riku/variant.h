namespace riku
{
  //! The interface for a variant, meant to hold any object. The various implementations of this class specify how that obj is stored.
  class variant_type
  {
  public:
    virtual typeinfo    type() const = 0;
    virtual void*       data() = 0;
    virtual void const* data() const = 0;

    virtual bool assign(variant_type const& rhs);
    virtual bool assignto(variant_type& rhs) const;

    virtual variant property(string n);
    virtual variant property(string n) const;
    virtual std::vector<string> properties() const;

    virtual var<riku::function> function(string n);
    virtual var<riku::function> function(string n) const;
    virtual variant invoke(string func, riku::array& args);
    virtual variant invoke(string func, riku::array& args) const;

    virtual bool is_array() const;
    virtual array_var* as_array();
    virtual array_var const* as_array() const;
    /*virtual bool is_map() const;
    virtual map_var& as_map();
    virtual map_var const& as_map() const;*/

    inline bool operator==(variant_type const& rhs) const;
    inline bool operator!=(variant_type const& rhs) const;
    inline variant operator[](string prop);
    inline variant operator[](string prop) const;
    inline variant operator()(string func, riku::array& args);
    inline variant operator()(string func, riku::array& args) const;

    template<typename T> inline bool     is() const; //!< THIS IS YOUR NULL-CHECK BEFORE TRYING TO CONVERT VARIANTS
    template<typename T> inline T&       as();
    template<typename T> inline T const& as() const;
    template<typename T> inline T*       to();
    template<typename T> inline T const* to() const;
    template<typename T> inline T   convert() const;

    template<typename T> inline variant_type& operator<<(T const& rhs);
    template<typename T> inline void          operator>>(T& rhs) const;

    template<typename... T> inline variant operator()(char const* f, T&&... args);
    template<typename... T> inline variant operator()(char const* f, T&&... args) const;

    virtual typeinfo meta() const;
    virtual variant_type const& internal() const;

  /*protected:
    virtual void* clone(void* mem = NULL)
    {
      return meta()->mem_funcs.copy(this, mem);
    }
    virtual void* clone_data(void* mem = NULL)
    {
      if (type() != NULL && data() != NULL && !meta()->has_parent(riku::get<array_var>()))
      {
        return type()->mem_funcs.copy(data(), mem);
      }
      return NULL;
    }*/
  };

  //! Acts as a pointer to an object, doesn't own the memory.
  struct ptr : public variant_type
  {
    ptr(typeinfo t = typeinfo(), void const* p = NULL);
    ptr(typeinfo t, void* p);

    template<typename T> ptr(T* pt);
    template<typename T> ptr(T const* cpt);
    template<typename T> ptr(T& rt);
    template<typename T> ptr(T const& crt);
    template<> ptr(ptr& rt);
    template<> ptr(ptr const& crt);

    virtual typeinfo type() const;
    virtual void*    data();
    virtual void const* data() const;

    virtual typeinfo meta() const;

  protected:
    union {
      const void* cptr;
      void* ptr = NULL;
    } d; //!< A trick to save memory but hold both const and non-const pointers.
    bool c = true;
    typeinfo contains = NULL;
  };

  //! Holds a copy of an object by value, using a lazy copy-on-write policy.
  struct val : public variant_type
  {
    val(typeinfo t = typeinfo() , void const* p = NULL);
    val& operator=(val const& rhs);
    ~val();
    
    template<typename T> val(T const& rhs);
    template<typename T> val(T& rhs);
    template<typename T> val(T&& rhs);

    template<> val(val const& rhs);
    template<> val(val& rhs);
    template<> val(val&& rhs);

    virtual typeinfo type() const;
    virtual void* data();
    virtual void const* data() const;

    virtual typeinfo meta() const;

  protected:
    union {
      struct {
        void* v;
        unsigned* refs;
      } p;
      byte raw[RIKU_MAX_VAR_SIZE];
    } d; //!< This holds one of two things: a reference-counted pointer to an allocated object, or if the object's small enough it's constructed right in the raw array.
    typeinfo contains = NULL;

    void fork();
    void free();
    void clear();
  };

  //! The actual variant, used to carry other implementations of variant_type. Only inherit from this class if you know exactly what you're doing.
  struct variant : public variant_type
  {
    template<typename T> inline variant(T const& rhs);
    template<typename T> inline variant(T& rhs);
    template<typename T> inline variant(T&& rhs);
    template<typename T> inline variant(T const* rhs);
    template<typename T> inline variant(T* rhs);

    variant();
    variant(variant const& rhs);
    variant(variant& rhs);
    variant(variant&& rhs);
    variant& operator=(variant const& rhs);
    variant& operator=(variant_type const& rhs);
    ~variant();

    virtual typeinfo type() const;
    virtual void*    data();
    virtual void const* data() const;
    virtual bool assign(variant_type const& rhs);
    virtual bool assignto(variant_type& rhs) const;
    virtual variant property(string n);
    virtual variant property(string n) const;
    virtual std::vector<string> properties() const;
    virtual var<riku::function> function(string n);
    virtual var<riku::function> function(string n) const;
    virtual variant invoke(string func, riku::array& args);
    virtual variant invoke(string func, riku::array& args) const;

    virtual bool is_array() const;
    virtual array_var* as_array();
    virtual array_var const* as_array() const;

    virtual bool operator==(variant_type const& rhs) const;

    virtual typeinfo meta() const;
    virtual variant_type const& internal() const;

  private:
    union {
      variant_type* impl;
      byte raw[sizeof(val)];
    } p;
    unsigned* refs = NULL;

  protected:
    void clear();
    bool empty() const;
    bool in_place() const;
    variant_type* pimpl();
    variant_type const* pimpl() const;
  };

  //! variant that only holds the specified type or something that inherits it
  template<typename T>
  struct var : public variant
  {
    var();
    var(var<T> const& rhs);
    var(variant_type const& rhs);
    var(variant_type& rhs);
    //var(variant_type&& rhs);
    var(variant const& rhs);
    var(variant& rhs);
    //var(variant&& rhs);
    ~var();

    var& operator=(variant const& rhs);
    var& operator=(variant_type const& rhs);

    operator T*();
    operator T const*() const;
    T& operator*();
    T const& operator*() const;
    T* operator->();
    T const* operator->() const;

#ifdef _DEBUG
  private:
	T const* _pDebug = NULL;
#endif
  };
}
