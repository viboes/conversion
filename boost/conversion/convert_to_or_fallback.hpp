//////////////////////////////////////////////////////////////////////////////
//
// (C) Copyright Vicente J. Botet Escriba 2011. Distributed under the Boost
// Software License, Version 1.0. (See accompanying file
// LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
//
// See http://www.boost.org/libs/conversion for documentation.
//
//////////////////////////////////////////////////////////////////////////////

/**
 *  @file
 *  @brief
 *  Defines the free function @c convert_to_or_fallback and its customization
 *  point @c converter_or_fallbacker_cp.
 *
 *  The @c convert_to_or_fallback function converts the @c from parameter to a
 *  @c Target type. If the conversion fails the fallback value is used to
 *  construct a Target @c instance.
 *
 *  The default implementation applies the conversion @c Target operator of the
 *  @c Source class or the copy constructor of the @c Target class.
 *  When an exception is thrown the fallback is returned.
 *   Of course if both exist the conversion is ambiguous.
 *
 * When the default behavior is not satisfactory or doesn't takes care of
 *  specific types, the user could customize the behavior of
 *  @c convert_to_or_fallback in two ways:
 *
 *    - overload the @c convert_to_or_fallback on any namespace found by ADL from the
 *  @c Source or the @c Target. A trick is used to overload on the return type
 *  by adding a dummy parameter depending on the @c Target.
 *    - partially specialize the @c boost::conversion::converter_or_fallbacker_cp struct.
 *
 *  @note As we can not add new functions on the @c std namespace, partial
 *  specialization is the only option in these cases.
 */

#ifndef BOOST_CONVERSION_CONVERT_TO_OR_FALLBACK_HPP
#define BOOST_CONVERSION_CONVERT_TO_OR_FALLBACK_HPP

#include <boost/conversion/config.hpp>
#include <boost/conversion/convert_to.hpp>
#include <boost/conversion/is_extrinsically_explicitly_convertible.hpp>
#include <boost/type_traits/is_same.hpp>
#include <boost/type_traits/integral_constant.hpp>
#include <boost/utility/enable_if.hpp>

namespace boost {
  namespace conversion {

    //! Customization point for @c convert_to_or_fallback.
    //!
    //! @tparam Target target type of the conversion.
    //! @tparam Source source type of the conversion.
    //! @tparam Fallback type of the fallback value which must be explicitly convertible to @c Target.
    //! @tparam Enable A dummy template parameter that can be used for SFINAE.
    //!
    //! This class must be specialized by the user.
    template < typename Target, typename Source, typename Fallback=Target, typename Enable = void>
    struct converter_or_fallbacker_cp : false_type {};

    //! Default @c converter_or_fallbacker.
    //!
    //! @tparam Target target type of the conversion.
    //! @tparam Source source type of the conversion.
    //! @tparam Fallback type of the fallback value which must be explicitly convertible to @c Target.
    //! @tparam Enable A dummy template parameter that can be used for SFINAE.
    //!
    //! The default implementation relies on the @c converter_or_fallbacker_cp customization point which must be specialized by the user.
    template < typename Target, typename Source, typename Fallback=Target, typename Enable = void>
    struct converter_or_fallbacker : converter_or_fallbacker_cp<Target,Source,Fallback,Enable> {};

    //! Specialization for @c converter_or_fallbacker when
    //! @c is_extrinsically_explicitly_convertible<Source,Target>
    //! @c && is_extrinsically_explicitly_convertible<Fallback,Target>.
    //!
    //! @tparam Target target type of the conversion.
    //! @tparam Source source type of the conversion.
    //! @tparam Fallback type of the fallback value which must be explicitly convertible to @c Target.
    //!
    //! @Requires @c is_extrinsically_explicitly_convertible<Source,Target> && @c is_extrinsically_explicitly_convertible<Fallback,Target>.
    template < typename Target, typename Source, typename Fallback>
    struct converter_or_fallbacker<Target, Source, Fallback,
#if defined(BOOST_CONVERSION_DOXYGEN_INVOKED)
        requires(ExtrinsicallyExplicitConvertible<Source,Target>
              && ExtrinsicallyExplicitConvertible<Fallback,Target>)
#else
        typename enable_if_c<
             is_extrinsically_explicitly_convertible<Source,Target>::value
          && is_extrinsically_explicitly_convertible<Fallback,Target>::value
        >::type
#endif
    > : true_type
    {
      //! @Returns The converted value if the conversion succeeds or the conversion of the fallback.
      //! @Throws  Whatever the conversion from @c Fallback to @c Target can throws when the conversion fails.
      Target operator()(const Source& val, Fallback const& fallback)
      {
        try
        {
          return boost::conversion::convert_to<Target>(val);
        }
        catch (...)
        {
          return boost::conversion::convert_to<Target>(fallback);
        }
      }
    };


#if !defined(BOOST_CONVERSION_DOXYGEN_INVOKED)
    namespace impl_2 {

