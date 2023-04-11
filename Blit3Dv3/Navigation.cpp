#include "Rendering.h"

Position::Position() {
	gPosX = 1;
	gPosY = 1;
	sPosX = -1;
	sPosY = -1;
	step = 1;
	std::vector<glm::vec2> movementPositions = {};
}

Position::Position(int x, int y)
{
	gPosX = x;
	gPosY = y;
	step = 1;

	int spriteSize = 64;

	float x_prime = x + y;
	float y_prime = 0.5 * y + 0.5 * y;

	sPosX = 0.5 * x_prime - y_prime * spriteSize;
	sPosY = 0.5 * x_prime + y_prime * spriteSize;
}

std::tuple<float, float> Position::TransformToScreenPosition(int x, int y)
{
	int spriteSize = 64;

	// Going from grid coordinate to screen coordinate
	std::tuple<float, float> position =
	{
		// For debug pur-pur
		//spriteSize * x,
		//spriteSize * y
		spriteSize * (x + y),
		spriteSize * (-0.5 * x + 0.5 * y)
	};

	return position;
}

std::tuple<float, float> Position::TransformToGridPosition(int x, int y)
{
	int spriteSize = 64;

	//Going from screen coordinate to grid coordinate
	std::tuple<float, float> position = {
			spriteSize * (0.5 * spriteSize * (x + y) - spriteSize * (-0.5 * x + 0.5 * y)),
			spriteSize * (-0.5 * spriteSize * (x + y) + spriteSize * (-0.5 * x + 0.5 * y))
	};

	return position;
}

bool Position::Update(Blit3D* blit3D)
{
	sPosX = blit3D->screenWidth / 3 + std::get<0>(TransformToScreenPosition(gPosX, gPosY));
	sPosY = blit3D->screenHeight / 2 + std::get<1>(TransformToScreenPosition(gPosX, gPosY));
	return true;
}

bool Position::WalkUp()
{
	gPosY++;
	return true;
}

bool Position::WalkDown()
{
	gPosY--;
	return true;
}

bool Position::WalkLeft()
{
	gPosX--;
	return true;
}

bool Position::WalkRight()
{
	gPosX++;
	return true;
}

glm::vec2 Position::getGridPosition() {
	return glm::vec2(gPosX, gPosY);
};

bool Position::isOnLineOfSight(Position otherPosition, Direction lookDirection) {
	std::cout << "\n\t" << lookDirection;

	switch (lookDirection) {
	case Direction::UP:
		if (gPosX == otherPosition.gPosX)
			return gPosY < otherPosition.gPosY;
		return false;
	case Direction::DOWN:
		if (gPosX == otherPosition.gPosX)
			return gPosY > otherPosition.gPosY;
		return false;
	case Direction::LEFT:
		if (gPosY == otherPosition.gPosY)
			return gPosX > otherPosition.gPosX;
		return false;
	case Direction::RIGHT:
		if (gPosY == otherPosition.gPosY)
			return gPosX < otherPosition.gPosX;
		return false;
	}
	return false;
}

bool Position::isNextToTarget(Position targetPosition) {
	if (targetPosition.gPosX >= gPosX - 1 &&
		targetPosition.gPosX <= gPosX + 1 &&
		targetPosition.gPosY >= gPosY - 1 &&
		targetPosition.gPosY <= gPosY + 1) {
		return true;
	}
	return false;
}