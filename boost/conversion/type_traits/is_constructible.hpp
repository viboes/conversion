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
   * @Condition: @c true_type if and only if the following variable definition would be well-formed for
   * some invented variable t:
   *
   * @code
   *   T t(decval<Args>()...);
   * @endcode
   *
   * @Requires @c T and all types in the parameter pack @c Args must be complete types, (possibly cv-qualified) void, or arrays of unknown bound.
   *
   * @Remark
   *   - On compilers providing an intrinsic for this trait, the intrinsic will be used.
   *   - On C++0x mode, @c std::is_constructible will be used when available.
   *   - On compilers supporting SFINAE for expressions the library provided a valid implementation.
   *   - Otherwise,
   *     - the library will provide specialization for the builtin types in this file,
   *     - the library will provide specialization for specific standard types in the associated @c type_traits/std/file.hpp
   *     - the library will provide specialization for specific boost types in the associated @c type_traits/boost/file.hpp
   *     - the user will need to provide other specific specializations.
   *
   */
  template < typename T, typename... Args>
  struct is_constructible
  {};

  //! Macro defined if and only if the compiler doesn't support the features needed to define the @c is_constructible type trait.
  #define BOOST_CONVERSION_NO_IS_CONSTRUCTIBLE
  //! Macro stating if the compiler doesn't support the features needed to define the @c is_default_constructible type trait.
  #define BOOST_CONVERSION_NO_IS_DEFAULT_CONSTRUCTIBLE
  //! Max number of arguments to is_constructible when using variadic templates emulation.
  #define BOOST_CONVERSION_TT_IS_CONSTRUCTIBLE_ARITY_MAX

}
#else

#include <boost/config.hpp>
#include <boost/conversion/type_traits/detail/nat.hpp>
#include <boost/conversion/type_traits/detail/any.hpp>
#include <boost/conversion/type_traits/detail/yes_no_types.hpp>
#include <boost/conversion/type_traits/detail/dummy_size.hpp>
#include <boost/preprocessor/repetition/enum_params.hpp>
#include <boost/preprocessor/repetition/enum_binary_params.hpp>
#include <boost/preprocessor/repetition/repeat.hpp>
#include <boost/preprocessor/repetition/repeat_from_to.hpp>
#include <boost/preprocessor/repetition/enum.hpp>
#include <boost/preprocessor/punctuation/comma_if.hpp>
#include <boost/preprocessor/facilities/intercept.hpp>
#include <boost/preprocessor/arithmetic/inc.hpp>
#include <boost/preprocessor/arithmetic/sub.hpp>
#include <boost/preprocessor/cat.hpp>
#include <boost/utility/declval.hpp>
#include <cstddef>

#include <boost/type_traits/integral_constant.hpp>
#include <boost/type_traits/common_type.hpp>
#include <boost/type_traits/is_scalar.hpp>
#include <boost/type_traits/is_reference.hpp>
#include <boost/type_traits/is_abstract.hpp>
#include <boost/type_traits/is_function.hpp>
#include <boost/type_traits/is_void.hpp>
#include <boost/conversion/type_traits/is_convertible.hpp>
#include <boost/type_traits/remove_all_extents.hpp>

#if ! defined BOOST_NO_DECLTYPE
  #if defined _MSC_VER
      #define BOOST_CONVERSION_NO_IS_CONSTRUCTIBLE
      #define BOOST_CONVERSION_NO_IS_DEFAULT_CONSTRUCTIBLE
  #elif defined __clang__
    #define BOOST_CONVERSION_IS_CONSTRUCTIBLE_USES_DECLTYPE
  #elif defined __GNUC__
     #if __GNUC__ < 4 || ( __GNUC__ == 4 && __GNUC_MINOR__ < 4 )
       #if ! defined BOOST_NO_SFINAE_EXPR
#error
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


#ifndef BOOST_NO_VARIADIC_TEMPLATES
namespace boost {
  namespace type_traits {
    namespace detail {
      namespace is_constructible {

        //      main is_constructible test
#if defined BOOST_CONVERSION_IS_CONSTRUCTIBLE_USES_DECLTYPE
        #ifndef BOOST_NO_RVALUE_REFERENCES
        template <typename T, typename ...Args>
        //decltype(move(T(declval<Args>()...)), true_type())
        decltype(T(declval<Args>()...), true_type())
        test(T&&, Args&& ...);
        template <typename ...Args>
        false_type
        test(any, Args&& ...);
        #else
        template <typename T, typename ...Args>
        decltype(T(declval<Args>()...), true_type())
        test(T&, Args& ...);
        template <typename ...Args>
        false_type
        test(any, Args& ...);
        #endif

