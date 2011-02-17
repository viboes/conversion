//////////////////////////////////////////////////////////////////////////////
//
// (C) Copyright Vicente J. Botet Escriba 2008-2009. Distributed under the Boost
// Software License, Version 1.0. (See accompanying file
// LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
//
// See http://www.boost.org/libs/synchro for documentation.
//
//////////////////////////////////////////////////////////////////////////////

#include <boost/conversion/convert_to.hpp>
#include <boost/conversion/boost/rational.hpp>
#include <iostream>
#include <boost/detail/lightweight_test.hpp>

using namespace boost;

typedef int A1;
typedef short B1;

void explicit_convert_to() {
    B1 b1(3);
    B1 b2(2);
    boost::rational<B1> b(1,2);
    boost::rational<A1> a1(boost::convert_to<boost::rational<A1> >(b));
    boost::rational<A1> a2(boost::convert_to<boost::rational<A1> >(boost::rational<B1>(b1,b2)));

}
void explicit_assign_to() {
    B1 b1(3);
    B1 b2(2);
    boost::rational<A1> a;
    boost::rational<B1> b(1,2);
    boost::assign_to(a, b);
    boost::assign_to(a, boost::rational<B1>(b1,b2));
}

int main( )
{
  explicit_convert_to();
  explicit_assign_to();
  return boost::report_errors();
}

