namespace riku
{
  namespace internal
  {
    class func_ptr; //!< interface for function to hold and interact with any kind of func pointer

    typedef void (*init_func)(function*);
    typedef bool (*check_func)(typeinfo*);
    typedef variant (*call_func)(func_ptr*, void*, function const*, void**);
    typedef void (*clean_func)(func_ptr*&);
  }

  //! Acts as universal function pointer, it can hold and invoke any kind of function. This should 
  //! only be derived from if you are adding some new kind of function (say a scripting language).
  struct function
  {
    function();
    function(const function& rhs);
    function(function&& rhs);
    function& operator=(function const& rhs);
    ~function();

    template<typename T> function(T func);

    template<typename... Targs>
    variant operator()(variant_type& obj, Targs&&... args) const;
    template<typename... Targs>
    variant operator()(variant_type const& obj, Targs&&... args) const;

    virtual variant invoke(variant_type& obj, array_var& args) const;
    virtual variant invoke(variant_type const& obj, array_var& args) const;

    bool is_const = false;
    bool is_lambda = false;

    struct arg
    {
      typeinfo type = NULL;
      bool is_const = false;
      bool is_ref = false;
    } *arg_list = NULL;
    unsigned arg_count = 0;

    typeinfo obj_type = NULL, ret_type = NULL;

  private:
    internal::init_func  ini = NULL;
    internal::check_func chk = NULL;
    internal::call_func  cal = NULL;
    internal::clean_func clr = NULL;
    internal::func_ptr*  ptr = NULL;

    variant internal_invoke(void* obj, array_var& args) const;
  };

  struct command
  {
    function f;
    variant obj;
    array args;
  };
}