#include "RIKU.h"

namespace riku
{
  bool type::has_parent(typeinfo t) const
  {
    if (t == this)
      return true;
    for (auto const& iter : parents())
      if (iter == t)
        return true;
    return false;
  }

  typelist type::children(bool full_list) const
  {
    riku::typelist ret;
    
    for (auto const& child : child_types)
    {
      if (full_list && child->child_types.size())
        for (auto const& ngchild : child->children(full_list))
          ret.push_back(ngchild);
      else
        ret.push_back(child);
    }

    return ret;
  }

  typeinfo type::meta() const { return riku::get<type>(); }
}
