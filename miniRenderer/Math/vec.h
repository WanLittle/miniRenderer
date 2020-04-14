#pragma once

#include <cmath>
#include <vector>
#include <ostream>
#include <cassert>

namespace glm
{
    template <size_t DIM, typename T> struct vec
    {
        vec() { for (size_t i = DIM; i--; data_[i] = T()); }

        T& operator[](const size_t i) { assert(i < DIM); return data_[i]; }

        const T& operator[](const size_t i) const { assert(i < DIM); return data_[i]; }

    private:
        T data_[DIM];
    };

    // vec<2, T>
    template <typename T> struct vec<2, T>
    {
        vec() : x(T()), y(T()) {}

        vec(T X, T Y) : x(X), y(Y) {}

        template <class U> vec<2, T>(const vec<2, U>& v);

        T& operator[](const size_t i) { assert(i < 2); return i <= 0 ? x : y; }

        const T& operator[](const size_t i) const { assert(i < 2); return i <= 0 ? x : y; }

        T x, y;
    };

    // vec<3, T>
    template <typename T> struct vec<3, T>
    {
        vec() : x(T()), y(T()), z(T()) {}

        vec(T X, T Y, T Z) : x(X), y(Y), z(Z) {}

        vec(const vec<4, T>& rhs) : x(rhs[0]), y(rhs[1]), z(rhs[2]) {}

        template <class U> vec<3, T>(const vec<3, U>& v);

        T& operator[](const size_t i) { assert(i < 3); return i <= 0 ? x : (1 == i ? y : z); }

        const T& operator[](const size_t i) const { assert(i < 3); return i <= 0 ? x : (1 == i ? y : z); }

        float norm() { return std::sqrt(x * x + y * y + z * z); }

        vec<3, T>& normalize(T l = 1) { *this = (*this) * (l / norm()); return *this; }

        T x, y, z;
    };

    // vec<4, T>
    template <typename T> struct vec<4, T>
    {
        vec() : x(T()), y(T()), z(T()), w(T()) {}

        vec(T X, T Y, T Z, T W) : x(X), y(Y), z(Z), w(W) {}

        vec(const vec<3, T> &rhs, T W) : x(rhs[0]), y(rhs[1]), z(rhs[2]), w() {}

        T& operator[](const size_t i)
        {
            assert(i < 4);
            if (i <= 0) return x;
            else if (i == 1) return y;
            else if (i == 2) return z;
            else return w;
        }

        const T& operator[](const size_t i) const
        {
            assert(i < 4);
            if (i <= 0) return x;
            else if (i == 1) return y;
            else if (i == 2) return z;
            else return w;
        }

        T x, y, z, w;
    };

    template<size_t DIM, typename T> T operator*(const vec<DIM, T>& lhs, const vec<DIM, T>& rhs)
    {
        T ret = T();
        for (size_t i = DIM; i--; ret += lhs[i] * rhs[i]);
        return ret;
    }


    template<size_t DIM, typename T>vec<DIM, T> operator+(vec<DIM, T> lhs, const vec<DIM, T>& rhs)
    {
        for (size_t i = DIM; i--; lhs[i] += rhs[i]);
        return lhs;
    }

    template<size_t DIM, typename T>vec<DIM, T> operator-(vec<DIM, T> lhs, const vec<DIM, T>& rhs)
    {
        for (size_t i = DIM; i--; lhs[i] -= rhs[i]);
        return lhs;
    }

    template<size_t DIM, typename T, typename U> vec<DIM, T> operator*(vec<DIM, T> lhs, const U& rhs)
    {
        for (size_t i = DIM; i--; lhs[i] *= rhs);
        return lhs;
    }

    template<size_t DIM, typename T, typename U> vec<DIM, T> operator/(vec<DIM, T> lhs, const U& rhs)
    {
        for (size_t i = DIM; i--; lhs[i] /= rhs);
        return lhs;
    }

    // 低维 -> 高维
    template<size_t LEN, size_t DIM, typename T> vec<LEN, T> embed(const vec<DIM, T>& v, T fill = 1)
    {
        vec<LEN, T> ret;
        for (size_t i = LEN; i--; ret[i] = (i < DIM ? v[i] : fill));
        return ret;
    }

    // 高维 -> 低维
    template<size_t LEN, size_t DIM, typename T> vec<LEN, T> proj(const vec<DIM, T>& v)
    {
        vec<LEN, T> ret;
        for (size_t i = LEN; i--; ret[i] = v[i]);
        return ret;
    }

    // 叉乘
    template <typename T> vec<3, T> cross(vec<3, T> v1, vec<3, T> v2)
    {
        return vec<3, T>(v1.y * v2.z - v1.z * v2.y, v1.z * v2.x - v1.x * v2.z, v1.x * v2.y - v1.y * v2.x);
    }

    template <typename T> vec<3, T> normalize(vec<3, T> v)
    {
        return v.normalize();
    }

    template <size_t DIM, typename T> std::ostream& operator<<(std::ostream& out, vec<DIM, T>& v)
    {
        for (unsigned int i = 0; i < DIM; i++) {
            out << v[i] << " ";
        }
        return out;
    }

    typedef vec<2, float> Vec2f;
    typedef vec<2, int>   Vec2i;
    typedef vec<3, float> Vec3f;
    typedef vec<3, int>   Vec3i;
    typedef vec<4, float> Vec4f;

}