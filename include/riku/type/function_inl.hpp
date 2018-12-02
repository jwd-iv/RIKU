namespace riku
{
  namespace internal
  {
    template<class Fp> struct func_holder;
    template<class Fp> struct func_creator;

    template<class Fp> struct invoker;
    template<class Fp, class R, class... Targs> class func_unwrapper;
    template<class Fp, class O, class R, class... Targs> class func_deducer;
    template<class Fp> struct deduce_traits;
  }

  // template instantiations for the various kinds of func pointer
  namespace internal
  {
    class func_ptr { public: virtual func_ptr* clone() const = 0; };

    template<class Fp> struct func_holder : public func_ptr
    {
      Fp f_ptr;
      func_holder(Fp f) : f_ptr(f) {}
      virtual func_ptr* clone() const { return new func_holder<Fp>(f_ptr); }
    };

    template<class Fp> struct base_creator
    {
      static func_ptr* create(Fp f)
      {
        return new func_holder<Fp>(f);
      }
      static void clear(func_ptr*& p)
      {
        delete p;
        p = NULL;
      }
    };

    template <class Fp> struct function_creator
    {
      static func_ptr* create(Fp)
      {
        return new func_holder<decltype(&Fp::operator())>(&Fp::operator());
      }
      static void clear(func_ptr*& p)
      {
        delete p;
        p = NULL;
      }
    };

    template <class R, class... VArgs>
    struct function_creator<R(*)(VArgs...)>
      : public base_creator < R(*)(VArgs...) > {};

    template <class O, class R, class... VArgs>
    struct function_creator<R(O::*)(VArgs...)>
      : public base_creator < R(O::*)(VArgs...) > {};

    template <class O, class R, class... VArgs>
    struct function_creator<R(O::*)(VArgs...)const>
      : public base_creator < R(O::*)(VArgs...)const > {};
  }

  // handles invoking the actual function pointer correctly
  namespace internal
  {
    template<class R, class... Targs>
    struct invoker<R(*)(Targs...)>
    {
      typedef func_holder<R(*)(Targs...)> holder;
      typedef typename std::conditional<
        std::is_reference<R>::value || std::is_pointer<R>::value,
        riku::ptr, riku::val>::type ret_container;
      static variant call(func_ptr* func, void* obj, Targs&&... args)
      {
        return ret_container(static_cast<holder*>(func)->f_ptr(std::forward<Targs&&>(args)...));
      }
    };
    template<class... Targs>
    struct invoker<void(*)(Targs...)>
    {
      typedef func_holder<void(*)(Targs...)> holder;
      static variant call(func_ptr* func, void* obj, Targs&&... args)
      {
        static_cast<holder*>(func)->f_ptr(std::forward<Targs&&>(args)...);
        return variant();
      }
    };

    template<class R, class O, class... Targs>
    struct invoker<R(O::*)(Targs...)>
    {
      typedef func_holder<R(O::*)(Targs...)> holder;
      typedef typename std::conditional<
        std::is_reference<R>::value || std::is_pointer<R>::value,
        riku::ptr, riku::val>::type ret_container;
      static variant call(func_ptr* func, void* obj, Targs&&... args)
      {
        return ret_container((static_cast<O*>(obj)->*(static_cast<holder*>(func)->f_ptr))(std::forward<Targs&&>(args)...));
      }
    };
    template<class O, class... Targs>
    struct invoker<void(O::*)(Targs...)>
    {
      typedef func_holder<void(O::*)(Targs...)> holder;
      static variant call(func_ptr* func, void* obj, Targs&&... args)
      {
        (static_cast<O*>(obj)->*(static_cast<holder*>(func)->f_ptr))(std::forward<Targs&&>(args)...);
        return variant();
      }
    };

    template<class R, class O, class... Targs>
    struct invoker<R(O::*)(Targs...)const>
    {
      typedef func_holder<R(O::*)(Targs...)const> holder;
      typedef typename std::conditional<
        std::is_reference<R>::value || std::is_pointer<R>::value,
        riku::ptr, riku::val>::type ret_container;
      static variant call(func_ptr* func, void* obj, Targs&&... args)
      {
        return ret_container((static_cast<O const*>(obj)->*(static_cast<holder*>(func)->f_ptr))(std::forward<Targs&&>(args)...));
      }
    };
    template<class O, class... Targs>
    struct invoker<void(O::*)(Targs...)const>
    {
      typedef func_holder<void(O::*)(Targs...)const> holder;
      static variant call(func_ptr* func, void* obj, Targs&&... args)
      {
        (static_cast<O const*>(obj)->*(static_cast<holder*>(func)->f_ptr))(std::forward<Targs&&>(args)...);
        return variant();
      }
    };
  }

  // recursive function unwrapping for verifying and forwarding arguments to the invoker
  namespace internal
  {
    template<class Fp, class R, class... Targs>
    class func_unwrapper
    {
    public:
      template<size_t> struct unwrap;
      typedef typename unwrap<sizeof...(Targs)> unwrap_start;

      static void init(function* f)
      {
        if (f->arg_count)
        {
          f->arg_list = new function::arg[f->arg_count];
          unwrap_start::init(f->arg_list);
        }
      }
      static bool check(typeinfo* args)
      {
        return unwrap_start::check(args);
      }
      static variant call(func_ptr* fp, void* obj, function const* f, void** args)
      {
        return unwrap_start::call(fp, obj, f->arg_list, args);
      }

      typedef std::tuple<Targs...> arg_tuple;

      template<size_t N> struct unwrap
      {
        enum { idx = sizeof...(Targs)-N };
        typedef typename std::tuple_element<idx, arg_tuple>::type Targ;
        typedef typename unwrap<N - 1> unwrap_next;

        static void init(function::arg* traits)
        {
          function::arg& a = traits[idx];
          a.type = riku::get<Targ>();
          a.is_const = std::is_const<Targ>::value;
          a.is_ref = std::is_reference<Targ>::value;
          unwrap_next::init(traits);
        }
        static bool check(typeinfo* args)
        {
          return args[idx] == riku::get<Targ>() && unwrap_next::check(args);
        }
        template<class... Uargs>
        static variant call(func_ptr* fp, void* obj, function::arg const* traits, void** args, Uargs&&... fwd_args)
        {
          return unwrap_next::call(fp, obj, traits, args, std::forward<Uargs&&>(fwd_args)...,
            static_cast<Targ>(*static_cast<std::remove_reference<Targ>::type*>(args[idx])));
        }
      };

      template<> struct unwrap<0>
      {
        static void init(function::arg*) { ; }
        static bool check(typeinfo*) { return true; }
        template<class... Uargs>
        static variant call(func_ptr* fp, void* obj, function::arg const* traits, void** args, Uargs&&... fwd_args)
        {
          return invoker<Fp>::call(fp, obj, std::forward<Targs&&>(fwd_args)...);
        }
      };
    };
  }

  // function trait deduction
  namespace internal
  {
    template<class Fp, class O, class R, class... Targs> class func_deducer : public func_unwrapper<Fp, R, Targs...>
    {
    public:
      enum {
        num_args = sizeof...(Targs),
        has_return_value = (std::is_same<R, void>::value == false),
        is_member_func = (std::is_same<O, void>::value == false)
      };

      typedef typename O obj_type;
      typedef typename Fp func_type;
      typedef typename std::conditional<has_return_value, R, nulltype>::type return_type;
    };

    //default deduction for functors and lambdas
    template<class T>
    struct deduce_traits : deduce_traits<decltype(&T::operator())>
    {
      enum { is_lambda = true, is_const = true };
      typedef typename nulltype obj_type;
      typedef typename func_deducer::func_type func_type;
      typedef typename func_deducer::return_type return_type;
    };

    //global functions
    template<class R, class... Targs>
    struct deduce_traits<R(*)(Targs...)> : public func_deducer<R(*)(Targs...), nulltype, R, Targs...>
    {
      enum { is_lambda = false, is_const = false };
      typedef typename func_deducer::obj_type obj_type;
      typedef typename func_deducer::func_type func_type;
      typedef typename func_deducer::return_type return_type;
    };

    //member functions
    template<class O, class R, class... Targs>
    struct deduce_traits<R(O::*)(Targs...)> : public func_deducer<R(O::*)(Targs...), O, R, Targs...>
    {
      enum { is_lambda = false, is_const = false };
      typedef typename func_deducer::obj_type obj_type;
      typedef typename func_deducer::func_type func_type;
      typedef typename func_deducer::return_type return_type;
    };

    //const member functions
    template<class O, class R, class... Targs>
    struct deduce_traits<R(O::*)(Targs...)const> : public func_deducer<R(O::*)(Targs...)const, O, R, Targs...>
    {
      enum { is_lambda = false, is_const = true };
      typedef typename func_deducer::obj_type obj_type;
      typedef typename func_deducer::func_type func_type;
      typedef typename func_deducer::return_type return_type;
    };
  }

  template<class T>
  function::function(T f) : is_const(internal::deduce_traits<T>::is_const),
    is_lambda(internal::deduce_traits<T>::is_lambda),
    arg_count(internal::deduce_traits<T>::num_args),
    obj_type(riku::get<internal::deduce_traits<T>::obj_type>()),
    ret_type(riku::get<internal::deduce_traits<T>::return_type>()),
    ini(&internal::deduce_traits<T>::init),
    chk(&internal::deduce_traits<T>::check),
    cal(internal::deduce_traits<T>::call),
    clr(internal::function_creator<T>::clear)
  {
    if (ini != NULL) ini(this);
    ptr = internal::function_creator<T>::create(f);
  }

  template<class... Targs>
  variant function::operator()(variant_type& obj, Targs&&... args) const
  {
    if (sizeof...(Targs) != arg_count
      || cal == NULL || chk == NULL
      || (obj_type != NULL && !obj.type()->has_parent(obj_type)))
      return variant();
    const unsigned size = sizeof...(args)+1;

    typeinfo types[size] = { riku::inspect(args)... };
    if (!chk(types))
      return variant();

    void* args_ptr[size] = { &args... };
    return cal(ptr, obj.data(), this, args_ptr);
  }

  template<class... Targs>
  variant function::operator()(variant_type const& obj, Targs&&... args) const
  {
    if (sizeof...(Targs) != arg_count
      || cal == NULL || chk == NULL
      || (obj_type != NULL && !obj.type()->has_parent(obj_type)))
      return variant();
    const unsigned size = sizeof...(args)+1;

    typeinfo types[size] = { riku::inspect(args)... };
    if (!chk(types))
      return variant();

    void* args_ptr[size] = { &args... };
    return cal(ptr, const_cast<void*>(obj.data()), this, args_ptr);
  }
}