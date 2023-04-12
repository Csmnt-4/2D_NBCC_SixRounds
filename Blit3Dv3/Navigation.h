#pragma once

#include <glm/detail/type_vec.hpp>
#include <vector>
#include <Blit3D.h>
#include "Player.h"

enum Direction {
	UP = 0, RIGHT, DOWN, LEFT,
	UP_LEFT, UP_RIGHT, DOWN_LEFT, DOWN_RIGHT,
	STATIC
};

class Position {
public:
	//Grid position
	int gPosX, gPosY, step;
	float sPosX, sPosY; //Screen position
	
	std::tuple<float, float> TransformToScreenPosition(int x, int y);
	std::tuple<float, float> TransformToGridPosition(int x, int y);

	bool WalkUp(), WalkDown(), WalkLeft(), WalkRight();
	glm::vec2 Position::getGridPosition();
	
	bool isOnLineOfSight(Position otherPosition, Direction lookDirection);
	bool isNextToTarget(Position targetPosition), isTargetInFornt(Position targetPosition, Direction lookDirection);

	Position(), Position(int gPosX, int gPosY);
	bool Update(Blit3D* blit3D);
};
