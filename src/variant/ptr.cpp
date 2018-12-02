#include "RIKU.h"

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
