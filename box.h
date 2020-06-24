#pragma once
#include "vec3.h"

template<typename T>
struct CBox
{
	T top;
	T bottom;
};

using CBoxd = CBox<CVec3d>;