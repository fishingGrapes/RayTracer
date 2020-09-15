#include "pch.h"

#include "vec3.h"
#include "color.h"
#include "ray.h"

#include "hittable_list.h"
#include "sphere.h"

#include "camera.h"

#include "lambertian.h"
#include "metal.h"


color ray_color( const ray& r, const hittable& world, int32_t depth )
{

	if (depth <= 0)
		return color( 0.0 );

	hit_record hit;
	if (world.hit( r, 0.001, Infinity, OUT hit ))
	{
		ray scattered;
		color attenuation;

		if (hit.MaterialPtr->scatter( r, hit, OUT attenuation, OUT scattered ))
		{
			return  attenuation * ray_color( scattered, world, depth - 1 );
		}

		return color( 0.0 );
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
	const int64_t image_width = 1280;
	const int64_t image_height = static_cast<int64_t>( image_width / aspect_ratio );
	const int32_t samples_per_pixel = 100;
	const int32_t max_depth = 50;

	//materials
	mat_ptr material_ground = std::make_shared<lambertian>( color( 0.8, 0.8, 0.0 ) );
	mat_ptr material_center = std::make_shared<lambertian>( color( 0.7, 0.3, 0.3 ) );
	mat_ptr material_left = std::make_shared<metal>( color( 0.8, 0.8, 0.8 ), 0.3 );
	mat_ptr material_right = std::make_shared<metal>( color( 0.8, 0.6, 0.2 ), 1.0 );

	//world
	hittable_list world;
	world.add( std::make_shared<sphere>( point3( 0.0, -100.5, -1.0 ), 100.0, material_ground ) );
	world.add( std::make_shared<sphere>( point3( 0.0, 0.0, -1.0 ), 0.5, material_center ) );
	world.add( std::make_shared<sphere>( point3( -1.0, 0.0, -1.0 ), 0.5, material_left ) );
	world.add( std::make_shared<sphere>( point3( 1.0, 0.0, -1.0 ), 0.5, material_right ) );

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
