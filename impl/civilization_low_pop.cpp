#include <memory>
#include <vector>
#include <map>
#include <unordered_map>
#include <assert.h> 
#include <string>
#include <iostream>
#include "life/world.h"
#include "life/civilization_low_pop.h"
#include <sstream>
#include <bitset>
namespace life {
	namespace {
		namespace locations {
			enum enum_ : short {
				top_left = 0,
				top_middle = 1,
				top_right = 2,
				middle_left = 3,
				middle = 4,
				middle_right = 5,
				bottom_left = 6,
				bottom_middle = 7,
				bottom_right = 8
			};
		}
		inline int64_t get_index(int64_t x, int64_t y)
		{
			return x + (CIVILIZATION_SIZE * y);
		}
		inline void spawn_civilization(world* the_world, coordinate const& adjacent_civilization, coordinate const& new_life)
		{
			if (adjacent_civilization == coordinate{ 8,8 })
			{
				int i = 0;
			}
			std::unordered_map<coordinate, civilization*, coordinate::hash_fn> map = get_world_map(the_world);
			std::unordered_map < coordinate, civilization*>::const_iterator it = map.find(adjacent_civilization);
			civilization* civ;
			if (it == map.end())
			{
				civ = new civilization_low_pop<std::bitset<CIVILIZATION_SIZE * CIVILIZATION_SIZE>>(adjacent_civilization, the_world);
				add_new_civilization(the_world, civ);
			}
			else
				civ = it->second;
			civ->spawning = true;
			civ->add_life(new_life);
		}

	}
	coordinate const BOTTOM_LEFT{ 0, 0 };
	coordinate const BOTTOM_RIGHT{ CIVILIZATION_SIZE - 1, 0 };
	coordinate const TOP_LEFT{ 0, CIVILIZATION_SIZE - 1 };
	coordinate const TOP_RIGHT{ CIVILIZATION_SIZE - 1, CIVILIZATION_SIZE - 1 };
	int const NEW_LIFE_AMOUNT = 3;

	template <> civilization_low_pop<std::vector<bool>>::civilization_low_pop(coordinate coord, world* the_world) :
		civilization(coord, the_world)
	{
		lifes = new std::vector<bool>(CIVILIZATION_SIZE * CIVILIZATION_SIZE);
		lifes_back_buffer = new std::vector<bool>(CIVILIZATION_SIZE * CIVILIZATION_SIZE);
	}

	template <typename T> civilization_low_pop<T>::civilization_low_pop(coordinate coord, world* the_world) :
		civilization(coord, the_world)
	{
		lifes = new T();
		lifes_back_buffer = new T();
	}

	template <typename T> void civilization_low_pop<T>::add_life(coordinate const& life_coord)
	{
		if (spawning)
			(*lifes_back_buffer)[get_index(life_coord.x, life_coord.y)] = true;
		else
			(*lifes)[get_index(life_coord.x, life_coord.y)] = true;
	}

	template <> void civilization_low_pop<std::unordered_map<coordinate, bool, coordinate::hash_fn>>::add_life(coordinate const& life_coord)
	{
		if (spawning)
			(*lifes_back_buffer)[life_coord] = true;
		else
			(*lifes)[life_coord] = true;
	}

	template <typename T> void civilization_low_pop<T>::set_upcoming_life(coordinate const& life_coord, bool living)
	{
		(*lifes_back_buffer)[get_index(life_coord.x, life_coord.y)] = living;
	}

	template <> void civilization_low_pop<std::unordered_map<coordinate,bool,coordinate::hash_fn>>::set_upcoming_life(coordinate const& life_coord, bool living)
	{
		if(living)
			(*lifes_back_buffer)[life_coord] = true;
	}

	template <typename T> bool civilization_low_pop<T>::get_life(coordinate const& life_coord)
	{
		return (*lifes)[get_index(life_coord.x, life_coord.y)];
	}

	template <> bool civilization_low_pop<std::unordered_map<coordinate, bool, coordinate::hash_fn >>::get_life(coordinate const& life_coord)
	{
		if (lifes->find(life_coord) == lifes->end())
			return false;
		return (*lifes)[life_coord];
	}

