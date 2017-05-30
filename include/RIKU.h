/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *

        RIKU (A C++ Reflection Library) COPYRIGHT 2017 JOSEPH W DILLEY         

Redistribution and use in source and binary forms, with or without modification,
are permitted provided that the following conditions are met:

1. Redistributions of source code must retain the above copyright notice, this
list of conditions and the following disclaimer.

2. Redistributions in binary form must reproduce the above copyright notice,
this list of conditions and the following disclaimer in the documentation and/or
other materials provided with the distribution.

3. Neither the name of the copyright holder nor the names of its contributors
may be used to endorse or promote products derived from this software without
specific prior written permission.

THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" AND
ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED
WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE LIABLE FOR
ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES
(INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES;
LOSS OF USE, DATA OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON
ANY THEORY OF LIABILITY, WHETHER IN ANY CONTRACT, STRICT LIABILITY, OR TORT
(INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS
SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.

* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
#include <string>
#include <vector>
#include <unordered_map>

namespace riku
{
  class type;
  struct pointer_type;
  struct primitive_type;
  struct generic_type;
  struct template_type;
  struct template_instance;
  typedef type const* typeinfo; //!< This is how type information is passed around 99% of the time, as the address of the static type block defining it.

  template<typename T> typeinfo inspect(T const& obj);  //!< Use this to get the type information on an object.
  template<typename T> typeinfo get();                  //!< Use this to get the type information for T.
                       typeinfo find(char const* name); //!< Use this to look up a type by name.

  class property;
  struct member;
  struct getset;

  struct function;

  class variant_type;
  class array_var;
  class map_var;
  struct val;
  struct ptr;
  struct variant;
  struct array;
  template<typename T> struct var;

  typedef unsigned char byte;
  typedef std::string string;   //!< Temporary measure, once compile-time string hashing is working RIKU will need a string refactor
  typedef std::vector<typeinfo> typelist;

  namespace internal { template<typename T> class registrar; }
  class nulltype {};
}

#include "riku\config.h"
#include "riku\macros.h"
#include "riku\metaprog.hpp"

#include "riku\variant.h"
#include "riku\factory.h"
#include "riku\type.h"
#include "riku\property.h"
#include "riku\array.h"
#include "riku\function.h"
#include "riku\library.h"

#include "riku\registrar.hpp"
#include "riku\variant_inl.hpp"
#include "riku\factory_inl.hpp"
#include "riku\array_inl.hpp"
#include "riku\function_inl.hpp"

//! @example test/main.cpp
