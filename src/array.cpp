#include "RIKU.h"

// array_var implementation
namespace riku
{
  array_var::iterator array_var::begin()
  {
    iterator i;
    i.arr = this;
    i.i = 0;
    return i;
  }

  array_var::iterator array_var::end()
  {
    iterator i;
    i.arr = this;
    i.i = length();
    return i;
  }

  array_var::const_iterator array_var::begin() const
  {
    const_iterator i;
    i.arr = this;
    i.i = 0;
    return i;
  }

  array_var::const_iterator array_var::end() const
  {
    const_iterator i;
    i.arr = this;
    i.i = length();
    return i;
  }

  variant array_var::for_each(var<riku::function> const f)
  {
    if (f.data() == NULL)
      return variant();

    array ret;
    for (auto& iter : *this)
    {
      array of_one;
      of_one.insert(iter, 0);
      variant const fret = f->invoke(variant(), of_one);
      if (fret.data() != NULL)
        ret.insert(fret, ret.length());
    }

    return ret;
  }

  variant array_var::for_each(var<riku::function> const f) const
  {
    if (f.data() == NULL)
      return variant();

    array ret;
    for (auto const& iter : *this)
    {
      array of_one;
      of_one.insert(iter, 0);
      variant const fret = f->invoke(variant(), of_one);
      if (fret.data() != NULL)
        ret.insert(fret, ret.length());
    }

    return ret;
  }

  bool array_var::push_back(variant_type const& v)
  {
    return insert(v, length());
  }

  variant_type& array_var::operator[](unsigned idx)
  {
    if (idx >= length())
      return variant();
    else
      return get(idx);
  }

  variant_type const& array_var::operator[](unsigned idx) const
  {
    if (idx >= length())
      return variant();
    else
      return get(idx);
  }

  bool array_var::assign(variant_type const& rhs)
  {
    bool assigned = false;
    for (auto& iter : *this)
    {
      assigned = iter.assign(rhs) || assigned;
    }
    return assigned;
  }

  variant array_var::property(string n)
  {
    array ret;
    for (auto& iter : *this)
    {
      auto const p = iter.property(n);
      if (p.data() != NULL)
        ret.insert(p, ret.length());
    }
    return ret;
  }

  variant array_var::property(string n) const
  {
    array ret;
    for (auto const& iter : *this)
    {
      auto const p = iter.property(n);
      if (p.data() != NULL)
        ret.insert(p, ret.length());
    }
    return ret;
  }

  var<riku::function> array_var::function(string n)
  {
    array ret;
    for (auto& iter : *this)
    {
      auto f = iter.function(n);
      if (f.data() != NULL)
        ret.insert(f, ret.length());
    }
    return ret;
  }

  var<riku::function> array_var::function(string n) const
  {
    array ret;
    for (auto const& iter : *this)
    {
      auto f = iter.function(n);
      if (f.data() != NULL)
        ret.insert(f, ret.length());
    }
    return ret;
  }

  bool array_var::operator==(variant_type const& rhs) const
  {
    //TODO: make this do something
    return false;
  }

  bool array_var::operator!=(variant_type const& rhs) const
  {
    //TODO: make this do something
    return false;
  }

  variant array_var::operator[](string prop)
  {
    return property(prop);
  }

  variant array_var::operator[](string prop) const
  {
    return property(prop);
  }

  variant array_var::invoke(string func, riku::array& args)
  {
    array ret;
    for (auto& iter : *this)
    {
      auto const r = iter(func, args);
      if(r.data() != NULL)
        ret.insert(r, ret.length());
    }
    return ret;
  }

  variant array_var::invoke(string func, riku::array& args) const
  {
    array ret;
    for (auto const& iter : *this)
    {
      auto const r = iter(func, args);
      if (r.data() != NULL)
        ret.insert(r, ret.length());
    }
    return ret;
  }

  bool array_var::is_array() const
  {
    return true;
  }

  array_var* array_var::as_array()
  {
    return this;
  }

  array_var const* array_var::as_array() const
  {
    return this;
  }

  typeinfo array_var::type() const
  {
    if (!length())
      return NULL;

    typeinfo ret = get(0).type();
    for (auto const& iter : *this)
      if (iter.type() != ret)
        return NULL;

    return ret;
  }

  void* array_var::data()
  {
    if (length() > 1)
      return this;
    else if (length() == 1)
      return get(0).data();
    else return NULL;
  }

  void const* array_var::data() const
  {
    if (length() > 1)
      return this;
    else if (length() == 1)
      return get(0).data();
    else return NULL;
  }

  typeinfo array_var::meta() const
  {
    return riku::get<array_var>();
  }
}

// iterator implementation
namespace riku
{
  bool array_var::iterator::operator==(array_var::iterator const& rhs) const
  {
    return arr == rhs.arr && i == rhs.i;
  }

  array_var::iterator& array_var::iterator::operator++()
  {
    if (arr != NULL && i < arr->length())
      ++i;
    return *this;
  }

  array_var::iterator array_var::iterator::operator++(int)
  {
    iterator tmp = *this;
    ++*this;
    return tmp;
  }

  array_var::iterator& array_var::iterator::operator--()
  {
    if (arr != NULL && i > 0)
      --i;
    return *this;
  }

