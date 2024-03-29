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
 Include this file when using conversions between @c boost::rational<> of convertible types.
 */


#ifndef BOOST_CONVERSION_PAIR__HPP
#define BOOST_CONVERSION_PAIR__HPP

#include <boost/conversion/type_traits/boost/rational.hpp>
#include <boost/conversion/convert_to.hpp>
#include <boost/conversion/assign_to.hpp>
#include <boost/config.hpp>
#include <boost/mpl/bool.hpp>
#include <boost/utility/enable_if.hpp>

namespace boost {
  namespace conversion {

    //! @brief @c implicit_converter specialization for source and target been @c boost::rational.
    //!
    template < typename Target, typename Source>
    struct implicit_converter_cp< rational<Target>, rational<Source>
#if defined(BOOST_CONVERSION_DOXYGEN_INVOKED)
        , requires(
        ExtrinsicallyConvertible<Source,Target>
        )
#else
        , typename enable_if_c<
        is_extrinsically_convertible<Source,Target>::value
        >::type
#endif
    > : true_type
    {
      //! @Returns the target rational having as numerator and denominator the conversion from the numerator and denominator of the source rational.
      rational<Target> operator()(rational<Source> const & from)
      {
        return rational<Target>(boost::conversion::convert_to<Target>(from.numerator()), boost::conversion::convert_to<Target>(from.denominator()));
      }
    };
    template < typename Target, typename Source>
    struct assigner_cp< rational<Target>, rational<Source>
#if defined(BOOST_CONVERSION_DOXYGEN_INVOKED)
        , requires(
        ExtrinsicallyConvertible<Source,Target>
        )
#else
        , typename enable_if_c<
        is_extrinsically_convertible<Source,Target>::value
        >::type
#endif
    > : true_type
    {
      rational<Target>& operator()(rational<Target>& to, const rational<Source>& from)
      {
        to.assign(boost::conversion::convert_to<Target>(from.numerator()), boost::conversion::convert_to<Target>(from.denominator()));
        return to;
      }
    };
  }
#if defined(BOOST_CONVERSION_DOXYGEN_INVOKED)
  namespace doxygen_trick
  {
    //! trick to generate the doc. Don't take care of it
    struct rational{};
  }
#endif

}

#endif

