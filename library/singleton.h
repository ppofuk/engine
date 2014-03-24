#ifndef OBSIDIAN_SINGLETON_H_
#define OBSIDIAN_SINGLETON_H_

// Creates a singleton of a type.
template<class type>
class Singleton {
 public:
   static type& Instance() {
     static type instance;
     return instance;
   }

 private:
  Singleton() {
  };

  Singleton(Singleton const&);
  void operator =(Singleton const&);
};

#endif // #define SIGNLETON_H_
