#ifndef MAT_HPP
#define MAT_HPP

#include <initializer_list>
#include <algorithm>

#include "vec.hpp"

/* column major storage */
template<typename T>
struct tmat4 {
    T data[16];

    tmat4() : data{1.0, 0.0, 0.0, 0.0,
                  0.0, 1.0, 0.0, 0.0,
                  0.0, 0.0, 1.0, 0.0,
                  0.0, 0.0, 0.0, 1.0,} {};

    tmat4(std::initializer_list<T> list)
    {
        std::copy(list.begin(), list.end(), data);
    };
};


template<typename T>
tvec4<T> operator*(const tmat4<T> &m, const tvec4<T> &v)
{
    return {m.data[0] * v.x + m.data[4] * v.y + m.data[8] * v.z + m.data[12] * v.w,
            m.data[1] * v.x + m.data[5] * v.y + m.data[9] * v.z + m.data[13] * v.w,
            m.data[2] * v.x + m.data[6] * v.y + m.data[10] * v.z + m.data[14] * v.w,
            m.data[3] * v.x + m.data[7] * v.y + m.data[11] * v.z + m.data[15] * v.w};
}


using mat4 = tmat4<float>;
using dmat4 = tmat4<double>;

#endif
