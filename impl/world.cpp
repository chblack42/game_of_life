#include <memory>
#include <vector>
#include <map>
#include <unordered_map>
#include <assert.h> 
#include <string>
#include <iostream>
#include "life/world.h"
#include "life/civilization_.h"
namespace life {

	struct world {
		std::unordered_map<coordinate, civilization *, coordinate::hash_fn> map;

		void populate(std::vector<coordinate> const& population)
		{
			std::unordered_map<coordinate, civilization *>::const_iterator hint = map.end();
			for (std::vector<coordinate>::const_iterator begin = population.begin(), end = population.end(); begin != end; ++begin)
			{
				coordinate coord = *begin;

				coordinate cloest_civilization = { ((coord.x / CIVILIZATION_SIZE) + (coord.x < 0 && coord.x > INT64_MIN && coord.x % CIVILIZATION_SIZE != 0 ? -1:0)) * CIVILIZATION_SIZE,
					((coord.y / CIVILIZATION_SIZE) + (coord.y < 0 && coord.y > INT64_MIN && coord.y % CIVILIZATION_SIZE != 0 ? -1 : 0)) * CIVILIZATION_SIZE };
				std::unordered_map<coordinate, civilization *>::const_iterator it = map.find(cloest_civilization);
				civilization * civ;
				if (it == map.end())
				{
					std::unordered_map<coordinate, civilization *>::const_iterator result = map.insert(hint,std::make_pair(cloest_civilization, new civilization_<std::vector<bool>>(cloest_civilization, this)));
					civ = result->second;
					hint = result;
				}
				else
				{
					civ = it->second;
					hint = it;
				}
				assert(civ);
				civ->add_life({ std::abs(coord.x - cloest_civilization.x),std::abs(coord.y - cloest_civilization.y) });
			}
		}

		civilization * add_new_civilization(coordinate coord)
		{
			std::unordered_map<coordinate, civilization *>::const_iterator map_it = map.find(coord);
			assert(map_it == map.end());
			civilization* civ = new civilization_<std::vector<bool>>(coord, this);
			map.insert(map_it, std::make_pair(civ->coord, civ));
			return civ;
		}

		void simulate()
		{
			std::vector<std::unordered_map<coordinate, civilization*>::const_iterator> to_delete;
			for (std::unordered_map<coordinate, civilization*>::const_iterator begin = map.begin(), end = map.end(); begin != end; ++begin)
			{
				if (begin->second->spawning)
					continue;

				if (begin->second->simulate()==0)
				{
					to_delete.push_back(begin);
				}
			}

			for (std::vector<std::unordered_map<coordinate, civilization*>::const_iterator>::const_iterator begin = to_delete.begin(), end = to_delete.end(); begin != end; ++begin)
			{
				delete (*begin)->second;
				map.erase((*begin));
			}

			for (std::unordered_map<coordinate, civilization*>::const_iterator begin = map.begin(), end = map.end(); begin != end; ++begin)
			{
				begin->second->post_simulate();
				begin->second->spawning = false;
			}
		}

		~world() {
			for (std::unordered_map<coordinate, civilization*>::const_iterator begin = map.begin(), end = map.end(); begin != end; ++begin)
			{
				delete begin->second;
			}
		}

		void to_cout()
		{
			std::cout << "#Life 1.06" << std::endl;
			for (std::unordered_map<coordinate, civilization*>::const_iterator begin = map.begin(), end = map.end(); begin != end; ++begin)
			{
				begin->second->to_cout();
			}
		}

		std::string to_string()
		{
			std::string result  = "#Life 1.06\n";
			for (std::unordered_map<coordinate, civilization*>::const_iterator begin = map.begin(), end = map.end(); begin != end; ++begin)
			{
				result += begin->second->to_string();
			}
			return result;
		}
	};


	std::shared_ptr<world> create_world(std::vector<coordinate> const& population)
	{
		std::shared_ptr<world> t = std::make_shared<world>();
		t->populate(population);
		return t;
	}
	civilization* add_new_civilization(world * w, coordinate coord)
	{
		return w->add_new_civilization(coord);
	}
	void simulate(world * w)
	{
		w->simulate();
	}
	std::unordered_map<coordinate, civilization*, coordinate::hash_fn> const & get_world_map(world * w)
	{
		return w->map;
	}
	void to_cout(world * w) 
	{
		w->to_cout();
	}
	std::string to_string(world* w) 
	{
		return w->to_string();
	}
	
}