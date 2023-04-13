#pragma once

#include <string>
#include <Blit3D.h>
#include "Navigation.h"

class Entity {
public:
	int actionPoints, maxActionPoints, healthPoints, maxHealthPoints;
	int currentFrameNumber = 0, maxFrameNumber = 10;
	bool isAttacking = false, isWalking = false, isDying = false, isGettingHit = false;
	bool performAttack = false;

	float internalTimer = 0;

	// In process of drawing the animations
	std::map<int, std::vector<Sprite*>> idleAnimations;
	std::map<int, std::vector<Sprite*>> walkAnimations;
	std::map<int, std::vector<Sprite*>> attackAnimations;
	std::map<int, std::vector<Sprite*>> hitAnimations;
	std::map<int, std::vector<Sprite*>> deathAnimations;

	Direction lookDirection;
	Direction hitFromDirection; // For future. 
								// Leads to needing 16(!) various animations, which is simply too much for a late project. 
	Position position;

	std::string spriteFilePath;
	std::string idleLeftSpriteFilePath,
		        idleRightSpriteFilePath, 
		        idleUpSpriteFilePath, 
		        idleDownSpriteFilePath;
	std::string walkLeftSpriteFilePath, 
		        walkRightSpriteFilePath, 
		        walkUpSpriteFilePath, 
		        walkDownSpriteFilePath;
	std::string attackLeftSpriteFilePath, 
		        attackRightSpriteFilePath, 
		        attackUpSpriteFilePath, 
		        attackDownSpriteFilePath;
	std::string hitLeftSpriteFilePath, 
		        hitRightSpriteFilePath, 
		        hitUpSpriteFilePath, 
		        hitDownSpriteFilePath;
	std::string deathLeftSpriteFilePath, 
		        deathRightSpriteFilePath, 
		        deathUpSpriteFilePath, 
		        deathDownSpriteFilePath;
	Sprite* currentSprite;
	std::vector<Sprite*> idleAnimation;

	Entity();
	bool Init(Blit3D* blit3D), Init(Blit3D* blit3D, int x, int y);
	bool WalkUp(), WalkDown(), WalkLeft(), WalkRight();
	bool TurnUp(), TurnDown(), TurnLeft(), TurnRight();
	bool Recover(), Attack(), RecieveDamage(Direction hitDirection);
	bool hasNoAP(), isBusy();

	void printInformation();

	std::string getSpriteFilePath();

	bool Update(Blit3D* blit3D, float seconds);
	void Draw(Blit3D* blit3D);
};

class Player : public Entity {
public:
	int revolverRounds, maxRevolverRounds;
	Player::Player();
	bool Shoot(), Reload();
	bool hasNoAmmo();
};

class Goblin : public Entity {
public:
	Goblin::Goblin();
	bool Act(Player player, std::vector<Goblin> goblins);
};