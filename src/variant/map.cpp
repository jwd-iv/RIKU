#include "RIKU.h"

// map_var implementation
namespace riku
{
  bool map_var::assign(variant_type const & rhs)
  {
    return false;
  }

  bool map_var::assignto(variant_type & rhs) const
  {
    return false;
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
    return variant();
  }
  
  variant map_var::invoke(string func, riku::array & args) const
  {
    return variant();
  }

  typeinfo map_var::meta() const
  {
    return get<map_var>();
  }
  
  variant map::property(string n)
  {
    return variant();
  }
  
  variant map::property(string n) const
  {
    return variant();
  }
  
  std::vector<string> map::properties() const
  {
    return std::vector<string>();
  }

  typeinfo map::meta() const
  {
    return get<map>();
  }
}
