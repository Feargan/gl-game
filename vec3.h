#pragma once

#include <cmath>

using std::pow;
using std::sin;
using std::cos;

template<typename T>
class CVec3
{
public:
	T x;
	T y;
	T z;

	CVec3() : x{}, y{}, z{} {}
	CVec3(T x, T y, T z) : x(x), y(y), z(z) {}

	CVec3 operator+(const CVec3& r) const
	{
		return CVec3(x + r.x, y + r.y, z + r.z);
	}
	CVec3 operator-(const CVec3& r) const
	{
		return CVec3(x - r.x, y - r.y, z - r.z);
	}

	CVec3& operator+=(const CVec3& r)
	{
		x += r.x;
		y += r.y;
		z += r.z;
		return *this;
	}
	CVec3& operator-=(const CVec3& r)
	{
		x -= r.x;
		y -= r.y;
		z -= r.y;
		return *this;
	}


	CVec3 operator*(T m) const
	{
		return CVec3{ x*m, y*m, z*m };
	}

	T operator*(const CVec3& r) const
	{
		return x * r.x + y * r.y + z * r.z;
	}

	template<typename U>
	CVec3& operator*=(U m)
	{
		static_assert(!std::is_arithmetic<U>::value, "scalar multiplication: only arithmetic types are allowed, use CVec3::vmult for vector cross product");
		x *= m;
		y *= m;
		z *= m;
		return *this;
	}

	CVec3& vmult(const CVec3& r)
	{
		T tx = x, ty = y, tz = z;
		x = ty * r.z - tz * r.y;
		y = tz * r.x - tx * r.z;
		z = tx * r.y - ty - r.x;
		return *this;
	}

	T len(double p = 2.0) const
	{
		return pow(pow(x, p) + pow(y, p) + pow(z, p), 1.0 / p);
	}

	T dist(const CVec3& r, double p = 2.0) const
	{
		CVec3 d = *this - r;
		return pow(pow(d.x, p) + pow(d.y, p) + pow(d.z, p), 1.0 / p);
	}


	CVec3& rotateX(double a)
	{
		T ny = y * cos(a) + z * sin(a);
		T nz = z * cos(a) - y * sin(a);
		y = ny;
		z = nz;
		return *this;
	}

	CVec3& rotateY(double a)
	{
		T nx = x * cos(a) + z * sin(a);
		T nz = z * cos(a) - x * sin(a);
		x = nx;
		z = nz;
		return *this;
	}

	CVec3& rotateZ(double a)
	{
		T nx = x * cos(a) - y * sin(a);
		T ny = x * sin(a) + y * cos(a);
		x = nx;
		y = ny;
		return *this;
	}
};

using CVec3d = CVec3<double>;