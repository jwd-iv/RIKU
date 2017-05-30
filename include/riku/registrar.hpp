namespace riku
{
  namespace internal
  {
    template<typename Metatype> class registrar;

    template<typename Metatype>
    class registrar
    {
    public:
      static generic_type* get();

      registrar();

    protected:
      typedef Metatype my_type;

      static void addmember(typeinfo mt, string name, unsigned off)
      {
        member* newmem = new member;
        newmem->p = get();
        newmem->t = mt;
        newmem->offset = off;
        get()->props[name] = newmem;
      }

      static void addproperty(string name, riku::function* g, riku::function* s)
      {
        if (g == NULL || s == NULL) return;
        get_set* newmem = new get_set;
        newmem->p = get();
        newmem->g = g;
        newmem->s = s;
        get()->props[name] = newmem;
      }
    };

    template<typename Metatype> class registrar<Metatype&> : public registrar<Metatype> {};
    template<typename Metatype> class registrar<Metatype&&> : public registrar<Metatype> {};
    template<typename Metatype> class registrar<Metatype const&> : public registrar<Metatype> {};

    template<typename Metatype> class registrar<Metatype*>
    {
    public:
      static typeinfo get() { return registrar<Metatype>::get()->pointer(); }
    };

    template<typename Metatype> class registrar<Metatype const*> : public registrar<Metatype*> {};

    /*template<template<typename...> typename T, typename... Targs>
    class registrar< T<Targs...> >
    {
    public:
      static type* get();
      static template_type* getbase();
    };*/
  }

  template<typename T> typeinfo inspect(T const& obj)
  {
    return internal::registrar<decltype(obj)>::get();
  }

  template<typename T> inline typeinfo get()
  {
    return internal::registrar<T>::get();
  }

  template<> inline typeinfo get<nulltype>()
  {
    return NULL;
  }

  template<> inline typeinfo get<void>()
  {
    return NULL;
  }
}
