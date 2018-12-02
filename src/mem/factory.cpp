#include "RIKU.h"

namespace riku
{
  void* factory::create(void* mem) const
  {
    if (mem == NULL && allocate != NULL)
      return allocate(NULL, NULL);
    else if (mem != NULL && construct != NULL)
      return construct(mem, NULL);
    else
      return NULL;
  }

  void* factory::copy(void* original, void* mem) const
  {
    if (original != NULL)
    {
      if (mem == NULL && clone != NULL)
        return clone(original, NULL);
      else if (mem != NULL && copy_construct != NULL)
        return copy_construct(mem, original);
    }

    return NULL;
  }

  void factory::destroy(void* mem) const
  {
    if (mem != NULL && destruct != NULL)
      destruct(mem, NULL);
  }

  void factory::del(void* mem) const
  {
    if (mem != NULL && free != NULL)
      free(mem, NULL);
  }
}
