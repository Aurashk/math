// Copyright Nick Thompson, 2019
// Use, modification and distribution are subject to the
// Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt
// or copy at http://www.boost.org/LICENSE_1_0.txt)
#ifndef BOOST_MATH_INTERPOLATORS_WHITAKKER_SHANNON_HPP
#define BOOST_MATH_INTERPOLATORS_WHITAKKER_SHANNON_HPP
#include <boost/math/constants/constants.hpp>
#include <boost/math/special_functions/sin_pi.hpp>

namespace boost { namespace math { namespace interpolators {

template<class RandomAccessContainer>
class whittaker_shannon {
public:

    using Real = typename RandomAccessContainer::value_type;
    whittaker_shannon(RandomAccessContainer&& y, Real const & t0, Real const & h) : m_y{std::move(y)}, m_t0{t0}, m_h{h}
    {
        for (size_t i = 1; i < m_y.size(); i += 2)
        {
            m_y[i] = -m_y[i];
        }
    }


    Real operator()(Real t) const {
        using boost::math::constants::pi;
        using std::sin;
        using std::isfinite;
        using std::floor;
        Real y = 0;
        Real x = (t - m_t0)/m_h;
        Real z = x;
        auto it = m_y.begin();
        while(it != m_y.end())
        {
            // Doing the repeated conversion of i to double is super expensive:
            // cvtsi2sd; avoid it via decrementing z:
            //Real denom = (x - i);
            y += *it++/z;
            z -= 1;
        }

        if (!isfinite(y))
        {
            BOOST_ASSERT_MSG(floor(x) == ceil(x), "Floor and ceiling should be equal.\n");
            size_t i = static_cast<size_t>(floor(x));
            if (i & 1)
            {
                return -m_y[i];
            }
            return m_y[i];
        }
        return y*sin_pi(x)/pi<Real>();
    }


    Real operator[](size_t i) const {
        if (i & 1)
        {
            return -m_y[i];
        }
        return m_y[i];
    }

    RandomAccessContainer&& return_data() {
        for (size_t i = 1; i < m_y.size(); i += 2)
        {
            m_y[i] = -m_y[i];
        }
        return std::move(m_y);
    }


private:
    RandomAccessContainer m_y;
    Real m_t0;
    Real m_h;
};
}}}
#endif
