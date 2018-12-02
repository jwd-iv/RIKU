#pragma once

namespace riku
{
  //! Represents a plain data type with no properties or functions.
  struct primitive_type : public type
  {
    string n;
    unsigned s = 0;
    pointer_type ptr_type;

    template<typename T> friend class riku::internal::registrar;
    virtual typeinfo meta() const;
  };
}
