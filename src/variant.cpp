#include "RIKU.h"

//variant_type
namespace riku
{
  bool variant_type::assign(variant_type const& rhs)
  {
    if (data() != NULL && rhs.data() != NULL)
    {
      if (rhs.type() == type() && type()->mem_funcs.assign != NULL)
      {
        return type()->mem_funcs.assign(data(), const_cast<void*>(rhs.data())) != NULL;
      }
      else if(type()->has_parent(get<variant>()))
      {
        if (rhs.meta()->has_parent(get<variant_type>()))
          return as<variant>().operator=(rhs).data() != NULL;
        else
          return as<variant>().assign(rhs);
      }
      else
      {
        bool anything_happened = false;

        for (auto const& n : type()->properties(true))
        {
          auto prop = type()->property(n.c_str());
          if (prop.data() != NULL && rhs.type()->property(n.c_str()).data() != NULL)
            anything_happened |= prop->set(*this, rhs[n]);
        }

        return anything_happened;
      }
    }
    else
      return false;
  }

  bool variant_type::assignto(variant_type& rhs) const
  {
    return rhs.assign(*this);
  }
  
  variant variant_type::property(string n)
  {
    if (type() == NULL || data() == NULL)
      return variant();
    
    auto prop = type()->property(n.c_str());

    if(prop.data() == NULL)
      return variant();

    return prop->get(*this);
  }
  
  variant variant_type::property(string n) const
  {
    if (type() == NULL || data() == NULL)
      return variant();

    auto prop = type()->property(n.c_str());

    if (prop.data() == NULL)
      return variant();

    return prop->get(*this);
  }

  std::vector<string> variant_type::properties() const
  {
    if (type() != NULL)
      return type()->properties(true);
    return std::vector<string>();
  }
  
  var<riku::function> variant_type::function(string n)
  {
    if (type() != NULL)
      return type()->function(n.c_str());

    return var<riku::function>();
  }
  
  var<riku::function> variant_type::function(string n) const
  {
    if (type() != NULL)
      return type()->function(n.c_str(), true);

    return var<riku::function>();
  }

  variant variant_type::invoke(string func, riku::array& args)
  {
    auto f = function(func);
    if (f.data() != NULL)
    {
      if(f->obj_type != NULL)
        return f->invoke(*this, args);
      else
      {
        args.insert(*this, 0);
        return f->invoke(riku::ptr(), args);
      }
    }

    return variant();
  }

  variant variant_type::invoke(string func, riku::array& args) const
  {
    auto f = function(func);
    if (f.data() != NULL)
      if (f->obj_type != NULL)
        return f->invoke(*this, args);
      else
      {
        args.insert(*this, 0);
        return f->invoke(riku::ptr(), args);
      }

    return variant();
  }

  bool variant_type::is_array() const
  {
    return meta()->has_parent(riku::get<array_var>());
  }

  array_var* variant_type::as_array()
  {
    return NULL;
  }

  array_var const* variant_type::as_array() const
  {
    return NULL;
  }

  typeinfo variant_type::meta() const
  {
    return NULL;
  }
}

//ptr
namespace riku
{
  ptr::ptr(typeinfo t, void const* p) : c(true), contains(t)
  {
    d.cptr = p;
  }

  ptr::ptr(typeinfo t, void* p) : c(false), contains(t)
  {
    d.ptr = p;
  }

  typeinfo ptr::type() const
  {
    return contains;
  }

  void* ptr::data()
  {
    return c ? NULL : d.ptr;
  }

  void const* ptr::data() const
  {
    return d.cptr;
  }

  typeinfo ptr::meta() const
  {
    return riku::get<ptr>();
  }
};

//val
namespace riku
{
  val::val(typeinfo t, void const* p) : contains(t)
  {
    clear();

    if (type() != NULL)
    {
      if (p != NULL) //create a copy of *p
      {
        if (type()->size() > RIKU_MAX_VAR_SIZE)
        {
          d.p.v = type()->mem_funcs.copy(const_cast<void*>(p));
          d.p.refs = new unsigned(1);
        }
        else
        {
          type()->mem_funcs.copy(const_cast<void*>(p), d.raw);
        }
      }
      else //create a default object
      {
        if (type()->size() > RIKU_MAX_VAR_SIZE)
        {
          d.p.v = type()->mem_funcs.create();
          d.p.refs = new unsigned(1);
        }
        else
        {
          type()->mem_funcs.create(d.raw);
        }
      }
    }
  }
  
  val& val::operator=(val const& rhs)
  {
    if (&rhs != this)
    {
      free();
      contains = rhs.type();

      //too big to hold in place, have to reference copy
      if (type() != NULL && type()->size() > RIKU_MAX_VAR_SIZE)
      {
        d.p.v = rhs.d.p.v;
        d.p.refs = rhs.d.p.refs;
        ++*d.p.refs;
      }
      //the other variant is holding it in place, we'll just copy it for ourselves
      else if (type() != NULL)
      {
        type()->mem_funcs.copy(const_cast<byte*>(rhs.d.raw), d.raw);
      }
    }
    return *this;
  }

  val::~val()
  {
    free();
  }

  typeinfo val::type() const
  {
    return contains; 
  }
  
  void* val::data()
  {
    if (type() != NULL)
    {
      fork();
      return type()->size() > RIKU_MAX_VAR_SIZE ? d.p.v : d.raw;
    }
    return NULL;
  }

  void const* val::data() const
  {
    if (type() != NULL)
      return type()->size() > RIKU_MAX_VAR_SIZE ? d.p.v : d.raw;
    return NULL;
  }

  void val::fork()
  {
    if (type() != NULL && type()->size() > RIKU_MAX_VAR_SIZE && *d.p.refs > 1)
    {
      void* tmp = d.p.v;
      free();

      d.p.v = type()->mem_funcs.copy(tmp);
      d.p.refs = new unsigned(1);
    }
  }

  void val::free()
  {
    if (type() != NULL)
    {
      if (type()->size() > RIKU_MAX_VAR_SIZE)
      {
        --*d.p.refs;
        if (*d.p.refs == 0)
          type()->mem_funcs.del(d.p.v);
      }
      else
        type()->mem_funcs.destroy(d.raw);

      clear();
    }
  }

  void val::clear()
  {
    for (auto& b : d.raw)
      b = 0;
  }

  typeinfo val::meta() const
  {
    return riku::get<val>();
  }
}

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
};
