// Copyright John Maddock 2006.

// Use, modification and distribution are subject to the
// Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt
// or copy at http://www.boost.org/LICENSE_1_0.txt)

// test_students_t.cpp

#define BOOST_MATH_THROW_ON_DOMAIN_ERROR
#define BOOST_MATH_THROW_ON_OVERFLOW_ERROR

#ifdef _MSC_VER
#  pragma warning(disable: 4127) // conditional expression is constant.
#  pragma warning(disable: 4100) // unreferenced formal parameter.
#  pragma warning(disable: 4512) // assignment operator could not be generated.
#  pragma warning(disable: 4510) // default constructor could not be generated.
#  pragma warning(disable: 4610) // can never be instantiated - user defined constructor required.
//#  pragma warning(disable: 4535) // calling _set_se_translator() requires /EHa (in Boost.test)
// Enable C++ Exceptions Yes With SEH Exceptions (/EHa) prevents warning 4535.
#endif

#include <boost/math/concepts/real_concept.hpp> // for real_concept
#include <boost/math/distributions/exponential.hpp>
	 using boost::math::exponential_distribution;

#include <boost/test/included/test_exec_monitor.hpp> // Boost.Test
#include <boost/test/floating_point_comparison.hpp>


#include <iostream>
	using std::cout;
	using std::endl;
	using std::setprecision;

template <class RealType>
void test_spot(RealType l, RealType x, RealType p, RealType q, RealType tolerance)
{
   BOOST_CHECK_CLOSE(
      ::boost::math::cdf(
         exponential_distribution<RealType>(l),      
         x),
         p,
			tolerance); // %
   BOOST_CHECK_CLOSE(
      ::boost::math::cdf(
         complement(exponential_distribution<RealType>(l),      
         x)),
         q,
			tolerance); // %
   if(p < 0.999)
   {
      BOOST_CHECK_CLOSE(
         ::boost::math::quantile(
            exponential_distribution<RealType>(l),      
            p),
            x,
			   tolerance); // %
   }
   if(q < 0.999)
   {
      BOOST_CHECK_CLOSE(
         ::boost::math::quantile(
            complement(exponential_distribution<RealType>(l),      
            q)),
            x,
			   tolerance); // %
   }
}

template <class RealType>
void test_spots(RealType T)
{
   // Basic sanity checks.
   // 50eps as a persentage, up to a maximum of double precision
   // (that's the limit of our test data).
   RealType tolerance = (std::max)(
      static_cast<RealType>(boost::math::tools::epsilon<double>()),
      boost::math::tools::epsilon<RealType>());
   tolerance *= 50 * 100;  

	cout << "Tolerance for type " << typeid(T).name()  << " is " << tolerance << " %" << endl;

   test_spot(
      static_cast<RealType>(0.5), // lambda
      static_cast<RealType>(0.125), // x
      static_cast<RealType>(0.060586937186524213880289175377695L), // p
      static_cast<RealType>(0.93941306281347578611971082462231L), //q
      tolerance);
   test_spot(
      static_cast<RealType>(0.5), // lambda
      static_cast<RealType>(5), // x
      static_cast<RealType>(0.91791500137610120483047132553284L), // p
      static_cast<RealType>(0.08208499862389879516952867446716L), //q
      tolerance);
   test_spot(
      static_cast<RealType>(2), // lambda
      static_cast<RealType>(0.125), // x
      static_cast<RealType>(0.22119921692859513175482973302168L), // p
      static_cast<RealType>(0.77880078307140486824517026697832L), //q
      tolerance);
   test_spot(
      static_cast<RealType>(2), // lambda
      static_cast<RealType>(5), // x
      static_cast<RealType>(0.99995460007023751514846440848444L), // p
      static_cast<RealType>(4.5399929762484851535591515560551e-5L), //q
      tolerance);

   BOOST_CHECK_CLOSE(
      ::boost::math::pdf(
         exponential_distribution<RealType>(0.5),      
         static_cast<RealType>(0.125)),              // x
         static_cast<RealType>(0.46970653140673789305985541231115L),                // probability.
			tolerance); // %
   BOOST_CHECK_CLOSE(
      ::boost::math::pdf(
         exponential_distribution<RealType>(0.5),      
         static_cast<RealType>(5)),              // x
         static_cast<RealType>(0.04104249931194939758476433723358L),                // probability.
			tolerance); // %
   BOOST_CHECK_CLOSE(
      ::boost::math::pdf(
         exponential_distribution<RealType>(2),      
         static_cast<RealType>(0.125)),              // x
         static_cast<RealType>(1.5576015661428097364903405339566L),                // probability.
			tolerance); // %
   BOOST_CHECK_CLOSE(
      ::boost::math::pdf(
         exponential_distribution<RealType>(2),      
         static_cast<RealType>(5)),              // x
         static_cast<RealType>(9.0799859524969703071183031121101e-5L),                // probability.
			tolerance); // %

   BOOST_CHECK_CLOSE(
      ::boost::math::mean(
         exponential_distribution<RealType>(2)),
         static_cast<RealType>(0.5),           
			tolerance); // %
   BOOST_CHECK_CLOSE(
      ::boost::math::standard_deviation(
         exponential_distribution<RealType>(2)), 
         static_cast<RealType>(0.5),
			tolerance); // %

   //
   // Things that are errors:
   //
   exponential_distribution<RealType> dist(0.5);
   BOOST_CHECK_THROW(
       quantile(dist, RealType(1.0)),
       std::overflow_error);
   BOOST_CHECK_THROW(
       quantile(complement(dist, RealType(0.0))),
       std::overflow_error);
   BOOST_CHECK_THROW(
       pdf(dist, RealType(-1)),
       std::domain_error);
   BOOST_CHECK_THROW(
       cdf(dist, RealType(-1)),
       std::domain_error);
   BOOST_CHECK_THROW(
       cdf(exponential_distribution<RealType>(-1), RealType(1)),
       std::domain_error);
   BOOST_CHECK_THROW(
       quantile(dist, RealType(-1)),
       std::domain_error);
   BOOST_CHECK_THROW(
       quantile(dist, RealType(2)),
       std::domain_error);

} // template <class RealType>void test_spots(RealType)

int test_main(int, char* [])
{
	 // Basic sanity-check spot values.
	// (Parameter value, arbitrarily zero, only communicates the floating point type).
  test_spots(0.0F); // Test float. OK at decdigits = 0 tolerance = 0.0001 %
  test_spots(0.0); // Test double. OK at decdigits 7, tolerance = 1e07 %
#ifndef BOOST_MATH_NO_LONG_DOUBLE_MATH_FUNCTIONS
  test_spots(0.0L); // Test long double.
#if !BOOST_WORKAROUND(__BORLANDC__, BOOST_TESTED_AT(0x582))
  test_spots(boost::math::concepts::real_concept(0.)); // Test real concept.
#endif
#else
   std::cout << "<note>The long double tests have been disabled on this platform "
      "either because the long double overloads of the usual math functions are "
      "not available at all, or because they are too inaccurate for these tests "
      "to pass.</note>" << std::cout;
#endif

   return 0;
} // int test_main(int, char* [])

