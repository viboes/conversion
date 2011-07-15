//////////////////////////////////////////////////////////////////////////////
//
// (C) Copyright Vicente J. Botet Escriba 2009-2011. Distributed under the Boost
// Software License, Version 1.0. (See accompanying file
// LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
//
// See http://www.boost.org/libs/conversion for documentation.
//
//////////////////////////////////////////////////////////////////////////////
/**
 * @file
 * @brief Defines the type trait @c is_constructible.
 */


#ifndef BOOST_CONVERSION_TT_IS_CONSTRUCTIBLE_HPP
#define BOOST_CONVERSION_TT_IS_CONSTRUCTIBLE_HPP

#if defined(BOOST_CONVERSION_DOXYGEN_INVOKED)
namespace boost {

  /**
   * States if @c T is constructible from @c Args.
   *
   * Condition: @c true_type if and only if the following variable definition would be well-formed for
   * some invented variable t:
   *
   *   T t(decval<Args>()...);
   *
   * @Requires @c T and all types in the parameter pack @c Args must be complete types, (possibly cv-qualified) void, or arrays of unknown bound.
   */
  template < class T, class... Args>
  struct is_constructible
  {};

  //! Macro stating if the compiler doesn't support the features needed to define the @c is_constructible type trait.
  #define BOOST_CONVERSION_NO_IS_CONSTRUCTIBLE
  //! Macro stating if the compiler doesn't support the features needed to define the @c is_default_constructible type trait.
  #define BOOST_CONVERSION_NO_IS_DEFAULT_CONSTRUCTIBLE
  //! Max number of arguments to is_constructible when using variadic templates emulation.
  #define BOOST_CONVERSION_TT_IS_CONSTRUCTIBLE_ARITY_MAX

}
#else

#include <boost/config.hpp>
#include <boost/preprocessor/repetition/enum_params.hpp>
#include <boost/preprocessor/repetition/enum_binary_params.hpp>
#include <boost/preprocessor/repetition/repeat.hpp>
#include <boost/preprocessor/repetition/enum.hpp>
#include <boost/preprocessor/punctuation/comma_if.hpp>
#include <boost/preprocessor/facilities/intercept.hpp>
#include <boost/preprocessor/arithmetic/inc.hpp>
#include <boost/preprocessor/cat.hpp>
#include <boost/type_traits/integral_constant.hpp>
#include <boost/utility/declval.hpp>
#include <cstddef>

#include <boost/type_traits/integral_constant.hpp>
#include <boost/type_traits/common_type.hpp>
#include <boost/type_traits/is_scalar.hpp>
#include <boost/type_traits/is_reference.hpp>
#include <boost/type_traits/is_abstract.hpp>
#include <boost/type_traits/is_function.hpp>
#include <boost/type_traits/is_void.hpp>
#include <boost/type_traits/is_convertible.hpp>
#include <boost/type_traits/remove_all_extents.hpp>


#ifndef BOOST_CONVERSION_TT_IS_CONSTRUCTIBLE_ARITY_MAX
#define BOOST_CONVERSION_TT_IS_CONSTRUCTIBLE_ARITY_MAX 3
#define BOOST_CONVERSION_TT_IS_CONSTRUCTIBLE_ARITY_MAXP1 4
#define BOOST_CONVERSION_TT_IS_CONSTRUCTIBLE_ARITY_MAXM2 1
#endif

#if ! defined BOOST_NO_DECLTYPE
  #if defined _MSC_VER
    #if ! defined BOOST_NO_SFINAE_EXPR
      #define BOOST_CONVERSION_IS_CONSTRUCTIBLE_USES_SIZEOF
    #else
      #define BOOST_CONVERSION_NO_IS_CONSTRUCTIBLE
      #define BOOST_CONVERSION_NO_IS_DEFAULT_CONSTRUCTIBLE
    #endif
  #elif defined __clang__
    #define BOOST_CONVERSION_IS_CONSTRUCTIBLE_USES_DECLTYPE
  #elif defined __GNUC__
     #if __GNUC__ < 4 || ( __GNUC__ == 4 && __GNUC_MINOR__ < 4 )
       #if ! defined BOOST_NO_SFINAE_EXPR
         #define BOOST_CONVERSION_IS_CONSTRUCTIBLE_USES_SIZEOF
       #else
         #define BOOST_CONVERSION_NO_IS_CONSTRUCTIBLE
         #define BOOST_CONVERSION_NO_IS_DEFAULT_CONSTRUCTIBLE
       #endif
     #else
       #define BOOST_CONVERSION_IS_CONSTRUCTIBLE_USES_DECLTYPE
     #endif
  #else
       #define BOOST_CONVERSION_IS_CONSTRUCTIBLE_USES_DECLTYPE
  #endif
