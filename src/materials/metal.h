#pragma once

#include "material.h"
#include "hittable.h"

class metal : public material
{
public:
	color Albedo;
	double_t Fuzziness;

public:

	metal( ) = delete;
	metal( const color& albedo, double_t fuzziness )
		:Albedo( albedo ), Fuzziness( fuzziness )
	{
	}

	virtual bool scatter( const ray& r_in, const hit_record& hit, OUT color& attenuation, OUT ray& r_scattered ) const override
	{
		vec3 reflected = reflect( r_in.Direction.normalized( ), hit.Normal );
		r_scattered = ray( hit.Point, reflected + Fuzziness * random_in_unit_sphere( ) );
		attenuation = Albedo;

		return ( vec3::dot( r_scattered.Direction, hit.Normal ) > 0.0 );
	}
};
