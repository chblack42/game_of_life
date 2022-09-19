#pragma once
#include <functional>
#include <cstdint>

struct coordinate {
	int64_t x;
	int64_t y;

	bool operator==(const coordinate& coord) const {
		return (x == coord.x) && (y == coord.y);
	}

	bool operator<(const coordinate& coord) const {
		if (x != coord.x)
		{
			return (x < coord.x);
		}
		return (y > coord.y);
	}

	struct hash_fn
	{
		size_t operator()(const coordinate& coord) const
		{
			size_t hash_x = std::hash<int64_t>()(coord.x);
			size_t hash_y = std::hash<int64_t>()(coord.y) << 1;
			return hash_x ^ hash_y;
		}
	};

};