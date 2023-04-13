#include "Rendering.h"
#include "Navigation.h"
#include "DieRoller.h"
#include <tuple>
#include <iostream>
#include <cstdlib>
#ifdef __cpp_lib_filesystem
#include <filesystem>
namespace fs = std::filesystem;
#elif __cpp_lib_experimental_filesystem
#define _SILENCE_EXPERIMENTAL_FILESYSTEM_DEPRECATION_WARNING 1;
#include <experimental/filesystem>
namespace fs = std::experimental::filesystem;

#endif
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
		if (distances[0][1] >= distances[1][0] && distances[1][0] != std::numeric_limits<float>::infinity())
			return Direction::LEFT;
		return Direction::DOWN;
	}
	else if (minI == 0 && minJ == 1) {
		return Direction::LEFT;
	}
	else if (minI == 0 && minJ == 2) {
		if (distances[0][2] >= distances[1][2] && distances[1][2] != std::numeric_limits<float>::infinity())
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
		if (distances[2][1] >= distances[1][0] && distances[1][0] != std::numeric_limits<float>::infinity())
			return Direction::RIGHT;
		return Direction::DOWN;
	}
	else if (minI == 2 && minJ == 1) {
		return Direction::RIGHT;
	}
	else if (minI == 2 && minJ == 2) {
		if (distances[2][1] >= distances[1][2] && distances[1][2] != std::numeric_limits<float>::infinity())
			return Direction::RIGHT;
		return Direction::UP;
	}
	else return Direction::STATIC;
}

