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
