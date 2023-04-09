#include "Rendering.h"
#include <tuple>

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

std::vector<glm::vec2> Position::GetParabolicTrajectory(glm::vec2 start, glm::vec2 end, int num_steps) {
	std::vector<glm::vec2> points;
	glm::vec2 control;

	// Calculate the control point by finding the midpoint
	control.x = (start.x + end.x) / 2;
	control.y = std::min(start.y, end.y) - std::abs(start.x - end.x) / 2;

	// Create the points along the curve
	double t_step = 1.0 / num_steps;
	for (double t = 0; t <= 1; t += t_step) {
		double t_1 = (1 - t);
		double t_1_sq = t_1 * t_1;
		double t_sq = t * t;

		glm::vec2 p;
		p.x = t_1_sq * start.x + 2 * t_1 * t * control.x + t_sq * end.x;
		p.y = t_1_sq * start.y + 2 * t_1 * t * control.y + t_sq * end.y;
		points.push_back(p);
	}

	return points;
}

Grid::Grid()
{
}

bool Grid::Update() 
{
	return false;

}

bool Grid::Draw(Blit3D* blit3D)
{
	player->Draw(blit3D);
	for (auto& goblin : goblins)
		goblin->Draw(blit3D);
	return false;
}

bool Grid::isOnLineOfSight(Position lookFromPosition, Position otherPosition, Direction lookDirection) {
	switch (lookDirection) {
	case Direction::UP:
		if (lookFromPosition.gPosX == otherPosition.gPosX)
			return lookFromPosition.gPosY < otherPosition.gPosY;
		return false;
	case Direction::DOWN:
		if (lookFromPosition.gPosX == otherPosition.gPosX)
			return lookFromPosition.gPosY > otherPosition.gPosY;
		return false;
	case Direction::LEFT:
		if (lookFromPosition.gPosY == otherPosition.gPosY)
			return lookFromPosition.gPosX > otherPosition.gPosX;
		return false;
	case Direction::RIGHT:
		if (lookFromPosition.gPosY == otherPosition.gPosY)
			return lookFromPosition.gPosX < otherPosition.gPosX;
		return false;
	}
	return false;
}

// Finding the closest entity on the line with the given direction
bool Grid::ShootClosestEntityOnTheLineOfSight() {
	int closestDistance = width * height;
	Goblin* shotGoblin;
	for (auto& goblin : goblins)
	{
		if (isOnLineOfSight(player->position, goblin->position, player->lookDirection))
		{
			switch (player->lookDirection)
			{
			case Direction::UP:
			case Direction::DOWN:
				if (std::pow(player->position.gPosY - goblin->position.gPosY, 2) < std::pow(closestDistance, 2))
				{
					closestDistance = std::sqrt(std::pow(player->position.gPosY - goblin->position.gPosY, 2));
					shotGoblin = goblin;
				}
				break;
			case Direction::LEFT:
			case Direction::RIGHT:
				if (std::pow(player->position.gPosX - goblin->position.gPosX, 2) < std::pow(closestDistance, 2))
				{
					closestDistance = std::sqrt(std::pow(player->position.gPosX - goblin->position.gPosX, 2));
					shotGoblin = goblin;
				}
				break;
			}
		}
	}

	for (auto& goblin : goblins)
	{
		if (goblin == shotGoblin) {
			goblin->RecieveDamage(player->lookDirection);
			return true;
		}
	}

	return closestDistance != width * height;
}