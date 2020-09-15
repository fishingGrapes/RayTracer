#include "pch.h"

#include "vec3.h"
#include "color.h"
#include "ray.h"

#include "hittable_list.h"
#include "sphere.h"

#include "camera.h"


color ray_color( const ray& r, const hittable& world, int32_t depth )
{

	if (depth <= 0)
		return color( 0.0 );

	hit_record hit;
	if (world.hit( r, 0.001, Infinity, OUT hit ))
	{
		point3 target = hit.Point + random_in_hemisphere( hit.Normal );
		return 0.5 * ray_color( ray( hit.Point, target - hit.Point ), world, depth - 1 );
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
	const int64_t image_width = 720;
	const int64_t image_height = static_cast<int64_t>( image_width / aspect_ratio );
	const int32_t samples_per_pixel = 100;
	const int32_t max_depth = 50;

	//world
	hittable_list world;
	world.add( std::make_shared<sphere>( point3( 0, 0, -1 ), 0.5 ) );
	world.add( std::make_shared<sphere>( point3( 0, -100.5, -1 ), 100 ) );

	//camera
	camera cam;

	//render
	std::cout << "P3\n" << image_width << ' ' << image_height << "\n255\n";

	for (int64_t x = image_height - 1; x >= 0; --x)
	{
		std::cerr << "\rScanlines Remaining: " << x << std::endl;

		for (int64_t y = 0; y < image_width; ++y)
		{

			color pixel_color( 0.0 );

			for (int64_t s = 0; s < samples_per_pixel; ++s)
			{
				// u => (0 -> 1)
				double_t u = ( y + random_double( ) ) / ( image_width - 1 );
				// v => (1 -> 0)
				double_t v = ( x + random_double( ) ) / ( image_height - 1 );

				ray r = cam.get_ray( u, v );
				pixel_color += ray_color( r, world, max_depth );
			}


			write_color( std::cout, pixel_color, samples_per_pixel );
		}
	}
	std::cerr << "\nDone." << std::endl;

	return EXIT_SUCCESS;
}
