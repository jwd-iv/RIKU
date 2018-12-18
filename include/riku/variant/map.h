#pragma once
#include "riku/variant/variant_type.h"

namespace riku
{
  class map_var : public variant_type
  {
  public:
    virtual bool assign(variant_type const& rhs);
    virtual bool assignto(variant_type& rhs) const;

    virtual variant property(string n) = 0;
    virtual variant property(string n) const = 0;
    virtual std::vector<string> properties() const = 0;

    virtual var<riku::function> function(string n);
    virtual var<riku::function> function(string n) const;
    virtual variant invoke(string func, riku::array& args);
    virtual variant invoke(string func, riku::array& args) const;
  };

  struct map : public map_var
  {
    virtual variant property(string n);
    virtual variant property(string n) const;
    virtual std::vector<string> properties() const;

  protected:
    std::unordered_map<string, variant>
  };
}
