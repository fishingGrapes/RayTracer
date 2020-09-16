#include "pch.h"

#include "vec3.h"
#include "color.h"
#include "ray.h"

#include "hittable_list.h"
#include "sphere.h"

#include "camera.h"

#include "materials/lambertian.h"
#include "materials/metal.h"
#include "materials/dielectric.h"


hittable_list random_scene( );


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
	const double_t aspect_ratio = 3.0 / 2.0;
	const int64_t image_width = 1200;
	const int64_t image_height = static_cast<int64_t>( image_width / aspect_ratio );
	const int32_t samples_per_pixel = 500;
	const int32_t max_depth = 50;

	//world
	hittable_list world = random_scene( );

	//camera
	point3 lookfrom( 13, 2, 3 );
	point3 lookat( 0.0 );
	double_t dist_to_focus = 10.0;
	double_t aperture = 0.1;
	double_t vfov = 20.0;

	camera cam( lookfrom, lookat, vec3::Up( ), vfov, aspect_ratio, aperture, dist_to_focus );

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


hittable_list random_scene( )
{
	hittable_list world;

	mat_ptr ground_material = std::make_shared<lambertian>( color( 0.5, 0.5, 0.5 ) );
	world.add( std::make_shared<sphere>( point3( 0, -1000, 0 ), 1000, ground_material ) );

	for (int a = -11; a < 11; a++)
	{
		for (int b = -11; b < 11; b++)
		{
			double_t choose_mat = random_double( );
			point3 center( a + 0.9 * random_double( ), 0.2, b + 0.9 * random_double( ) );

			if (( center - point3( 4, 0.2, 0 ) ).length( ) > 0.9)
			{
				std::shared_ptr<material> sphere_material;

				if (choose_mat < 0.8)
				{
					// diffuse
					color albedo = color::random( ) * color::random( );
					sphere_material = std::make_shared<lambertian>( albedo );
					world.add( std::make_shared<sphere>( center, 0.2, sphere_material ) );
				}
				else if (choose_mat < 0.95)
				{
					// metal
					color albedo = color::random( 0.5, 1 );
					double_t fuzz = random_double( 0, 0.5 );
					sphere_material = std::make_shared<metal>( albedo, fuzz );
					world.add( std::make_shared<sphere>( center, 0.2, sphere_material ) );
				}
				else
				{
					// glass
					sphere_material = std::make_shared<dielectric>( 1.5 );
					world.add( std::make_shared<sphere>( center, 0.2, sphere_material ) );
				}
			}
		}
	}

	mat_ptr material1 = std::make_shared<dielectric>( 1.5 );
	world.add( std::make_shared<sphere>( point3( 0, 1, 0 ), 1.0, material1 ) );

	mat_ptr material2 = std::make_shared<lambertian>( color( 0.4, 0.2, 0.1 ) );
	world.add( std::make_shared<sphere>( point3( -4, 1, 0 ), 1.0, material2 ) );

	mat_ptr material3 = std::make_shared<metal>( color( 0.7, 0.6, 0.5 ), 0.0 );
	world.add( std::make_shared<sphere>( point3( 4, 1, 0 ), 1.0, material3 ) );

	return world;
}