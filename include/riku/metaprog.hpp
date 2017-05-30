namespace riku
{
  namespace metaprog
  {
    struct constructor_func
    {
      template<class Functor> constructor_func(Functor f) { f(); }
    };
  }
}
