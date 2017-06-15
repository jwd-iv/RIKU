README
=========
A C++14 reflection markup library, originally developed for use in small-scale custom
game engines to help automate tasks like memory management, script binding, serialization,
and communication.


Example
=======
### Documenting a type

    struct Object {
    	std::string name;
    	Object *children;
    	unsigned child_count;
    
    	Object& GetChild(int idx);
    }

### Somewhere in a .cpp file...

    rkType(Object, rkDefaultFactory
        rkMember(name)
    	rkMember(children)
    	rkMember(child_count)
    
    	rkMethod(GetChild)
    
    	rkInlineGetSet(
    		ObjName,
    		[](Object& obj) -> std::string& {
    			return obj.name;
    		},
    		[](riku::variant obj, std::string& str) {
    			obj["name"].as<std::string>() = str;
    		}
    	)
    )

### Using riku::variants

    Object obj;
    
    // Three ways to get type information, all return the same typeinfo.
    riku::typeinfo obj_type = riku::inspect(obj),
                   by_type  = riku::get<Object>(),
                   by_name  = riku::get("Object");

    riku::variant allocated = riku::val(obj_type); // constructs a new default object if possible
    riku::variant null = riku::ptr(obj_type);      // acts like a null pointer but won't dereference if you mess with it
    
    // variants can hold any documented type, either by copying the value or as a pointer
    riku::variant as_val(obj);  // copies obj
    riku::variant as_ptr(&obj); // acts the same as as_val but will modify obj in-place.
    
    // They can also be reassigned after the fact.
    as_val = riku::ptr(obj); // Now it's the same as as_ptr and the memory it was holding is safely deleted.
    as_val = riku::val(obj); // And it's a value again, with another independent copy.
    
    // variants are primarily used to access properties and functions in a generic fashion
    as_ptr["children"] << new Object[5];
    as_ptr["child_count"].as<unsigned>() = 5;
    
    as_ptr("GetChild", 3); // The same as obj.GetChild(3);
    
    // Custom properties look just like any other
    as_val["ObjName"] << std::string("Test");

Other examples of how to use riku::function, riku::array as well as the above can
be found in `/test/main.cpp`, included with RIKU source code.


Building RIKU
=============
short version: Build it with CMake.


Using RIKU in Your Project
===========================
short version: either link it statically or connect it to your CMake project with ExternalProject_Add().


Testing RIKU
============
short version: run the riku_test target of the CMake build, which is the default if RIKU_TEST is left on.


To-do List
==================
* Replace `riku::variant` with `riku::var<T>` and add `typedef var<variant_type> variant`
* Remove templated variant constructors, replace with `::create<T>` and etc.
* Transition registrar and rkLibrary() to using functor_constructors
* Compile-time type and property name string hashing for faster lookup
* More robust+readable unit tests
* Much more comprehensive documentation
* Type conversion (especially for numeric types)
* riku::command for runtime function argument binding and delegated function calls
* `#rkInlineGet()`
* Cross-platform testing
* enum reflection
* riku::factory move functions and full variant r-value forwarding for memory efficiency
* custom factory macros - `rkConstructor(...)`, `rkMoveConstructor(...)`, `rkCustomAssign(func)`, etc.
* Shared library implementation
* riku::array_property and/or riku::array_type
* riku::map_var, riku::map_property, and/or riku::map_type
* complete reflection of reference and pointer types, both const and non-const
