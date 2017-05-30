namespace riku
{
  template<typename... Targs>
  inline array array::create(Targs&&... args)
  {
    const unsigned size = sizeof...(args)+1;
    variant vals[size] = { variant(std::forward<Targs&&>(args))... };
    array ret;

    for (auto& iter : vals)
      ret.vec.push_back(iter);
    ret.vec.pop_back();

    return ret;
  }
}