//////////////////////////////////////////////////////////////////////////////
//
// (C) Copyright Vicente J. Botet Escriba 2009-2011. Distributed under the Boost
// Software License, Version 1.0. (See accompanying file
// LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
//
// See http://www.boost.org/libs/synchro for documentation.
//
//////////////////////////////////////////////////////////////////////////////

/*!
 @file
 @brief
 Include this file when using conversions between @c boost::fusion::tuple<> of convertible types.
 */
#ifndef BOOST_CONVERSION_TUPLE_HPP
#define BOOST_CONVERSION_TUPLE_HPP

#include <boost/conversion/type_traits/boost/fusion/tuple.hpp>
#include <boost/conversion/explicit_convert_to.hpp>
#include <boost/conversion/assign_to.hpp>
#include <boost/config.hpp>

namespace boost {
  namespace conversion {

    template < typename T1, typename T2, typename S1, typename S2>
    struct explicit_converter_cp< fusion::tuple<T1,T2>, fusion::tuple<S1,S2>
#if defined(BOOST_CONVERSION_DOXYGEN_INVOKED)
    , requires(
    ExtrinsicallyConvertible<S1,T1>
    && ExtrinsicallyConvertible<S2,T2>
    )
#else
        , typename enable_if_c<
        is_extrinsically_convertible<S1,T1>::value
     && is_extrinsically_convertible<S2,T2>::value
        >::type
#endif
    > : true_type
    {
      fusion::tuple<T1,T2> operator()(fusion::tuple<S1,S2> const & from)
      {
        return fusion::tuple<T1,T2>(
            boost::conversion::implicit_convert_to<T1>(fusion::get<0>(from))
            , boost::conversion::implicit_convert_to<T2>(fusion::get<1>(from))
        );
      }
    };
    template < typename T1, typename T2, typename T3, typename S1, typename S2, typename S3>
    struct explicit_converter_cp< fusion::tuple<T1,T2,T3>, fusion::tuple<S1,S2,S3>
#if defined(BOOST_CONVERSION_DOXYGEN_INVOKED)
        , requires(
        ExtrinsicallyConvertible<S1,T1>
        && ExtrinsicallyConvertible<S2,T2>
        && ExtrinsicallyConvertible<S3,T3>
        )
#else
        , typename enable_if_c<
        is_extrinsically_convertible<S1,T1>::value
        && is_extrinsically_convertible<S2,T2>::value
        && is_extrinsically_convertible<S3,T3>::value
        >::type
#endif
    > : true_type
    {
      fusion::tuple<T1,T2,T3> operator()(fusion::tuple<S1,S2,S3> const & from)
      {
        return fusion::tuple<T1,T2, T3>(
              boost::conversion::convert_to<T1>(fusion::get<0>(from))
            , boost::conversion::convert_to<T2>(fusion::get<1>(from))
            , boost::conversion::convert_to<T3>(fusion::get<2>(from))
        );
      }
    };

  }
#if defined(BOOST_CONVERSION_DOXYGEN_INVOKED)
  namespace doxygen_trick
  {
    //! trick to generate the doc. Don't take care of it
    struct tuple{};
  }
#endif

}

#endif

