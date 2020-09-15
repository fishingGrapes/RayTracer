#pragma once

#include <memory>
class material;

class shadeable
{
public:
	std::shared_ptr<material> MaterialPtr;

public:
	shadeable( ) = delete;

	shadeable( std::shared_ptr<material> mat_ptr )
		: MaterialPtr( mat_ptr )
	{
	}
};