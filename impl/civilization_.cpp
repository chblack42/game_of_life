#include <memory>
#include <vector>
#include <map>
#include <unordered_map>
#include <assert.h> 
#include <string>
#include <iostream>
#include "life/world.h"
#include "life/civilization_.h"
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
			std::unordered_map<coordinate, civilization*, coordinate::hash_fn> map = get_world_map(the_world);
			std::unordered_map < coordinate, civilization*>::const_iterator it = map.find(adjacent_civilization);
			civilization* civ;
			if (it == map.end())
			{
				civ = add_new_civilization(the_world, adjacent_civilization);
			}
			else
				civ = it->second;
			civ->spawning = true;
			civ->add_life(new_life);
		}

	}

	template <> civilization_<std::vector<bool>>::civilization_(coordinate coord, world* the_world) :
		civilization(coord, the_world)
	{
		lifes = new std::vector<bool>(CIVILIZATION_SIZE * CIVILIZATION_SIZE);
		lifes_back_buffer = new std::vector<bool>(CIVILIZATION_SIZE * CIVILIZATION_SIZE);
	}

	template <> civilization_<std::unordered_map<coordinate, bool, coordinate::hash_fn>>::civilization_(coordinate coord, world* the_world) :
		civilization(coord, the_world)
	{
		lifes = new std::unordered_map<coordinate, bool, coordinate::hash_fn>();
		lifes_back_buffer = new std::unordered_map<coordinate, bool, coordinate::hash_fn>();
	}

	template <typename T> civilization_<T>::civilization_(coordinate coord, world* the_world) :
		civilization(coord, the_world)
	{
		lifes = new T();
		lifes_back_buffer = new T();
	}

	template <typename T> void civilization_<T>::add_life(coordinate const& life_coord)
	{
		if (spawning)
			(*lifes_back_buffer)[get_index(life_coord.x, life_coord.y)] = true;
		else
			(*lifes)[get_index(life_coord.x, life_coord.y)] = true;
	}

	template <> void civilization_<std::unordered_map<coordinate, bool, coordinate::hash_fn>>::add_life(coordinate const& life_coord)
	{
		if (spawning)
			(*lifes_back_buffer)[life_coord] = true;
		else
			(*lifes)[life_coord] = true;
	}

	template <typename T> void civilization_<T>::set_upcoming_life(coordinate const& life_coord, bool living)
	{
		(*lifes_back_buffer)[get_index(life_coord.x, life_coord.y)] = living;
	}

	template <> void civilization_<std::unordered_map<coordinate, bool, coordinate::hash_fn>>::set_upcoming_life(coordinate const& life_coord, bool living)
	{
		if (living)
			(*lifes_back_buffer)[life_coord] = true;
	}

	template <typename T> bool civilization_<T>::get_life(coordinate const& life_coord)
	{
		return (*lifes)[get_index(life_coord.x, life_coord.y)];
	}

	template <> bool civilization_<std::unordered_map<coordinate, bool, coordinate::hash_fn >>::get_life(coordinate const& life_coord)
	{
		if (lifes->find(life_coord) == lifes->end())
			return false;
		return (*lifes)[life_coord];
	}

	template <typename T> void civilization_<T>::corner_case(status neighborhood[9], coordinate const& life_coord)
	{

		for (int i = 0; i != NUM_NEIGHBORS; ++i)
		{
			if (neighborhood[i] == status::uncharted) // if out of bound this would be neightbor::dead
			{
				switch (i) {
				case 0: // top left
				{
					if (coord.x == INT64_MIN)
						break;
					int num_alive = (neighborhood[locations::top_middle] == status::alive) + (neighborhood[locations::top_right] == status::alive) + (neighborhood[locations::middle] == status::alive);
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
					int num_alive = (neighborhood[locations::top_left] == status::alive) + +(neighborhood[locations::top_right] == status::alive) + (neighborhood[locations::middle_left] == status::alive)
						+ (neighborhood[locations::middle] == status::alive) + (neighborhood[locations::middle_right] == status::alive);
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
					int num_alive = (neighborhood[locations::top_middle] == status::alive) + (neighborhood[locations::middle] == status::alive) + (neighborhood[locations::middle_right] == status::alive);
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
					int num_alive = (neighborhood[locations::top_left] == status::alive) + (neighborhood[locations::top_middle] == status::alive) + (neighborhood[locations::middle] == status::alive)
						+ (neighborhood[locations::bottom_left] == status::alive) + (neighborhood[locations::bottom_middle] == status::alive);
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
					int num_alive = (neighborhood[locations::top_middle] == status::alive) + (neighborhood[locations::top_right] == status::alive)
						+ (neighborhood[locations::middle] == status::alive) + (neighborhood[locations::bottom_middle] == status::alive) + (neighborhood[locations::bottom_right] == status::alive);
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
					int num_alive = (neighborhood[locations::middle_left] == status::alive) + (neighborhood[locations::middle] == status::alive) + (neighborhood[locations::bottom_middle] == status::alive);
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
					int num_alive = (neighborhood[locations::middle_left] == status::alive) + (neighborhood[locations::middle] == status::alive)
						+ (neighborhood[locations::middle_right] == status::alive) + (neighborhood[locations::bottom_left] == status::alive) + (neighborhood[locations::bottom_right] == status::alive);
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
					int num_alive = (neighborhood[locations::middle] == status::alive) + (neighborhood[locations::middle_right] == status::alive) + (neighborhood[locations::bottom_middle] == status::alive);
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

	template <typename T> void civilization_<T>::check_and_spawn_external_life(status neighborhood[NUM_NEIGHBORS], coordinate const& life_coord)
	{
		int num_alive = get_life(life_coord);
		if (life_coord == TOP_LEFT || life_coord == TOP_RIGHT || life_coord == BOTTOM_RIGHT || life_coord == BOTTOM_LEFT) // corner case
		{
			corner_case(neighborhood, life_coord);
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
				num_alive += get_life({ life_coord.x, life_coord.y + 1 });
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

	template <typename T> status civilization_<T>::find_external_life(coordinate const& life_coord, coordinate const& coord_step, bool check_life)
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

		std::unordered_map<coordinate, civilization*, coordinate::hash_fn> const& world_map = get_world_map(the_world);
		std::unordered_map<coordinate, civilization*>::const_iterator adjacent_civilization = world_map.find(adjacent_texture_coord);
		if (world_map.end() == adjacent_civilization || adjacent_civilization->second->spawning)
		{
			return status::uncharted; // adjacent civ didn't exist or is new return uncharted
		}
		if (check_life)
			(*adjacent_civilization->second).check_surrounding_life(adjacent_life_coord);
		return (*adjacent_civilization->second).get_life(adjacent_life_coord) ? status::alive : status::dead;
	}



	template <typename T> int civilization_<T>::border_life(coordinate const& life_coord)
	{
		int num_alive_adjacent = 0;
		status neighborhood[9];
		int neighborhood_idx = 0;
		bool has_uncharted = false;

		for (int y = 1; y != -2; --y)// move top left to bottom right
		{
			for (int x = -1; x != 2; ++x)
			{
				assert(neighborhood_idx != 9);
				coordinate adjacent_coord{ life_coord.x + x, life_coord.y + y };
				if (x == 0 && y == 0)
				{
					neighborhood[neighborhood_idx] = get_life(life_coord) ? status::alive : status::dead;

				}
				else if ((adjacent_coord.x == CIVILIZATION_SIZE && (coord.x == INT64_MAX || coord.x + (CIVILIZATION_SIZE - 1) == INT64_MAX)) || (adjacent_coord.x < 0 && (coord.x == INT64_MIN || coord.x - CIVILIZATION_SIZE == INT64_MIN)) ||
					(adjacent_coord.y == CIVILIZATION_SIZE && (coord.y == INT64_MAX || coord.y + (CIVILIZATION_SIZE - 1) == INT64_MAX)) || (adjacent_coord.y < 0 && (coord.y == INT64_MIN || coord.y - CIVILIZATION_SIZE == INT64_MIN)))
				{
					neighborhood[neighborhood_idx] = status::dead; //outside the scope of the problem
				}
				else if (adjacent_coord.x == CIVILIZATION_SIZE || adjacent_coord.x < 0 ||
					adjacent_coord.y == CIVILIZATION_SIZE || adjacent_coord.y < 0)
				{
					status value = find_external_life(life_coord, { x,y },true);

					neighborhood[neighborhood_idx] = value;
					if (value == status::alive)
						num_alive_adjacent++;
					else if (value == status::uncharted)
						has_uncharted = true;
				}
				else
				{
					assert(adjacent_coord.x <= CIVILIZATION_SIZE);
					assert(adjacent_coord.y <= CIVILIZATION_SIZE);
					num_alive_adjacent += get_life(adjacent_coord);
					neighborhood[neighborhood_idx] = get_life(adjacent_coord) ? status::alive : status::dead;
					
					check_surrounding_life(adjacent_coord);
				}

				neighborhood_idx++;
			}

		}
		if (has_uncharted)
		{
			check_and_spawn_external_life(neighborhood, life_coord);
		}
		return num_alive_adjacent;
	}

	template <> void civilization_<std::unordered_map<coordinate,bool,coordinate::hash_fn>>::check_surrounding_life(coordinate const& life_coord)
	{
		int num_arround = 0;
		for (int y = 1; y != -2; --y)// move top left to bottom right
		{
			for (int x = -1; x != 2; ++x)
			{
				if (x == 0 && y == 0)
				{
					continue;
				}
				coordinate adjacent_coord{ life_coord.x + x, life_coord.y + y };
				if ((adjacent_coord.x == CIVILIZATION_SIZE && (coord.x == INT64_MAX || coord.x + (CIVILIZATION_SIZE - 1) == INT64_MAX)) || (adjacent_coord.x < 0 && (coord.x == INT64_MIN || coord.x - CIVILIZATION_SIZE == INT64_MIN)) ||
					(adjacent_coord.y == CIVILIZATION_SIZE && (coord.y == INT64_MAX || coord.y + (CIVILIZATION_SIZE - 1) == INT64_MAX)) || (adjacent_coord.y < 0 && (coord.y == INT64_MIN || coord.y - CIVILIZATION_SIZE == INT64_MIN)))
				{
					continue; //outside the scope of the problem
				}
				else if (adjacent_coord.x == CIVILIZATION_SIZE || adjacent_coord.x < 0 ||
					adjacent_coord.y == CIVILIZATION_SIZE || adjacent_coord.y < 0)
				{
					num_arround += find_external_life(life_coord, { x,y }, false) == status::alive;
				}
				else
				{
					num_arround += get_life(adjacent_coord);
				}
			}
		}
		if (num_arround == NEW_LIFE_AMOUNT)
		{
			set_upcoming_life(life_coord, true);
		}
	}

	template <typename T> void civilization_<T>::check_surrounding_life(coordinate const& life_coord)
	{

	}

	template <typename T> int civilization_<T>::inner_life(coordinate const& life_coord)
	{
		int num_alive_adjacent = 0;
		int64_t x = life_coord.x;
		int64_t y = life_coord.y;
		// just hard code, no need for the overhead with increments and loops -- checking all spaces around life
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

	template <> int civilization_<std::unordered_map<coordinate, bool, coordinate::hash_fn>>::inner_life(coordinate const& life_coord)
	{
		int num_alive_adjacent = 0;
		int64_t x = life_coord.x;
		int64_t y = life_coord.y;
		// just hard code, no need for the overhead with increments and loops -- checking all spaces surrounding the life
		num_alive_adjacent += get_life({ x - 1, y - 1 });
		check_surrounding_life({ x - 1, y - 1 });
		num_alive_adjacent += get_life({ x - 1, y });
		check_surrounding_life({ x - 1, y });
		num_alive_adjacent += get_life({ x - 1, y + 1 });
		check_surrounding_life({ x - 1, y + 1 });
		num_alive_adjacent += get_life({ x, y - 1 });
		check_surrounding_life({ x, y - 1 });
		num_alive_adjacent += get_life({ x, y + 1 });
		check_surrounding_life({ x, y + 1 });
		num_alive_adjacent += get_life({ x + 1, y - 1 });
		check_surrounding_life({ x + 1, y - 1 });
		num_alive_adjacent += get_life({ x + 1, y });
		check_surrounding_life({ x + 1, y });
		num_alive_adjacent += get_life({ x + 1, y + 1 });
		check_surrounding_life({ x + 1, y + 1 });

		return num_alive_adjacent;
	}

	template <typename T> int civilization_<T>::discover_neighborhood(coordinate const& life_coord)
	{
		int64_t x = life_coord.x;
		int64_t y = life_coord.y;
		if (x == 0 || y == 0 || x == CIVILIZATION_SIZE - 1 || y == CIVILIZATION_SIZE - 1)
		{
			return border_life(life_coord);
		}
		return inner_life(life_coord);
	}

	template <typename T> bool civilization_<T>::simulate_coord(coordinate const& life)
	{
		int num_alive_adjacent = 0;
		num_alive_adjacent += discover_neighborhood(life);


		bool living = get_life(life);

		if (living && num_alive_adjacent < 2 || num_alive_adjacent > 3)//we are now dead
		{
			set_upcoming_life(life, false);
			return false;
		}
		else if (!living && num_alive_adjacent == NEW_LIFE_AMOUNT) //new life
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
	template <typename T> uint64_t civilization_<T>::simulate()
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

	template <> uint64_t civilization_<std::unordered_map<coordinate, bool, coordinate::hash_fn>>::simulate()
	{
		for (std::unordered_map<coordinate, bool, coordinate::hash_fn>::const_iterator it = lifes->begin(), end = lifes->end(); it != end; ++it)
		{
			simulate_coord(it->first);
		}

		return lifes_back_buffer->size();
	}

	template <typename T> civilization_<T>::~civilization_() {
		delete lifes;
		delete lifes_back_buffer;
	}

	template <typename T> void civilization_<T>::to_cout()
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

	template <> void civilization_<std::unordered_map<coordinate, bool, coordinate::hash_fn>>::to_cout()
	{
		for (std::unordered_map<coordinate, bool>::const_iterator it = lifes->begin(), end = lifes->end(); it != end;++it)
		{
			std::cout << coord.x + it->first.x << " " << coord.y + it->first.y << std::endl;
		}
	}

	template <typename T> std::string civilization_<T>::to_string()
	{
		std::stringstream ss;
		for (int64_t life_idx = 0; life_idx != lifes->size();++life_idx)
		{
			int64_t x = life_idx % CIVILIZATION_SIZE;
			int64_t y = life_idx / CIVILIZATION_SIZE;
			if (get_life(coordinate{ x, y }))
			{
				ss << coord.x + x << " " << coord.y + y << std::endl;
			}
		}
		return ss.str();
	}

	template <> std::string civilization_<std::unordered_map<coordinate, bool, coordinate::hash_fn>>::to_string()
	{
		std::stringstream ss;
		for (std::unordered_map<coordinate, bool>::const_iterator it = lifes->begin(), end = lifes->end(); it != end;++it)
		{
			ss << coord.x + it->first.x << " " << coord.y + it->first.y << std::endl;
		}
		return ss.str();
	}

	template <> void civilization_<std::unordered_map<coordinate, bool, coordinate::hash_fn>>::post_simulate()
	{
		lifes->swap(*lifes_back_buffer);
		lifes_back_buffer->clear();
	}

	template <typename T> void civilization_<T>::post_simulate()
	{
		lifes->swap(*lifes_back_buffer);
		std::fill(lifes_back_buffer->begin(), lifes_back_buffer->end(), 0);

	}
	template <> void civilization_<std::bitset<CIVILIZATION_SIZE* CIVILIZATION_SIZE>>::post_simulate()
	{
		std::bitset<CIVILIZATION_SIZE* CIVILIZATION_SIZE>* tmp = lifes;
		lifes = lifes_back_buffer;
		lifes_back_buffer = tmp;
		lifes_back_buffer->reset();
	}
}
