#pragma once

#include <iostream>

#include "common.h"

class vec3
{
public:
	union
	{
		double_t X;
		double_t R;
	};

	union
	{
		double_t Y;
		double_t G;
	};

	union
	{
		double_t Z;
		double_t B;
	};


public:

	constexpr vec3( ) : X( 0 ), Y( 0 ), Z( 0 )
	{
	}

	constexpr vec3( double_t n )
		: X( n ), Y( n ), Z( n )
	{
	}

	constexpr vec3( double_t x, double_t y, double_t z )
		: X( x ), Y( y ), Z( z )
	{
	}

	vec3( const vec3& other )
		:X( other.X ), Y( other.Y ), Z( other.Z )
	{

	}

	vec3& operator=( const vec3& other )
	{
		X = other.X;
		Y = other.Y;
		Z = other.Z;

		return *this;
	}

	inline vec3 operator-( ) const
	{
		return vec3( -X, -Y, -Z );
	}

	inline vec3 operator+=( const vec3& v )
	{
		X += v.X;
		Y += v.Y;
		Z += v.Z;

		return *this;
	}

	inline double_t length( ) const
	{
		return std::sqrt( length_squared( ) );
	}

	inline double_t length_squared( ) const
	{
		return ( X * X + Y * Y + Z * Z );
	}

	inline vec3 normalized( ) const
	{
		double_t length_inverse = 1.0 / length( );
		return vec3( X * length_inverse, Y * length_inverse, Z * length_inverse );
	}

	inline static double_t dot( const vec3& v1, const vec3& v2 )
	{
		return v1.X * v2.X + v1.Y * v2.Y + v1.Z * v2.Z;
	}

	inline static vec3 cross( const vec3& v1, const vec3& v2 )
	{
		return vec3( v1.Y * v2.Z - v1.Z * v2.Y, v1.Z * v2.X - v1.X * v2.Z, v1.X * v2.Y - v1.Y * v2.X );
	}

	inline static vec3 normalize( vec3& v )
	{
		double_t length_inverse = 1.0 / v.length( );

		v.X = v.X * length_inverse;
		v.Y = v.Y * length_inverse;
		v.Z = v.Z * length_inverse;
	}

	inline static vec3 random( )
	{
		return vec3( random_double( ), random_double( ), random_double( ) );
	}

	inline static vec3 random( double_t min, double_t max )
	{
		return vec3( random_double( min, max ), random_double( min, max ), random_double( min, max ) );
	}

	inline static constexpr vec3 Up( )
	{
		return vec3( 0.0, 1.0, 0.0 );
	}

	inline static constexpr vec3 Right( )
	{
		return vec3( 1.0, 0.0, 0.0 );
	}

	inline static constexpr vec3 Forward( )
	{
		return vec3( 0.0, 0.0, -1.0 );
	}
};

using point3 = vec3;
using color = vec3;

inline std::ostream& operator<<( std::ostream& out, const vec3& v )
{
	return out << '(' << v.X << ", " << v.Y << ", " << v.Z << ')';
}


inline vec3 operator+( const vec3& v1, const vec3& v2 )
{
	return vec3( v1.X + v2.X, v1.Y + v2.Y, v1.Z + v2.Z );
}

inline vec3 operator-( const vec3& v1, const vec3& v2 )
{
	return vec3( v1.X - v2.X, v1.Y - v2.Y, v1.Z - v2.Z );
}

inline vec3 operator*( const vec3& v, double_t scale )
{
	return vec3( v.X * scale, v.Y * scale, v.Z * scale );
}

inline vec3 operator*( double_t scale, const vec3& v )
{
	return vec3( v.X * scale, v.Y * scale, v.Z * scale );
}

inline vec3 operator/( const vec3& v, double_t t )
{
	return v * ( 1.0 / t );
}

vec3 random_unit_vec3( )
{
	while (true)
	{
		vec3 p = vec3::random( -1, 1 );
		if (p.length_squared( ) >= 1) continue;
		return p;
	}
}

vec3 random_in_hemisphere( const vec3& normal )
{
	vec3 unit_vector = random_unit_vec3( );

	//in the same hemishpere as normal
	if (vec3::dot( unit_vector, normal ) > 0.0)
	{
		return unit_vector;
	}
	else
	{
		return -unit_vector;
	}
}


