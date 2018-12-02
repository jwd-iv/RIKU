#include "RIKU.h"

namespace riku
{
  typeinfo primitive_type::meta() const { return riku::get<primitive_type>(); }
}
