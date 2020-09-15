#pragma once

#include "material.h"
#include "hittable.h"

class lambertian : public material
{

public:
	color Albedo;

public:
	lambertian( const color& albedo )
		:Albedo( albedo )
	{
	}

	virtual bool scatter( const ray& r_in, const hit_record& hit, OUT color& attenuation, OUT ray& r_scattered ) const override
	{
		vec3 scatter_direction = hit.Normal + random_unit_vec3( );
		r_scattered = ray( hit.Point, scatter_direction );
		attenuation = Albedo;

		return true;
	}
};