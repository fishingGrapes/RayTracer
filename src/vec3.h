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

vec3 random_in_unit_sphere( )
{
	while (true)
	{
		vec3 p = vec3::random( -1, 1 );
		if (p.length_squared( ) >= 1) continue;
		return p;
	}
}

vec3 random_unit_vec3( )
{
	double_t a = random_double( 0, 2 * PI );
	double_t z = random_double( -1, 1 );

	//Since this is a unit vector the total magintude is x2 + y2 + z2 = 1^2
	//hence the remaing x2 + y2 = sqrt(1^2  - z2);
	double_t r = std::sqrt( 1 - z * z );

	return vec3( r * std::cos( a ), r * std::sin( a ), z );
}

vec3 random_in_hemisphere( const vec3& normal )
{
	vec3 unit_vector = random_in_unit_sphere( );

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

vec3 random_in_unit_disk( )
{
	while (true)
	{
		vec3 p = vec3( random_double( -1, 1 ), random_double( -1, 1 ), 0.0 );

		if (p.length_squared( ) >= 1) continue;
		return p;
	}
}

inline vec3 reflect( const vec3& v, const vec3& n )
{
	return v - ( 2 * vec3::dot( v, n ) * n );
}

vec3 refract( const vec3& v, const vec3& n, double_t etai_over_etat )
{
	/*
	R => Incident, R' => Refracted
	R'per = eta_incident/eta_other * (R + cos (theta) . n)
	R'par =  - sqrt(1 - |Rper|^2) . n
	a.b = |a||b| cos (theta) => if a and b awe unit vectors => a.b = cos (theta)
	therefore, Rper = eta_incident/eta_other * (R + (-R.n). n)
	*/

	double_t cos_theta = vec3::dot( -v, n );
	vec3 Rper = etai_over_etat * ( v + cos_theta * n );
	vec3 Rpar = -1.0 * std::sqrt( 1.0 - Rper.length_squared( ) ) * n;
	return Rper + Rpar;
}



