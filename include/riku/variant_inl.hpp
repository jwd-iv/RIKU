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
    assignto(ptr(rhs));
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

//ptr, val, variant
namespace riku
{
  template<typename T> ptr::ptr(T& rt) : ptr(riku::get<T>(), &rt) {}
  template<typename T> ptr::ptr(T* pt) : ptr(riku::get<T>(), pt) {}
  template<typename T> ptr::ptr(T const& crt) : ptr(riku::get<T>(), &crt) {}
  template<typename T> ptr::ptr(T const* cpt) : ptr(riku::get<T>(), cpt) {}
  template<> ptr::ptr(ptr& rt) : c(rt.c), contains(rt.type()) { d.cptr = rt.d.cptr; }
  template<> ptr::ptr(ptr const& crt) : c(crt.c), contains(crt.type()) { d.cptr = crt.d.cptr; }

  template<typename T> val::val(T const& rhs) : val(riku::get<T>(), &rhs) {}
  template<typename T> val::val(T& rhs) : val(riku::get<T>(), &rhs) {}
  template<typename T> val::val(T&& rhs) : val(riku::get<T>(), &rhs) {} //TODO: add factory move
  
  template<> val::val(val const& rhs) { clear(); operator=(rhs); }
  template<> val::val(val& rhs) { clear(); operator=(rhs); }

  template<> val::val(val&& rhs)
  {
    contains = rhs.contains;
    clear();

    if (type() != NULL && type()->size() > RIKU_MAX_VAR_SIZE)
    {
      d.p.v = rhs.d.p.v;
      d.p.refs = rhs.d.p.refs;
    }
    else if (type() != NULL)
    {
      type()->mem_funcs.copy(rhs.d.raw, d.raw);
    }

    rhs.clear();
    rhs.contains = NULL;
  }

  template<typename T> inline variant::variant(T const& rhs)
  { if (get<T>()->has_parent(get<variant_type>())) operator=(rhs); else operator=(val(rhs)); }
  template<typename T> inline variant::variant(T& rhs)
  { if (get<T>()->has_parent(get<variant_type>())) operator=(rhs); else operator=(val(rhs)); }
  template<typename T> inline variant::variant(T&& rhs)
  { if (get<T>()->has_parent(get<variant_type>())) operator=(rhs); else operator=(val(rhs)); }
  template<typename T> inline variant::variant(T const* rhs) { operator=(ptr(rhs)); }
  template<typename T> inline variant::variant(T* rhs) { operator=(ptr(rhs)); }
}

//var<T>
namespace riku
{
#ifdef _DEBUG
#define _RKDBP _pDebug = reinterpret_cast<T const*>(data());
#else
#define _RKDBP
#endif

  template<typename T> var<T>::var() : variant() { _RKDBP }
  template<typename T> var<T>::var(var<T> const& rhs) : variant((variant&)rhs) { _RKDBP };
  template<typename T> var<T>::~var() { variant::~variant(); }

  template<typename T>
  var<T>::var(variant_type const& rhs) : variant() { _RKDBP
    if (rhs.type() != NULL && rhs.type()->has_parent(riku::get<T>())) variant::operator=(rhs); _RKDBP
  }
  template<typename T>
  var<T>::var(variant_type& rhs) : variant() { _RKDBP
    if (rhs.type() != NULL && rhs.type()->has_parent(riku::get<T>())) variant::operator=(rhs); _RKDBP
  }
  template<typename T>
  var<T>::var(variant const& rhs) : variant() { _RKDBP
    if (rhs.type() != NULL && rhs.type()->has_parent(riku::get<T>())) variant::operator=(rhs); _RKDBP
  }
  template<typename T>
  var<T>::var(variant& rhs) : variant() { _RKDBP
    if (rhs.type() != NULL && rhs.type()->has_parent(riku::get<T>())) variant::operator=(rhs); _RKDBP
  }

  template<typename T>
  var<T>& var<T>::operator=(variant_type const& rhs) { _RKDBP
    if (rhs.type() != NULL && rhs.type()->has_parent(riku::get<T>())) variant::operator=(rhs); _RKDBP
    return *this;
  }
  template<typename T>
  var<T>& var<T>::operator=(variant const& rhs) { _RKDBP
    if (rhs.type() != NULL && rhs.type()->has_parent(riku::get<T>())) variant::operator=(rhs);
    return *this;
  }

  template<typename T> var<T>::operator T*() { _RKDBP
    return reinterpret_cast<T*>(data());
  }
  template<typename T> var<T>::operator T const*() const {
    return reinterpret_cast<T const*>(data());
  }
  template<typename T> T& var<T>::operator*() { _RKDBP return *(operator T*()); }
  template<typename T> T* var<T>::operator->() { _RKDBP return operator T*(); }
  template<typename T> T const& var<T>::operator*() const { return *(operator T const*()); }
  template<typename T> T const* var<T>::operator->() const { return operator T const*(); }
}
