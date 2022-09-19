#pragma once
#include "life/civilization.h"
namespace life {


	struct civilization_high_pop : public civilization{
		std::vector<bool>* lifes = new std::vector<bool>(CIVILIZATION_SIZE * CIVILIZATION_SIZE); //
		std::vector<bool>* lifes_back_buffer = new std::vector<bool>(CIVILIZATION_SIZE * CIVILIZATION_SIZE);

		civilization_high_pop(coordinate coord, world * the_world);
		~civilization_high_pop();
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
		bool get_life(coordinate const& life_cord);
	};
}