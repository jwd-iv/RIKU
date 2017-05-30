namespace riku
{
  namespace default_mem
  {
    template<typename T> struct heap_default_new {
      static void* run(void* obj, void* arg) {
        try {
          return new T();
        } catch (std::exception const&) { return NULL; }
      }
    };

    template<typename T> struct heap_copy {
      static void* run(void* obj, void* arg) {
        try {
          return new T(*reinterpret_cast<T*>(obj));
        } catch (std::exception const&) { return NULL; }
      }
    };

    template<typename T> struct heap_delete {
      static void* run(void* obj, void* arg) {
        try {
          delete reinterpret_cast<T*>(obj);
        } catch (std::exception const&) { return obj; }
        return NULL;
      }
    };

    /*template<typename T, typename... args> struct heap_new {
      static void* run(void* obj, void* arg) {
        return NULL;
      }
    };*/

    template<typename T> struct placement_default_new {
      static void* run(void* obj, void* arg) {
        try {
          return new (obj) T();
        } catch (std::exception const&) { return NULL; }
      }
    };

    template<typename T> struct placement_copy {
      static void* run(void* obj, void* arg) {
        try {
          return new (obj) T(*reinterpret_cast<T*>(arg));
        } catch (std::exception const&) { return NULL; }
      }
    };

    template<typename T> struct placement_delete {
      static void* run(void* obj, void* arg) {
        try {
          reinterpret_cast<T*>(obj)->~T();
        } catch (std::exception const&) { return obj; }
        return NULL;
      }
    };

    /*template<typename T, typename... args> struct placement_new {
      static void* run(void* obj, void* arg) {
        return NULL;
      }
    };*/

    template<typename T> struct assign {
      static void* run(void* obj, void* arg) {
        try {
          *reinterpret_cast<T*>(obj) = *reinterpret_cast<T*>(arg);
          return obj;
        } catch (std::exception const&) { return NULL; }
      }
    };
  }
}
