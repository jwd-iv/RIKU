#include "RIKU.h"

#define rkPrimitive(N) rkType(N, rkDefaultFactory);
#define rkNumPrimitive(N) rkType(N, rkDefaultFactory rkNumeric);

namespace riku
{
  typeinfo find(char const* name)
  {
    return library::get()->find(name);
  }

  RIKU_API library* library::modify()
  {
    static library s_lib;
    return &s_lib;
  }

  RIKU_API library const* library::get()
  {
    return modify();
  }

  typeinfo library::find(char const* name) const
  {
    auto pair = types.find(name);
    if (pair == types.end())
      return typeinfo();
    return pair->second;
  }
}

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

static riku::metaprog::constructor_func pVoidType( []() {
  static riku::generic_type voidP, voidCP;
  if (voidP.s == 0 || voidCP.s == 0)
  {
    voidP.n = "void*"; voidCP.n = "void const*";
    voidP.s = sizeof(void*); voidCP.s = sizeof(void const*);

    voidP.mem_funcs.allocate       = riku::default_mem::heap_default_new<void*>::run;
    voidP.mem_funcs.free           = riku::default_mem::heap_delete<void*>::run;
    voidP.mem_funcs.construct      = riku::default_mem::placement_default_new<void*>::run;
    voidP.mem_funcs.destruct       = riku::default_mem::placement_delete<void*>::run;
    voidP.mem_funcs.clone          = riku::default_mem::heap_copy<void*>::run;
    voidP.mem_funcs.copy_construct = riku::default_mem::placement_copy<void*>::run;
    voidP.mem_funcs.assign         = riku::default_mem::assign<void*>::run;

    voidCP.mem_funcs.allocate       = riku::default_mem::heap_default_new<void const*>::run;
    voidCP.mem_funcs.free           = riku::default_mem::heap_delete<void const*>::run;
    voidCP.mem_funcs.construct      = riku::default_mem::placement_default_new<void const*>::run;
    voidCP.mem_funcs.destruct       = riku::default_mem::placement_delete<void const*>::run;
    voidCP.mem_funcs.clone          = riku::default_mem::heap_copy<void const*>::run;
    voidCP.mem_funcs.copy_construct = riku::default_mem::placement_copy<void const*>::run;
    voidCP.mem_funcs.assign         = riku::default_mem::assign<void const*>::run;

    const_cast<riku::library*>(riku::library::get())->types["void*"]       = &voidP;
    const_cast<riku::library*>(riku::library::get())->types["void const*"] = &voidCP;
  }
});

rkPrimitive(bool)
rkPrimitive(char)
//rkPrimitive(void*)
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
