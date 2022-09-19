#pragma once
#pragma once
#include "life/civilization.h"
namespace life {

	coordinate const BOTTOM_LEFT{ 0, 0 };
	coordinate const BOTTOM_RIGHT{ CIVILIZATION_SIZE - 1, 0 };
	coordinate const TOP_LEFT{ 0, CIVILIZATION_SIZE - 1 };
	coordinate const TOP_RIGHT{ CIVILIZATION_SIZE - 1, CIVILIZATION_SIZE - 1 };

	template <typename T>  struct civilization_ : public civilization {
		T * lifes; //
		T * lifes_back_buffer;

		civilization_<T>(coordinate coord, world* the_world);
		~civilization_();

		//adds a life to the current life container
		void add_life(coordinate const& life_coord);

		// retrieves the value of the life at the coordinate
		bool get_life(coordinate const& life_coord);

		// adds a life to the currently renderering life container
		void set_upcoming_life(coordinate const& life_coord, bool living);

		// specific case for testing the corners of our civilization and has to deal with external lives)
		void corner_case(status neighbors[9], coordinate const& life_coord); 

		//A simpler edge case like corner_case but only need to worry about one point when dealing with external lives
		int border_life(coordinate const& life_coord);

		// when dealing with a point that is outside our civlization see if we need to spawn a new civilization when 3 lifes are around
		void check_and_spawn_external_life(status neighborhood[9], coordinate const& life_coord);

		// check for life in other civilizations
		status find_external_life(coordinate const& life_coord, coordinate const& coord_step, bool check_life = false);

		// figure out who is living around you
		int discover_neighborhood(coordinate const& life_coord);

		// for data structures that don't check every position, this call will check if a coordinate is going to come to life (should only be called if you know there is an adjacent life)
		void check_surrounding_life(coordinate const& life_coord);

		// The most common case, since we inside we know we can safely check all coordinates around us
		int inner_life(coordinate const& life_coord);

		// simulates 1 round of the game of life for this civilization
		uint64_t simulate();
		
		// used to clear/swap memory and prepare state for the next round of simulation
		void post_simulate();

		// prints the coordinates in life 1.06 format to cout
		void to_cout();

		// prints the coordinates in life 1.06 format to string 
		std::string to_string();

		//runs the simulation at the coordinate
		bool simulate_coord(coordinate const& life);

	};
}