	template <typename T> void civilization_low_pop<T>::corner_case(neighbor neighbors[9], coordinate const& life_coord)
	{

		for (int i = 0; i != NUM_NEIGHBORS; ++i)
		{
			if (neighbors[i] == neighbor::uncharted) // if out of bound this would be neightbor::dead
			{
				switch (i) {
				case 0: // top left
				{
					if (coord.x == INT64_MIN)
						break;
					int num_alive = (neighbors[locations::top_middle] == neighbor::alive) + (neighbors[locations::top_right] == neighbor::alive) + (neighbors[locations::middle] == neighbor::alive);
					if (num_alive == NEW_LIFE_AMOUNT)
					{
						spawn_civilization(the_world, { coord.x - CIVILIZATION_SIZE, coord.y + CIVILIZATION_SIZE }, BOTTOM_RIGHT);
					}
					break;
				}
				case 1:// top middle
				{
					if (coord.y + (CIVILIZATION_SIZE - 1) == INT64_MAX)
						break;
					int num_alive = (neighbors[locations::top_left] == neighbor::alive) + +(neighbors[locations::top_right] == neighbor::alive) + (neighbors[locations::middle_left] == neighbor::alive)
						+ (neighbors[locations::middle] == neighbor::alive) + (neighbors[locations::middle_right] == neighbor::alive);
					if (num_alive == NEW_LIFE_AMOUNT)
					{
						coordinate new_life = life_coord.x == 0 ? BOTTOM_LEFT : BOTTOM_RIGHT;
						spawn_civilization(the_world, { coord.x, coord.y + CIVILIZATION_SIZE }, new_life);
					}
					break;
				}
				case 2: // top right
				{
					if (coord.y + (CIVILIZATION_SIZE - 1) == INT64_MAX)
						break;
					int num_alive = (neighbors[locations::top_middle] == neighbor::alive) + (neighbors[locations::middle] == neighbor::alive) + (neighbors[locations::middle_right] == neighbor::alive);
					if (num_alive == NEW_LIFE_AMOUNT)
					{
						spawn_civilization(the_world, { coord.x + CIVILIZATION_SIZE, coord.y + CIVILIZATION_SIZE }, BOTTOM_LEFT);
					}
					break;
				}
				case 3:// middle left
				{
					if (coord.x == INT64_MIN)
						break;
					int num_alive = (neighbors[locations::top_left] == neighbor::alive) + (neighbors[locations::top_middle] == neighbor::alive) + (neighbors[locations::middle] == neighbor::alive)
						+ (neighbors[locations::bottom_left] == neighbor::alive) + (neighbors[locations::bottom_middle] == neighbor::alive);
					if (num_alive == NEW_LIFE_AMOUNT)
					{
						coordinate new_life = life_coord.y == 0 ? BOTTOM_RIGHT : TOP_RIGHT;
						spawn_civilization(the_world, { coord.x - CIVILIZATION_SIZE, coord.y }, new_life);
					}
					break;
				}
				case 5: // middle right
				{
					if (coord.x + (CIVILIZATION_SIZE - 1) == INT64_MAX)
						break;
					int num_alive = (neighbors[locations::top_middle] == neighbor::alive) + (neighbors[locations::top_right] == neighbor::alive)
						+ (neighbors[locations::middle] == neighbor::alive) + (neighbors[locations::bottom_middle] == neighbor::alive) + (neighbors[locations::bottom_right] == neighbor::alive);
					if (num_alive == NEW_LIFE_AMOUNT)
					{
						coordinate new_life = life_coord.y == 0 ? BOTTOM_LEFT : TOP_LEFT;
						spawn_civilization(the_world, { coord.x + CIVILIZATION_SIZE, coord.y }, new_life);
					}
					break;
				}
				case 6: // bottom left
				{
					if (coord.x == INT64_MIN)
						break;
					int num_alive = (neighbors[locations::middle_left] == neighbor::alive) + (neighbors[locations::middle] == neighbor::alive) + (neighbors[locations::bottom_middle] == neighbor::alive);
					if (num_alive == NEW_LIFE_AMOUNT)
					{
						spawn_civilization(the_world, { coord.x - CIVILIZATION_SIZE, coord.y - CIVILIZATION_SIZE }, TOP_RIGHT);
					}
					break;
				}
				case 7:
				{
					if (coord.y == INT64_MIN)
						break;
					int num_alive = (neighbors[locations::middle_left] == neighbor::alive) + (neighbors[locations::middle] == neighbor::alive)
						+ (neighbors[locations::middle_right] == neighbor::alive) + (neighbors[locations::bottom_left] == neighbor::alive) + (neighbors[locations::bottom_right] == neighbor::alive);
					if (num_alive == NEW_LIFE_AMOUNT)
					{
						coordinate new_life = life_coord.x == 0 ? TOP_LEFT : TOP_RIGHT;
						spawn_civilization(the_world, { coord.x, coord.y - CIVILIZATION_SIZE }, new_life);
					}
					break;
				}
				case 8:
				{
					if (coord.x + (CIVILIZATION_SIZE - 1) == INT64_MAX)
						break;
					int num_alive = (neighbors[locations::middle] == neighbor::alive) + (neighbors[locations::middle_right] == neighbor::alive) + (neighbors[locations::bottom_middle] == neighbor::alive);
					if (num_alive == NEW_LIFE_AMOUNT)
					{
						spawn_civilization(the_world, { coord.x + CIVILIZATION_SIZE, coord.y - CIVILIZATION_SIZE }, TOP_RIGHT);
					}
					break;
				}
				default: {}
				}
			}
		}

	}