        template <bool, typename T, typename... Args>
        struct imp // false, T is not a scalar
            : public common_type
                     <
                         decltype(test(declval<T>(), declval<Args>()...))
                     >::type
        {};
#elif defined BOOST_CONVERSION_IS_CONSTRUCTIBLE_USES_SIZEOF

        template <bool, typename T, typename... Args>
        struct imp // false, T is not a scalar
        {
          template<typename X>
          static yes_type
          test(dummy_size<sizeof(X(declval<Args>()...))>*);

          template<typename X>
          static no_type
          test(...);

          static const bool value =
                    sizeof(test<T>(0)) == sizeof(yes_type);
          typedef boost::integral_constant<bool,value> type;
        };


#else
        template <bool, typename T, typename... Args>
        struct imp // false, T is not a scalar
               : public false_type
           {};
#endif

        //      function types are not constructible

        template <typename R, typename... A1, typename... A2>
        struct imp<false, R(A1...), A2...>
           : public false_type
           {};

        //      handle scalars and reference types

        //      Scalars are default constructible, references are not

        template <typename T>
        struct imp<true, T>
           : public is_scalar<T>
           {};

        //      Scalars and references are constructible from one arg if that arg is
        //          implicitly convertible to the scalar or reference.

        template <typename T, typename A0>
        struct imp<true, T, A0> : is_convertible<A0,T>
           {};

        //      Scalars and references are not constructible from multiple args.

        template <typename T, typename A0, typename ...Args>
        struct imp<true, T, A0, Args...>
            : public false_type
           {};

        //      Treat scalars and reference types separately

        template <bool, typename T, typename... Args>
        struct void_check
           : public imp<is_scalar<T>::value || is_reference<T>::value,
                                       T, Args...>
           {};

        //      If any of T or Args is void, is_constructible should be false

        template <typename T, typename... Args>
        struct void_check<true, T, Args...>
           : public false_type
           {};

        template <typename ...Args> struct contains_void;

        template <> struct contains_void<> : false_type {};

        template <typename A0, typename ...Args>
        struct contains_void<A0, Args...>
        {
           static const bool value = is_void<A0>::value ||
                                     contains_void<Args...>::value;
        };

      } // is_constructible
    } // detail
  } // type_traits

  //      is_constructible entry point

  template <typename T, typename... Args>
  struct  is_constructible
      : public type_traits::detail::is_constructible::void_check<
          type_traits::detail::is_constructible::contains_void<T, Args...>::value
                                          || is_abstract<T>::value,
                                             T, Args...>
      {};

  namespace type_traits {
    namespace detail {
      namespace is_constructible {
    //      Array types are default constructible if their element type
    //      is default constructible

    template <typename A, std::size_t N>
    struct imp<false, A[N]>
        : public ::boost::is_constructible<typename remove_all_extents<A>::type>
        {};

    //      Otherwise array types are not constructible by this syntax

    template <typename A, std::size_t N, typename ...Args>
    struct imp<false, A[N], Args...>
        : public false_type
        {};

    //      Incomplete array types are not constructible

    template <typename A, typename ...Args>
    struct imp<false, A[], Args...>
        : public false_type
        {};

      } // is_constructible
    } // detail
  } // type_traits
} // boost
#else  // BOOST_NO_VARIADIC_TEMPLATES

/////////////////

#ifndef BOOST_CONVERSION_TT_IS_CONSTRUCTIBLE_ARITY_MAX
#define BOOST_CONVERSION_TT_IS_CONSTRUCTIBLE_ARITY_MAX 3
#endif

namespace boost {

  template <typename T,  BOOST_PP_ENUM_BINARY_PARAMS(BOOST_PP_INC(BOOST_CONVERSION_TT_IS_CONSTRUCTIBLE_ARITY_MAX), typename A, = type_traits::detail::nat BOOST_PP_INTERCEPT)>      \
  struct is_constructible;

  namespace type_traits {
    namespace detail {
      namespace is_constructible {
#if defined BOOST_CONVERSION_IS_CONSTRUCTIBLE_USES_DECLTYPE

    // specific test functions for 0 args which are not templates
    template <typename T>
    decltype((T(), true_type()))
    test0(T&);
    false_type
    test0(any);

