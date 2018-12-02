#pragma once

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
    };

    template<typename Metatype> class registrar<Metatype&> : public registrar<Metatype> {};
    template<typename Metatype> class registrar<Metatype&&> : public registrar<Metatype> {};
    template<typename Metatype> class registrar<Metatype const&> : public registrar<Metatype> {};

    //the ideal solution to the void* problem, that unfortunately doesn't work due to reasons unknown
    /*template<> class registrar<void*> {
    public:
      static generic_type* get() { static riku::generic_type s_type; return &s_type; }
      registrar() { get()->n = "void*"; get()->s = sizeof(void*); riku::library::modify()->types["void*"] = get(); }
    };

    template<> class registrar<void const*> {
    public:
      static generic_type* get() { static riku::generic_type s_type; return &s_type; }
      registrar() { get()->n = "void const*"; get()->s = sizeof(void*); riku::library::modify()->types["void const*"] = get(); }
    };*/

    template<typename Metatype> class registrar<Metatype*>
    {
    public:
      static typeinfo get() { return registrar<Metatype>::get()->pointer(); }
    };

    template<typename Metatype> class registrar<Metatype const*> : public registrar<Metatype*> {};

    //template<> class registrar<void> { public: static generic_type* get(); registrar(); };
  }
}
