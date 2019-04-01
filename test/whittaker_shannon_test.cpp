/*
 * Copyright Nick Thompson, 2019
 * Use, modification and distribution are subject to the
 * Boost Software License, Version 1.0. (See accompanying file
 * LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
 */

#include "math_unit_test.hpp"
#include <numeric>
#include <utility>
#include <random>
#include <boost/core/demangle.hpp>
#include <boost/math/interpolators/whittaker_shannon.hpp>
#ifdef BOOST_HAS_FLOAT128
#include <boost/multiprecision/float128.hpp>
using boost::multiprecision::float128;
#endif

using boost::math::interpolators::whittaker_shannon;

template<class Real>
void test_knots()
{
    Real t0 = 0;
    Real h = Real(1)/Real(16);
    size_t n = 512;
    std::vector<Real> v(n);
    std::mt19937 gen(323723);
    std::uniform_real_distribution<long double> dis(1.0, 2.0);

    for(size_t i = 0;  i < n; ++i) {
      v[i] = static_cast<Real>(dis(gen));
    }
    
    auto ws = whittaker_shannon(std::move(v), t0, h);
  
    size_t i = 0;
    while (i < n) {
      Real t = t0 + i*h;
      CHECK_ULP_CLOSE(ws[i], ws(t), 16);
      ++i;
    }
}

template<class Real>
void test_bump()
{
    using std::exp;
    using std::abs;
    auto bump = [](Real x) { if (abs(x) >= 1) { return Real(0); } return exp(-Real(1)/(Real(1)-x*x)); };

    Real t0 = -1;
    size_t n = 2049;
    Real h = Real(2)/Real(n-1);

    std::vector<Real> v(n);
    for(size_t i = 0; i < n; ++i) {
        Real t = t0 + i*h;
        v[i] = bump(t);
    }


    auto ws = whittaker_shannon(std::move(v), t0, h);

    std::mt19937 gen(323723);
    std::uniform_real_distribution<long double> dis(-0.95, 0.95);

    size_t i = 0;
    while (i++ < 1000)
    {
        Real t = static_cast<Real>(dis(gen));
        Real expected = bump(t);
        if(!CHECK_MOLLIFIED_CLOSE(expected, ws(t), 50*std::numeric_limits<Real>::epsilon())) {
            std::cerr << "  Problem occured at abscissa " << t << "\n";
        }
    }
}


int main()
{
    test_knots<float>();
    test_knots<double>();
    test_knots<long double>();
#ifdef BOOST_HAS_FLOAT128
    test_knots<float128>(); 
#endif

    test_bump<float>();
    test_bump<double>();
    test_bump<long double>();

    return boost::math::test::report_errors();
}
