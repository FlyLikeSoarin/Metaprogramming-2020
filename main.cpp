#include <iostream>

struct A {};
struct B {};
struct C {};
struct D {};

struct Nulltype {
  using Head = Nulltype;
  using Tail = Nulltype;
};

template<typename H, typename ...T>
struct TypeList {
  using Head = H;
  using Tail = TypeList<T...>;
};

template<typename H, typename ...T>
struct TypeList<H, TypeList<T...>> {
  using Head = H;
  using Tail = TypeList<T...>;
};

template<typename ...T>
struct TypeList<TypeList<T...>> {
  using Head = typename TypeList<T...>::Head;
  using Tail = typename TypeList<T...>::Tail;
};

template<typename H>
struct TypeList<H> {
  using Head = H;
  using Tail = Nulltype;
};

template<typename H>
struct TypeList<H, Nulltype> {
  using Head = H;
  using Tail = Nulltype;
};

//...................................................................//


template<typename T>
struct Length {
  static const int value = 1 + Length<typename T::Tail>::value;
};

template<>
struct Length<Nulltype> {
  static const int value = 0;
};

//...................................................................//

template<size_t ind, typename ...T>
struct TypeAt {
  using res = Nulltype;
};

template<size_t ind, typename ...T>
struct TypeAt<ind, TypeList<T...>> {
  using res = typename TypeAt<ind - 1, typename TypeList<T...>::Tail>::res;
};

template<size_t ind>
struct TypeAt<ind, Nulltype> {
  using res = Nulltype;
};

template<typename ...T>
struct TypeAt<0, TypeList<T...>> {
  using res = typename TypeList<T...>::Head;
};

//...................................................................//

template<int n, typename ...T>
struct FrontN {};

template<int n, typename ...T>
struct FrontN<n, TypeList<T...>> {
  using res = TypeList<typename TypeList<T...>::Head, typename FrontN<n - 1, typename TypeList<T...>::Tail>::res>;
};

template<typename ...T>
struct FrontN<0, TypeList<T...>> {
  using res = Nulltype;
};

template<int n>
struct FrontN<n, Nulltype> {
  using res = Nulltype;
};

template<>
struct FrontN<0, Nulltype> {
  using res = Nulltype;
};

//...................................................................//

template<int n, typename ...T>
struct PopFrontN {};

template<int n, typename ...T>
struct PopFrontN<n, TypeList<T...>> {
  using res = typename PopFrontN<n - 1, typename TypeList<T...>::Tail>::res;
};

template<typename ...T>
struct PopFrontN<0, TypeList<T...>> {
  using res = TypeList<T...>;
};

template<int n>
struct PopFrontN<n, Nulltype> {
  using res = Nulltype;
};

template<>
struct PopFrontN<0, Nulltype> {
  using res = Nulltype;
};

//...................................................................//

template <typename T>
struct BaseUnit {
  T obj;
};



template <typename T, typename NextGen>
 struct LeftUnit: public NextGen, BaseUnit<T> {};

template <typename T, typename NextGen>
struct CenterUnit: public NextGen, BaseUnit<T> {};

template <typename T, typename NextGen>
struct RightUnit: public NextGen, BaseUnit<T> {};

template <typename T, typename NextGen>
struct LinearUnit: public NextGen, BaseUnit<T> {};

//...................................................................//

template <class Types, template <class, class> class Unit1, template <class, class> class Unit2, template <class, class> class Unit3, template <class, class> class Unit4, int depth, int offset>
struct GenCustomHierarchy;

template <class Types, template <class, class> class Unit1, template <class, class> class Unit2, template <class, class> class Unit3, template <class, class> class Unit4, int depth, int offset>
struct GenLinearHierarchy;

//...................................................................//

template <class ...Types, template <class, class> class Unit1, template <class, class> class Unit2, template <class, class> class Unit3, template <class, class> class Unit4, int depth, int offset>
struct GenLinearHierarchy<TypeList<Types...>, Unit1, Unit2, Unit3, Unit4, depth, offset>:
    public Unit1<typename PopFrontN<offset, TypeList<Types...>>::res::Head, GenCustomHierarchy<typename PopFrontN<offset + depth * 3, TypeList<Types...>>::res, Unit1, Unit2, Unit3, Unit4, depth * 3, offset * 3>> {
};

template <typename T, template <class, class> class Unit1, template <class, class> class Unit2, template <class, class> class Unit3, template <class, class> class Unit4, int depth, int offset>
struct GenLinearHierarchy<TypeList<T>, Unit1, Unit2, Unit3, Unit4, depth, offset>:
    public LinearUnit<T, Nulltype> {
};

template <template <class, class> class Unit1, template <class, class> class Unit2, template <class, class> class Unit3, template <class, class> class Unit4, int depth, int offset>
struct GenLinearHierarchy<Nulltype, Unit1, Unit2, Unit3, Unit4, depth, offset>:
    public Nulltype {
};

//...................................................................//

template <class ...Types, template <class, class> class Unit1, template <class, class> class Unit2, template <class, class> class Unit3, template <class, class> class Unit4, int depth, int offset>
  struct GenCustomHierarchy<TypeList<Types...>, Unit1, Unit2, Unit3, Unit4, depth, offset>:
      public Unit2<
          typename TypeList<Types...>::Head,
          GenLinearHierarchy<
            typename PopFrontN<depth, TypeList<Types...>>::res
            , Unit1, Unit2, Unit3, Unit4, depth, 0 + offset>>,
      public Unit3<
          typename TypeList<Types...>::Tail::Head,
          GenLinearHierarchy<
              typename PopFrontN<depth, TypeList<Types...>>::res
              , Unit1, Unit2, Unit3, Unit4, depth, 1 + offset>>,
      public Unit4<
          typename TypeList<Types...>::Tail::Tail::Head,
          GenLinearHierarchy<
              typename PopFrontN<depth, TypeList<Types...>>::res
              , Unit1, Unit2, Unit3, Unit4, depth, 2 + offset>> {

};

template <template <class, class> class Unit1, template <class, class> class Unit2, template <class, class> class Unit3, template <class, class> class Unit4, int depth, int offset>
struct GenCustomHierarchy<Nulltype, Unit1, Unit2, Unit3, Unit4, depth, offset>:
    public Nulltype {};

template <typename ...Types>
struct GenerateCustomHierarchy: GenCustomHierarchy<TypeList<Nulltype, TypeList<Types...>>, LinearUnit, LeftUnit, CenterUnit, RightUnit, 1, 0> {};


int main() {;
//  GenCustomHierarchy<TypeList<Nulltype, int, float, double, char, A, B, C, D>, LinearUnit, LeftUnit, CenterUnit, RightUnit, 1, 0> A;

  GenerateCustomHierarchy<TypeList<int, float, double, char, A, B, C, D>> Result;
}