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
 Include this file when using conversions between @c boost::ptime and @c boost::chrono::time_point<system_clock>.
 */

//[CHRONO_TIME_POINT_TO_POSIX_TIME_PTIME_HPP
#ifndef BOOST_CONVERSION_CHRONO_TIME_POINT_TO_POSIX_TIME_PTIME_HPP
#define BOOST_CONVERSION_CHRONO_TIME_POINT_TO_POSIX_TIME_PTIME_HPP

#include <boost/conversion/type_traits/boost/chrono/time_point.hpp>
#include <boost/chrono/chrono.hpp>
#include <boost/conversion/type_traits/boost/date_time/posix_time/posix_time_types.hpp>
#include <boost/date_time/posix_time/conversion.hpp>
#include <boost/conversion/convert_to.hpp>
#include <boost/conversion/assign_to.hpp>
#include <boost/config.hpp>

namespace boost {
  namespace conversion {

    template < typename Duration>
    struct implicit_converter_cp<posix_time::ptime, chrono::time_point<chrono::system_clock, Duration>
        > : true_type
    {
      posix_time::ptime operator()(const chrono::time_point<chrono::system_clock, Duration>& from)
      {
        typedef chrono::time_point<chrono::system_clock, Duration> time_point_t;
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

    template < typename Duration>
    struct implicit_converter_cp<chrono::time_point<chrono::system_clock, Duration>, posix_time::ptime
    > : true_type
    {
      chrono::time_point<chrono::system_clock, Duration> operator()(const posix_time::ptime& from)
      {
        posix_time::time_duration const time_since_epoch=from-posix_time::from_time_t(0);
        chrono::time_point<chrono::system_clock, Duration> t=chrono::system_clock::from_time_t(time_since_epoch.total_seconds());
        long long nsec=time_since_epoch.fractional_seconds()*(1000000000/time_since_epoch.ticks_per_second());
        return  t+chrono::duration_cast<Duration>(chrono::nanoseconds(nsec));
      }
    };
  }

#if defined(BOOST_CONVERSION_DOXYGEN_INVOKED)
  namespace trick_doxygen
  {
    //! trick to generate the doc. Don't take care of it
    struct chrono_time_point_to_posix_time_ptime{};
  }
#endif
}


#endif
//]
