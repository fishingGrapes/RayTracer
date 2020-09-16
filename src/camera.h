#pragma once

#include "ray.h"

class camera
{
public:

	camera( ) = delete;
	camera( point3 look_from, point3 look_at, vec3 world_up, double_t vfov, double_t aspect_ratio, double_t aperture, double_t focus_distance )
	{

		double_t theta = degrees_to_radians( vfov );
		double_t h = std::tan( theta / 2.0 );


		double_t viewport_height = 2.0 * h;
		double_t viewport_width = viewport_height * aspect_ratio;

		inverse_cam_direction_ = ( look_from - look_at ).normalized( );
		cam_right_ = vec3::cross( world_up, inverse_cam_direction_ ).normalized( );
		cam_up_ = vec3::cross( inverse_cam_direction_, cam_right_ );

		origin_ = look_from;
		horizontal_ = focus_distance * viewport_width * cam_right_;
		vertical_ = focus_distance * viewport_height * cam_up_;

		//raytracing starts from this point
		lower_left_ = origin_ - ( horizontal_ * 0.5 ) - ( vertical_ * 0.5 ) - ( focus_distance * inverse_cam_direction_ );

		lens_radius_ = aperture / 2.0;
	}

	inline ray get_ray( double_t u, double_t v ) const
	{
		vec3 rd = lens_radius_ * random_in_unit_disk( );
		vec3 offset = cam_right_ * rd.X + cam_up_ * rd.Y;

		return ray( origin_ + offset, lower_left_ + u * horizontal_ + v * vertical_ - origin_ - offset );
	}

private:
	point3 origin_;
	vec3 horizontal_, vertical_;
	point3 lower_left_;
	vec3 inverse_cam_direction_, cam_right_, cam_up_;
	double_t lens_radius_;
};