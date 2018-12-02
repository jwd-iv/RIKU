namespace riku
{
  //! The interface for inspecting/modifying data contained within a type.
  class property
  {
  public:
    virtual typeinfo type() const = 0;
    virtual typeinfo parent() const;

    virtual variant get(variant_type& obj) const = 0;
    virtual variant get(variant_type const& obj) const = 0;
    virtual bool set(variant_type& obj, variant_type const& data) const = 0;

    template<typename T> friend class internal::registrar;

  protected:
    typeinfo p;
  };

  //! Represents a data member of the type.
  struct member : public property
  {
    virtual typeinfo type() const;

    virtual variant get(variant_type& obj) const;
    virtual variant get(variant_type const& obj) const;
    virtual bool set(variant_type& obj, variant_type const& data) const;

    template<typename T> friend class internal::registrar;

  private:
    typeinfo t;
    unsigned offset;
  };

  //! Represents a Get() and Set() function for custom properties
  struct get_set : public property
  {
    virtual typeinfo type() const;
    virtual variant get(variant_type& obj) const;
    virtual variant get(variant_type const& obj) const;
    virtual bool set(variant_type& obj, variant_type const& data) const;

    template<typename T> friend class internal::registrar;

  private:
    riku::function *g, *s;
  };
}
