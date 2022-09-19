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
		void corner_case(status neighbors[9], coordinate const& life_coord);
		void check_and_spawn_external_life(status neighborhood[9], coordinate const& life_cord);
		status find_external_life(coordinate const& life_coord, coordinate const& coord_step, bool check_life = false);
		int discover_neighborhood(coordinate const& life_cord);
		int border_life(coordinate const& life_cord);
		void check_surrounding_life(coordinate const& life_cord);
		int inner_life(coordinate const& life_cord);
		uint64_t simulate();
		void post_simulate();
		void to_cout();
		void to_cout_bck();
		std::string to_string();
		void add_life(coordinate const& life_cord);
		void set_upcoming_life(coordinate const& life_cord, bool living);
		bool simulate_coord(coordinate const& life);
		bool get_life(coordinate const& life_cord);
	};
}