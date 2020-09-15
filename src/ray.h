#pragma once

#include "vec3.h"

class ray
{
public:
	point3 Origin;
	vec3 Direction;

public:

	ray( )
	{
	}

	ray( const point3& origin, const vec3& direction )
		: Origin( origin ), Direction( direction )
	{
	}

	point3 at( double_t t ) const
	{
		return Origin + t * Direction;
	}
};
