#include "RIKU.h"

//variant
namespace riku
{
  variant::variant()
  {
    clear();
  }

  variant::variant(variant const& rhs) { operator=(rhs); }

  variant::variant(variant& rhs) { operator=(rhs); }

  variant::variant(variant&& rhs)
  {
    operator=(rhs); //TODO: actually make an attempt at moving the memory
    rhs.clear();
  }
  
  variant& variant::operator=(variant const& rhs) 
  {
    if (&rhs != this)
    {
      clear();

      if (rhs.in_place())
      {
        rhs.pimpl()->meta()->mem_funcs.copy(const_cast<byte*>(rhs.p.raw), p.raw);
        refs = (unsigned*)p.raw;
      }
      else if (!rhs.empty())
      {
        refs = rhs.refs;
        p.impl = rhs.p.impl;
        ++*refs;
      }
    }
    return *this;
  }
  
  variant& variant::operator=(variant_type const& rhs)
  {
    if (&rhs != this)
    {
      if (rhs.meta()->has_parent(get<variant>()))
        operator=(*dynamic_cast<variant const*>(&rhs));
      else
      {
        clear();

        if (rhs.meta()->size() <= sizeof(val)) //copy in-place
        {
          rhs.meta()->mem_funcs.copy(const_cast<variant_type*>(&rhs), p.raw);
          refs = (unsigned*)p.raw;
        }
        else //allocate ref-counted copy
        {
          p.impl = static_cast<variant_type*>(rhs.meta()->mem_funcs.copy(const_cast<variant_type*>(&rhs)));
          refs = new unsigned(1);
        }
      }
    }
    return *this;
  }
  
  variant::~variant()
  {
    clear();
  }

  typeinfo variant::type() const
  {
    if (empty())
      return NULL;

    return (pimpl()->type() == NULL && pimpl()->data() != NULL) ? pimpl()->meta() : pimpl()->type();
  }

  void* variant::data()
  {
    if (!empty() && !in_place() && *refs > 1)
    {
      variant_type* temp = pimpl();
      clear();

      p.impl = static_cast<variant_type*>(temp->meta()->mem_funcs.copy(temp));
      refs = new unsigned(1);
    }
    return empty() ? NULL : pimpl()->data();
  }

  void const* variant::data() const
  {
    return empty() ? NULL : pimpl()->data();
  }

  bool variant::assign(variant_type const& rhs)
  {
    if (pimpl() == NULL)
      return false;
    return pimpl()->assign(rhs);
  }

  bool variant::assignto(variant_type& rhs) const
  {
    if (pimpl() == NULL)
      return false;
    return pimpl()->assignto(rhs);
  }

  variant variant::property(string n)
  {
    if (pimpl() == NULL)
      return variant();
    return pimpl()->property(n);
  }

  variant variant::property(string n) const
  {
    if (pimpl() == NULL)
      return variant();
    return pimpl()->property(n);
  }

  std::vector<string> variant::properties() const
  {
    if(pimpl() == NULL)
      return std::vector<string>();
    return pimpl()->properties();
  }

  var<riku::function> variant::function(string n)
  {
    if (pimpl() == NULL)
      return var<riku::function>();
    return pimpl()->function(n);
  }

  var<riku::function> variant::function(string n) const
  {
    if (pimpl() == NULL)
      return var<riku::function>();
    return pimpl()->function(n);
  }

  bool variant::operator==(variant_type const& rhs) const
  {
    if (pimpl() == NULL)
      return rhs.data() == NULL;
    return (*pimpl()) == rhs;
  }

  bool variant::is_array() const
  {
    return empty() ? false : pimpl()->is_array();
  }

  array_var* variant::as_array()
  {
    return empty() ? NULL : pimpl()->as_array();
  }

  array_var const* variant::as_array() const
  {
    return empty() ? NULL : pimpl()->as_array();
  }

  bool variant::is_map() const
  {
    return empty() ? false : pimpl()->is_map();
  }

  map_var* variant::as_map()
  {
    return empty() ? NULL : pimpl()->as_map();
  }

  map_var const* variant::as_map() const
  {
    return empty() ? NULL : pimpl()->as_map();
  }

  variant variant::invoke(string func, riku::array& args)
  {
    if (empty())
      return variant();
    return pimpl()->invoke(func, args);
  }

  variant variant::invoke(string func, riku::array& args) const
  {
    if (empty())
      return variant();
    return pimpl()->invoke(func, args);
  }

  void variant::clear()
  {
    if (in_place())
    {
      pimpl()->meta()->mem_funcs.destroy(pimpl());
    }
    else if (!empty())
    {
      --*refs;
      if (*refs == 0)
      {
        pimpl()->meta()->mem_funcs.del(pimpl());
        delete refs;
      }
    }
    
    for (auto& b : p.raw)
      b = 0;
    refs = NULL;
  }

  bool variant::empty() const
  {
    return refs == NULL;
  }

  bool variant::in_place() const
  {
    return !empty() && (void*)refs == (void*)p.raw;
  }

  variant_type* variant::pimpl()
  {
    return in_place() ? reinterpret_cast<variant_type*>(p.raw) : p.impl;
  }

  variant_type const* variant::pimpl() const
  {
    return in_place() ? reinterpret_cast<variant_type const*>(p.raw) : p.impl;
  }

  typeinfo variant::meta() const
  {
    return riku::get<variant>();
  }

  variant_type const& variant::internal() const
  {
    return empty() ? *this : *pimpl();
  }
};
