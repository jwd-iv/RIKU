namespace riku
{
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
}
