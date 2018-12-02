#pragma once

namespace riku
{
  //! Represents *only* a type template, not instantiations, so this is incomplete on its own.
  struct template_type : public primitive_type
  {
    // Inherited via primitive_type
    virtual string name() const;
    virtual unsigned size() const;
    virtual std::vector<string> properties(bool inherited) const;
    virtual var<riku::property> property(char const* name) const;
    virtual std::vector<string> functions(bool c, bool inherited) const;
    virtual var<riku::function> function(char const* name, bool c) const;
    virtual typeinfo pointer() const;
    virtual typelist parents(bool full_list = true) const;

    virtual typeinfo meta() const;

    std::vector<template_instance const*> instantiations;
  };

  //! Represents an instantiated template_type.
  struct template_instance : public generic_type
  {
    virtual string name() const;
    virtual typeinfo meta() const;

    template_type* base;
    typelist params;
  };
}
