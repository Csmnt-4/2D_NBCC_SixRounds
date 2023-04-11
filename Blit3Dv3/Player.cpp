#include "Player.h"
#include "DieRoller.h"
#include "Rendering.h"

Entity::Entity()
{
	lookDirection = Direction::DOWN;
	position = Position();
}

bool Entity::Init(Blit3D* blit3D)
{
	// TODO: Push the sprites via loop, more elegantly.
	// And make that so a-l-l sprites are being pushed simultaneously!

	std::vector<Sprite*> walkAnimationsLeft;
	std::vector<Sprite*> walkAnimationsRight;
	std::vector<Sprite*> walkAnimationsUp;
	std::vector<Sprite*> walkAnimationsDown;

	for (int i = 0; i < maxFrameNumber; ++i) {
		idleAnimation.push_back(blit3D->MakeSprite(i * 64, 0, 64, 64, spriteFilePath));
		walkAnimationsLeft.push_back(blit3D->MakeSprite(i * 192, 0, 192, 192, walkLeftSpriteFilePath));
		walkAnimationsRight.push_back(blit3D->MakeSprite(i * 192, 0, 192, 192, walkRightSpriteFilePath));
		walkAnimationsUp.push_back(blit3D->MakeSprite(i * 192, 0, 192, 192, walkUpSpriteFilePath));
		walkAnimationsDown.push_back(blit3D->MakeSprite(i * 192, 0, 192, 192, walkDownSpriteFilePath));
	}

	walkAnimations.insert(std::pair<int, std::vector<Sprite*>>((int)Direction::LEFT, walkAnimationsLeft));
	walkAnimations.insert(std::pair<int, std::vector<Sprite*>>((int)Direction::RIGHT, walkAnimationsRight));
	walkAnimations.insert(std::pair<int, std::vector<Sprite*>>((int)Direction::UP, walkAnimationsUp));
	walkAnimations.insert(std::pair<int, std::vector<Sprite*>>((int)Direction::DOWN, walkAnimationsDown));

	position = Position(1, 1);
	return true;
}

bool Entity::Init(Blit3D* blit3D, int x, int y)
{
	std::vector<Sprite*> walkAnimationsLeft;
	std::vector<Sprite*> walkAnimationsRight;
	std::vector<Sprite*> walkAnimationsUp;
	std::vector<Sprite*> walkAnimationsDown;

	for (int i = 0; i < maxFrameNumber; ++i) {
		idleAnimation.push_back(blit3D->MakeSprite(i * 64, 0, 64, 64, spriteFilePath));
		walkAnimationsLeft.push_back(blit3D->MakeSprite(i * 192, 0, 192, 192, walkLeftSpriteFilePath));
		walkAnimationsRight.push_back(blit3D->MakeSprite(i * 192, 0, 192, 192, walkRightSpriteFilePath));
		walkAnimationsUp.push_back(blit3D->MakeSprite(i * 192, 0, 192, 192, walkUpSpriteFilePath));
		walkAnimationsDown.push_back(blit3D->MakeSprite(i * 192, 0, 192, 192, walkDownSpriteFilePath));
	}

	walkAnimations.insert(std::pair<int, std::vector<Sprite*>>((int)Direction::LEFT, walkAnimationsLeft));
	walkAnimations.insert(std::pair<int, std::vector<Sprite*>>((int)Direction::RIGHT, walkAnimationsRight));
	walkAnimations.insert(std::pair<int, std::vector<Sprite*>>((int)Direction::UP, walkAnimationsUp));
	walkAnimations.insert(std::pair<int, std::vector<Sprite*>>((int)Direction::DOWN, walkAnimationsDown));

	position = Position(x, y);
	return true;
}

bool Entity::Update(Blit3D* blit3D)
{
	currentFrameNumber++;
	if (currentFrameNumber == maxFrameNumber) {
		currentFrameNumber = 0;
		isWalking = false;
		isGettingHit = false;
		isAttacking = false;
	}
	if (isWalking || isGettingHit || isAttacking) {
		//if (isGettingHit)
		//	currentSprite = hitAnimations.at((int) hitFromDirection).at(currentFrameNumber);
		//else if (isAttacking)
		//	currentSprite = attackAnimations.at(lookDirection)->at(currentFrameNumber);
		//else if (isWalking)
		if (isWalking)
			currentSprite = walkAnimations.at((int) lookDirection).at(currentFrameNumber);
		return true;
	}
	else {
		currentSprite = idleAnimation[currentFrameNumber];
		position.Update(blit3D);
		//	return true;
	}
	return false;
}

// TODO: Check for other obstacles.
bool Entity::WalkUp()
{
	if (!hasNoAP() && !isBusy())
	{
		actionPoints--;
		isWalking = true;
		currentFrameNumber = 0;
		lookDirection = Direction::UP;
		position.WalkUp();
		return true;
	}
	return false;
}

// TODO: Check for other obstacles.
bool Entity::WalkDown()
{
	if (!hasNoAP() && !isBusy())
	{
		actionPoints--;
		isWalking = true;
		currentFrameNumber = 0;
		lookDirection = Direction::DOWN;
		position.WalkDown();
		return true;
	}
	return false;
}

// TODO: Check for other obstacles.
bool Entity::WalkLeft()
{
	if (!hasNoAP() && !isBusy())
	{
		actionPoints--;
		isWalking = true;
		currentFrameNumber = 0;
		lookDirection = Direction::LEFT;
		position.WalkLeft();
		return true;
	}
	return false;
}