      //! @brief Default @c convert_to_or_fallback overload, used when ADL fails.
      //!
      //! @Effects  Converts the @c from parameter to an instance of the @c Target type, using by default the conversion operator or copy constructor.
      //! @Returns the converted value if the conversion succeeds or the fallback.
      //! @NoThrow
      //! Forwards the call to the overload workaround, which can yet be specialized by the user for standard C++ types.
      template < typename Target, typename Source, typename Fallback >
      typename enable_if_c<
        converter_or_fallbacker<Target,Source,Fallback>::value
      ,
      Target
      >::type
      convert_to_or_fallback(const Source& from, Fallback const& fallback, dummy::type_tag<Target> const&) {
        return converter_or_fallbacker<Target,Source,Fallback>()(from, fallback);
      }
    }

    namespace impl {
      template <typename Target, typename Source, typename Fallback>
      Target
      convert_to_or_fallback_impl(Source const& from, Fallback const& fallback) {
        using namespace boost::conversion::impl_2;
        //use boost::conversion::impl_2::convert_to_or_fallback if ADL fails
        return convert_to_or_fallback(from, fallback, dummy::type_tag<Target>());
      }
    }
#endif
  }
}

#include <boost/conversion/detail/is_extrinsically_explicitly_convertible_or_fallback_tagged.hpp>

namespace boost {
  namespace conversion {

    //! @brief Extrinsic conversion function with fallback.
    //! Converts the @c from parameter to a @c Target type. If the conversion fails the fallback value is used to construct a Target @c instance.
    //! @tparam Target target type of the conversion.
    //! @tparam Source source type of the conversion.
    //! @tparam Fallback type of the fallback value which must be explicitly convertible to @c Target.
    //!
    //! @Returns the converted value if the conversion succeeds or the fallback.
    //! @Throws  Whatever the conversion from @c Fallback to @c Target can throws when the conversion fails.
    //! @Remark This function doesn't participate in function overload when the @c Fallback is the same as the @c Target.
    //! @Example
    //! @code
    //! std::string s="not an int";
    //! int t=boost::conversion::convert_to_or_fallback<int>(s,-1);
    //! @endcode
    template <typename Target, typename Source, typename Fallback>
    typename enable_if_c<
         ! is_same<Target,Fallback>::value
         && is_extrinsically_explicitly_convertible_or_fallback_tagged<Source,Target,Fallback>::value
    , Target>::type
    convert_to_or_fallback(Source const& from, Fallback const& fallback) {
      return conversion::impl::convert_to_or_fallback_impl<Target>(from, fallback);
    }
    //! @brief Extrinsic conversion function with fallback.
    //! Converts the @c from parameter to a @c Target type. If the conversion fails the fallback value is used to construct a Target @c instance.
    //! @tparam Target target type of the conversion.
    //! @tparam Source source type of the conversion.
    //!
    //! @Returns the converted value if the conversion succeeds or the fallback.
    //! @Throws  Whatever the conversion from @c Target to @c Target can throws when the conversion fails.
    //! @Example
    //! @code
    //! std::string s="not an int";
    //! int t=boost::conversion::convert_to_or_fallback(s,-1);
    //! @endcode
    template <typename Target, typename Source>
    typename enable_if_c<
         is_extrinsically_explicitly_convertible_or_fallback_tagged<Source,Target,Target>::value
    , Target>::type
    convert_to_or_fallback(Source const& from, Target const& fallback) {
      return conversion::impl::convert_to_or_fallback_impl<Target>(from, fallback);
    }
  }
}

#endif

