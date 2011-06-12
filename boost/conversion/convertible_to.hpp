//////////////////////////////////////////////////////////////////////////////
//
// (C) Copyright Vicente J. Botet Escriba 2009. Distributed under the Boost
// Software License, Version 1.0. (See accompanying file
// LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
//
// See http://www.boost.org/libs/conversion for documentation.
//
//////////////////////////////////////////////////////////////////////////////

/*!
 @file
 @brief
 Defines the free function @c mca.
 */
#ifndef BOOST_CONVERSION_CONVERTIBLE_TO_HPP
#define BOOST_CONVERSION_CONVERTIBLE_TO_HPP

#include <boost/conversion/convert_to.hpp>
#include <boost/conversion/assign_to.hpp>

namespace boost {
  namespace conversion {

    //! wrapper providing implicit conversion operation to @c Target.

    template <typename Target>
    class convertible_to {
      Target val_;
    public:

      //! constructor from a extrinsic implicitly convertible to @c Target.
      //! @Effects Store the extrinsic conversion from @c source to @ Target.
      //! @Throws Whatever extrinsic implicit conversion from @c source to @c Target could throw.
      template  <typename Source>
      convertible_to(Source const& source)
        : val_(boost::conversion::convert_to<Target>(source))
      {}

      //! Implicit conversion to @c Target.
      //! @Returns @c val_
      //! @Throws Whatever @c target copy constructor could throw.
      operator Target() const
      {
        return val_;
      }
      //! explicit conversion to @c Target.
      //! @Returns @c val_
      //! @Throws Whatever @c target copy constructor could throw.
      Target get() const
      {
        return val_;
      }

    };
  }
}

#endif

