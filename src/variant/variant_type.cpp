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

  variant_type const & variant_type::internal() const
  {
    return *this;
  }
}
