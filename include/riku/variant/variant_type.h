#pragma once

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
}
