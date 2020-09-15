#pragma once
#include "vec3.h"


void write_color( std::ostream& out, const color& pixel_color, int32_t samples_per_pixel )
{
	double_t scale = 1.0 / samples_per_pixel;
	double_t gamma_correction = 1.0 / 2.2;

	double_t r = std::pow( pixel_color.R * scale, gamma_correction );
	double_t g = std::pow( pixel_color.G * scale, gamma_correction );
	double_t b = std::pow( pixel_color.B * scale, gamma_correction );

	out << static_cast<int16_t>( 255.999 * r ) << ' ' << static_cast<int16_t>( 255.999 * g ) << ' ' << static_cast<int16_t>( 255.999 * b ) << '\n';
}