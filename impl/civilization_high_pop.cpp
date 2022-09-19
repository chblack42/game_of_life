#include <memory>
#include <vector>
#include <map>
#include <unordered_map>
#include <assert.h> 
#include <string>
#include <iostream>
#include "life/world.h"
#include "life/civilization_high_pop.h"
#include <sstream>
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
			std::unordered_map<coordinate,civilization *, coordinate::hash_fn> map = get_world_map(the_world); 
			std::unordered_map < coordinate, civilization*>::const_iterator it= map.find(adjacent_civilization);
			civilization* civ;
			if (it == map.end())
			{
				civ = new civilization_high_pop(adjacent_civilization, the_world);
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
	coordinate const TOP_LEFT { 0, CIVILIZATION_SIZE - 1 };
	coordinate const TOP_RIGHT{ CIVILIZATION_SIZE - 1, CIVILIZATION_SIZE - 1 };
	int const NEW_LIFE_AMOUNT = 3;

	civilization_high_pop::civilization_high_pop(coordinate coord, world * the_world) :
		civilization(coord,the_world)
	{
	}

	void civilization_high_pop::add_life(coordinate const& life_cord)
	{
		if(spawning)
			(*lifes_back_buffer)[get_index(life_cord.x, life_cord.y)] = true;
		else
			(*lifes)[get_index(life_cord.x, life_cord.y)] = true;
	}


	inline bool civilization_high_pop::get_life(coordinate const& life_cord)
	{
		return (*lifes)[get_index(life_cord.x, life_cord.y)];
	}

	void civilization_high_pop::corner_case(neighbor neighbors[9], coordinate const & life_coord)
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
						int num_alive = (neighbors[locations::top_left] == neighbor::alive)  + +(neighbors[locations::top_right] == neighbor::alive) + (neighbors[locations::middle_left] == neighbor::alive)
							+ (neighbors[locations::middle] == neighbor::alive) + (neighbors[locations::middle_right] == neighbor::alive) ;
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
						int num_alive = (neighbors[locations::top_middle] == neighbor::alive)  + (neighbors[locations::middle] == neighbor::alive) + (neighbors[locations::middle_right] == neighbor::alive);
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
					default:{}
				}
			}
		}
		
	}

	void civilization_high_pop::check_and_spawn_external_life(neighbor neighbors[NUM_NEIGHBORS], coordinate const& life_cord)
	{
		int num_alive = (*lifes)[get_index(life_cord.x, life_cord.y)];
		if (life_cord == TOP_LEFT || life_cord == TOP_RIGHT || life_cord == BOTTOM_RIGHT || life_cord == BOTTOM_LEFT) // corner case
		{
			corner_case(neighbors, life_cord);
		}
		else if(num_alive == 1)
		{
			coordinate adjacent_civ_coord = coord;
			coordinate adjacent_life_coord = life_cord;
			if (num_alive && life_cord.x == 0 || life_cord.x == CIVILIZATION_SIZE - 1) // left/right edge
			{
				if (coord.x == INT64_MIN || coord.x + (CIVILIZATION_SIZE - 1) == INT64_MAX)
					return;
				num_alive += (*lifes)[get_index(life_cord.x, life_cord.y - 1)];
				num_alive += (*lifes)[get_index(life_cord.x, life_cord.y + 1)];
				adjacent_civ_coord.x += life_cord.x == 0 ? -CIVILIZATION_SIZE : CIVILIZATION_SIZE;
				adjacent_life_coord.x = life_cord.x == 0 ? CIVILIZATION_SIZE - 1 : 0;
			}
			else if (num_alive && life_cord.y == 0 || life_cord.y == CIVILIZATION_SIZE - 1) // top/bottom edge
			{
				if (coord.y == INT64_MIN || coord.y + (CIVILIZATION_SIZE - 1) == INT64_MAX)
					return;
				num_alive += (*lifes)[get_index(life_cord.x - 1, life_cord.y)];
				num_alive += (*lifes)[get_index(life_cord.x + 1, life_cord.y)];
				adjacent_civ_coord.y += life_cord.y == 0 ? -CIVILIZATION_SIZE : CIVILIZATION_SIZE;
				adjacent_life_coord.y = life_cord.y == 0 ? CIVILIZATION_SIZE - 1 : 0;

			}
			if (num_alive == NEW_LIFE_AMOUNT)
			{
				spawn_civilization(the_world, adjacent_civ_coord, adjacent_life_coord);
			}
		}
	}

	neighbor civilization_high_pop::find_external_life(coordinate const& life_cord, coordinate const& coord_step)
	{
		coordinate adjacent_texture_coord = coord;
		coordinate adjacent_life_coord = { life_cord.x + coord_step.x, life_cord.y + coord_step.y };

		
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
		if (life_cord == coordinate{0, 2})
		{
			int i = 0;
		}
		std::unordered_map<coordinate, civilization*, coordinate::hash_fn> const & world_map = get_world_map(the_world);
		std::unordered_map<coordinate, civilization*>::const_iterator adjacent_civilization = world_map.find(adjacent_texture_coord);
		if (world_map.end() == adjacent_civilization || adjacent_civilization->second->spawning)
		{
			return neighbor::uncharted; // adjacent civ didn't exist or is new return uncharted
		}

		return (*adjacent_civilization->second).get_life(adjacent_life_coord) ? neighbor::alive : neighbor::dead;
	}


	int civilization_high_pop::boarder_life(coordinate const & life_cord) {
		int num_alive_adjacent = 0;
		neighbor neighborhood_arr[9];
		int neighborhood_idx = 0;
		bool has_uncharted = false;
		if ((*lifes)[get_index(life_cord.x, life_cord.y)])
			int i = 0;

		for (int y = 1; y != -2; --y)// move top left to bottom right
		{
			for (int x = -1; x != 2; ++x)
			{
				coordinate adjacent_coord{ life_cord.x + x, life_cord.y + y };
				if (x == 0 && y == 0)
				{
					neighborhood_arr[neighborhood_idx] = (*lifes)[get_index(life_cord.x, life_cord.y)] ? neighbor::alive : neighbor::dead;
				}
				else if((adjacent_coord.x == CIVILIZATION_SIZE && coord.x == INT64_MAX) || (adjacent_coord.x < 0 && coord.x == INT64_MIN) ||
					(adjacent_coord.y == CIVILIZATION_SIZE && coord.y == INT64_MAX) || (adjacent_coord.y < 0 && coord.y == INT64_MIN))
				{
					neighborhood_arr[neighborhood_idx] = neighbor::dead; //outside the scope of the problem
				}
				else if (adjacent_coord.x == CIVILIZATION_SIZE|| adjacent_coord.x < 0 ||
					adjacent_coord.y == CIVILIZATION_SIZE  || adjacent_coord.y < 0)
				{
					neighbor value = find_external_life(life_cord, { x,y });

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
					num_alive_adjacent += (*lifes)[get_index(adjacent_coord.x, adjacent_coord.y)];
					neighborhood_arr[neighborhood_idx] = (*lifes)[get_index(adjacent_coord.x, adjacent_coord.y)] ? neighbor::alive : neighbor::dead;

				}

				neighborhood_idx++;
			}

		}
		if (has_uncharted)
		{
			check_and_spawn_external_life(neighborhood_arr, life_cord);
		}
		return num_alive_adjacent;
	};

	int civilization_high_pop::inner_life(coordinate const & life_cord) 
	{
		int num_alive_adjacent = 0;
		int x = life_cord.x;
		int y = life_cord.y;
		// just hard code, no need for the overhead with increments and looks
		num_alive_adjacent += (*lifes)[get_index(x - 1, y - 1)];
		num_alive_adjacent += (*lifes)[get_index(x - 1, y)];
		num_alive_adjacent += (*lifes)[get_index(x - 1, y + 1)];
		num_alive_adjacent += (*lifes)[get_index(x, y - 1)];
		num_alive_adjacent += (*lifes)[get_index(x, y + 1)];
		num_alive_adjacent += (*lifes)[get_index(x + 1, y - 1)];
		num_alive_adjacent += (*lifes)[get_index(x + 1, y)];
		num_alive_adjacent += (*lifes)[get_index(x + 1, y + 1)];

		return num_alive_adjacent;
	}

	int civilization_high_pop::discover_neighborhood(coordinate const& life_cord)
	{
		int x = life_cord.x;
		int y = life_cord.y;
		if (x == 0 || y == 0 || x == CIVILIZATION_SIZE - 1 || y == CIVILIZATION_SIZE - 1 ) 
		{
			return boarder_life(life_cord);
		}
		return inner_life(life_cord);
	}
	// return true if there are living things in this block
	uint64_t civilization_high_pop::simulate()
	{
		uint64_t current_lifes = 0;
		for (uint64_t life_idx = 0; life_idx != lifes->size(); ++life_idx)
		{
			int64_t x = life_idx % CIVILIZATION_SIZE;
			int64_t y = life_idx / CIVILIZATION_SIZE;

			int num_alive_adjacent = 0;

			num_alive_adjacent += discover_neighborhood({x,y});


			bool life = (*lifes)[get_index(x, y)];

			if (life && num_alive_adjacent < 2 || num_alive_adjacent > 3)//we are now dead
				(*lifes_back_buffer)[get_index(x, y)] = false;
			else if (!life && num_alive_adjacent == 3) //new life
			{
				current_lifes++;
				(*lifes_back_buffer)[get_index(x, y)] = true;
			}
			else if (life) //living on to the next generation
			{
				(*lifes_back_buffer)[get_index(x, y)] = true;
				current_lifes++;
			}
			else
				(*lifes_back_buffer)[get_index(x, y)] = false;

		}
		return current_lifes;
	}

	civilization_high_pop::~civilization_high_pop() {
		delete lifes;
		delete lifes_back_buffer;
	}

	void civilization_high_pop::to_cout()
	{
		for (int64_t life_idx = 0; life_idx != lifes->size();++life_idx)
		{
			int64_t x = life_idx % CIVILIZATION_SIZE;
			int64_t y = life_idx / CIVILIZATION_SIZE;
			if ((*lifes)[get_index(x, y)])
			{
				std::cout << coord.x + x << " " << coord.y + y << std::endl;
			}
		}
	}
	std::string civilization_high_pop::to_string()
	{
		std::string result = "";
		for (int64_t life_idx = 0; life_idx != lifes->size();++life_idx)
		{
			int64_t x = life_idx % CIVILIZATION_SIZE;
			int64_t y = life_idx / CIVILIZATION_SIZE;
			if ((*lifes)[get_index(x, y)])
			{
				std::stringstream ss;
				ss << coord.x + x << " " << coord.y + y << std::endl;
				result += ss.str();
			}
		}
		return result;
	}

	void civilization_high_pop::post_simulate()
	{
		lifes->swap(*lifes_back_buffer);
		std::fill(lifes_back_buffer->begin(), lifes_back_buffer->end(), 0);
	}

}
