#include "RIKU.h"

namespace riku
{
  bool type::has_parent(typeinfo t) const
  {
    if (t == this)
      return true;
    for (auto const& iter : parents())
      if (iter == t)
        return true;
    return false;
  }

  typelist type::children(bool full_list) const
  {
    riku::typelist ret;
    
    for (auto const& child : child_types)
    {
      if (full_list && child->child_types.size())
        for (auto const& ngchild : child->children(full_list))
          ret.push_back(ngchild);
      else
        ret.push_back(child);
    }

    return ret;
  }

  typeinfo type::meta() const { return riku::get<type>(); }

  string generic_type::name() const
  {
    return n;
  }
  
  unsigned generic_type::size() const
  {
    return s;
  }
  
  std::vector<string> generic_type::properties(bool inherited) const
  {
    std::vector<string> names;

    for (auto const& iter : props)
      names.push_back(iter.first);

    if(inherited)
      for (auto const& iter : parents(false))
      {
        auto ret = iter->properties();
        names.insert(names.end(), ret.begin(), ret.end());
      }

    return names;
  }
  
  var<riku::property> generic_type::property(char const* name) const
  {
    auto iter = props.find(name);

    if (iter == props.end())
    {
      for (auto const& iter : parents(false))
      {
        auto ret = iter->property(name);
        if (ret.data() != NULL)
          return ret;
      }

      return var<riku::property>();
    }
    else
      return ptr(iter->second);
  }
  
  std::vector<string> generic_type::functions(bool c, bool inherited) const
  {
    std::vector<string> names;

    for (auto const& iter : funcs) //TODO: intervening if for const-only functions
      names.push_back(iter.first);

    if(inherited)
      for (auto const& iter : parents(false))
      {
        auto ret = iter->functions();
        names.insert(names.end(), ret.begin(), ret.end());
      }

    return names;
  }
  
  var<riku::function> generic_type::function(char const* name, bool c) const
  {
    auto iter = funcs.find(name);
    if (iter == funcs.end())
    {
      for (auto const& iter : parents(false))
      {
        auto f = iter->function(name, c);
        if (f.data() != NULL)
          return f;
      }
      return var<riku::function>();
    }

    while ((iter->second == NULL || (!iter->second->is_const && c)) && iter != funcs.end())
      ++iter;

    if (iter == funcs.end() || iter->first != name)
    {
      for (auto const& iter : parents(false))
      {
        auto f = iter->function(name, c);
        if (f.data() != NULL)
          return f;
      }
      return var<riku::function>();
    }

    return iter->second;
  }
  
  typeinfo generic_type::pointer() const
  {
    if (ptr_type.points_to == NULL)
    {
      pointer_type* ptr = const_cast<pointer_type*>(&ptr_type);
      ptr->points_to = this;

      ptr->mem_funcs.allocate = riku::default_mem::heap_default_new<void*>::run;
      ptr->mem_funcs.free = riku::default_mem::heap_delete<void*>::run;
      ptr->mem_funcs.construct = riku::default_mem::placement_default_new<void*>::run;
      ptr->mem_funcs.destruct = riku::default_mem::placement_delete<void*>::run;
      ptr->mem_funcs.clone = riku::default_mem::heap_copy<void*>::run;
      ptr->mem_funcs.copy_construct = riku::default_mem::placement_copy<void*>::run;
      ptr->mem_funcs.assign = riku::default_mem::assign<void*>::run;
    }

    return &ptr_type;
  }

  typelist generic_type::parents(bool full_list) const
  {
    typelist tmp = ps, result;

    while (!tmp.empty())
    {
      result.push_back(tmp.front());
      if(full_list)
        for (auto const& iter : tmp.front()->parents())
          tmp.push_back(iter);
      tmp.erase(tmp.begin());
    }

    return result;
  }

  typeinfo generic_type::meta() const { return riku::get<generic_type>(); }

  typeinfo pointer_type::pointer() const
  {
    if (dbl_ptr == NULL)
    {
      *const_cast<pointer_type**>(&dbl_ptr) = new pointer_type;
      dbl_ptr->points_to = this;

      dbl_ptr->mem_funcs.allocate = riku::default_mem::heap_default_new<void*>::run;
      dbl_ptr->mem_funcs.free = riku::default_mem::heap_delete<void*>::run;
      dbl_ptr->mem_funcs.construct = riku::default_mem::placement_default_new<void*>::run;
      dbl_ptr->mem_funcs.destruct = riku::default_mem::placement_delete<void*>::run;
      dbl_ptr->mem_funcs.clone = riku::default_mem::heap_copy<void*>::run; 
      dbl_ptr->mem_funcs.copy_construct = riku::default_mem::placement_copy<void*>::run;
      dbl_ptr->mem_funcs.assign = riku::default_mem::assign<void*>::run;
    }

    return dbl_ptr;
  }

  typelist pointer_type::parents(bool) const
  {
    if (points_to == NULL || points_to->parents().size() == 0)
      return typelist();

    typelist ret;
    for (auto const& iter : points_to->parents())
      ret.push_back(iter->pointer());
    return ret;
  }

  typeinfo pointer_type::meta() const { return riku::get<pointer_type>(); }
  typeinfo primitive_type::meta() const { return riku::get<primitive_type>(); }

  string template_type::name() const
  {
    return n;
  }

  unsigned template_type::size() const
  {
    return 0;
  }

  std::vector<string> template_type::properties(bool inherited) const
  {
    return std::vector<string>();
  }

  var<riku::property> template_type::property(char const* name) const
  {
    return var<riku::property>();
  }

  std::vector<string> template_type::functions(bool c, bool inherited) const
  {
    return std::vector<string>();
  }

  var<riku::function> template_type::function(char const* name, bool c) const
  {
    return var<riku::function>();
  }

  typeinfo template_type::pointer() const
  {
    return typeinfo();
  }

  typelist template_type::parents(bool full_list) const
  {
    return typelist();
  }

  typeinfo template_type::meta() const
  {
    return riku::get<template_type>();
  }

  string template_instance::name() const
  {
    return n;
  }
  typeinfo template_instance::meta() const
  {
    return typeinfo();
  }
}
