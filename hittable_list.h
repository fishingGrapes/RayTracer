#pragma once

#include "hittable.h"

#include <memory>
#include <vector>

class hittable_list : public hittable
{
public:

	void add( std::shared_ptr<hittable> object )
	{
		objects_.push_back( object );
	}

	void clear( )
	{
		objects_.clear( );
	}

	virtual bool hit( const ray& r, double_t t_min, double_t t_max, OUT hit_record& record ) const;


private:
	std::vector<std::shared_ptr<hittable>> objects_;
};

bool hittable_list::hit( const ray& r, double_t t_min, double_t t_max, OUT hit_record& record ) const
{
	hit_record temp_record;
	bool is_hit = false;
	double_t closest_hit = t_max;


	for (const std::shared_ptr<hittable>& object : objects_)
	{
		if (object->hit( r, t_min, closest_hit, OUT temp_record ))
		{
			is_hit = true;
			closest_hit = temp_record.Time;
			record = temp_record;
		}
	}

	return is_hit;
}
