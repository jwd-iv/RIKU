//variant_type
namespace riku
{
  inline bool variant_type::operator==(variant_type const& rhs) const { return type() == rhs.type() && data() == rhs.data(); }
  inline bool variant_type::operator!=(variant_type const& rhs) const { return !operator==(rhs); }
  inline variant variant_type::operator[](string prop)       { return property(prop); }
  inline variant variant_type::operator[](string prop) const { return property(prop); }
  inline variant variant_type::operator()(string func, riku::array& args)       { return invoke(func, args); }
  inline variant variant_type::operator()(string func, riku::array& args) const { return invoke(func, args); }

  template<typename T> inline bool variant_type::is() const
  {
    return type() == NULL ? false : type()->has_parent(riku::get<T>());
  }

  template<typename T> inline T& variant_type::as()
  {
    return *reinterpret_cast<T*>(data());
  }

  template<typename T> inline T const& variant_type::as() const
  {
    return *reinterpret_cast<T const*>(data());
  }

  template<typename T> inline T* variant_type::to()
  {
    return reinterpret_cast<T*>(data());
  }

  template<typename T> inline T const* variant_type::to() const
  {
    return reinterpret_cast<T const*>(data());
  }

  template<typename T> inline T variant_type::convert() const
  {
    return T();
  }

  template<typename T> inline variant_type& variant_type::operator<<(T const& rhs)
  {
    assign(ptr(rhs));
    return *this;
  }

  template<typename T> inline void variant_type::operator>>(T& rhs) const
  {
    modify(ptr(rhs));
  }

  template<typename... T> inline variant variant_type::operator()(char const* f, T&&... args)
  {
    return (*this)(riku::string(f), riku::array::create(std::forward<T&&>(args)...));
  }

  template<typename... T> inline variant variant_type::operator()(char const* f, T&&... args) const
  {
    return (*this)(riku::string(f), riku::array::create(std::forward<T&&>(args)...));
  }
}

// variant
namespace riku
{
  template<typename T> inline variant::variant(T const& rhs)
  { if (get<T>()->has_parent(get<variant_type>())) operator=(rhs); else operator=(val(rhs)); }
  template<typename T> inline variant::variant(T& rhs)
  { if (get<T>()->has_parent(get<variant_type>())) operator=(rhs); else operator=(val(rhs)); }
  template<typename T> inline variant::variant(T&& rhs)
  { if (get<T>()->has_parent(get<variant_type>())) operator=(rhs); else operator=(val(rhs)); }
  template<typename T> inline variant::variant(T const* rhs) { operator=(ptr(rhs)); }
  template<typename T> inline variant::variant(T* rhs) { operator=(ptr(rhs)); }
}
