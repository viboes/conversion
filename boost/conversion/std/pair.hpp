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
 Include this file when using conversions between @c std::pair<> of convertible types.
 */


//[PAIR_HPP
#ifndef BOOST_CONVERSION_STD_PAIR_HPP
#define BOOST_CONVERSION_STD_PAIR_HPP

#include <boost/conversion/config.hpp>
#include <boost/conversion/assign_to.hpp>
#include <boost/conversion/convert_to.hpp>
#include <boost/conversion/type_traits/std/utility.hpp>


namespace boost {
  namespace conversion {

    // std namespace can not be overloaded

    /**
     * Partial specialization of @c implicit_converter_cp for @c std::pair of extrinsic convertibles.
     */
    template < typename T1, typename T2, typename S1, typename S2>
    struct implicit_converter_cp< std::pair<T1,T2>, std::pair<S1,S2>
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
        std::pair<T1,T2> operator()(std::pair<S1,S2> const & from)
        {
            return std::pair<T1,T2>(boost::conversion::implicit_convert_to<T1>(from.first), boost::conversion::implicit_convert_to<T2>(from.second));
        }
    };

    /**
     * Partial specialization of @c assigner_cp for @c std::pair of extrinsic convertibles.
     */
    template < typename T1, typename T2, typename S1, typename S2>
    struct assigner_cp< std::pair<T1,T2>, std::pair<S1,S2>
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
        std::pair<T1,T2>& operator()(std::pair<T1,T2>& to, const std::pair<S1,S2>& from)
        {
            to.first = boost::conversion::implicit_convert_to<T1>(from.first);
            to.second = boost::conversion::implicit_convert_to<T2>(from.second);
            return to;
        }
    };
  }
#if defined(BOOST_CONVERSION_DOXYGEN_INVOKED)
  namespace doxygen_trick
  {
    //! trick to generate the doc. Don't take care of it
    struct pair{};
  }
#endif
}

#endif

//]
