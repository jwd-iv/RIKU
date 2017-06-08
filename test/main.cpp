#include "RIKU.h"
#include <iostream>

struct Base
{
  int i = 32;
  typedef riku::var<Base> Handle;

  void DoStuff(std::string) const
  {
  }
  bool IsStuff() const
  {
    return true;
  }
  int& Get() { return i; }
  void Set(int const& j) { i = j; }
};

struct Derived : public Base
{
  typedef riku::var<Derived> Handle;
};

struct Sub : public Derived
{
  typedef riku::var<Sub> Handle;
};

struct Sib : public Base
{
  typedef riku::var<Sib> Handle;
};

struct Outer
{
  Base b;
};

template<typename... Targs> struct ArgStack;
template<typename T> struct ArgStack<T> {
  T val;
  bool next;
};
template<typename T, typename... Targs>
struct ArgStack<T, Targs...> {
  T val;
  typename ArgStack<Targs...> next;
};

rkTemplatedType(ArgStack, rkDefaultFactory rkMember(val) rkMember(next))

rkType(Base,
  rkDefaultFactory
  rkMember(i)
  rkMethod(DoStuff)
  rkMethod(IsStuff)
  rkMethod(Get)
  rkMethod(Set)
  rkGetSet(Eye, Get, Set)
)

rkType(Derived,
  rkDefaultFactory
  rkParent(Base)
  rkInlineGetSet(Eye,
    [](Derived& obj) -> int& {
      return obj.i;
    },
    [](riku::variant obj, int const& i) {
      obj["i"] << i;
    }
  )
)

rkType(Sub,
  rkDefaultFactory
  rkParent(Derived)
)

rkType(Sib,
  rkDefaultFactory
  rkParent(Base)
  rkInlineGetSet(Eye,
    [](riku::variant_type const& obj) -> int& {
      return obj["i"].as<int>();
    },
    [](Sib* obj, int const& i) {
      obj->i = i;
    }
  )
)

rkType(Outer,
  rkDefaultFactory
  rkMember(b)
)

#define fail_if(condition) if(condition) {__debugbreak(); return false;}

template<typename T>
bool unittest_ptr(T test, T start = T())
{
  T obj = start;
  const T cobj = start;

  riku::ptr r(obj), cr(cobj), p(&obj), cp(&cobj);
  
  *r.to<T>() = test;
  fail_if(obj != test);

  p.as<T>() = start;
  fail_if(obj != start);

  return cr.data() == NULL && cp.data() == NULL;
}

template<typename T>
bool unittest_val(T v)
{
  riku::val test(riku::get<T>());
  const riku::val ctest(test);

  if (sizeof(T) > RIKU_MAX_VAR_SIZE)
  {
    fail_if(((riku::val const&)test).data() != ctest.data());
  }
  else fail_if(((riku::val const&)test).data() == ctest.data());

  test.as<T>() = v;
  fail_if(test.data() == ctest.data());
  fail_if(ctest.as<T>() == v);

  test.as<T>() = ctest.as<T>();
  fail_if(test.data() == ctest.data());
  fail_if(test.as<T>() != T());

  test = ctest;
  if (sizeof(T) > RIKU_MAX_VAR_SIZE)
  {
    fail_if(((riku::val const&)test).data() != ctest.data());
  }
  else fail_if(((riku::val const&)test).data() == ctest.data());

  return true;
}

template<typename T, typename U>
bool unittest_variant(T tval1, U tval2)
{
  riku::variant obj(tval1),
    pobj(&obj); //a variant HOLDING A MOTHERFUCKING VARIANT

  fail_if(obj.type() != riku::get<T>());
  fail_if(pobj.type() != riku::get<riku::variant>());

  auto& v = pobj.as<riku::variant>();
  fail_if(&v != &obj);

  v = tval2;
  fail_if(obj.as<U>() != tval2);

  return true;
}

