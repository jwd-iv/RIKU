#pragma once

namespace riku
{
  //! Acts as a pointer to an object, doesn't own the memory.
  struct ptr : public variant_type
  {
    ptr(typeinfo t = typeinfo(), void const* p = NULL);
    ptr(typeinfo t, void* p);

    template<typename T> ptr(T* pt);
    template<typename T> ptr(T const* cpt);
    template<typename T> ptr(T& rt);
    template<typename T> ptr(T const& crt);
    template<> ptr(ptr& rt);
    template<> ptr(ptr const& crt);

    virtual typeinfo type() const;
    virtual void*    data();
    virtual void const* data() const;

    virtual typeinfo meta() const;

  protected:
    union {
      const void* cptr;
      void* ptr = NULL;
    } d; //!< A trick to save memory but hold both const and non-const pointers.
    bool c = true;
    typeinfo contains = NULL;
  };
}