	template <typename T> void civilization_low_pop<T>::check_and_spawn_external_life(neighbor neighbors[NUM_NEIGHBORS], coordinate const& life_coord)
	{
		int num_alive = get_life(life_coord);
		if (life_coord == TOP_LEFT || life_coord == TOP_RIGHT || life_coord == BOTTOM_RIGHT || life_coord == BOTTOM_LEFT) // corner case
		{
			corner_case(neighbors, life_coord);
		}
		else if (num_alive == 1)
		{
			coordinate adjacent_civ_coord = coord;
			coordinate adjacent_life_coord = life_coord;
			if (num_alive && life_coord.x == 0 || life_coord.x == CIVILIZATION_SIZE - 1) // left/right edge
			{
				if (coord.x == INT64_MIN || coord.x + (CIVILIZATION_SIZE - 1) == INT64_MAX)
					return;
				num_alive += get_life({ life_coord.x, life_coord.y - 1 });
				num_alive += get_life({life_coord.x, life_coord.y + 1});
				adjacent_civ_coord.x += life_coord.x == 0 ? -CIVILIZATION_SIZE : CIVILIZATION_SIZE;
				adjacent_life_coord.x = life_coord.x == 0 ? CIVILIZATION_SIZE - 1 : 0;
			}
			else if (num_alive && life_coord.y == 0 || life_coord.y == CIVILIZATION_SIZE - 1) // top/bottom edge
			{
				if (coord.y == INT64_MIN || coord.y + (CIVILIZATION_SIZE - 1) == INT64_MAX)
					return;
				num_alive += get_life({ life_coord.x - 1, life_coord.y });
				num_alive += get_life({ life_coord.x + 1, life_coord.y });
				adjacent_civ_coord.y += life_coord.y == 0 ? -CIVILIZATION_SIZE : CIVILIZATION_SIZE;
				adjacent_life_coord.y = life_coord.y == 0 ? CIVILIZATION_SIZE - 1 : 0;

			}
			if (num_alive == NEW_LIFE_AMOUNT)
			{
				spawn_civilization(the_world, adjacent_civ_coord, adjacent_life_coord);
			}
		}
	}

	template <typename T> neighbor civilization_low_pop<T>::find_external_life(coordinate const& life_coord, coordinate const& coord_step)
	{
		coordinate adjacent_texture_coord = coord;
		coordinate adjacent_life_coord = { life_coord.x + coord_step.x, life_coord.y + coord_step.y };


		if (adjacent_life_coord.x < 0) // left edge
		{
			adjacent_texture_coord.x -= CIVILIZATION_SIZE;
			adjacent_life_coord.x = CIVILIZATION_SIZE - 1;
		}
		else if (adjacent_life_coord.x == CIVILIZATION_SIZE) // right edge
		{
			adjacent_texture_coord.x += CIVILIZATION_SIZE;
			adjacent_life_coord.x = 0;
		}

		if (adjacent_life_coord.y < 0) // bottom edge
		{
			adjacent_texture_coord.y -= CIVILIZATION_SIZE;
			adjacent_life_coord.y = CIVILIZATION_SIZE - 1;
		}
		else if (adjacent_life_coord.y == CIVILIZATION_SIZE) // top edge
		{
			adjacent_texture_coord.y += CIVILIZATION_SIZE;
			adjacent_life_coord.y = 0;
		}
		if (life_coord == coordinate{ 0, 2 })
		{
			int i = 0;
		}
		std::unordered_map<coordinate, civilization*, coordinate::hash_fn> const& world_map = get_world_map(the_world);
		std::unordered_map<coordinate, civilization*>::const_iterator adjacent_civilization = world_map.find(adjacent_texture_coord);
		if (world_map.end() == adjacent_civilization || adjacent_civilization->second->spawning)
		{
			return neighbor::uncharted; // adjacent civ didn't exist or is new return uncharted
		}

		return (*adjacent_civilization->second).get_life(adjacent_life_coord) ? neighbor::alive : neighbor::dead;
	}