// TODO: Check for other obstacles.
bool Entity::WalkRight()
{
	if (!hasNoAP() && !isBusy())
	{
		actionPoints--;
		isWalking = true;
		currentFrameNumber = 0;
		lookDirection = Direction::RIGHT;
		position.WalkRight();
		return true;
	}
	return false;
}

bool Entity::hasNoAP()
{
	return actionPoints == 0;
}

bool Entity::isBusy()
{
	return isWalking || isAttacking || isGettingHit || isDying;
}

void Entity::printInformation()
{
	printf("\n");
	printf("\n\thp = %i"           , healthPoints);
	printf("\n\tap = %i"          , actionPoints);
	printf("\n");
	printf("\n\tisWalking = %i"   , isWalking);
	printf("\n\tisAttacking = %i" , isAttacking);
	printf("\n\tisDying = %i"     , isDying);
	printf("\n\tisGettingHit = %i", isGettingHit);
	printf("\n");
	printf("\n\tx = %i, y = %i"	  , position.gPosX, position.gPosY);

}

bool Entity::Attack()
{
	if (!hasNoAP() && !isBusy())
	{
		actionPoints--;
		currentFrameNumber = 0;
		performAttack = true;
		isAttacking = true;
		return true;
	}
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

	spriteFilePath = "Media\\BananaMan_idle.png";

	walkLeftSpriteFilePath = "Media\\BananaMan_walkLeft.png";
	walkRightSpriteFilePath = "Media\\BananaMan_walkRight.png";
	walkUpSpriteFilePath = "Media\\BananaMan_walkUp.png";
	walkDownSpriteFilePath = "Media\\BananaMan_walkDown.png";

	shootLeftSpriteFilePath = "Media\\BananaMan_shootLeft.png";
	shootRightSpriteFilePath = "Media\\BananaMan_shootRight.png";
	shootUpSpriteFilePath = "Media\\BananaMan_shootUp.png";
	shootDownSpriteFilePath = "Media\\BananaMan_shootDown.png";

	hitLeftSpriteFilePath = "Media\\BananaMan_hitLeft.png";
	hitRightSpriteFilePath = "Media\\BananaMan_hitRight.png";
	hitUpSpriteFilePath = "Media\\BananaMan_hitUp.png";
	hitDownSpriteFilePath = "Media\\BananaMan_hitDown.png";
}

bool Player::Shoot()
{
	if (!hasNoAP() && !hasNoAmmo())
	{
		revolverRounds--;
		actionPoints--;
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

bool Player::hasNoAmmo()
{
	return revolverRounds == 0;
}

Goblin::Goblin()
{
	maxHealthPoints = 1;
	healthPoints = 1;
	maxActionPoints = 1;
	actionPoints = 1;

	spriteFilePath = "Media\\BananaMan_idle.png";

	walkLeftSpriteFilePath = "Media\\BananaMan_walkLeft.png";
	walkRightSpriteFilePath = "Media\\BananaMan_walkRight.png";
	walkUpSpriteFilePath = "Media\\BananaMan_walkUp.png";
	walkDownSpriteFilePath = "Media\\BananaMan_walkDown.png";

	shootLeftSpriteFilePath = "Media\\BananaMan_shootLeft.png";
	shootRightSpriteFilePath = "Media\\BananaMan_shootRight.png";
	shootUpSpriteFilePath = "Media\\BananaMan_shootUp.png";
	shootDownSpriteFilePath = "Media\\BananaMan_shootDown.png";

	hitLeftSpriteFilePath = "Media\\BananaMan_hitLeft.png";
	hitRightSpriteFilePath = "Media\\BananaMan_hitRight.png";
	hitUpSpriteFilePath = "Media\\BananaMan_hitUp.png";
	hitDownSpriteFilePath = "Media\\BananaMan_hitDown.png";
}

bool Goblin::Act(Player player, std::vector<Goblin> goblins) {
	if (!position.isNextToTarget(player.position))
	{
		DiceRoller dice = DiceRoller();
		switch (FindOpenPath(position, player.position, goblins)) {
		case Direction::UP: {
			// TODO: Check for other obstacles?
			// No, because OpenPath is open, by definition.

			WalkUp();
			break;
		}

		case Direction::UP_LEFT: {
			switch (dice.Roll1DN(2))
			{
			case 1:
				WalkUp();
				break;
			case 2:
				WalkLeft();
				break;
			default:
				break;
			}
		}

		case Direction::UP_RIGHT: {
			switch (dice.Roll1DN(2))
			{
			case 1:
				WalkUp();
				break;
			case 2:
				WalkRight();
				break;
			default:
				break;
			}
		}

		case Direction::DOWN: {
			WalkDown();
			break;
		}

		case Direction::DOWN_LEFT: {
			switch (dice.Roll1DN(2))
			{
			case 1:
				WalkDown();
				break;
			case 2:
				WalkLeft();
				break;
			default:
				break;
			}
		}

		case Direction::DOWN_RIGHT: {
			switch (dice.Roll1DN(2))
			{
			case 1:
				WalkDown();
				break;
			case 2:
				WalkRight();
				break;
			default:
				break;
			}
		}

		case Direction::LEFT: {
			WalkLeft();
			break;
		}

		case Direction::RIGHT: {
			WalkRight();
			break;
		}

		case Direction::STATIC: {
			// Uhm.... Maybe do something stupid?
			// Like, start attacking or anything.
			break;
		}
		}
	}
	else
	{
		// Attack!
		Attack();
	}

	return true;
}