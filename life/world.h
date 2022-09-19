#pragma once
#include <vector>
#include "life/civilization.h"
#include "life/coordinate.h"
#include <memory>
#include <string>
namespace life {
	struct world;
	std::shared_ptr<world> create_world(std::vector<coordinate> const& population);
	std::unordered_map<coordinate, civilization*, coordinate::hash_fn> const & get_world_map(world * w);
	civilization* add_new_civilization(world * w, coordinate coord);
	void simulate(world * w);
	void to_cout(world * w);
	std::string to_string(world* w);
}