#define PASTE(_, __) _##__
#define NGINTERNAL(_) PASTE(GENERATED_NAME_, _)
#define GENERATE_NAME() NGINTERNAL(__COUNTER__)

/*! @defgroup registration Registration Macros
    @brief These macros are the 'language' for RIKU's type reflection.
    @{ */

/*! @brief Add this to a class to give it a virtual function, meta(), that
    returns the proper type. Helpful for discerning dynamic object's types
    at runtime.
    @param Type
      Tye type this macro is being added to. */
#define rkMetaHook(Type) \
  public: template<typename T> friend class riku::internal::registrar; \
  virtual riku::typeinfo meta() const {return riku::get<Type>();}

#define rkMetaHandle(Type) rkMetaHook(Type); typedef riku::var<Type> handle;

#ifndef RIKU_SHARED
#  define rkExportLib(Name) void* __rklib_##Name (void*) {return NULL;}
#else
#  define rkExportLib(Name) void* RIKU_GENAPI __rklib_##Name (void*) {return NULL;}
#endif

#define rkLink(Name)                                \
  extern void* __rklib_##Name (void*);              \
  struct _rklibinit_##Name { _rklibinit_##Name () { \
    __rklib_##Name(NULL); }} GENERATE_NAME();

#define rkImport()

/*! @brief Use this to define the information about a C/C++ type.
    @param Name
      This is the exact name of the type as it was defined and will become the
      string name of the type in the library. You can call this macro inside a
      namespace block or use 'using' if you do/don't want a namespace to show up
      in the registered name.
    @param Definition
      This is where the specification of properties, members, and etc. goes,
      usually as a combination of the other rk macros. */
