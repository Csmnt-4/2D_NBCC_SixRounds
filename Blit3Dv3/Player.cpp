#include "Player.h"

Entity::Entity()
{
	lookDirection = Direction::DOWN;
	position = Position();
}

bool Entity::Init(Blit3D* blit3D)
{
	staticSprite = blit3D->MakeSprite(0, 0, 64, 64, spriteFilePath);
	position = Position(1, 1);
	return true;
}

bool Entity::Update(Blit3D* blit3D)
{
	if (currentFrameNumber == maxFrameNumber) {
		currentFrameNumber = 0;
		isWalking = false;
		isGettingHit = false;
		isAttacking = false;
	}
	else if (isWalking || isGettingHit || isAttacking) {
		currentFrameNumber++;
	}

	if (isWalking) {
		currentSprite = walkAnimations.at(lookDirection)->at(currentFrameNumber);
		return true;
	}
	else {
		position.Update(blit3D);

		if (isGettingHit)
			currentSprite = hitAnimations.at(hitFromDirection)->at(currentFrameNumber);
		else if (isAttacking)
			currentSprite = attackAnimations.at(lookDirection)->at(currentFrameNumber);
		else
			currentSprite = staticSprite;

		return true;
	}
	return false;
}

bool Entity::WalkUp()
{
	if (!hasNoAP())
	{
		actionPoints--;
		isWalking = true;
		lookDirection = Direction::UP;
		position.WalkUp();
		return true;
	}
	return false;
}

bool Entity::WalkDown()
{
	if (!hasNoAP())
	{
		actionPoints--;
		isWalking = true;
		lookDirection = Direction::DOWN;
		position.WalkDown();
		return true;
	}
	return false;
}

bool Entity::WalkLeft()
{
	if (!hasNoAP())
	{
		actionPoints--;
		isWalking = true;
		lookDirection = Direction::LEFT;
		position.WalkLeft();
		return true;
	}
	return false;
}

bool Entity::WalkRight()
{
	if (!hasNoAP())
	{
		actionPoints--;
		isWalking = true;
		lookDirection = Direction::RIGHT;
		position.WalkRight();
		return true;
	}
	return false;
}

bool Entity::hasNoAP()
{
	if (!hasNoAP())
	{
		actionPoints--;
		return actionPoints == 0;
	}
}

bool Entity::Attack()
{
	performAttack = true;
	isAttacking = true;
	return true;
}

bool Entity::Recover()
{
	actionPoints = maxActionPoints;
	return true;
}

bool Entity::RecieveDamage(Direction hitDirection)
{
	healthPoints--;
	hitFromDirection = hitDirection;
	if (healthPoints == 0)
	{
		isDying = true;
		return true;
	}
	isGettingHit = true;
	return true;
}

void Entity::Draw(Blit3D* blit3D)
{
	currentSprite->Blit(position.sPosX, position.sPosY);
}

std::string Entity::getSpriteFilePath()
{
	return spriteFilePath;
}

Player::Player()
{
	revolverRounds = 6;
	maxRevolverRounds = 6;
	maxHealthPoints = 6;
	healthPoints = 6;
	maxActionPoints = 2;
	actionPoints = 2;

	spriteFilePath = "Media\\GreyChunk.png";
}

bool Player::Shoot()
{
	if (!hasNoAP())
	{
		revolverRounds--;
		Attack();
		return true;
	}
	return false;
}

bool Player::Reload()
{
	revolverRounds = maxRevolverRounds;
	return true;
}

Goblin::Goblin()
{
	maxHealthPoints = 1;
	healthPoints = 1;
	maxActionPoints = 1;
	actionPoints = 1;

	spriteFilePath = "Media\\GreyChunk.png";
}