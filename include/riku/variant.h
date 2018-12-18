#pragma once

namespace riku
{
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
    virtual bool modify(variant_type& rhs) const;
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
    virtual bool is_map() const;
    virtual map_var* as_map();
    virtual map_var const* as_map() const;

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
}
