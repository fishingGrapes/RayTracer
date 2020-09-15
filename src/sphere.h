#pragma once


#include "hittable.h"
#include "shadeable.h"

#include "evals.h"

class sphere : public hittable, shadeable
{

public:
	point3 Center;
	double_t Radius;

public:

	sphere( point3 center, double_t radius, std::shared_ptr<material> mat_ptr )
		:shadeable( mat_ptr ), Center( center ), Radius( radius )
	{
	}

	virtual bool hit( const ray& r, double_t t_min, double_t t_max, OUT hit_record& record ) const override;
};

bool sphere::hit( const ray& r, double_t t_min, double_t t_max, OUT hit_record& record ) const
{
	vec3 oc = r.Origin - Center;

	double_t a = ( r.Direction ).length_squared( );
	double_t half_b = vec3::dot( oc, r.Direction );
	double_t c = oc.length_squared( ) - ( Radius * Radius );

	double_t discriminant = half_b * half_b - a * c;

	if (discriminant > 0.0)
	{
		double_t root = std::sqrt( discriminant );

		//first root
		double_t t = ( -half_b - root ) / a;
		if (t < t_max && t > t_min)
		{
			record.Time = t;
			record.Point = r.at( t );
			record.set_face_normal( r, ( ( record.Point - Center ) / Radius ) );
			record.MaterialPtr = MaterialPtr;

			return true;
		}

		//second root
		t = ( -half_b + root ) / a;
		if (t < t_max && t > t_min)
		{
			record.Time = t;
			record.Point = r.at( t );
			record.set_face_normal( r, ( ( record.Point - Center ) / Radius ) );
			record.MaterialPtr = MaterialPtr;

			return true;
		}
	}

	return false;
}

