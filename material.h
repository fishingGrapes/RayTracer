#pragma once

#include "evals.h"
#include "color.h"
#include "ray.h"

struct hit_record;

class material
{
public:
	virtual bool scatter( const ray& r_in, const hit_record& hit, OUT color& attenuation, OUT ray& r_scattered ) const = 0;
};

using mat_ptr = std::shared_ptr<material>;