	template <typename T> int civilization_low_pop<T>::boarder_life(coordinate const& life_coord) {
		int num_alive_adjacent = 0;
		neighbor neighborhood_arr[9];
		int neighborhood_idx = 0;
		bool has_uncharted = false;
		if (get_life(life_coord))
			int i = 0;

		for (int y = 1; y != -2; --y)// move top left to bottom right
		{
			for (int x = -1; x != 2; ++x)
			{
				coordinate adjacent_coord{ life_coord.x + x, life_coord.y + y };
				if (x == 0 && y == 0)
				{
					neighborhood_arr[neighborhood_idx] = get_life(life_coord) ? neighbor::alive : neighbor::dead;
				}
				else if ((adjacent_coord.x == CIVILIZATION_SIZE && coord.x == INT64_MAX) || (adjacent_coord.x < 0 && coord.x == INT64_MIN) ||
					(adjacent_coord.y == CIVILIZATION_SIZE && coord.y == INT64_MAX) || (adjacent_coord.y < 0 && coord.y == INT64_MIN))
				{
					neighborhood_arr[neighborhood_idx] = neighbor::dead; //outside the scope of the problem
				}
				else if (adjacent_coord.x == CIVILIZATION_SIZE || adjacent_coord.x < 0 ||
					adjacent_coord.y == CIVILIZATION_SIZE || adjacent_coord.y < 0)
				{
					neighbor value = find_external_life(life_coord, { x,y });

					neighborhood_arr[neighborhood_idx] = value;
					if (value == neighbor::alive)
						num_alive_adjacent++;
					else if (value == neighbor::uncharted)
						has_uncharted = true;
				}
				else
				{
					assert(adjacent_coord.x <= CIVILIZATION_SIZE);
					assert(adjacent_coord.y <= CIVILIZATION_SIZE);
					num_alive_adjacent += get_life(adjacent_coord);
					neighborhood_arr[neighborhood_idx] = get_life(adjacent_coord) ? neighbor::alive : neighbor::dead;

				}

				neighborhood_idx++;
			}

		}
		if (has_uncharted)
		{
			check_and_spawn_external_life(neighborhood_arr, life_coord);
		}
		return num_alive_adjacent;
	};

	template <typename T> int civilization_low_pop<T>::inner_life(coordinate const& life_coord)
	{
		int num_alive_adjacent = 0;
		int64_t x = life_coord.x;
		int64_t y = life_coord.y;
		// just hard code, no need for the overhead with increments and loops
		num_alive_adjacent += get_life({ x - 1, y - 1 });
		num_alive_adjacent += get_life({ x - 1, y });
		num_alive_adjacent += get_life({ x - 1, y + 1 });
		num_alive_adjacent += get_life({ x, y - 1 });
		num_alive_adjacent += get_life({ x, y + 1 });
		num_alive_adjacent += get_life({ x + 1, y - 1 });
		num_alive_adjacent += get_life({ x + 1, y });
		num_alive_adjacent += get_life({ x + 1, y + 1 });

		return num_alive_adjacent;
	}

	template <> int civilization_low_pop<std::unordered_map<coordinate,bool,coordinate::hash_fn>>::inner_life(coordinate const& life_coord)
	{
		int num_alive_adjacent = 0;
		int64_t x = life_coord.x;
		int64_t y = life_coord.y;
		// just hard code, no need for the overhead with increments and loops
		num_alive_adjacent += get_life({ x - 1, y - 1 });
		num_alive_adjacent += get_life({ x - 1, y });
		num_alive_adjacent += get_life({ x - 1, y + 1 });
		num_alive_adjacent += get_life({ x, y - 1 });
		num_alive_adjacent += get_life({ x, y + 1 });
		num_alive_adjacent += get_life({ x + 1, y - 1 });
		num_alive_adjacent += get_life({ x + 1, y });
		num_alive_adjacent += get_life({ x + 1, y + 1 });

		return num_alive_adjacent;
	}

