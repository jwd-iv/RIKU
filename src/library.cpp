#include "RIKU.h"

#define rkPrimitive(N) rkType(N, rkDefaultFactory);
#define rkNumPrimitive(N) rkType(N, rkDefaultFactory rkNumeric);

template<> riku::generic_type* riku::internal::registrar<void>::get()
{
  static riku::generic_type s_type;
  return &s_type;
}
template<> riku::internal::registrar<void>::registrar()
{
  get()->n = "void";
  riku::library::modify()->types["void"] = get();
}
riku::internal::registrar<void> void_register;

rkPrimitive(bool)
rkPrimitive(char)
rkPrimitive(std::string)
rkNumPrimitive(short)
rkNumPrimitive(int)
rkNumPrimitive(long)
rkNumPrimitive(long long)
rkNumPrimitive(unsigned char)
rkNumPrimitive(unsigned short)
rkNumPrimitive(unsigned int)
rkNumPrimitive(unsigned long)
rkNumPrimitive(unsigned long long)
rkNumPrimitive(float)
rkNumPrimitive(double)
rkNumPrimitive(long double)

rkType(riku::nulltype, )
rkType(riku::type, )
rkType(riku::pointer_type,      rkParent(type)           rkDefaultFactory)
rkType(riku::primitive_type,    rkParent(type))
rkType(riku::generic_type,      rkParent(primitive_type) rkDefaultFactory)
rkType(riku::template_type,     rkParent(primitive_type) rkDefaultFactory)
rkType(riku::template_instance, rkParent(generic_type)   rkDefaultFactory)

rkType(riku::factory, rkDefaultFactory)

rkType(riku::variant_type, )
rkType(riku::ptr,     rkParent(variant_type) rkDefaultFactory)
rkType(riku::val,     rkParent(variant_type) rkDefaultFactory)
rkType(riku::variant, rkParent(variant_type) rkDefaultFactory)

rkType(riku::array_var,                 rkParent(variant_type))
rkType(riku::array_var::iterator,       rkParent(variant_type) rkDefaultFactory)
rkType(riku::array_var::const_iterator, rkParent(variant_type) rkDefaultFactory)
rkType(riku::array,                     rkParent(array_var) rkDefaultFactory)

rkType(riku::property, )
rkType(riku::member,  rkParent(property) rkDefaultFactory)
rkType(riku::get_set, rkParent(property))

rkType(riku::function, rkDefaultFactory)

namespace riku
{
  typeinfo find(char const* name)
  {
    return library::get()->find(name);
  }

  RIKU_API library* riku::library::modify()
  {
    static library s_lib;
    return &s_lib;
  }

  RIKU_API library const* riku::library::get()
  {
    return library::modify();
  }

  typeinfo riku::library::find(char const* name) const
  {
    auto pair = types.find(name);
    if(pair == types.end())
      return typeinfo();
    return pair->second;
  }
}
