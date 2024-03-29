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
 Include this file when using conversions between @c std::vector<> of convertible types.
 */

//[VECTOR_HPP
#ifndef BOOST_CONVERSION_STD_VECTOR_HPP
#define BOOST_CONVERSION_STD_VECTOR_HPP

#include <boost/conversion/type_traits/std/vector.hpp>
#include <boost/conversion/convert_to.hpp>
#include <boost/conversion/assign_to.hpp>
#include <boost/conversion/is_extrinsically_assignable.hpp>

namespace boost {
#if defined(BOOST_CONVERSION_DOXYGEN_INVOKED)
  namespace doxygen_trick
  {
    //! trick to generate the doc. Don't take care of it
    struct vector{};
  }
#endif
  
  namespace conversion {

    // std namespace can not be overloaded

    /**
     * Partial specialization of @c implicit_converter_cp for @c std::vector of extrinsic convertibles.
     */
    template < typename T1, typename A1, typename T2, typename A2>
    struct implicit_converter_cp< std::vector<T1,A1>, std::vector<T2,A2>
#if defined(BOOST_CONVERSION_DOXYGEN_INVOKED)
        , requires
        ExtrinsicallyAssignable<T1,T2>
        )
#else
        , typename enable_if_c<
        is_extrinsically_assignable<T1,T2>::value
        >::type
#endif
    > : true_type
    {
        std::vector<T1,A1> operator()(std::vector<T2,A2> const & from)
        {
            std::vector<T1,A1> to;
            //boost::conversion::assign_to(to, from);
            to.resize(from.size());
            for (unsigned int i=0; i<from.size(); ++i) {
                boost::conversion::assign_to(to[i], from[i]);
            }
            return to;
        }
    };

    /**
     * Partial specialization of @c assigner_cp for @c std::vector of extrinsic convertibles.
     */
    template < typename T1, typename A1, typename T2, typename A2>
    struct assigner_cp< std::vector<T1,A1>, std::vector<T2,A2>
#if defined(BOOST_CONVERSION_DOXYGEN_INVOKED)
        , requires
        ExtrinsicallyAssignable<T1,T2>
        )
#else
        , typename enable_if_c<
        is_extrinsically_assignable<T1,T2>::value
        >::type
#endif
    > : true_type
    {
        std::vector<T1,A1>& operator()(
            std::vector<T1,A1>& to,
            std::vector<T2,A2> const & from)
        {
            to.resize(from.size());
            for (unsigned int i=0; i<from.size(); ++i) {
                boost::conversion::assign_to(to[i], from[i]);
            }
            return to;
        }
    };

  }
}

#endif

//]
