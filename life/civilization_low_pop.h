#pragma once
#pragma once
#include "life/civilization.h"
namespace life {




	template <typename T>  struct civilization_low_pop : public civilization {
		T * lifes; //
		T * lifes_back_buffer;

		civilization_low_pop<T>(coordinate coord, world* the_world);
		~civilization_low_pop();
		void corner_case(neighbor neighbors[9], coordinate const& life_coord);
		void check_and_spawn_external_life(neighbor neighbors[9], coordinate const& life_cord);
		neighbor find_external_life(coordinate const& life_cord, coordinate const& coord_step);
		int discover_neighborhood(coordinate const& life_cord);
		int boarder_life(coordinate const& life_cord);
		int inner_life(coordinate const& life_cord);
		uint64_t simulate();
		void post_simulate();
		void to_cout();
		std::string to_string();
		void add_life(coordinate const& life_cord);
		void set_upcoming_life(coordinate const& life_cord, bool living);
		bool simulate_coord(coordinate const& life);
		bool get_life(coordinate const& life_cord);
	};
}