#define rkType(Name, Definition) template<>                    \
  riku::generic_type* riku::internal::registrar<Name>::get() { \
    static riku::generic_type s_type; return &s_type; }        \
  template<> riku::internal::registrar<Name>::registrar() {    \
    get()->n = #Name; get()->s = sizeof(Name); Definition;     \
    riku::library::modify()->types[#Name] = get();             \
  } riku::internal::registrar<Name> GENERATE_NAME();

#define rkTemplatedType(Name, Definition)                                                                               \
  template<typename... Targs> class ::riku::internal::registrar< Name<Targs...> >                                       \
  { public: typedef Name<Targs...> my_type; static template_instance* get()                                             \
    { static template_type* base_template = NULL; static template_instance* instance = NULL;                            \
      if (base_template == NULL) { base_template = dynamic_cast<template_type*>(const_cast<type*>(riku::find(#Name)));  \
        if (base_template == NULL) { base_template = new template_type; base_template->n = #Name; base_template->s = 0; \
          riku::library::modify()->types[base_template->n.c_str()] = base_template; } }                                 \
      if (instance == NULL) { instance = new template_instance; instance->s = sizeof(Name<Targs...>);                   \
        instance->base = base_template; instance->params = metaprog::typelist<Targs...>::create();                      \
        instance->n = base_template->name() + "<"; for (auto const& t : instance->params) {                             \
          instance->n += t->name() + ", "; } instance->n.pop_back(); instance->n.pop_back();                            \
        instance->n += ">"; Definition; base_template->instantiations.push_back(instance);                              \
      } return instance; } };

//! Adds a default new and delete implementation to the type's factory.
#define rkConstructable                                                                \
  get()->mem_funcs.allocate = riku::default_mem::heap_default_new<my_type>::run;       \
  get()->mem_funcs.free = riku::default_mem::heap_delete<my_type>::run;                \
  get()->mem_funcs.construct = riku::default_mem::placement_default_new<my_type>::run; \
  get()->mem_funcs.destruct = riku::default_mem::placement_delete<my_type>::run;
//! Adds a default copy constructor to the type's factory.
#define rkCopiable                                                     \
  get()->mem_funcs.clone = riku::default_mem::heap_copy<my_type>::run; \
  get()->mem_funcs.copy_construct = riku::default_mem::placement_copy<my_type>::run;
//! Adds the assignment operator to the type's factory.
#define rkAssignable \
  get()->mem_funcs.assign = riku::default_mem::assign<my_type>::run;
//! Not yet implemented.
#define rkMovable \
  ;

//! For brevity, this adds a full default factory to the type
#define rkDefaultFactory rkConstructable rkCopiable rkAssignable rkMovable
//! Adds all factory functions except assignment
#define rkStaticFactory rkConstructable rkCopiable
//! Not yet implemented.
#define rkCustomFactory(...) ;

//! Not yet implemented.
#define rkCustomConvertsTo(Type, lambda)   ;
//! Not yet implemented.
#define rkConvertsTo(Type) ;
//! Adds conversion operators for this type to all the numeric primitive types.
#define rkNumeric \
  rkConvertsTo(float) rkConvertsTo(double) rkConvertsTo(long double) \
  rkConvertsTo(int)          rkConvertsTo(short) \
  rkConvertsTo(unsigned int) rkConvertsTo(unsigned short) \
  rkConvertsTo(long)          rkConvertsTo(long long) \
  rkConvertsTo(unsigned long) rkConvertsTo(unsigned long long)

/*! @brief Adds a member property to the type.
    @param Name The name of the member as it is in the type definition. */
#define rkMember(Name) { member* newmem = new member; newmem->p = get(); \
  newmem->t = riku::get<decltype(my_type::Name)>();                      \
  newmem->offset = offsetof(my_type, Name); get()->props[#Name] = newmem; }

/*! @brief Adds a custom Get()/Set() property to the type. See main.cpp for example usage.
    @param Name
      The name of the property as you would like it to appear (no quotes!)
    @param getlambda
      Must either be a function pointer or a lambda that returns the desired
      property. If the pointer is to a member function of the reflected type,
      then it must take no parameters. If it's a lambda or a pointer to a
      non-member function, then it must take one parameter, either as the type
      itself by value, pointer, or reference, or as a variant_type& or variant.
    @param setlambda
      Similar to getlambda, but the set function doesn't have to return anything. 
      If it returns a bool, true should represent a success and false a failure. This return
      value will be passed along in property::set() and variant::assign() if applicable.
      Like getlambda, if this is a nonmember function it must take the this pointer either
      as the correct type or as a variant. The last parameter of the set function is how the 
      value to assign is passed in, again either as the correct type or as a variant. */
#define rkInlineGetSet(Name, getlambda, setlambda) { get_set* newmem = new get_set; \
  newmem->p = get(); newmem->g = new riku::function(getlambda);                     \
  newmem->s = new riku::function(setlambda); get()->props[#Name] = newmem; }

/*! @brief Adds a Get()/Set() property of only member functions to the reflected type.
    @param Name
      The name of the property as you would like it to appear (no quotes!)
    @param getfunc
      The name of the get function as it is in the type. (no quotes!)
    @param setfunc
      The name of the set function as it is in the type. (no quotes!) */
#define rkGetSet(Name, getfunc, setfunc) rkInlineGetSet(Name, &my_type::getfunc, &my_type::setfunc)

/*! @brief Adds a custom member function to the reflected type.
    @param Name
      The name of the method as it should be in the type definition. (no quotes!) */
#define rkInlineMethod(Name, Func) get()->funcs.insert( \
  std::pair<riku::string, riku::function*>(#Name, new riku::function(Func)));

/*! @brief Adds a member function to the reflected type.
    @param Name
      The name of the method as it is in the type definition. (no quotes!) */
#define rkMethod(Name) rkInlineMethod(Name, &my_type::Name)

/*! @brief Adds a member function to the reflected type.
    @param Type
      The name of the parent type. (no quotes!) */
#define rkParent(Type) get()->ps.push_back(riku::get<Type>()); \
  riku::internal::registrar<Type>::get()->child_types.push_back(get());

/*! @} */
