#pragma once

#include <limits>
#include <random>

//constants
constexpr double_t Infinity = std::numeric_limits<double_t>::infinity( );
constexpr double_t PI = 3.1415926535897932385;

//Utils
inline double_t degrees_to_radians( double_t degrees )
{
	return degrees * PI / 180.0;
}

inline double_t random_double( )
{
	static std::uniform_real_distribution<double_t> distribution( 0.0, 1.0 );
	static std::mt19937 generator;

	return distribution( generator );
}

inline double_t random_double( double_t min, double_t max )
{
	return min + ( max - min ) * random_double( );
}

inline double_t clamp( double_t x, double_t min, double_t max )
{
	return ( x < min ) ? min : ( ( x > max ) ? max : x );
}

