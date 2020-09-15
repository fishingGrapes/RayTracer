#pragma once

#include <memory>

#include "ray.h"
#include "evals.h"

class material;

struct hit_record
{
public:
	point3 Point;
	double_t Time;
	vec3 Normal;
	bool IsFrontFace;
	std::shared_ptr<material> MaterialPtr;

public:

	//we always want the normal to point outward irrespective of the origin of the ray
	// i.e always in the opposite diretion of the ray
	inline void set_face_normal( const ray& r, const vec3& outward_normal )
	{
		//is the ray and the normal in opposite directions?
		IsFrontFace = vec3::dot( r.Direction, outward_normal ) < 0.0;
		Normal = IsFrontFace ? outward_normal : -outward_normal;
	}

};

class hittable
{
public:
	virtual bool hit( const ray& r, double_t t_min, double_t t_max, OUT hit_record& record ) const = 0;
};