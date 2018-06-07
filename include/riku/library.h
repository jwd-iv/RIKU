namespace riku
{
  struct library
  {
    static RIKU_API library const* get();

    typeinfo find(char const* name) const;

    std::unordered_map<std::string, typeinfo> types;

  private:
    static RIKU_API library* modify();
    template<typename T> friend class internal::registrar;
  };

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

  template<> inline typeinfo get<void*>()
  {
    return library::get()->find("void*");
  }

  template<> inline typeinfo get<void const*>()
  {
    return library::get()->find("void const*");
  }
}

rkTemplatedType(riku::var, rkParent(riku::variant) rkDefaultFactory)

rkTemplatedType(std::vector, rkDefaultFactory)
rkTemplatedType(std::allocator, rkDefaultFactory)
//rkTemplatedType(std::map,           rkDefaultFactory) //TODO: FIGURE THIS OUT
//rkTemplatedType(std::unordered_map, rkDefaultFactory) //TODO: FIGURE THIS OUT
