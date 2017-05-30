#include "RIKU.h"

namespace riku
{
  typeinfo property::parent() const
  {
    return p;
  }

  typeinfo member::type() const
  {
    return t;
  }

  variant member::get(variant_type& obj) const
  {
    if (obj.type()->has_parent(p))
      return ptr(t, ((byte*)obj.data()) + offset);

    return variant();
  }

  variant member::get(variant_type const& obj) const
  {
    if (obj.type()->has_parent(p))
      return ptr(t, ((byte const*)obj.data()) + offset);

    return variant();
  }

  bool member::set(variant_type& obj, variant_type const& data) const
  {
    if (obj.type() == NULL || !obj.type()->has_parent(p) || data.data() == NULL)
      return false;

    variant var = get(obj);
    if (var.data() == NULL)
      return false;

    return var.assign(data);
  }
}

namespace riku
{
  typeinfo get_set::type() const
  {
    typeinfo getT = g->ret_type;
    if (getT->meta()->has_parent(riku::get<pointer_type>()))
      getT = dynamic_cast<pointer_type const*>(getT)->points_to;
    return getT;
  }
  
  variant get_set::get(variant_type& obj) const
  {
    variant ret;

    if (g->is_lambda && g->arg_count == 1)
    { // if it's a lambda we have to pass in the obj as a parameter
      if (g->arg_list[0].type->has_parent(riku::get<variant_type>()))
      { // if the get function just takes a variant we'll forward our "obj" to it
        ret = g->invoke(variant(), array(ptr(riku::get<variant_type>(), &obj)));
      }
      else if (obj.type()->pointer()->has_parent(g->arg_list[0].type))
      { // if it takes a pointer to the obj type we pass a pointer to obj's data
        __debugbreak();
        void* d = obj.data();
        ret = g->invoke(variant(), array(val(obj.type()->pointer(), &d)));
      }
      else
      { // try calling it with what we've got and hope it works
        ret = g->invoke(variant(), array(ptr(obj.type(), obj.data())));
      }
    }
    else if (g->arg_count == 0)
    { // it's a regular member function, just call it on the obj see what happens
      ret = (*g)(obj);
    }

    return ret;
  }

  variant get_set::get(variant_type const& obj) const
  {
    variant ret;

    if (g->is_lambda && g->arg_count == 1)
    {
      if (g->arg_list[0].type->has_parent(riku::get<variant_type>()))
      {
        __debugbreak();
        ret = g->invoke(variant(), array(ptr(riku::get<variant_type>(), &obj)));
      }
      else if (obj.type()->pointer()->has_parent(g->arg_list[0].type))
      {
        __debugbreak();
        void const* d = obj.data();
        ret = g->invoke(variant(), array(val(obj.type()->pointer(), &d)));
      }
      else
        ret = g->invoke(variant(), array(ptr(obj.type(), obj.data())));
    }
    else if (g->arg_count == 0)
      ret = (*g)(obj);

    return ret;
  }

  bool get_set::set(variant_type& obj, variant_type const& data) const
  {
    if (obj.data() == NULL || data.data() == NULL)
      return false;

    array params;

    if (s->is_lambda && s->arg_count == 2)
    {
      if (s->arg_list[0].type->has_parent(riku::get<variant_type>()))
      {
        __debugbreak();
        params.push_back(ptr(riku::get<variant_type>(), &obj));
      }
      else if (obj.type()->pointer()->has_parent(s->arg_list[0].type))
      {
        void const* d = obj.data();
        params.push_back(val(obj.type()->pointer(), &d));
      }
      else
      {
        params.push_back(obj);
      }
    }

    unsigned darg = s->is_lambda ? 1 : 0;
    if (s->arg_list[darg].type->has_parent(riku::get<variant_type>()))
    {
      __debugbreak();
      params.push_back(ptr(riku::get<variant_type>(), const_cast<variant_type*>(&data)));
    }
    else if (data.type()->pointer()->has_parent(s->arg_list[darg].type))
    {
      __debugbreak();
      void const* d = data.data();
      params.push_back(val(data.type()->pointer(), &d));
    }
    else
      params.push_back(data);

    if((s->is_lambda && params.length() != 2) || (!s->is_lambda && params.length() != 1))
      return false;

    s->invoke(obj, params);
    return true;
  }
}
