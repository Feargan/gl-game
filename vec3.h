#pragma once

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

	// todo
	// operator*=
	// operator*
	// euclidian distance
};

using CVec3d = CVec3<double>;