    // specific positive test functions for #Args>0
#define M1(z,n,t) declval<A##n>()

#define M0(z,n,t)                                       \
  template <typename T, BOOST_PP_ENUM_PARAMS(n, typename A)>  \
  decltype((T(BOOST_PP_ENUM(n, M1, ~)), true_type()))   \
  test##n(T&, BOOST_PP_ENUM_PARAMS(n, A));

BOOST_PP_REPEAT_FROM_TO(1,BOOST_CONVERSION_TT_IS_CONSTRUCTIBLE_ARITY_MAX, M0, ~)
#undef M0
#undef M1

    // specific negative test functions for #Args>0
#define M0(z,n,t)                                       \
  template <BOOST_PP_ENUM_PARAMS(n, typename A)>           \
  false_type                                            \
  test##n(any, BOOST_PP_ENUM_PARAMS(n, A));

BOOST_PP_REPEAT_FROM_TO(1,BOOST_CONVERSION_TT_IS_CONSTRUCTIBLE_ARITY_MAX, M0, ~)
#undef M0


// specialization for void, abstract, function or any of the parameters is void, scalar or reference
// depend is whether T(declval<Args>() ...) is well formed

#define M1(z,n,t) declval<A##n>()
#define M0(z,n,t)                                                                     \
    template <bool, typename T BOOST_PP_COMMA_IF(n) BOOST_PP_ENUM_PARAMS(n, typename A)>    \
    struct imp##n : public common_type                                                \
                 <                                                                    \
        decltype(test##n(declval<T&>() BOOST_PP_COMMA_IF(n) BOOST_PP_ENUM(n, M1, ~))) \
                 >::type                                                              \
                 {};

BOOST_PP_REPEAT(BOOST_CONVERSION_TT_IS_CONSTRUCTIBLE_ARITY_MAX, M0, ~)
#undef M0
#undef M1

#elif defined BOOST_CONVERSION_IS_CONSTRUCTIBLE_USES_SIZEOF

// specialization for void, abstract, function or any of the parameters is void, scalar or reference
// and at least 1 arg:
// depend is whether X(declval<Args>() ...) is well formed

#define M1(z,n,t) declval<A##n>()

#define M0(z,n,t)                                                                                   \
template<bool, typename T, BOOST_PP_ENUM_PARAMS(n, typename A)>                                           \
struct imp##n                                                                                       \
{                                                                                                   \
    template<typename X>                                                                               \
    static yes_type                                            \
    test(dummy_size<sizeof(X(BOOST_PP_ENUM(n, M1, ~)))>*);          \
                                                                                                    \
    template<typename X>                                                                               \
    static no_type                                             \
    test(...);                                                                                      \
                                                                                                    \
    static const bool value =                                                                       \
              sizeof(test<T>(0)) == sizeof(yes_type);          \
    typedef boost::integral_constant<bool,value> type;                                              \
};

BOOST_PP_REPEAT_FROM_TO(1,BOOST_CONVERSION_TT_IS_CONSTRUCTIBLE_ARITY_MAX, M0, ~)
#undef M0
#undef M1

#if ! defined BOOST_CONVERSION_NO_IS_DEFAULT_CONSTRUCTIBLE

// specialization for void, abstract, function or any of the parameters is void, scalar or reference
// and no arg:
// depend is whether X() seen as a constructor call is well formed
    template<bool, typename T>
    struct imp0
    {
        template<typename X>
        static yes_type
        test(dummy_size<sizeof(X(),int())>*);

        template<typename X>
        static no_type
        test(...);

        static const bool value = sizeof(test<T>(0)) == sizeof(yes_type);
        typedef boost::integral_constant<bool,value> type;
    };
#else
    template<bool, typename T>
    struct imp0 : public false_type  {};
#endif


#else

// specialization for void, abstract, function or any of the parameters is void, scalar or reference when
// the compiler doesn't support any know technique that
// allows to detect if X(declval<Args>() ...) is well formed
// always false. The user of the library needs to specialize this trait for its owns types.

#define M0(z,n,t)                                                                 \
template<bool, typename T BOOST_PP_COMMA_IF(n) BOOST_PP_ENUM_PARAMS(n, typename A)>     \
struct imp##n                                                                     \
  : boost::false_type                                                             \
{                                                                                 \
};

    BOOST_PP_REPEAT_FROM_TO(0,BOOST_CONVERSION_TT_IS_CONSTRUCTIBLE_ARITY_MAX, M0, ~)
#undef M0

#endif

// specialization for scalar or reference with 1 arg: depend is the arg is scalar
      template <typename T>
      struct imp0<true, T>
          : public is_scalar<T>
          {};

// specialization for scalar or reference with 1 arg: depend is the arg is convertible to the type
      template <typename T, typename A1>
      struct imp1<true, T, A1>
          : public is_convertible<A1, T>
          {};
// specialization for scalar or reference with multiple args: always false

#define M0(z,n,t)                                                 \
template <typename T, BOOST_PP_ENUM_PARAMS(n, typename A)>              \
struct imp##n<true, T, BOOST_PP_ENUM_PARAMS(n, A) >               \
  : public false_type                                             \
  {};

BOOST_PP_REPEAT_FROM_TO(2,BOOST_CONVERSION_TT_IS_CONSTRUCTIBLE_ARITY_MAX, M0, ~)
#undef M0

// specialization for NOT void, abstract, function or any of the parameters is void:
      // depends on whether the type is scalar or reference

#define M0(z,n,t)                                                                   \
  template <bool, typename T BOOST_PP_COMMA_IF(n) BOOST_PP_ENUM_PARAMS(n, typename A)>    \
  struct void_check##n                                                              \
    : public imp##n<is_scalar<T>::value || is_reference<T>::value,                  \
                              T BOOST_PP_COMMA_IF(n) BOOST_PP_ENUM_PARAMS(n, A)>    \
    {};

BOOST_PP_REPEAT(BOOST_CONVERSION_TT_IS_CONSTRUCTIBLE_ARITY_MAX, M0, ~)
#undef M0

// specialization for void, abstract, function or any of the parameters is void: always false
#define M0(z,n,t)                                                               \
  template <typename T BOOST_PP_COMMA_IF(n) BOOST_PP_ENUM_PARAMS(n, typename A)>      \
  struct void_check##n<true, T BOOST_PP_COMMA_IF(n) BOOST_PP_ENUM_PARAMS(n, A)> \
    : public false_type    \
  {};

BOOST_PP_REPEAT(BOOST_CONVERSION_TT_IS_CONSTRUCTIBLE_ARITY_MAX, M0, ~)
#undef M0


// specialization for unbounded arrays: always false

#define M0(z,n,t)                                                               \
  template <typename A BOOST_PP_COMMA_IF(n) BOOST_PP_ENUM_PARAMS(n, typename A)>      \
  struct imp##n<false, A[] BOOST_PP_COMMA_IF(n) BOOST_PP_ENUM_PARAMS(n, A)>     \
    : public false_type                                                         \
  {};

BOOST_PP_REPEAT(BOOST_CONVERSION_TT_IS_CONSTRUCTIBLE_ARITY_MAX, M0, ~)
#undef M0

      } // is_constructible
    } // detail
  } // type_traits


// defines is_constructible, depending on whether T is void, abstract, function or any of the parameters is void
#define M1(z,n,t) || is_void<A##n>::value

#define M0(z,n,t)                                                                 \
  template <typename T BOOST_PP_COMMA_IF(n) BOOST_PP_ENUM_PARAMS(n, typename A)>        \
  struct is_constructible<T BOOST_PP_COMMA_IF(n) BOOST_PP_ENUM_PARAMS(n, A)>      \
    : public type_traits::detail::is_constructible::void_check##n<is_void<T>::value \
              || is_abstract<T>::value  || is_function<T>::value                  \
              BOOST_PP_REPEAT(n, M1, ~),                                          \
              T BOOST_PP_COMMA_IF(n) BOOST_PP_ENUM_PARAMS(n, A)>                  \
  {};

BOOST_PP_REPEAT(BOOST_CONVERSION_TT_IS_CONSTRUCTIBLE_ARITY_MAX, M0, ~)
#undef M0

  namespace type_traits {
    namespace detail {
      namespace is_constructible {
// specialization for bounded arrays without arguments: depends on the type
    template <typename A, std::size_t N>
    struct imp0<false, A[N]>
        : public boost::is_constructible<typename remove_all_extents<A>::type>
        {};

    // specialization for bounded arrays with arguments: always false
#define M0(z,n,t)                                                         \
  template <typename A, std::size_t N, BOOST_PP_ENUM_PARAMS(n, typename A)>     \
  struct imp##n<false, A[N], BOOST_PP_ENUM_PARAMS(n, A)>                  \
    : public false_type                                                   \
      {};

    BOOST_PP_REPEAT_FROM_TO(1,BOOST_CONVERSION_TT_IS_CONSTRUCTIBLE_ARITY_MAX, M0, ~)
#undef M0


      } // is_constructible
    } // detail
  } // type_traits
} // boost

#endif // variadic
#endif // doc
#endif // header

