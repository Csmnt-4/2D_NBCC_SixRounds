#include <tuple>
#include "Rendering.h"
#include "Navigation.h"
#include "DieRoller.h"

Grid::Grid()
{
	// TODO: Create the constructor. Maybe.
}

Direction FindOpenPath(Position currentPosition, Position targetPosition, std::vector<Goblin> otherEntities) {
	// Compute the distance to the target for each adjacent position
	std::vector<glm::vec2> otherEntityPositions;
	for each (auto oe in otherEntities)
	{
		otherEntityPositions.push_back(oe.position.getGridPosition());
	}
	// distances[i][j] stores the distance from entityPosition + vec2(i-1, j-1) to targetPosition
	float distances[3][3] = { 0 };
	for (int i = 0; i < 3; i++) {
		for (int j = 0; j < 3; j++) {
			if (i == 1 && j == 1) {
				// skip the entity's current position
				distances[i][j] = std::numeric_limits<float>::infinity();
			}
			glm::vec2 adjacentPos = currentPosition.getGridPosition() + glm::vec2(i - 1, j - 1);
			if (std::find(otherEntityPositions.begin(), otherEntityPositions.end(), adjacentPos) != otherEntityPositions.end()) { // check if the path is blocked by another entity
				distances[i][j] = std::numeric_limits<float>::infinity(); // mark the distance as infinite to avoid selecting this path
			}
			else {
				distances[i][j] = glm::length(targetPosition.getGridPosition() - adjacentPos);
			}
		}
	}

	printf("\n\n\Looking for a path...");
	// Determine the direction of the closest open path
	int minI = 1, minJ = 1;
	float minDistance = std::numeric_limits<float>::infinity();
	for (int i = 0; i < 3; i++) {
		for (int j = 0; j < 3; j++) {
			if (distances[i][j] < minDistance) {
				minDistance = distances[i][j];
				minI = i;
				minJ = j;
			}
		}
	}

	// Convert the direction to a Direction enum value
	if (minI == 0 && minJ == 0) {
		if (distances[0][1] >= distances[1][0])
			return Direction::LEFT;
		return Direction::DOWN;
	}
	else if (minI == 0 && minJ == 1) {
		return Direction::LEFT;
	}
	else if (minI == 0 && minJ == 2) {
		if (distances[0][2] >= distances[1][2])
			return Direction::LEFT;
		return Direction::UP;
	}
	else if (minI == 1 && minJ == 0) {
		return Direction::DOWN;
	}
	else if (minI == 1 && minJ == 2) {
		return Direction::UP;
	}
	else if (minI == 2 && minJ == 0) {
		if (distances[2][1] >= distances[1][0])
			return Direction::RIGHT;
		return Direction::DOWN;
	}
	else if (minI == 2 && minJ == 1) {
		return Direction::RIGHT;
	}
	else if (minI == 2 && minJ == 2) {
		if (distances[2][1] >= distances[1][2])
			return Direction::RIGHT;
		return Direction::UP;
	}
	else return Direction::STATIC;
}

