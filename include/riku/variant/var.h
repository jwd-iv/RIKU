#pragma once

namespace riku
{
  //! variant that only holds the specified type or something that inherits it
  template<typename T>
  struct var : public variant
  {
    var();
    var(var<T> const& rhs);
    var(variant_type const& rhs);
    var(variant_type& rhs);
    //var(variant_type&& rhs);
    var(variant const& rhs);
    var(variant& rhs);
    //var(variant&& rhs);
    ~var();

    var& operator=(variant const& rhs);
    var& operator=(variant_type const& rhs);

    operator T*();
    operator T const*() const;
    T& operator*();
    T const& operator*() const;
    T* operator->();
    T const* operator->() const;

#ifdef _DEBUG
  private:
	T const* _pDebug = NULL;
#endif
  };
}
