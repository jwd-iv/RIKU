namespace riku
{
  template<typename T> ptr::ptr(T& rt) : ptr(riku::get<T>(), &rt) {}
  template<typename T> ptr::ptr(T* pt) : ptr(riku::get<T>(), pt) {}
  template<typename T> ptr::ptr(T const& crt) : ptr(riku::get<T>(), &crt) {}
  template<typename T> ptr::ptr(T const* cpt) : ptr(riku::get<T>(), cpt) {}
  template<> ptr::ptr(ptr& rt) : c(rt.c), contains(rt.type()) { d.cptr = rt.d.cptr; }
  template<> ptr::ptr(ptr const& crt) : c(crt.c), contains(crt.type()) { d.cptr = crt.d.cptr; }
}
