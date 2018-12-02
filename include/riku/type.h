#pragma once

namespace riku
{
  //! Interface for retrieving information about a given data type.
  class type
  {
  public:
    factory mem_funcs;

    virtual string name() const = 0;
    virtual unsigned size() const = 0;
    
    virtual std::vector<string> properties(bool inherited = false) const = 0;
    virtual var<riku::property> property(char const* name) const = 0;

    virtual std::vector<string> functions(bool c = false, bool inherited = false) const = 0;
    virtual var<riku::function> function(char const* name, bool c = false) const = 0;

    virtual typeinfo pointer() const = 0;
    virtual bool has_parent(typeinfo t) const;
    virtual typelist parents(bool full_list = true) const = 0;
    virtual typelist children(bool full_list = false) const;

    virtual typeinfo meta() const;

  private:
    template<typename T> friend class riku::internal::registrar;
    friend struct generic_type;

    typelist child_types;
  };
}
