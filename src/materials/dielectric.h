#pragma once

#include "material.h"
#include "hittable.h"


constexpr double_t eta_air = 1.0;

class dielectric : public material
{
public:
	double_t RefractiveIndex;


public:

	dielectric( ) = delete;
	dielectric( double_t ref_index )
		: RefractiveIndex( ref_index )
	{
	}

	virtual bool scatter( const ray& r_in, const hit_record& hit, OUT color& attenuation, OUT ray& r_scattered ) const override
	{
		attenuation = color( 1.0 );
		// 1.0 is the refractive index of air
		// If the ray is from oustide(in the atmosphere) to the dieletric, then eta_incident is the refratcive index of air
		// otherwise, etai is the refractive index of the dielectric
		double_t etai_over_etat = hit.IsFrontFace ? ( eta_air / RefractiveIndex ) : ( RefractiveIndex / eta_air );

		vec3 unit_direction = r_in.Direction.normalized( );

		double_t cos_theta = std::min( vec3::dot( -unit_direction, hit.Normal ), 1.0 );
		double_t sin_theta = std::sqrt( 1 - ( cos_theta * cos_theta ) );

		// According to Snell's law, sin (theta)' = eta_incident / eta_other * sin (theta)
		// but if eta_incident > eta_other, the sin (theta)' will be greater than 1.0, which cannot exist.
		//So, we refact the ray instead, and this called Total Interanl Reflection.
		if (etai_over_etat * sin_theta > 1.0)
		{
			vec3 reflected = reflect( unit_direction, hit.Normal );
			r_scattered = ray( hit.Point, reflected );

			return true;
		}

		double_t reflect_probablity = schlick_approximation( cos_theta, etai_over_etat );
		if (random_double( ) < reflect_probablity)
		{
			vec3 reflected = reflect( unit_direction, hit.Normal );
			r_scattered = ray( hit.Point, reflected );

			return true;
		}

		vec3 refracted = refract( unit_direction, hit.Normal, etai_over_etat );
		r_scattered = ray( hit.Point, refracted );

		return true;
	}

private:

	double_t schlick_approximation( double_t cosine, double_t ref_index ) const
	{
		double_t  r0 = ( 1 - ref_index ) / ( 1 + ref_index );
		r0 = r0 * r0;
		return r0 + ( 1 - r0 ) * std::pow( ( 1 - cosine ), 5 );
	}
};