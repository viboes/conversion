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
 Include this file when using conversions between @c boost::ptime and @c boost::chrono::time_point<>.
 */

//[CHRONO_TIME_POINT_TO_POSIX_TIME_PTIME_HPP
#ifndef BOOST_CONVERSION_CHRONO_TIME_POINT_TO_POSIX_TIME_PTIME_HPP
#define BOOST_CONVERSION_CHRONO_TIME_POINT_TO_POSIX_TIME_PTIME_HPP

#include <boost/chrono/time_point.hpp>
#include <boost/chrono/chrono.hpp>
#include <boost/date_time/posix_time/posix_time_types.hpp>
#include <boost/date_time/posix_time/conversion.hpp>
#include <boost/conversion/convert_to.hpp>
#include <boost/conversion/assign_to.hpp>
#include <boost/config.hpp>

namespace boost {
  #if defined(BOOST_CONVERSION_DOXYGEN_INVOKED)
    //! trick to generate the doc. Don't take care of it
    struct trick_chrono_time_point{};
  #endif
#if defined(BOOST_CONVERSION_NO_IS_DEFAULT_CONSTRUCTIBLE)
  template <> struct is_constructible< boost::posix_time::ptime >  : true_type {};           
#endif
#if defined(BOOST_CONVERSION_NO_IS_CONSTRUCTIBLE)
  template <> struct is_constructible< boost::posix_time::ptime, boost::posix_time::ptime const& >  : true_type {}; 
#endif
#if defined(BOOST_CONVERSION_NO_IS_ASSIGNABLE)
  template <> struct is_assignable< boost::posix_time::ptime&, boost::posix_time::ptime const& >  : true_type {};   
  template <> struct is_assignable< boost::posix_time::ptime, boost::posix_time::ptime >  : true_type {};   
#endif

  
#if defined(BOOST_CONVERSION_NO_IS_DEFAULT_CONSTRUCTIBLE)
  template < class Clock, class Duration>
  struct is_constructible< chrono::time_point<Clock, Duration> >  : true_type {};
#endif
#if defined(BOOST_CONVERSION_NO_IS_CONSTRUCTIBLE)
  template < class Clock, class Duration>
  struct is_constructible< chrono::time_point<Clock, Duration>, chrono::time_point<Clock, Duration> > : true_type {};
#endif
#if defined(BOOST_CONVERSION_NO_IS_ASSIGNABLE)
  template < class Clock, class Duration>
  struct is_assignable< chrono::time_point<Clock, Duration>&, chrono::time_point<Clock, Duration> const& >  : true_type {};
  template < class Clock, class Duration>
  struct is_assignable< chrono::time_point<Clock, Duration>, chrono::time_point<Clock, Duration> >  : true_type {};
#endif
  
  namespace conversion {

    template < class Clock, class Duration>
    struct implicit_converter_cp<posix_time::ptime, chrono::time_point<Clock, Duration>
        > : true_type
    {
      posix_time::ptime operator()(const chrono::time_point<Clock, Duration>& from)
      {
        typedef chrono::time_point<Clock, Duration> time_point_t;
        typedef chrono::nanoseconds duration_t;
        typedef duration_t::rep rep_t;
        rep_t d = chrono::duration_cast<duration_t>(from.time_since_epoch()).count();
        rep_t sec = d/1000000000;
        rep_t nsec = d%1000000000;
        return  posix_time::from_time_t(0)+
                posix_time::seconds(static_cast<long>(sec))+
#ifdef BOOST_DATE_TIME_HAS_NANOSECONDS
                posix_time::nanoseconds(nsec);
#else
                posix_time::microseconds((nsec+500)/1000);
#endif
      }
    };

    template < class Clock, class Duration>
    struct implicit_converter_cp<chrono::time_point<Clock, Duration>, posix_time::ptime
    > : true_type
    {
      chrono::time_point<Clock, Duration> operator()(const posix_time::ptime& from)
      {
        posix_time::time_duration const time_since_epoch=from-posix_time::from_time_t(0);
        chrono::time_point<Clock, Duration> t=chrono::system_clock::from_time_t(time_since_epoch.total_seconds());
        long long nsec=time_since_epoch.fractional_seconds()*(1000000000/time_since_epoch.ticks_per_second());
        return  t+chrono::duration_cast<Duration>(chrono::nanoseconds(nsec));
      }
    };
  }

#if defined(BOOST_CONVERSION_DOUBLE_CP2)
#ifndef BOOST_NO_FUNCTION_TEMPLATE_ORDERING
  namespace chrono {
    template < class Clock, class Duration>
    inline chrono::time_point<Clock, Duration>& assign_to(chrono::time_point<Clock, Duration>& to, const posix_time::ptime& from)
    {
      to = boost::conversion::convert_to<chrono::time_point<Clock, Duration> >(from);
      return to;
    }
  }

  namespace posix_time {
    template < class Clock, class Duration>
    inline ptime& assign_to(ptime& to, const chrono::time_point<Clock, Duration>& from)
    {
      to = boost::conversion::convert_to<ptime>(from);
      return to;
    }
  }
#endif
#endif
}

#endif
//]
