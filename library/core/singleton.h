#ifndef OBSIDIAN_SINGLETON_H_
#define OBSIDIAN_SINGLETON_H_

// Creates a singleton of a Type.
  template<class Type>
  class Singleton {
   public:
     static Type& Instance() {
       static Type instance;
       return instance;
     }

   private:
    Singleton() {
    };

    Singleton(Singleton const&);
    void operator =(Singleton const&);
  };

#endif // #define SIGNLETON_H_
