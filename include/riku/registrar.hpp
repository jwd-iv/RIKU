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

    template<typename Metatype> class registrar<Metatype*>
    {
    public:
      static typeinfo get() { return registrar<Metatype>::get()->pointer(); }
    };

    template<typename Metatype> class registrar<Metatype const*> : public registrar<Metatype*> {};
  }
}
