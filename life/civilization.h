#pragma once
#include <unordered_map>
#include "life/coordinate.h"
#include <assert.h>

namespace life {
	struct world;


	enum class status : unsigned char
	{
		dead,
		alive,
		uncharted
	};
	const int NUM_NEIGHBORS = 9;
	const int64_t CIVILIZATION_SIZE = 16;
	int const NEW_LIFE_AMOUNT = 3;


	struct civilization {
		coordinate coord;
		world * the_world;
		bool spawning = false;
		civilization(coordinate coord, world * the_world) :
			coord(coord),
			the_world(the_world) 
		{
			assert(CIVILIZATION_SIZE != 0);
			assert((CIVILIZATION_SIZE & (CIVILIZATION_SIZE - 1)) == 0);
			assert(CIVILIZATION_SIZE != UINT64_MAX);
		}

		virtual uint64_t simulate() = 0;
		virtual void post_simulate() = 0;
		virtual void to_cout() = 0;
		virtual void check_surrounding_life(coordinate const& life_coord) = 0;
		virtual std::string to_string() = 0;
		virtual void add_life(coordinate const& life_coord) = 0;
		virtual bool get_life(coordinate const& life_coord) = 0;
	};
	
}
