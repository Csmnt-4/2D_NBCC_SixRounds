#pragma once
#include <tuple>
#include <Blit3D.h>
#include "Player.h"

class Position {
public:
	//Grid position
	int gPosX, gPosY, step;
	float sPosX, sPosY; //Screen position
	std::vector<glm::vec2> movementPositions;
	std::tuple<float, float> TransformToScreenPosition(int x, int y);
	std::tuple<float, float> TransformToGridPosition(int x, int y);

	bool WalkUp(), WalkDown(), WalkLeft(), WalkRight();
	std::vector<glm::vec2> Position::GetParabolicTrajectory(glm::vec2 start, glm::vec2 end, int num_steps);
	glm::vec2 Position::getGridPosition();

	Position(),	Position(int gPosX, int gPosY);
	bool Update(Blit3D* blit3D);
};

class Grid {
public:
	int height, width;
	//int grid[height][width];
	Player* player;
	std::vector<Goblin*> goblins;

	bool isOnLineOfSight(Position lookFromPosition, Position otherPosition, Direction lookDirection);
	bool ShootClosestEntityOnTheLineOfSight();
		
	Grid();
	bool Update();
	bool Draw(Blit3D*);
	
	// TODO: Goblin's Attack() function, Draw() grid function to draw tiles for height by width;
	// main Update() function for entities logic and 

};
