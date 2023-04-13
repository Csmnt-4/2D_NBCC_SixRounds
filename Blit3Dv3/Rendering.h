#pragma once

#include <glm/detail/type_vec.hpp>
#include <tuple>
#include <Blit3D.h>
#include "Player.h"

class Grid {
public:
	int height = 10, width = 10;
	int maxGoblinsAmount = 5;
	int score = 0;
	float internalTimer = 0;

	Player player;
	std::vector<Goblin> goblins;

	Sprite* heartSprite = nullptr, * bulletSprite = nullptr;
	std::string bufferString = "";
	std::string	scoreString = "";

	bool ShootClosestEntityOnTheLineOfSight();
	void AddRandomGoblin(Blit3D* blit3D), AddRandomGoblins(Blit3D* blit3D);
	bool isBusy();
	
	bool WalkUp(), WalkDown(), WalkLeft(), WalkRight();

	void appendToFile(std::string fileName, int score);
	std::string loadTextFile(const std::string fileName);
	void outputString(std::map<char, Sprite*> alphabet, int startX, int startY);


	Grid();
	bool Update(Blit3D* blit3D, float seconds);
	bool Draw(Blit3D*);
	
	// TODO: Goblin's Attack() function, Draw() grid function to draw tiles for height by width;
	// main Update() function for entities logic and main loop.
};

Direction FindOpenPath(Position currentPosition, Position targetPosition, std::vector<Goblin> otherEntities);
