#include "RIKU.h"

// map_var implementation
namespace riku
{
  typeinfo map_var::type() const
  {
    return meta();
  }

  void* map_var::data()
  {
    return this;
  }

  void const* map_var::data() const
  {
    return this;
  }

  bool map_var::assign(variant_type const & rhs)
  {
    bool anything = false;

    for (auto const& name : rhs.properties())
      anything |= property(name).assign(rhs.property(name));

    return anything;
  }

  bool map_var::modify(variant_type & rhs) const
  {
    bool anything = false;

    for (auto const& name : properties())
      anything |= property(name).modify(rhs.property(name));

    return anything;
  }
  
  var<riku::function> map_var::function(string n)
  {
    return var<riku::function>();
  }
  
  var<riku::function> map_var::function(string n) const
  {
    return var<riku::function>();
  }
  
  variant map_var::invoke(string func, riku::array & args)
  {
    map ret;

    for (auto const& prop : properties())
      ret.contents[prop] = property(prop).invoke(func, args);

    return variant(ret);
  }
  
  variant map_var::invoke(string func, riku::array & args) const
  {
    map ret;

    for (auto const& prop : properties())
      ret.contents[prop] = property(prop).invoke(func, args);

    return variant(ret);
  }

  typeinfo map_var::meta() const
  {
    return get<map_var>();
  }
  
  variant map::property(string n)
  {
    auto find = contents.find(n);

    if (find != contents.end())
      return find->second;

    return contents[n];
  }
  
  variant map::property(string n) const
  {
    auto find = contents.find(n);

    if (find != contents.end())
      return ptr(find->second.type(), find->second.data());

    return variant();
  }
  
  std::vector<string> map::properties() const
  {
    std::vector<string> ret;

    for (auto const& pair : contents)
      ret.push_back(pair.first);

    return ret;
  }

  typeinfo map::meta() const
  {
    return get<map>();
  }
}
