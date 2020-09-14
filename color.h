#pragma once
#include "vec3.h"


void write_color( std::ostream& out, const color& c )
{
	out << static_cast<int16_t>( 255.999 * c.R ) << ' ' << static_cast<int16_t>( 255.999 * c.G ) << ' ' << static_cast<int16_t>( 255.999 * c.B ) << '\n';
}