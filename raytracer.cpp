#include <iostream>

#include "vec3.h"
#include "color.h"
#include "ray.h"


bool hit_sphere( const point3& center, double_t radius, const ray& r )
{
	vec3 oc = r.Origin - center;

	double_t a = vec3::dot( r.Direction, r.Direction );
	double_t b = 2.0 * vec3::dot( oc, r.Direction );
	double_t c = vec3::dot( oc, oc ) - ( radius * radius );

	double_t discriminant = b * b - 4 * a * c;
	return discriminant > 0;
}


color ray_color( const ray& r )
{
	if (hit_sphere( point3( 0, 0, -1 ), 0.5, r ))
	{
		return color( 1, 0, 0 );
	}

	vec3 unit_direction = r.Direction.normalized( );
	//map (-1 > 1) to (0 -> 1)
	double_t t = 0.5 * ( unit_direction.Y + 1.0 );
	//Sky Gradient
	return ( 1 - t ) * color( 1.0, 1.0, 1.0 ) + t * color( 0.5, 0.7, 1.0 );
}

int main( )
{
	//image
	const double_t aspect_ratio = 16.0 / 9.0;
	const int64_t image_width = 400;
	const int64_t image_height = static_cast<int64_t>( image_width / aspect_ratio );

	//camera
	double_t viewport_height = 2.0;
	double_t viewport_width = viewport_height * aspect_ratio;
	double_t focal_length = 1.0;

	point3 origin( 0.0 );
	vec3 horizontal = vec3( viewport_width, 0, 0 );
	vec3 vertical = vec3( 0, viewport_height, 0 );

	//raytracing starts from this point
	point3 lower_left = origin - ( horizontal * 0.5 ) - ( vertical * 0.5 ) - vec3( 0, 0, focal_length );

	//render
	std::cout << "P3\n" << image_width << ' ' << image_height << "\n255\n";

	for (int64_t x = image_height - 1; x >= 0; --x)
	{
		std::cerr << "\rScanlines Remaining: " << x << std::endl;

		for (int64_t y = 0; y < image_width; ++y)
		{
			// u => (0 -> 1)
			double_t u = double_t( y ) / ( image_width - 1 );
			// v => (1 -> 0)
			double_t v = double_t( x ) / ( image_height - 1 );


			//scan from top-left to bottom-right
			ray r( origin, lower_left + u * horizontal + v * vertical - origin );
			color pixel_color = ray_color( r );

			write_color( std::cout, pixel_color );
		}
	}
	std::cerr << "\nDone." << std::endl;

	return EXIT_SUCCESS;
}
