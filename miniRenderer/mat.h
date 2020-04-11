#pragma once

#include "vec.h"

#include <cmath>
#include <vector>
#include <cassert>
#include <iostream>

template<size_t DimCols, size_t DimRows, typename T> class mat;

/////////////////////////////////////////////// 行列式

template<size_t DIM, typename T> struct dt
{
	static T det(const mat<DIM, DIM, T>& src)
	{
		T ret = 0;
		for (size_t i = DIM; i--; ret += src[0][i] * src.cofactor(0, i));
		return ret;
	}
};

template<typename T> struct dt<1, T>
{
	static T det(const mat<1, 1, T>& src)
	{
		return src[0][0];
	}
};


/////////////////////////////////////////////// 矩阵
template<size_t DimRows, size_t DimCols, typename T> class mat
{
	vec<DimCols, T> rows[DimRows]; // 行优先存储
public:
	mat() {}

	// 获取行
	vec<DimCols, T>& operator[] (const size_t idx)
	{
		assert(idx < DimRows);
		return rows[idx];
	}

	// 获取行 const
	const vec<DimCols, T>& operator[] (const size_t idx) const
	{
		assert(idx < DimRows);
		return rows[idx];
	}

	// 获取列
	vec<DimRows, T> col(const size_t idx) const
	{
		assert(idx < DimCols);
		vec<DimRows, T> ret;
		for (size_t i = DimRows; i--; )
            ret[i] = rows[i][idx];
		return ret;
	}

	// 设置列
	void set_col(size_t idx, vec<DimRows, T> v) {
		assert(idx < DimCols);
		for (size_t i = DimRows; i--; )
            rows[i][idx] = v[i];
	}

	// 单位矩阵
	static mat<DimRows, DimCols, T> identity() {
		mat<DimRows, DimCols, T> ret;
		for (size_t i = DimRows; i--; )
			for (size_t j = DimCols; j--; )
                ret[i][j] = (i == j);
		return ret;
	}

	// 矩阵表示的行列式的值
	T det() const
	{
		return dt<DimCols, T>::det(*this);
	}

	mat<DimRows - 1, DimCols - 1, T> get_minor(size_t row, size_t col) const
	{
		mat<DimRows - 1, DimCols - 1, T> ret;
		for (size_t i = DimRows - 1; i--; )
			for (size_t j = DimCols - 1; j--; ret[i][j] = rows[i < row ? i : i + 1][j < col ? j : j + 1]);
		return ret;
	}

	// 代数余子式
	T cofactor(size_t row, size_t col) const
	{
		return get_minor(row, col).det() * ((row + col) % 2 ? -1 : 1);
	}

	// 伴随矩阵 A*：A各元素的代数余子式构成的矩阵
	mat<DimRows, DimCols, T> adjugate() const {
		mat<DimRows, DimCols, T> ret;
		for (size_t i = DimRows; i--; )
			for (size_t j = DimCols; j--; ret[i][j] = cofactor(i, j));
		return ret;
	}

	// 逆的转置A-T，A*=|A|·A- 则 A-T = A * T / |A|
	mat<DimRows, DimCols, T> invert_transpose()
	{
		mat<DimRows, DimCols, T> ret = adjugate();
		T tmp = ret[0] * rows[0];
		return ret / tmp;
	}

	// 逆矩阵 A-
	mat<DimRows, DimCols, T> invert()
	{
		return invert_transpose().transpose();
	}

	// 转置 AT
	mat<DimCols, DimRows, T> transpose() {
		mat<DimCols, DimRows, T> ret;
		for (size_t i = DimCols; i--; ret[i] = this->col(i));
		return ret;
	}
};


/////////////////////////////////////////////////////////////////////////////////

// 矩阵×向量
template<size_t DimRows, size_t DimCols, typename T>
vec<DimRows, T> operator*(const mat<DimRows, DimCols, T>& lhs, const vec<DimCols, T>& rhs)
{
	vec<DimRows, T> ret;
	for (size_t i = DimRows; i--; )
        ret[i] = lhs[i] * rhs;
	return ret;
}

// 矩阵×矩阵
template<size_t R1, size_t C1, size_t C2, typename T>
mat<R1, C2, T> operator*(const mat<R1, C1, T>& lhs, const mat<C1, C2, T>& rhs)
{
	mat<R1, C2, T> result;
	for (size_t i = R1; i--; )
		for (size_t j = C2; j--; )
            result[i][j] = lhs[i] * rhs.col(j);
	return result;
}

template<size_t DimRows, size_t DimCols, typename T>
mat<DimCols, DimRows, T> operator/(mat<DimRows, DimCols, T> lhs, const T& rhs)
{
	for (size_t i = DimRows; i--; )
        lhs[i] = lhs[i] / rhs;
	return lhs;
}

template <size_t DimRows, size_t DimCols, class T>
std::ostream& operator<<(std::ostream& out, mat<DimRows, DimCols, T>& m)
{
	for (size_t i = 0; i < DimRows; i++)
		out << m[i] << std::endl;
	return out;
}

typedef mat<4, 4, float> Matrix;
