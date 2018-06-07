#include "RIKU.h"

namespace riku
{
  function::function()
  {
  }

  function::function(const function& rhs) : function()
  {
    operator=(rhs);
  }
  
  function::function(function && rhs) : is_const(rhs.is_const), is_lambda(rhs.is_lambda),
    arg_list(rhs.arg_list), arg_count(rhs.arg_count), obj_type(rhs.obj_type), ret_type(rhs.ret_type),
    ini(rhs.ini), chk(rhs.chk), cal(rhs.cal), clr(rhs.clr), ptr(rhs.ptr)
  {
    rhs.arg_list = NULL;
    rhs.ptr = NULL;
  }
  
  function& function::operator=(function const& rhs)
  {
    if (&rhs != this)
    {
      if (arg_list != NULL)
        delete[] arg_list;
      if (ptr != NULL && clr != NULL)
        clr(ptr);

      is_const = rhs.is_const;
      is_lambda = rhs.is_lambda;
      obj_type = rhs.obj_type;
      ret_type = rhs.ret_type;
      ini = rhs.ini;
      chk = rhs.chk;
      cal = rhs.cal;
      clr = rhs.clr;

      arg_count = rhs.arg_count;
      arg_list = new function::arg[arg_count];
      for (unsigned i = 0; i < arg_count; ++i)
        arg_list[i] = rhs.arg_list[i];

      ptr = rhs.ptr->clone();
    }
    return *this;
  }
  
  function::~function()
  {
    if (arg_list != NULL)
      delete[] arg_list;
    arg_list = NULL;

    if (ptr != NULL && clr != NULL)
      clr(ptr);
    clr = NULL;
    ptr = NULL;
  }
  
  variant function::invoke(variant_type& obj, array_var& args) const
  {
    if (args.length() < arg_count  // wrong number of arguments
      || cal == NULL || chk == NULL // this isn't a valid func pointer
      || (obj_type != NULL && (obj.type() == NULL || !obj.type()->has_parent(obj_type))))
      return variant();             // or this function needs an object and was passed none
    
    return internal_invoke(obj.data(), args);
  }

  variant function::invoke(variant_type const& obj, array_var& args) const
  {
    if (args.length() < arg_count
      || cal == NULL || chk == NULL
      || (obj_type != NULL && (obj.type() == NULL || !obj.type()->has_parent(obj_type))))
      return variant();

    if (obj_type != NULL && !is_const) //additional fail if this is non-const)
      return variant();

    return internal_invoke(const_cast<void*>(obj.data()), args);
  }

  variant function::invoke(array_var& args) const
  {
    if (obj_type != NULL)
    {
      //pull first arg off and pass as obj
      array lastArgs;

      for (int i = 1; i < args.length(); ++i)
        lastArgs.push_back(args[i]);

      //TODO: make invoke(args[0], array(++args.begin())) work

      return invoke(args[0], lastArgs);
    }
    else
    {
      //call with no obj
      return invoke(variant(), args);
    }
  }

  variant function::internal_invoke(void* obj, array_var& args) const
  {
    unsigned size = args.length() + 1;

    typeinfo* types = new typeinfo[size];
    void** args_ptr = new void*[size];

    for (unsigned i = 0; i < size - 1; ++i)
    { //treat the argument as const so that we get its pointer either way,
      // then remove the const qualification, the unwrapper will fix it back
      args_ptr[i] = const_cast<void*>(((variant const&)args[i]).data());
      types[i] = args[i].type();
    }

    variant ret;
    if (!chk(types))
    { // try to fix the arguments in place
      void** fix_buffer = new void*[size];

      for (unsigned i = 0; i < size - 1; ++i)
      {
        if (arg_list[i].type->meta()->has_parent(riku::get<pointer_type>())
          && types[i]->pointer()->has_parent(arg_list[i].type))
        { // if this takes a pointer, put the address of the argument being passed in a temporary
          // container, and put the address of that temporary slot in the argument list.
          // I think it could be written to do this recursively for any level of pointer,
          // but that would be kind of ridiculous so I won't. It stays at one level.
          __debugbreak();
          types[i] = types[i]->pointer();
          fix_buffer[i] = args_ptr[i];
          args_ptr[i] = fix_buffer + i;
        }
        else if (types[i] != arg_list[i].type && types[i]->has_parent(arg_list[i].type))
        { // just need to adjust the type pointer because the arg is of the correct type
          types[i] = arg_list[i].type;
        }
        else if (arg_list[i].type == get<variant>())
        {
          args_ptr[i] = &args[i];
          types[i] = get<variant>();
        }
      }

      if (chk(types))
        ret = cal(ptr, obj, this, args_ptr);

      delete[] fix_buffer;
    }
    else
      ret = cal(ptr, obj, this, args_ptr);

    delete[] args_ptr;
    delete[] types;
    return ret;
  }
}
