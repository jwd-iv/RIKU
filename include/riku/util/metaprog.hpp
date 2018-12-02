#pragma once

namespace riku
{
  namespace metaprog
  {
    struct constructor_func
    {
      template<class Functor> constructor_func(Functor f) { f(); }
    };

    template<typename... Targs> class typelist;

    template<typename T, typename... Targs> class typelist<T, Targs...>
    {
    public:
      typedef typelist<Targs...> NextArg;
      static size_t const len = NextArg::len + 1;

      static ::riku::typelist create()
      {
        ::riku::typelist list;
        list.resize(len);
        fill_list(list);
        return list;
      }

    private:
      template<typename... Targs> friend class typelist;
      static void fill_list(::riku::typelist& list)
      {
        *(list.end() - len) = riku::get<T>();
        NextArg::fill_list(list);
      }
    };

    template<typename T> class typelist<T>
    {
    public:
      static size_t const len = 1;

      static ::riku::typelist create()
      {
        ::riku::typelist list;
        list.push_back(riku::get<T>());
        return list;
      }

    private:
      template<typename... Targs> friend class typelist;
      static void fill_list(::riku::typelist& list)
      {
        *(list.end() - len) = riku::get<T>();
      }
    };
  }
}
