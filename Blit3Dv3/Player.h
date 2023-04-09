#pragma once
#include "Rendering.h"
#include <glm/detail/type_vec.hpp>
#include <string>
#include <Blit3D.h>

enum Direction { UP = 0, RIGHT, DOWN, LEFT };

class Entity {
public:
	int actionPoints, maxActionPoints, healthPoints, maxHealthPoints;
	int currentFrameNumber, maxFrameNumber = 10;
	bool isAttacking, isWalking, isDying, isGettingHit;
	bool performAttack;
	std::map<Direction, std::vector<Sprite*>*> walkAnimations;
	std::map<Direction, std::vector<Sprite*>*> hitAnimations;
	std::map<Direction, std::vector<Sprite*>*> attackAnimations;

	Direction lookDirection, hitFromDirection;
	Position position;

	std::string spriteFilePath;
	Sprite* currentSprite, * staticSprite;

	Entity();
	bool Init(Blit3D* blit3D);
	bool WalkUp(), WalkDown(), WalkLeft(), WalkRight();
	bool hasNoAP(), Recover(), Attack(), RecieveDamage(Direction hitDirection);

	std::string getSpriteFilePath();

	bool Update(Blit3D* blit3D);
	void Draw(Blit3D* blit3D);
};

class Player : public Entity {
public:
	int revolverRounds, maxRevolverRounds;
	Player::Player();
	bool Shoot(), Reload();
};

class Goblin : public Entity {
public:
	Goblin::Goblin();
	// TODO: WalkTo() function and Act() function, that would (probably) decide,
	//  whether the goblin attacs, walks to the player or runs away.
};