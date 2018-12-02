#include "RIKU.h"

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