bool unittest_varT()
{
  Base b;
  Derived d; d.i = 75;
  Sub sub; sub.i = -81;
  Sib sib; sib.i = 17;

  Derived::Handle h1(b),
    h2(&sub);
  (*h2).i = -99;

  fail_if(h1.data() != NULL);
  fail_if(sub.i != -99);

  Base::Handle bh1;
  bh1 = sib;
  bh1->i = 18;

  fail_if(sib.i == 18);
  fail_if(bh1.as<Derived>().i != 18);

  Base::Handle bh2(bh1);
  Base::Handle bh3((riku::ptr(d)));

  fail_if(bh2.data() == bh1.data());
  fail_if(bh3.data() != &d);

  return true;
}

bool unittest_varops()
{
  Outer o;
  riku::variant v(&o);

  int i = v["b"]["i"].as<int>();
  fail_if(i != 32);

  v["b"]["i"] << -25;
  fail_if(o.b.i != -25);

  riku::variant d((Derived()));
  d["i"].as<int>() = 19;
  Sib s; s.i = -99;
  d >> s;
  fail_if(s.i != 19);

  return true;
}

bool unittest_array()
{
  auto jumble = riku::array::create(1, 2U, std::string("Shpoople"), Outer());
  riku::array copy(jumble);
  riku::array const& cc = copy;

  fail_if(copy[0].type() != riku::get<int>());
  fail_if(copy.get(1).type() != riku::get<unsigned>());
  fail_if(cc[2].type() != riku::get<std::string>());
  fail_if(cc.get(3).type() != riku::get<Outer>());

  Base b; Derived d; Sub s1; Sib s2; Outer o;
  riku::array objs;
  objs.insert(riku::ptr(b), 0);
  objs.insert(riku::ptr(d), 1);
  objs.insert(riku::ptr(s1), 2);
  objs.insert(riku::ptr(s2), 3);
  objs.insert(riku::ptr(o), 4);

  riku::variant blob = objs["i"]; //the array of properties is returned as one variant holding that array
                                  //you can interact with it normally, or extract the internal array
  riku::array eyes = blob.as<riku::array>(); //the array just holds other variants, eyes is a copy of what
                                             //blob is holding but they both still point at the memory inside objs[]

  fail_if(eyes.length() != 4);  //Outer doesn't have "i" so the new array is 1 shorter
  for (auto const& iter : eyes) //enhanced for loops work with array_var
    fail_if(iter.as<int>() != 32);

  eyes.assign(riku::val(19));
  fail_if((b.i != 19) || (d.i != 19) || (s1.i != 19) || (s2.i != 19));

  fail_if(objs["b"]["i"].data() != &o.b.i); //if there's only one element in the array it'll act as a single variant
  fail_if(objs[4]["b"]["i"].data() != &o.b.i); //no need to extract the array from the return value of objs["b"]["i"]

  return true;
}

char testfunc(int& a, char c, std::string s)
{
  return c;
}

bool unittest_func()
{
  riku::function global(testfunc), lambda([](std::string& s) {}),
    basedo(&Base::DoStuff), baseis(&Base::IsStuff),
    baseget(&Base::Get), baseset(&Base::Set);

  fail_if(global.obj_type != NULL);
  fail_if(basedo.obj_type != riku::get<Base>());
  fail_if(lambda.obj_type != NULL);

  fail_if(global.ret_type != riku::get<char>());
  fail_if(!global.arg_list[0].is_ref);
  fail_if(!basedo.is_const);
  fail_if(lambda.arg_count != 1 || lambda.arg_list[0].type != riku::get<std::string>());

  Base b; b.i = 18;
  fail_if(global(riku::variant(), b.i, '{', std::string("For real?")).as<char>() != '{');
  fail_if(basedo(riku::ptr(b), std::string("ya rly")).type() != NULL);
  fail_if(!baseis(riku::ptr(b)).as<bool>());

  fail_if(baseget(riku::ptr(b)).data() != &b.i);
  baseset(riku::ptr(b), 19);
  fail_if(b.i != 19);

  riku::variant obj((riku::val(Base())));
  fail_if(!obj("IsStuff").as<bool>());

  return true;
}