  array_var::iterator array_var::iterator::operator--(int)
  {
    iterator tmp = *this;
    --*this;
    return tmp;
  }

  variant_type& array_var::iterator::operator*()
  {
    if(arr == NULL || i >= arr->length())
      return *this;

    return arr->get(i);
  }

  variant_type* array_var::iterator::operator->()
  {
    if (arr == NULL || i >= arr->length())
      return NULL;

    return &arr->get(i);
  }

  variant_type const& array_var::iterator::operator*() const
  {
    if (arr == NULL || i >= arr->length())
      return *this;

    return arr->get(i);
  }

  variant_type const* array_var::iterator::operator->() const
  {
    if (arr == NULL || i >= arr->length())
      return NULL;

    return &arr->get(i);
  }

  typeinfo array_var::iterator::type() const
  {
    if (arr == NULL || i >= arr->length())
      return NULL;

    return operator*().type();
  }

  void* array_var::iterator::data()
  {
    if (arr == NULL || i >= arr->length())
      return NULL;

    return operator*().data();
  }

  void const* array_var::iterator::data() const
  {
    if (arr == NULL || i >= arr->length())
      return NULL;

    return operator*().data();
  }

  typeinfo array_var::iterator::meta() const
  {
    return riku::get<iterator>();
  }
}

// const_iterator implementation
namespace riku
{
  bool array_var::const_iterator::operator==(array_var::const_iterator const& rhs) const
  {
    return arr == rhs.arr && i == rhs.i;
  }

  array_var::const_iterator& array_var::const_iterator::operator++()
  {
    if (arr != NULL && i < arr->length())
      ++i;
    return *this;
  }

  array_var::const_iterator array_var::const_iterator::operator++(int)
  {
    const_iterator tmp = *this;
    ++*this;
    return tmp;
  }

  array_var::const_iterator& array_var::const_iterator::operator--()
  {
    if (arr != NULL && i > 0)
      --i;
    return *this;
  }

  array_var::const_iterator array_var::const_iterator::operator--(int)
  {
    const_iterator tmp = *this;
    --*this;
    return tmp;
  }

  variant_type const& array_var::const_iterator::operator*() const
  {
    if (arr == NULL || i >= arr->length())
      return *this;

    return arr->get(i);
  }

  variant_type const* array_var::const_iterator::operator->() const
  {
    if (arr == NULL || i >= arr->length())
      return NULL;

    return &arr->get(i);
  }

  typeinfo array_var::const_iterator::type() const
  {
    if (arr == NULL || i >= arr->length())
      return NULL;

    return operator*().type();
  }

  void* array_var::const_iterator::data()
  {
    return NULL;
  }

  void const* array_var::const_iterator::data() const
  {
    if (arr == NULL || i >= arr->length())
      return NULL;

    return operator*().data();
  }

  typeinfo array_var::const_iterator::meta() const
  {
    return riku::get<const_iterator>();
  }
}

// array implementation
namespace riku
{
  array::array() : vec() {}

  array::array(array const& rhs) : vec(rhs.vec) {}

  array::array(array&& rhs) { vec = std::move(rhs.vec); }

  array::array(array_var const& rhs)
  {
    for (auto const& iter : rhs)
      vec.push_back(iter);
  }

  array::array(variant_type const& rhs) : array() {
    if (rhs.is_array())
      operator=(*rhs.as_array());
    else
      vec.push_back(rhs);
  }

  array& array::operator=(array const& rhs)
  {
    if (&rhs != this)
    {
      vec.clear();
      vec = rhs.vec;
    }
    return *this;
  }

  array& array::operator=(array_var const& rhs)
  {
    if (&rhs != this)
    {
      vec.clear();
      for (auto const& iter : rhs)
        vec.push_back(iter);
    }
    return *this;
  }

  unsigned array::length() const
  {
    return vec.size();
  }

  variant_type& array::get(unsigned idx)
  {
    if (idx < length())
      return vec[idx];
    return *this;
  }

  variant_type const& array::get(unsigned idx) const
  {
    if (idx < length())
      return vec[idx];
    return *this;
  }

  bool array::insert(variant_type const& v, unsigned idx)
  {
    if (idx > length())
      idx = length();

    if (v.meta()->has_parent(riku::get<array>()))
    {
      array const* av = dynamic_cast<array const*>(v.as_array());
      if (av != NULL)
      {
        for (auto const& iter : av->vec)
          vec.insert(vec.begin() + idx++, iter);
        return true;
      }
      return false;
    }
    else if (v.meta()->has_parent(riku::get<array_var>()))
    {
      unsigned l = length();
      for (auto const& iter : *v.as_array())
        vec.insert(vec.begin() + idx++, iter);
      return length() > l;
    }
    else
    {
      auto i = vec.insert(vec.begin() + idx, v);
      return i != vec.end();
    }
  }

  bool array::erase(unsigned idx)
  {
    if (idx >= length())
      idx = length() - 1;

    unsigned l = length();
    vec.erase(vec.begin() + idx);
    return l == length() + 1;
  }

  typeinfo array::meta() const
  {
    return riku::get<array>();
  }
};