bool Grid::Update(Blit3D* blit3D)
{
	{
		// TODO: If player is attacking, i.e. performAttack == true,
		//
		//  try ShootClosestEntityOnTheLineOfSight();
		if (player.performAttack) {
			ShootClosestEntityOnTheLineOfSight();
			player.performAttack = false;
		}

		// TODO: If goblin is attacking,  i.e. performAttack == true,
		// and player is next to goblin (doublechecking), player recieves damage.

		if (!goblins.empty())
			for (auto& goblin : goblins)
			{
				if (goblin.performAttack) {
					if (goblin.position.isNextToTarget(player.position))
						player.RecieveDamage(goblin.lookDirection);
					goblin.performAttack = false;
				}
			}
	}

	player.Update(blit3D);
	if (!goblins.empty())
		for (auto& goblin : goblins)
		{
			goblin.Update(blit3D);
		}
	if (!goblins.empty())
		for (int i = goblins.size() - 1; i >= 0; i--) {
			if (/*!goblins.at(i).isBusy() && */goblins.at(i).healthPoints == 0) {
				printf("\n\tdead - %i", i);
				goblins.erase(goblins.begin() + i);
			}
		}

	if (player.actionPoints == 0 &&
		// If there is animation going on, no updates or new actions, please!
		// Shouldn't takee too long, right?
		!player.isWalking &&
		!player.isAttacking &&
		!player.isGettingHit &&
		!player.isDying)
	{
		bool recover = true;
		if (!goblins.empty())
			for (auto& goblin : goblins)
			{
				if (!goblin.isWalking &&
					!goblin.isAttacking &&
					!goblin.isDying &&
					!goblin.isGettingHit)
				{
					if (goblin.actionPoints > 0)
					{
						recover = false;
						goblin.Act(player, goblins);
						printf("\n\tap = %i", goblin.actionPoints);
						break;
					}
					else {
						continue;
					}
				}
				else {
					recover = false;
					break;
				}
			}

		if (recover)
		{
			player.Recover();
			if (!goblins.empty())
				for (auto& goblin : goblins)
				{
					goblin.Recover();
				}
		}
	}
	if (player.healthPoints == 0)
		printf("\n\tded");

	return true;
}

bool Grid::Draw(Blit3D* blit3D)
{
	if (player.isBusy() || player.healthPoints > 0)
		player.Draw(blit3D);

	if (!goblins.empty())
		for (auto& goblin : goblins)
			goblin.Draw(blit3D);
	return false;
}

// Finding the closest entity on the line with the given direction
bool Grid::ShootClosestEntityOnTheLineOfSight() {
	int closestDistance = width * height;
	glm::vec2 shotGoblinPosition;
	for (auto& goblin : goblins)
	{
		if (player.position.isOnLineOfSight(goblin.position, player.lookDirection))
		{
			switch (player.lookDirection)
			{
			case Direction::UP:
			case Direction::DOWN:
			{
				printf("\n\n\tattempt!");
				std::cout << closestDistance;
				if (goblin.position.gPosY - player.position.gPosY < closestDistance)
				{
					closestDistance = player.position.gPosY - goblin.position.gPosY;
					shotGoblinPosition = goblin.position.getGridPosition();
					//std::cout << "\n\t" << shotGoblinPosition.x << "-x shot y-" << shotGoblinPosition.y;
				}
				break;
			}
			case Direction::LEFT:
			case Direction::RIGHT:
			{
				printf("\n\n\tattempt!");
				std::cout << closestDistance;
				if (goblin.position.gPosX - player.position.gPosX < closestDistance)
				{
					closestDistance = player.position.gPosX - goblin.position.gPosX;
					shotGoblinPosition = goblin.position.getGridPosition();
					//std::cout << "\n\t" << shotGoblinPosition.x << "-x shot y-" << shotGoblinPosition.y;
				}
				break;
			}
			}
		}
	}

	for (auto& goblin : goblins)
	{
		//std::cout << "\n\t" << goblin.position.getGridPosition().x << "-x find y-" << goblin.position.getGridPosition().y;

		if (goblin.position.getGridPosition() == shotGoblinPosition) {
			goblin.RecieveDamage(player.lookDirection);
			return true;
		}
	}

	return closestDistance != width * height;
}

void Grid::AddRandomGoblin(Blit3D* blit3D)
{
	DiceRoller dice = DiceRoller();
	Goblin newGoblin = Goblin();
	newGoblin.Init(blit3D, dice.Roll1DN(10), dice.Roll1DN(10));
	newGoblin.Update(blit3D);
	goblins.push_back(newGoblin);
}

void Grid::AddRandomGoblins(Blit3D* blit3D)
{
	DiceRoller dice = DiceRoller();
	for (int i = 0; i < dice.Roll1DN(5); ++i) {
		Goblin newGoblin = Goblin();
		newGoblin.Init(blit3D, dice.Roll1DN(10), dice.Roll1DN(10));
		newGoblin.Update(blit3D);
		goblins.push_back(newGoblin);
	}
}