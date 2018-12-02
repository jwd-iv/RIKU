#include "RIKU.h"

namespace riku
{
  string template_type::name() const
  {
    return n;
  }

  unsigned template_type::size() const
  {
    return 0;
  }

  std::vector<string> template_type::properties(bool inherited) const
  {
    return std::vector<string>();
  }

  var<riku::property> template_type::property(char const* name) const
  {
    return var<riku::property>();
  }

  std::vector<string> template_type::functions(bool c, bool inherited) const
  {
    return std::vector<string>();
  }

  var<riku::function> template_type::function(char const* name, bool c) const
  {
    return var<riku::function>();
  }

  typeinfo template_type::pointer() const
  {
    return typeinfo();
  }

  typelist template_type::parents(bool full_list) const
  {
    return typelist();
  }

  typeinfo template_type::meta() const
  {
    return riku::get<template_type>();
  }

  string template_instance::name() const
  {
    return n;
  }

  typeinfo template_instance::meta() const
  {
    return typeinfo();
  }
}