#elif ! defined BOOST_NO_SFINAE_EXPR
  #define BOOST_CONVERSION_IS_CONSTRUCTIBLE_USES_SIZEOF
  #if defined __clang__
  #elif defined __GNUC__
     #if __GNUC__ < 4 || ( __GNUC__ == 4 && __GNUC_MINOR__ < 7 )
       #define BOOST_CONVERSION_NO_IS_DEFAULT_CONSTRUCTIBLE
     #endif
  #endif
#else
  #define BOOST_CONVERSION_NO_IS_CONSTRUCTIBLE
  #define BOOST_CONVERSION_NO_IS_DEFAULT_CONSTRUCTIBLE
#endif

namespace boost {
  namespace type_traits_detail_is_constructible {
        //! type used instead of ... to accept any type
        struct any {
          template <typename T>
          any(T);
        };

        //! not a type. used as default of variadic templates
        struct nat {};

        //! type useful to compare with the sizeof
        typedef char yes_type;
        //! type useful to compare with the sizeof
        struct no_type { char a[2]; };

        //! type useful to accept a sizeof as parameter
        template<std::size_t N>
        struct dummy;
  }

#if defined BOOST_CONVERSION_IS_CONSTRUCTIBLE_USES_DECLTYPE

  namespace type_traits_detail_is_constructible {

    template <class T>
    decltype((T(), true_type()))
    test0(T&);
    false_type
    test0(any);



#define M1(z,n,t) declval<A##n>()

#define M0(z,n,t)                                                           \
  template <class T, class A BOOST_PP_COMMA_IF(n) BOOST_PP_ENUM_PARAMS(n, class A)>  \
  decltype((T(declval<A>() BOOST_PP_COMMA_IF(n) BOOST_PP_ENUM(n, M1, ~)), true_type()))                       \
  BOOST_PP_CAT(test,BOOST_PP_INC(n))(T&, A BOOST_PP_COMMA_IF(n) BOOST_PP_ENUM_PARAMS(n, A));

BOOST_PP_REPEAT(BOOST_CONVERSION_TT_IS_CONSTRUCTIBLE_ARITY_MAX, M0, ~)
#undef M0
#undef M1

#define M0(z,n,t)                                                           \
  template <class A BOOST_PP_COMMA_IF(n) BOOST_PP_ENUM_PARAMS(n, class A)>                               \
  false_type                                                                \
  BOOST_PP_CAT(test,BOOST_PP_INC(n))(any, A BOOST_PP_COMMA_IF(n) BOOST_PP_ENUM_PARAMS(n, A));

BOOST_PP_REPEAT(BOOST_CONVERSION_TT_IS_CONSTRUCTIBLE_ARITY_MAX, M0, ~)
#undef M0

#define M1(z,n,t) declval<A##n>()
#define M0(z,n,t)                                                                     \
    template <bool, class T BOOST_PP_COMMA_IF(n) BOOST_PP_ENUM_PARAMS(n, class A)>    \
    struct imp##n : public common_type                                                \
                 <                                                                    \
        decltype(test##n(declval<T&>() BOOST_PP_COMMA_IF(n) BOOST_PP_ENUM(n, M1, ~))) \
                 >::type                                                              \
                 {};

BOOST_PP_REPEAT(BOOST_CONVERSION_TT_IS_CONSTRUCTIBLE_ARITY_MAXP1, M0, ~)
#undef M0
#undef M1


      template <class T>
      struct imp0<true, T>
          : public is_scalar<T>
          {};

      template <class T, class A1>
      struct imp1<true, T, A1>
          : public is_convertible<A1, T>
          {};

#define M0(z,n,t)                                                                         \
template <class T, class X, class Y BOOST_PP_COMMA_IF(n) BOOST_PP_ENUM_PARAMS(n, class A)>\
struct imp2<true, T, X, Y BOOST_PP_COMMA_IF(n) BOOST_PP_ENUM_PARAMS(n, A) >               \
  : public false_type                                                                     \
  {};

BOOST_PP_REPEAT(BOOST_CONVERSION_TT_IS_CONSTRUCTIBLE_ARITY_MAXM2, M0, ~)
#undef M0

#define M0(z,n,t)                                                                   \
  template <bool, class T BOOST_PP_COMMA_IF(n) BOOST_PP_ENUM_PARAMS(n, class A)>    \
  struct void_check##n                                                              \
    : public imp##n<is_scalar<T>::value || is_reference<T>::value,                  \
                              T BOOST_PP_COMMA_IF(n) BOOST_PP_ENUM_PARAMS(n, A)>    \
    {};

BOOST_PP_REPEAT(BOOST_CONVERSION_TT_IS_CONSTRUCTIBLE_ARITY_MAXP1, M0, ~)
#undef M0

#define M0(z,n,t)                                                               \
  template <class T BOOST_PP_COMMA_IF(n) BOOST_PP_ENUM_PARAMS(n, class A)>      \
  struct void_check##n<true, T BOOST_PP_COMMA_IF(n) BOOST_PP_ENUM_PARAMS(n, A)> \
    : public false_type    \
  {};

BOOST_PP_REPEAT(BOOST_CONVERSION_TT_IS_CONSTRUCTIBLE_ARITY_MAXP1, M0, ~)
#undef M0

#define M0(z,n,t)                                                               \
  template <class A BOOST_PP_COMMA_IF(n) BOOST_PP_ENUM_PARAMS(n, class A)>      \
  struct imp##n<false, A[] BOOST_PP_COMMA_IF(n) BOOST_PP_ENUM_PARAMS(n, A)>     \
    : public false_type                                                         \
  {};

BOOST_PP_REPEAT(BOOST_CONVERSION_TT_IS_CONSTRUCTIBLE_ARITY_MAXP1, M0, ~)
#undef M0

  }