	template <typename T> int civilization_low_pop<T>::discover_neighborhood(coordinate const& life_coord)
	{
		int64_t x = life_coord.x;
		int64_t y = life_coord.y;
		if (x == 0 || y == 0 || x == CIVILIZATION_SIZE - 1 || y == CIVILIZATION_SIZE - 1)
		{
			return boarder_life(life_coord);
		}
		return inner_life(life_coord);
	}

	template <typename T> bool civilization_low_pop<T>::simulate_coord(coordinate const & life)
	{
		int num_alive_adjacent = 0;
		num_alive_adjacent += discover_neighborhood(life);


		bool living = get_life(life);

		if (living && num_alive_adjacent < 2 || num_alive_adjacent > 3)//we are now dead
		{
			set_upcoming_life(life, false);
			return false;
		}
		else if (!living && num_alive_adjacent == 3) //new life
		{
			set_upcoming_life(life, true);
			return true;
		}
		else if (living) //living on to the next generation
		{
			set_upcoming_life(life, true);
			return true;
		}
		else
			set_upcoming_life(life, false);
		return false;
	}
	// return true if there are living things in this block
	template <typename T> uint64_t civilization_low_pop<T>::simulate()
	{
		uint64_t current_lifes = 0;
		for (uint64_t life_idx = 0; life_idx != lifes->size(); ++life_idx)
		{
			int64_t x = life_idx % CIVILIZATION_SIZE;
			int64_t y = life_idx / CIVILIZATION_SIZE;
			coordinate life_coord{ x, y };
			current_lifes += simulate_coord(life_coord);
		}
		return current_lifes;
	}

	template <> uint64_t civilization_low_pop<std::unordered_map<coordinate,bool,coordinate::hash_fn>>::simulate()
	{

		for (std::unordered_map<coordinate, bool, coordinate::hash_fn>::const_iterator it = lifes->begin(), end = lifes->end(); it != end; ++it)
		{
			simulate_coord(it->first);
		}
		return lifes->size();
	}

	template <typename T> civilization_low_pop<T>::~civilization_low_pop() {
		delete lifes;
		delete lifes_back_buffer;
	}

	template <typename T> void civilization_low_pop<T>::to_cout()
	{
		for (int64_t life_idx = 0; life_idx != lifes->size();++life_idx)
		{
			int64_t x = life_idx % CIVILIZATION_SIZE;
			int64_t y = life_idx / CIVILIZATION_SIZE;
			if (get_life(coordinate{ x, y }))
			{
				std::cout << coord.x + x << " " << coord.y + y << std::endl;
			}
		}
	}
	
	template <typename T> std::string civilization_low_pop<T>::to_string()
	{
		std::string result = "";
		for (int64_t life_idx = 0; life_idx != lifes->size();++life_idx)
		{
			int64_t x = life_idx % CIVILIZATION_SIZE;
			int64_t y = life_idx / CIVILIZATION_SIZE;
			if (get_life(coordinate{ x, y }))
			{
				std::stringstream ss;
				ss << coord.x + x << " " << coord.y + y << std::endl;
				result += ss.str();
			}
		}
		return result;
	}

	template <> void civilization_low_pop<std::unordered_map<coordinate,bool, coordinate::hash_fn>>::post_simulate()
	{
		lifes->swap(*lifes_back_buffer);
		lifes_back_buffer->clear();
	}

	template <typename T> void civilization_low_pop<T>::post_simulate()
	{
		lifes->swap(*lifes_back_buffer);
		std::fill(lifes_back_buffer->begin(), lifes_back_buffer->end(), 0);

	}
	template <> void civilization_low_pop<std::bitset<CIVILIZATION_SIZE * CIVILIZATION_SIZE>>::post_simulate()
	{
		std::bitset<CIVILIZATION_SIZE* CIVILIZATION_SIZE> * tmp = lifes;
		lifes = lifes_back_buffer;
		lifes_back_buffer = tmp;
		lifes_back_buffer->reset();
	}
}
