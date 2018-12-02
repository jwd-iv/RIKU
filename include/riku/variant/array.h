#pragma once

namespace riku
{
  //! Additional interface for variants that contain a series of objects
  class array_var : public variant_type
  {
  public:
    struct iterator;
    struct const_iterator;

    virtual unsigned length() const = 0;
    virtual variant_type& get(unsigned idx) = 0;
    virtual variant_type const& get(unsigned idx) const = 0;
    virtual bool insert(variant_type const& v, unsigned idx) = 0;
    virtual bool erase(unsigned idx) = 0;

    virtual iterator begin();
    virtual iterator end();
    virtual const_iterator begin() const;
    virtual const_iterator end() const;
    virtual variant for_each(var<riku::function> const f);
    virtual variant for_each(var<riku::function> const f) const;

    virtual bool push_back(variant_type const& v);

    virtual variant_type& operator[](unsigned idx);
    virtual variant_type const& operator[](unsigned idx) const;

    //overrides of variant_type
    virtual bool assign(variant_type const& rhs);
    virtual variant property(string n);
    virtual variant property(string n) const;
    virtual var<riku::function> function(string n);
    virtual var<riku::function> function(string n) const;

    virtual bool operator==(variant_type const& rhs) const;
    virtual bool operator!=(variant_type const& rhs) const;

    virtual variant operator[](string prop);
    virtual variant operator[](string prop) const;

    virtual variant invoke(string func, riku::array& args);
    virtual variant invoke(string func, riku::array& args) const;

    virtual bool is_array() const;
    virtual array_var* as_array();
    virtual array_var const* as_array() const;

    virtual typeinfo    type() const;
    virtual void*       data();
    virtual void const* data() const;

    virtual typeinfo meta() const;

    struct iterator : public variant_type
    {
      virtual bool operator==(iterator const& rhs) const;
      virtual iterator& operator++();
      virtual iterator  operator++(int);
      virtual iterator& operator--();
      virtual iterator  operator--(int);

      virtual variant_type& operator*();
      virtual variant_type* operator->();
      virtual variant_type const& operator*() const;
      virtual variant_type const* operator->() const;

      virtual typeinfo    type() const;
      virtual void*       data();
      virtual void const* data() const;

      virtual typeinfo meta() const;

    private:
      array_var* arr = NULL;
      unsigned i = 0;

      friend class array_var;
    };

    struct const_iterator : public variant_type
    {
      virtual bool operator==(const_iterator const& rhs) const;
      virtual const_iterator& operator++();
      virtual const_iterator  operator++(int);
      virtual const_iterator& operator--();
      virtual const_iterator  operator--(int);

      virtual variant_type const& operator*() const;
      virtual variant_type const* operator->() const;

      virtual typeinfo    type() const;
      virtual void*       data();
      virtual void const* data() const;

      virtual typeinfo meta() const;

    private:
      array_var const* arr = NULL;
      unsigned i = 0;

      friend class array_var;
    };
  };

  //! Default implentation of array_var, holds a vector of other variants
  struct array : public array_var
  {
    template<typename... Targs>
    static inline array create(Targs&&... args);

    array();
    array(array const& rhs);
    array(array&& rhs);
    array(array_var const& rhs);
    array(variant_type const& rhs);
    array& operator=(array const& rhs);
    array& operator=(array_var const& rhs);

    virtual unsigned length() const;
    virtual variant_type& get(unsigned idx);
    virtual variant_type const& get(unsigned idx) const;

    virtual bool insert(variant_type const& v, unsigned idx);
    virtual bool erase(unsigned idx);

    virtual typeinfo meta() const;

    std::vector<variant> vec;
  };
}