  template <class T,  BOOST_PP_ENUM_BINARY_PARAMS(BOOST_CONVERSION_TT_IS_CONSTRUCTIBLE_ARITY_MAXP1, class A, = type_traits_detail_is_constructible::nat BOOST_PP_INTERCEPT)>      \
  struct is_constructible;

#define M1(z,n,t) || is_void<A##n>::value

#define M0(z,n,t)                                                                 \
  template <class T BOOST_PP_COMMA_IF(n) BOOST_PP_ENUM_PARAMS(n, class A)>        \
  struct is_constructible<T BOOST_PP_COMMA_IF(n) BOOST_PP_ENUM_PARAMS(n, A)>     \
    : public type_traits_detail_is_constructible::void_check##n<is_void<T>::value \
              || is_abstract<T>::value  || is_function<T>::value                  \
              BOOST_PP_REPEAT(n, M1, ~),                                          \
              T BOOST_PP_COMMA_IF(n) BOOST_PP_ENUM_PARAMS(n, A)>                  \
  {};

BOOST_PP_REPEAT(BOOST_CONVERSION_TT_IS_CONSTRUCTIBLE_ARITY_MAXP1, M0, ~)
#undef M0


  namespace type_traits_detail_is_constructible {
    template <class A, std::size_t N>
    struct imp0<false, A[N]>
        : public boost::is_constructible<typename remove_all_extents<A>::type>
        {};

#define M0(z,n,t)                                                                                           \
  template <class A, std::size_t N, class X BOOST_PP_COMMA_IF(n) BOOST_PP_ENUM_PARAMS(n, class A)>          \
  struct BOOST_PP_CAT(imp,BOOST_PP_INC(n))<false, A[N], X BOOST_PP_COMMA_IF(n) BOOST_PP_ENUM_PARAMS(n, A)>  \
    : public false_type                                                                                     \
      {};

BOOST_PP_REPEAT(BOOST_CONVERSION_TT_IS_CONSTRUCTIBLE_ARITY_MAX, M0, ~)
#undef M0


  }


#elif defined BOOST_CONVERSION_IS_CONSTRUCTIBLE_USES_SIZEOF

