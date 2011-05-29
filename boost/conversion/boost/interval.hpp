//////////////////////////////////////////////////////////////////////////////
//
// (C) Copyright Vicente J. Botet Escriba 2009. Distributed under the Boost
// Software License, Version 1.0. (See accompanying file
// LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
//
// See http://www.boost.org/libs/synchro for documentation.
//
//////////////////////////////////////////////////////////////////////////////

/*!
 \file
 \brief
 Include this file when using conversions between @c boost::numeric::interval<> of convertible types.
 */

#ifndef BOOST_CONVERSION_INTERVAL_HPP
#define BOOST_CONVERSION_INTERVAL_HPP

#include <boost/numeric/interval.hpp>
#include <boost/conversion/convert_to.hpp>
#include <boost/conversion/assign_to.hpp>

namespace boost {

    #ifdef BOOST_NO_FUNCTION_TEMPLATE_ORDERING && ! defined(BOOST_CONVERSION_DOXYGEN_INVOKED)
    namespace conversion { namespace overload_workaround {
        template < class T, class PT, class U, class PU>
        struct convert_to< numeric::interval<T,PT>, numeric::interval<U,PU> > {
            inline static numeric::interval<T,PT> apply(numeric::interval<U,PU> const & from)
            {
                return numeric::interval<T,PT>(boost::conversion::convert_to<T>(from.lower()), boost::conversion::convert_to<U>(from.upper()));
            }
        };
        template < class T, class PT, class U, class PU>
        struct assign_to< numeric::interval<T,PT>, numeric::interval<U,PU> > {
            inline static numeric::interval<T,PT>& apply(numeric::interval<T,PT>& to, const numeric::interval<U,PU>& from)
            {
                to.assign(boost::conversion::convert_to<T>(from.lower()), boost::conversion::convert_to<U>(from.upper()));
                return to;
            }
        };

    }}
    #else
    namespace numeric {
      //! @brief @c convert_to overloading for source and target been @c boost::numeric::interval.
      //!
      //! @Returns the target interval having as extremes the conversion from the source interval extremes.
        template < class T, class PT, class U, class PU>
        inline interval<T,PT> convert_to(interval<U,PU> const & from
                        , conversion::dummy::type_tag<interval<T,PT> > const&)
        {
            return interval<T,PT>(boost::conversion::convert_to<T>(from.lower()), boost::conversion::convert_to<U>(from.upper()));
        }
        template < class T, class PT, class U, class PU>
        inline interval<T,PT>& assign_to(interval<T,PT>& to, const interval<U,PU>& from
                        , conversion::dummy::type_tag<interval<T,PT> > const&
                    )
        {
            to.assign(boost::conversion::convert_to<T>(from.lower()),boost::conversion::convert_to<U>(from.upper()));
            return to;
        }
    }
    #endif
}

#endif

