#pragma once

#include "vec3.h"

template<typename T>
struct CHitSphere
{
	T r;
	CVec3<T> pos;

	CHitSphere(T r = {}, const CVec3<T>& pos = {}) : r(r), pos(pos) {}
};

using CHitSphered = CHitSphere<double>;