#pragma once

namespace riku
{
  //! Represents most types, holds lists of properties and functions.
  struct generic_type : public primitive_type
  {
    virtual string name() const;
    virtual unsigned size() const;

    virtual std::vector<string> properties(bool inherited = false) const;
    virtual var<riku::property> property(char const* name) const;

    virtual std::vector<string> functions(bool c = false, bool inherited = false) const;
    virtual var<riku::function> function(char const* name, bool c = false) const;

    virtual typeinfo pointer() const;
    virtual typelist parents(bool full_list = true) const;

    template<typename T> friend class riku::internal::registrar;
    virtual typeinfo meta() const;

  protected:
    std::unordered_map<string, riku::property*> props;
    std::unordered_multimap<string, riku::function*> funcs;

    typelist ps;
  };
}