bool Grid::Update(Blit3D* blit3D, float seconds)
{
	// If player is attacking, i.e. performAttack == true,
	// try ShootClosestEntityOnTheLineOfSight();
	if (player.performAttack) {
		ShootClosestEntityOnTheLineOfSight();
		player.isAttacking = true;
		player.performAttack = false;
	}

	// If goblin is attacking,  i.e. performAttack == true,
	// and player is next to goblin (doublechecking), player recieves damage.
	if (!goblins.empty())
		for (auto& goblin : goblins)
		{
			if (goblin.performAttack) {
				for (auto& otherGoblin : goblins)
				{
					if (goblin.position.isTargetInFornt(otherGoblin.position, goblin.lookDirection)) {
						otherGoblin.RecieveDamage(goblin.lookDirection);
						score++;
					}
				}

				if (goblin.position.isTargetInFornt(player.position, goblin.lookDirection))
					player.RecieveDamage(goblin.lookDirection);
				goblin.performAttack = false;
			}
		}

	if (player.healthPoints > 0 || player.isBusy())
		player.Update(blit3D, seconds);

	if (!goblins.empty())
		for (auto& goblin : goblins)
		{
			goblin.Update(blit3D, seconds);
		}

	if (!goblins.empty())
		for (int i = goblins.size() - 1; i >= 0; i--) {
			// isBusy seems to be not working properly without all animations present
			if (!goblins.at(i).isBusy() && goblins.at(i).healthPoints == 0) {
				goblins.erase(goblins.begin() + i);
			}
		}

	if (player.actionPoints == 0 &&
		// If there is animation going on, no updates or new actions, please!
		// Shouldn't takee too long, right?
		!player.isBusy())
	{
		bool recover = true;
		if (!goblins.empty())
			for (auto& goblin : goblins)
			{
				if (!goblin.isBusy())
				{
					if (goblin.actionPoints > 0)
					{
						recover = false;
						goblin.Act(player, goblins);
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
	return true;
}

bool Grid::Draw(Blit3D* blit3D)
{
	if (player.isBusy() || player.healthPoints > 0)
	{
		player.Draw(blit3D);
		for (int i = 1; i <= player.healthPoints; i++) {
			heartSprite->Blit(200.f + i * 40.f, 1080.f / 1.1f);
		}
		for (int i = 1; i <= player.revolverRounds; i++) {
			bulletSprite->Blit(1500.f + i * 30, 1080.f / 1.1f);
		}
	}
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
				if (goblin.position.gPosY - player.position.gPosY < closestDistance)
				{
					closestDistance = player.position.gPosY - goblin.position.gPosY;
					shotGoblinPosition = goblin.position.getGridPosition();
				}
				break;
			}
			case Direction::LEFT:
			case Direction::RIGHT:
			{
				if (goblin.position.gPosX - player.position.gPosX < closestDistance)
				{
					closestDistance = player.position.gPosX - goblin.position.gPosX;
					shotGoblinPosition = goblin.position.getGridPosition();
				}
				break;
			}
			}
		}
	}

	for (auto& goblin : goblins)
	{
		if (goblin.position.getGridPosition() == shotGoblinPosition) {
			score += goblins.size();
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
	newGoblin.Init(blit3D, dice.Roll1DN(-5) + dice.Roll1DN(5), dice.Roll1DN(5) + dice.Roll1DN(-5));
	newGoblin.Update(blit3D, 1);
	if (newGoblin.position.getGridPosition() != player.position.getGridPosition())
		goblins.push_back(newGoblin);
}

void Grid::AddRandomGoblins(Blit3D* blit3D)
{
	DiceRoller dice = DiceRoller();
	for (int i = 0; i < dice.Roll1DN(maxGoblinsAmount); ++i) {
		AddRandomGoblin(blit3D);
	}
}

bool Grid::isBusy()
{
	if (player.isBusy())
		return true;

	for (auto goblin : goblins)
		if (goblin.isBusy())
			return true;

	return false;
}

bool Grid::WalkUp()
{
	for (auto g : goblins)
		if (g.position.getGridPosition() == player.position.getGridPosition() + glm::vec2(0, 1))
			return false;
	player.WalkUp();
	return true;
}

bool Grid::WalkDown()
{
	for (auto g : goblins)
		if (g.position.getGridPosition() == player.position.getGridPosition() + glm::vec2(0, -1))
			return false;
	player.WalkDown();
	return true;
}

bool Grid::WalkLeft()
{
	for (auto g : goblins)
		if (g.position.getGridPosition() == player.position.getGridPosition() + glm::vec2(-1, 0))
			return false;
	player.WalkLeft();
	return true;
}

bool Grid::WalkRight()
{
	for (auto g : goblins)
		if (g.position.getGridPosition() == player.position.getGridPosition() + glm::vec2(1, 0))
			return false;
	player.WalkRight();
	return true;
}

void Grid::outputString(std::map <char, Sprite*> alphabet, int startX, int startY) {
	int x = startX; // starting x position
	int y = startY; // starting y position
	for (char c : scoreString) { // iterate through each character in input string
		if (c == '//') { // if character is equivalent to '\t', move to next 4th position
			x += 4 * 64;
		}
		else if (c == '/') { // if character is equivalent to '\n', move to next line
			y += 64;
			x = startX;
		}
		else { // otherwise, output the corresponding letter sprite at the current position
			alphabet[c]->Blit(x, y); // output the sprite at the current position
			x += 64; // move to next position
		}
	}
}

void Grid::appendToFile(std::string fileName, int score) {
#define INFO_BUFFER_SIZE 32767
	TCHAR  infoBuf[INFO_BUFFER_SIZE];
	DWORD  bufCharCount = INFO_BUFFER_SIZE;

	std::fstream file(fileName, std::ios::in | std::ios::out);
	std::string line;

	file.seekg(0, std::ios::end); // put the "cursor" at the end of the file
	int length = file.tellg(); // find the position of the cursor
	file.close();

	if (length <= 0) {
		file.open(fileName, std::ios::app);
		file << "top scores\n";
	}

	file.close();
	file.open(fileName, std::ios::app);

	if (file.is_open()) {
		if (!GetUserName(infoBuf, &bufCharCount)) { // BLACK MAGIC?!?
			std::string sysName = std::getenv("USERDOMAIN");
			file << sysName << "\t-\t" << score << "\n";
		}
		else {
			std::wstring usrname_wstring(&infoBuf[0]); //convert to wstring (WHAT IS IT???)
			std::string usrname(usrname_wstring.begin(), usrname_wstring.end()); //and convert to string (THANK YOU, O, MIGTHY MACHINE!!!)
			file << usrname << "\t-\t" << score << "\n";
		}
		file.close();

		file.open(fileName, std::ios::in);
		// Read all lines from the file into a vector
		std::vector<std::string> lines;
		std::string line;
		while (std::getline(file, line)) {
			lines.push_back(line);
			std::cout << "\n\t" << line;
		}

		// Sort the lines after the first one based on the number at the end of the line
		if (lines.size() > 3) {
			std::sort(lines.begin() + 1, lines.end(), [](const std::string& a, const std::string& b) {
				int num_a = std::stoi(a.substr(a.find_last_of('\t') + 1));
				int num_b = std::stoi(b.substr(b.find_last_of('\t') + 1));
				return num_a > num_b;
				});
		}
		file.close();

		file.open(fileName, std::ios::in | std::ios::out | std::ios::trunc);
		// Truncate the file if it has more than 11 lines
		
		for (int i = 0; i < lines.size() && i <= 11; i++) {
			file << lines[i] << "\n";
		}

		// Close the file
		file.close();
	}
}

std::string Grid::loadTextFile(const std::string fileName) {
	std::ifstream file(fileName);
	std::string content;
	if (file.is_open()) {
		std::string line;
		while (std::getline(file, line)) {
			size_t pos = line.find("\t");
			while (pos != std::string::npos) {
				line.replace(pos, 1, "//");
				pos = line.find("\t", pos + 2);
			}
			content += line + "/";
		}
		file.close();
	}
	bufferString = content;
	return content;
}