bool unittest_arrayfunc()
{
  Base b; Derived d; Sub s1; Sib s2; Outer o;
  riku::variant objs = riku::array();

  objs.as_array()->push_back(riku::ptr(b));
  objs.as_array()->push_back(riku::ptr(d));
  objs.as_array()->push_back(riku::ptr(s1));
  objs.as_array()->push_back(riku::ptr(s2));
  objs.as_array()->push_back(riku::ptr(o));

  riku::array gets = objs("Get");
  riku::array eyes = (*objs.as_array())["Eye"];

  for (unsigned i = 0; i < gets.length() && i < eyes.length(); ++i)
  {
    fail_if(gets[i].data() != eyes[i].data());
  }

  riku::array func_rets = *
    objs.as_array()->for_each(riku::ptr(riku::function(
      [](Base& b) -> std::string {
        return std::string(1, (char)b.i);
  }))).as_array();

  for (auto const& iter : func_rets)
  {
    fail_if(iter.as<std::string>() != std::string(1, char(32)));
  }

  return true;
}

template<typename T>
bool unittest_ptrmeta()
{
  T* arr = new T[2];

  riku::val ptr(arr);
  fail_if(ptr.as<T*>() != arr);

  riku::val ptr2(ptr);
  fail_if(ptr2.as<T*>() != arr);

  std::cout << ptr2.type()->name() << "\n";

  delete[] arr;
  return true;
}

bool unittest_templatemeta()
{
  fail_if(riku::get<riku::var<riku::variant_type> >()
       != riku::get<riku::var<riku::variant_type> >());

  auto typelisttype = dynamic_cast<riku::template_instance const*>(riku::get<riku::typelist>());
  auto vecvartype = dynamic_cast<riku::template_instance const*>(riku::get< std::vector<riku::variant> >());
  fail_if(typelisttype == NULL);
  fail_if(vecvartype == NULL);
  fail_if(typelisttype->base != vecvartype->base);
  fail_if(typelisttype->params.size() != 2 || vecvartype->params.size() != 2);

  ArgStack<int, bool, riku::typelist> as3;
  ArgStack<std::string, float, long long, Base> as4;
  auto as3type = dynamic_cast<riku::template_instance const*>(riku::inspect(as3));
  auto as4type = dynamic_cast<riku::template_instance const*>(riku::inspect(as4));

  fail_if(as3type == NULL);
  fail_if(as4type == NULL);
  fail_if(as3type->base != as4type->base);
  fail_if(as3type->params.size() != 3 || as4type->params.size() != 4);

  fail_if(as3type->params[0] != riku::get<int>()
       || as3type->params[1] != riku::get<bool>()
       || as3type->params[2] != riku::get<riku::typelist>());

  fail_if(as4type->params[0] != riku::get<std::string>()
       || as4type->params[1] != riku::get<float>()
       || as4type->params[2] != riku::get<long long>()
       || as4type->params[3] != riku::get<Base>());

  auto endval3 = riku::ptr(as3)["next"]["next"]["val"];
  fail_if(endval3.type() != riku::get<riku::typelist>());

  return true;
}

int main(void)
{
#undef fail_if
#define fail_if_not(condition) std::cout << #condition << "... "; if(!(condition)) { \
  std::cout << "FAILED!!\n"; system("pause"); return -1;} else std::cout << "PASSED!!\n\n"

  fail_if_not(unittest_ptr(23));
  fail_if_not(unittest_ptr('J'));
  fail_if_not(unittest_ptr(3.14159f));
  fail_if_not(unittest_ptr<unsigned short>(17));

  fail_if_not(unittest_val(5.3));
  fail_if_not(unittest_val<std::string>("Garbanzo"));

  fail_if_not(unittest_variant(93U, 54.3));
  fail_if_not(unittest_variant(25l, '\\'));
  fail_if_not(unittest_variant(1.618f, std::string("no bamboozle")));

  fail_if_not(unittest_varT());
  fail_if_not(unittest_varops());
  fail_if_not(unittest_array());
  fail_if_not(unittest_func());
  fail_if_not(unittest_arrayfunc());

  fail_if_not(unittest_ptrmeta<std::string>());
  fail_if_not(unittest_ptrmeta<int**>());
  fail_if_not(unittest_ptrmeta<char*******>());

  fail_if_not(unittest_templatemeta());

  system("pause");

  return 0;
}