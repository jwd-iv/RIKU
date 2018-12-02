#pragma once

namespace riku
{
  //! Represents a pointer to another type.
  struct pointer_type : public type
  {
    pointer_type* dbl_ptr = NULL;
    typeinfo points_to = NULL;

    virtual string name() const { return points_to != NULL ? points_to->name() + "*" : string(); }
    virtual unsigned size() const { return sizeof(void*); }

    virtual std::vector<string> properties(bool) const { return std::vector<string>(); }
    virtual var<riku::property> property(char const*) const { return var<riku::property>(); }

    virtual std::vector<string> functions(bool, bool) const { return std::vector<string>(); }
    virtual var<riku::function> function(char const*, bool) const { return var<riku::function>(); }

    virtual typeinfo pointer() const;
    virtual typelist parents(bool) const;

    template<typename T> friend class riku::internal::registrar;
    virtual typeinfo meta() const;
  };
}
