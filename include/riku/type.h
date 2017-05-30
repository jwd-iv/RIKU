namespace riku
{
  //! Interface for retrieving information about a given data type.
  class type
  {
  public:
    factory mem_funcs;

    virtual string name() const = 0;
    virtual unsigned size() const = 0;
    
    virtual std::vector<string> properties(bool inherited = false) const = 0;
    virtual var<riku::property> property(char const* name) const = 0;

    virtual std::vector<string> functions(bool c = false, bool inherited = false) const = 0;
    virtual var<riku::function> function(char const* name, bool c = false) const = 0;

    virtual typeinfo pointer() const = 0;
    virtual bool has_parent(typeinfo t) const;
    virtual typelist parents(bool full_list = true) const = 0;
    virtual typelist children(bool full_list = false) const;

    virtual typeinfo meta() const;

  private:
    template<typename T> friend class riku::internal::registrar;
    typelist child_types;
  };

  //! Represents a pointer to another type.
  struct pointer_type : public type
  {
    pointer_type* dbl_ptr = NULL;
    typeinfo points_to = NULL;

    virtual string name() const { return points_to != NULL ? points_to->name() + "*" : string(); }
    virtual unsigned size() const { return sizeof(void*); }

    virtual std::vector<string> properties(bool) const { return std::vector<string>(); }
    virtual var<riku::property> property(char const*) const { return var<riku::property>(); }

    virtual std::vector<string> functions(bool, bool) const { return std::vector<string>(); }
    virtual var<riku::function> function(char const*, bool) const { return var<riku::function>(); }

    virtual typeinfo pointer() const;
    virtual typelist parents(bool) const;

    template<typename T> friend class riku::internal::registrar;
    virtual typeinfo meta() const;
  };

  //! Represents a plain data type with no properties or functions.
  struct primitive_type : public type
  {
    string n;
    unsigned s = 0;
    pointer_type ptr_type;

    template<typename T> friend class riku::internal::registrar;
    virtual typeinfo meta() const;
  };

  //! Represents most types, holds lists of properties and functions.
  struct generic_type : public primitive_type
  {
    virtual string name() const;
    virtual unsigned size() const;

    virtual std::vector<string> properties(bool inherited = false) const;
    virtual var<riku::property> property(char const* name) const;

    virtual std::vector<string> functions(bool c = false, bool inherited = false) const;
    virtual var<riku::function> function(char const* name, bool c = false) const;

    virtual typeinfo pointer() const;
    virtual typelist parents(bool full_list = true) const;

    template<typename T> friend class riku::internal::registrar;
    virtual typeinfo meta() const;

  protected:
    std::unordered_map<string, riku::property*> props;
    std::unordered_multimap<string, riku::function*> funcs;

    typelist ps;
  };

  //! Represents *only* a type template, not instantiations, so this is incomplete on its own.
  struct template_type : public generic_type {};
  //! Represents an instantiated template_type.
  struct template_instance : public primitive_type
  {
    template_type* base;
    typelist params;
  };

  /* ???
  struct container_type : public type {};
  struct array_type : public container_type {};
  struct map_type : public container_type {};
  */
}
