namespace riku
{
  struct library
  {
    static RIKU_API library const* get();

    typeinfo find(char const* name) const;

  private:
    std::unordered_map<std::string, typeinfo> types;

    static RIKU_API library* modify();
    template<typename T> friend class internal::registrar;
  };
}