  template<class T, BOOST_PP_ENUM_BINARY_PARAMS(BOOST_CONVERSION_TT_IS_CONSTRUCTIBLE_ARITY_MAXP1, class A, = type_traits_detail_is_constructible::nat BOOST_PP_INTERCEPT)>
  struct is_constructible;

#define M1(z,n,t) declval<A##n>()

#define M0(z,n,t)                                                                                   \
template<class T, class A BOOST_PP_COMMA_IF(n) BOOST_PP_ENUM_PARAMS(n, class A)>                    \
struct is_constructible<T,A BOOST_PP_COMMA_IF(n) BOOST_PP_ENUM_PARAMS(n, A)>                        \
{                                                                                                   \
    template<class X>                                                                               \
    static type_traits_detail_is_constructible::yes_type                                                            \
    test(type_traits_detail_is_constructible::dummy<sizeof(X(declval<A>() BOOST_PP_COMMA_IF(n) BOOST_PP_ENUM(n, M1, ~)))>*);   \
                                                                                                    \
    template<class X>                                                                               \
    static type_traits_detail_is_constructible::no_type                                             \
    test(...);                                                                                      \
                                                                                                    \
    static const bool value = sizeof(test<T>(0)) == sizeof(type_traits_detail_is_constructible::yes_type);          \
    typedef boost::integral_constant<bool,value> type;                                              \
};

BOOST_PP_REPEAT(BOOST_CONVERSION_TT_IS_CONSTRUCTIBLE_ARITY_MAX, M0, ~)
#undef M0
#undef M1

#if ! defined BOOST_CONVERSION_NO_IS_DEFAULT_CONSTRUCTIBLE
#define M1(z,n,t) type_traits_detail_is_constructible::nat

    template<class T>
    struct is_constructible<T>
    {
        template<class X>
        static type_traits_detail_is_constructible::yes_type
        test(type_traits_detail_is_constructible::dummy<sizeof(X(),int())>*);

        template<class X>
        static type_traits_detail_is_constructible::no_type
        test(...);

        static const bool value = sizeof(test<T>(0)) == sizeof(type_traits_detail_is_constructible::yes_type);
        typedef boost::integral_constant<bool,value> type;
    };

    template<>
    struct is_constructible<void, BOOST_PP_ENUM(BOOST_CONVERSION_TT_IS_CONSTRUCTIBLE_ARITY_MAXP1, M1, ~)>
    : boost::false_type
    {
    };

#undef M1

#endif


#define M0(z,n,t)                                                                                   \
template<class A BOOST_PP_COMMA_IF(n) BOOST_PP_ENUM_PARAMS(n, class A)>                             \
struct is_constructible<void, A BOOST_PP_COMMA_IF(n) BOOST_PP_ENUM_PARAMS(n, A)>                          \
  : boost::false_type                                                                               \
{                                                                                                   \
};

BOOST_PP_REPEAT(BOOST_CONVERSION_TT_IS_CONSTRUCTIBLE_ARITY_MAXP1, M0, ~)
#undef M0


#else

    template<class T, BOOST_PP_ENUM_BINARY_PARAMS(BOOST_CONVERSION_TT_IS_CONSTRUCTIBLE_ARITY_MAXP1, class A, = void BOOST_PP_INTERCEPT)>
    struct is_constructible;

#define M0(z,n,t)                                                                                   \
template<class T, class A BOOST_PP_COMMA_IF(n) BOOST_PP_ENUM_PARAMS(n, class A)>                             \
struct is_constructible<T, A BOOST_PP_COMMA_IF(n) BOOST_PP_ENUM_PARAMS(n, A)>                          \
  : boost::false_type                                                                               \
{                                                                                                   \
};

BOOST_PP_REPEAT(BOOST_CONVERSION_TT_IS_CONSTRUCTIBLE_ARITY_MAX, M0, ~)
#undef M0

#endif

#ifdef  BOOST_CONVERSION_NO_IS_DEFAULT_CONSTRUCTIBLE
    // default constructor
    template <> struct is_constructible< int  >  : true_type {};
#endif
#ifdef  BOOST_CONVERSION_NO_IS_CONSTRUCTIBLE
    template <class T> struct is_constructible< T*, T* const &  >  : true_type {};
    template <> struct is_constructible< int, const int  >  : true_type {};
    template <> struct is_constructible< int, int const& >  : true_type {};
    template <> struct is_constructible< double, const double  >  : true_type {};
    template <> struct is_constructible< double, double const& >  : true_type {};
#endif

}

#endif
#endif

