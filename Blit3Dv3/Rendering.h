#pragma once

#include <glm/detail/type_vec.hpp>
#include <tuple>
#include <Blit3D.h>
#include "Player.h"

class Grid {
public:
	int height = 10, width = 10;
	int maxGoblinsAbmount;

	Player player;
	std::vector<Goblin> goblins;

	bool ShootClosestEntityOnTheLineOfSight();
	void AddRandomGoblin(Blit3D* blit3D), AddRandomGoblins(Blit3D* blit3D);

	Grid();
	bool Update(Blit3D* blit3D);
	bool Draw(Blit3D*);
	
	// TODO: Goblin's Attack() function, Draw() grid function to draw tiles for height by width;
	// main Update() function for entities logic and main loop.
};

Direction FindOpenPath(Position currentPosition, Position targetPosition, std::vector<Goblin> otherEntities);
