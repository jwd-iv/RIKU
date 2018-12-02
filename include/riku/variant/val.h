#pragma once

namespace riku
{	
  //! Holds a copy of an object by value, using a lazy copy-on-write policy.
  struct val : public variant_type
  {
    val(typeinfo t = typeinfo() , void const* p = NULL);
    val& operator=(val const& rhs);
    ~val();
    
    template<typename T> val(T const& rhs);
    template<typename T> val(T& rhs);
    template<typename T> val(T&& rhs);

    template<> val(val const& rhs);
    template<> val(val& rhs);
    template<> val(val&& rhs);

    virtual typeinfo type() const;
    virtual void* data();
    virtual void const* data() const;

    virtual typeinfo meta() const;

  protected:
    union {
      struct {
        void* v;
        unsigned* refs;
      } p;
      byte raw[RIKU_MAX_VAR_SIZE];
    } d; //!< This holds one of two things: a reference-counted pointer to an allocated object, or if the object's small enough it's constructed right in the raw array.
    typeinfo contains = NULL;

    void fork();
    void free();
    void clear();
  };
}
