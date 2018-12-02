#include "RIKU.h"

namespace riku
{
  typeinfo pointer_type::pointer() const
  {
    if (dbl_ptr == NULL)
    {
      *const_cast<pointer_type**>(&dbl_ptr) = new pointer_type;
      dbl_ptr->points_to = this;

      dbl_ptr->mem_funcs.allocate = riku::default_mem::heap_default_new<void*>::run;
      dbl_ptr->mem_funcs.free = riku::default_mem::heap_delete<void*>::run;
      dbl_ptr->mem_funcs.construct = riku::default_mem::placement_default_new<void*>::run;
      dbl_ptr->mem_funcs.destruct = riku::default_mem::placement_delete<void*>::run;
      dbl_ptr->mem_funcs.clone = riku::default_mem::heap_copy<void*>::run; 
      dbl_ptr->mem_funcs.copy_construct = riku::default_mem::placement_copy<void*>::run;
      dbl_ptr->mem_funcs.assign = riku::default_mem::assign<void*>::run;
    }

    return dbl_ptr;
  }

  typelist pointer_type::parents(bool) const
  {
    if (points_to == NULL || points_to->parents().size() == 0)
      return typelist();

    typelist ret;
    for (auto const& iter : points_to->parents())
      ret.push_back(iter->pointer());
    return ret;
  }

  typeinfo pointer_type::meta() const { return riku::get<pointer_type>(); }
}
