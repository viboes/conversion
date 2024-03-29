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
 @brief Defines the @c assignable_to class.
 */
#ifndef BOOST_CONVERSION_ASSIGNABLE_TO_HPP
#define BOOST_CONVERSION_ASSIGNABLE_TO_HPP

#include <boost/conversion/convert_to.hpp>
#include <boost/conversion/assign_to.hpp>
#include <boost/conversion/is_extrinsically_convertible.hpp>
#include <boost/conversion/is_extrinsically_assignable.hpp>
#include <boost/utility/enable_if.hpp>

namespace boost {
  namespace conversion {

    //! wrapper providing assignment from extrinsic assignable to @c Target.

    template <typename Target>
    class assignable_to {
    public:
      Target& ref_;

      //! default copy constructor
      //! @NoThrow.
      //assignable_to(assignable_to const& r) : ref_(r.ref_) { }

      //! constructor from a reference
      //! @NoThrow.
      assignable_to(Target& r) : ref_(r) {}

      //! Assignment.
      //!
      //! @Effects Forwards the assignment to the reference.
      //! @Returns @c *this
      //! @Throws Whatever @c Target assignment can throws.
      assignable_to& operator =(assignable_to<Target> const& u)
      {
        ref_ = u.ref_;
        return *this;
      }

      //! Assignment from a assignable_to wrapping a type @c Source convertible to Target.
      //!
      //! @Effects Forwards the assignment to the reference using @c assign_to.
      //! @Returns @c *this
      //! @Throws Whatever @c assign_to throws.
      //! @Remark This constructor doesn't participates on overload resolution if @c Source is not extrinsic assignable to @c Target.
      template <typename Source>
#if !defined(BOOST_NO_FUNCTION_TEMPLATE_DEFAULT_ARGS)
      typename boost::enable_if< is_extrinsically_assignable<Target,Source>, assignable_to& >::type
#else
      assignable_to&
#endif
      operator =(assignable_to<Source> const& s)
      {
        boost::conversion::assign_to(ref_, s.ref_);
        return *this;
      }

      //! Assignment from a type @c Source assignable to @c Target.
      //!
      //! @Effects Forwards the assignment to the reference using @c assign_to
      //! @Returns @c *this
      //! @Throws Whatever @c assign_to throws.
      //! @Remark This constructor doesn't participates on overload resolution if @c Source is not extrinsic assignable to @c Target.
      template <typename Source>
#if !defined(BOOST_NO_FUNCTION_TEMPLATE_DEFAULT_ARGS)
      typename boost::enable_if< is_extrinsically_assignable<Target,Source>, assignable_to& >::type
#else
      assignable_to&
#endif
      operator =(Source const& u)
      {
        boost::conversion::assign_to(ref_, u);
        return *this;
      }
    };

  }
}

#endif

