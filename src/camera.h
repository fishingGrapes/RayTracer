#pragma once

#include "ray.h"

class camera
{
public:
	camera( )
	{
		double_t aspect_ratio = 16.0 / 9.0;

		double_t viewport_height = 2.0;
		double_t viewport_width = viewport_height * aspect_ratio;
		double_t focal_length = 1.0;

		origin_ = point3( 0.0 );
		horizontal_ = vec3( viewport_width, 0, 0 );
		vertical_ = vec3( 0, viewport_height, 0 );

		//raytracing starts from this point
		lower_left_ = origin_ - ( horizontal_ * 0.5 ) - ( vertical_ * 0.5 ) - vec3( 0, 0, focal_length );
	}

	inline ray get_ray( double_t u, double_t v ) const
	{
		return ray( origin_, lower_left_ + u * horizontal_ + v * vertical_ - origin_ );
	}

private:
	point3 origin_;
	vec3 horizontal_;
	vec3 vertical_;
	point3 lower_left_;
};