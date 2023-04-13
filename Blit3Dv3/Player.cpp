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
	std::vector<Sprite*> idleAnimationsLeft;
	std::vector<Sprite*> idleAnimationsRight;
	std::vector<Sprite*> idleAnimationsUp;
	std::vector<Sprite*> idleAnimationsDown;

	std::vector<Sprite*> walkAnimationsLeft;
	std::vector<Sprite*> walkAnimationsRight;
	std::vector<Sprite*> walkAnimationsUp;
	std::vector<Sprite*> walkAnimationsDown;

	std::vector<Sprite*> attackAnimationsLeft;
	std::vector<Sprite*> attackAnimationsRight;
	std::vector<Sprite*> attackAnimationsUp;
	std::vector<Sprite*> attackAnimationsDown;

	std::vector<Sprite*> deathAnimationsLeft;
	std::vector<Sprite*> deathAnimationsRight;
	std::vector<Sprite*> deathAnimationsUp;
	std::vector<Sprite*> deathAnimationsDown;

	for (int i = 0; i < maxFrameNumber; ++i) {
		idleAnimationsLeft   .push_back(blit3D->MakeSprite(i * 384, 0, 384, 384, idleLeftSpriteFilePath));
		idleAnimationsRight  .push_back(blit3D->MakeSprite(i * 384, 0, 384, 384, idleRightSpriteFilePath));
		idleAnimationsUp     .push_back(blit3D->MakeSprite(i * 384, 0, 384, 384, idleUpSpriteFilePath));
		idleAnimationsDown   .push_back(blit3D->MakeSprite(i * 384, 0, 384, 384, idleDownSpriteFilePath));

		walkAnimationsLeft   .push_back(blit3D->MakeSprite(i * 384, 0, 384, 384, walkLeftSpriteFilePath));
		walkAnimationsRight  .push_back(blit3D->MakeSprite(i * 384, 0, 384, 384, walkRightSpriteFilePath));
		walkAnimationsUp     .push_back(blit3D->MakeSprite(i * 384, 0, 384, 384, walkUpSpriteFilePath));
		walkAnimationsDown   .push_back(blit3D->MakeSprite(i * 384, 0, 384, 384, walkDownSpriteFilePath));

		attackAnimationsLeft .push_back(blit3D->MakeSprite(i * 384, 0, 384, 384, attackLeftSpriteFilePath));
		attackAnimationsRight.push_back(blit3D->MakeSprite(i * 384, 0, 384, 384, attackRightSpriteFilePath));
		attackAnimationsUp   .push_back(blit3D->MakeSprite(i * 384, 0, 384, 384, attackUpSpriteFilePath));
		attackAnimationsDown .push_back(blit3D->MakeSprite(i * 384, 0, 384, 384, attackDownSpriteFilePath));

		deathAnimationsLeft  .push_back(blit3D->MakeSprite(i * 384, 0, 384, 384, deathLeftSpriteFilePath));
		deathAnimationsRight .push_back(blit3D->MakeSprite(i * 384, 0, 384, 384, deathRightSpriteFilePath));
		deathAnimationsUp    .push_back(blit3D->MakeSprite(i * 384, 0, 384, 384, deathUpSpriteFilePath));
		deathAnimationsDown  .push_back(blit3D->MakeSprite(i * 384, 0, 384, 384, deathDownSpriteFilePath));
	}

	idleAnimations  .insert(std::pair<int, std::vector<Sprite*>>((int)Direction::LEFT, idleAnimationsLeft));
	idleAnimations  .insert(std::pair<int, std::vector<Sprite*>>((int)Direction::RIGHT, idleAnimationsRight));
	idleAnimations  .insert(std::pair<int, std::vector<Sprite*>>((int)Direction::UP, idleAnimationsUp));
	idleAnimations  .insert(std::pair<int, std::vector<Sprite*>>((int)Direction::DOWN, idleAnimationsDown));

	walkAnimations  .insert(std::pair<int, std::vector<Sprite*>>((int)Direction::LEFT, walkAnimationsLeft));
	walkAnimations  .insert(std::pair<int, std::vector<Sprite*>>((int)Direction::RIGHT, walkAnimationsRight));
	walkAnimations  .insert(std::pair<int, std::vector<Sprite*>>((int)Direction::UP, walkAnimationsUp));
	walkAnimations  .insert(std::pair<int, std::vector<Sprite*>>((int)Direction::DOWN, walkAnimationsDown));

	attackAnimations.insert(std::pair<int, std::vector<Sprite*>>((int)Direction::LEFT, attackAnimationsLeft));
	attackAnimations.insert(std::pair<int, std::vector<Sprite*>>((int)Direction::RIGHT, attackAnimationsRight));
	attackAnimations.insert(std::pair<int, std::vector<Sprite*>>((int)Direction::UP, attackAnimationsUp));
	attackAnimations.insert(std::pair<int, std::vector<Sprite*>>((int)Direction::DOWN, attackAnimationsDown));

	deathAnimations .insert(std::pair<int, std::vector<Sprite*>>((int)Direction::LEFT, deathAnimationsLeft));
	deathAnimations .insert(std::pair<int, std::vector<Sprite*>>((int)Direction::RIGHT, deathAnimationsRight));
	deathAnimations .insert(std::pair<int, std::vector<Sprite*>>((int)Direction::UP, deathAnimationsUp));
	deathAnimations .insert(std::pair<int, std::vector<Sprite*>>((int)Direction::DOWN, deathAnimationsDown));

	position = Position(1, 1);
	return true;
}

bool Entity::Init(Blit3D* blit3D, int x, int y)
{
	std::vector<Sprite*> idleAnimationsLeft;
	std::vector<Sprite*> idleAnimationsRight;
	std::vector<Sprite*> idleAnimationsUp;
	std::vector<Sprite*> idleAnimationsDown;

	std::vector<Sprite*> walkAnimationsLeft;
	std::vector<Sprite*> walkAnimationsRight;
	std::vector<Sprite*> walkAnimationsUp;
	std::vector<Sprite*> walkAnimationsDown;

	std::vector<Sprite*> attackAnimationsLeft;
	std::vector<Sprite*> attackAnimationsRight;
	std::vector<Sprite*> attackAnimationsUp;
	std::vector<Sprite*> attackAnimationsDown;

	std::vector<Sprite*> deathAnimationsLeft;
	std::vector<Sprite*> deathAnimationsRight;
	std::vector<Sprite*> deathAnimationsUp;
	std::vector<Sprite*> deathAnimationsDown;

	for (int i = 0; i < maxFrameNumber; ++i) {
		idleAnimationsLeft .push_back(blit3D->MakeSprite(i * 384, 0, 384, 384, idleLeftSpriteFilePath));
		idleAnimationsRight.push_back(blit3D->MakeSprite(i * 384, 0, 384, 384, idleRightSpriteFilePath));
		idleAnimationsUp   .push_back(blit3D->MakeSprite(i * 384, 0, 384, 384, idleUpSpriteFilePath));
		idleAnimationsDown .push_back(blit3D->MakeSprite(i * 384, 0, 384, 384, idleDownSpriteFilePath));

		walkAnimationsLeft .push_back(blit3D->MakeSprite(i * 384, 0, 384, 384, walkLeftSpriteFilePath));
		walkAnimationsRight.push_back(blit3D->MakeSprite(i * 384, 0, 384, 384, walkRightSpriteFilePath));
		walkAnimationsUp   .push_back(blit3D->MakeSprite(i * 384, 0, 384, 384, walkUpSpriteFilePath));
		walkAnimationsDown .push_back(blit3D->MakeSprite(i * 384, 0, 384, 384, walkDownSpriteFilePath));

		attackAnimationsLeft .push_back(blit3D->MakeSprite(i * 384, 0, 384, 384, attackLeftSpriteFilePath));
		attackAnimationsRight.push_back(blit3D->MakeSprite(i * 384, 0, 384, 384, attackRightSpriteFilePath));
		attackAnimationsUp   .push_back(blit3D->MakeSprite(i * 384, 0, 384, 384, attackUpSpriteFilePath));
		attackAnimationsDown .push_back(blit3D->MakeSprite(i * 384, 0, 384, 384, attackDownSpriteFilePath));

		deathAnimationsLeft .push_back(blit3D->MakeSprite(i * 384, 0, 384, 384, deathLeftSpriteFilePath));
		deathAnimationsRight.push_back(blit3D->MakeSprite(i * 384, 0, 384, 384, deathRightSpriteFilePath));
		deathAnimationsUp   .push_back(blit3D->MakeSprite(i * 384, 0, 384, 384, deathUpSpriteFilePath));
		deathAnimationsDown .push_back(blit3D->MakeSprite(i * 384, 0, 384, 384, deathDownSpriteFilePath));
	}

	idleAnimations.insert(std::pair<int, std::vector<Sprite*>>((int)Direction::LEFT , idleAnimationsLeft));
	idleAnimations.insert(std::pair<int, std::vector<Sprite*>>((int)Direction::RIGHT, idleAnimationsRight));
	idleAnimations.insert(std::pair<int, std::vector<Sprite*>>((int)Direction::UP   , idleAnimationsUp));
	idleAnimations.insert(std::pair<int, std::vector<Sprite*>>((int)Direction::DOWN , idleAnimationsDown));
	
	walkAnimations.insert(std::pair<int, std::vector<Sprite*>>((int)Direction::LEFT , walkAnimationsLeft));
	walkAnimations.insert(std::pair<int, std::vector<Sprite*>>((int)Direction::RIGHT, walkAnimationsRight));
	walkAnimations.insert(std::pair<int, std::vector<Sprite*>>((int)Direction::UP   , walkAnimationsUp));
	walkAnimations.insert(std::pair<int, std::vector<Sprite*>>((int)Direction::DOWN , walkAnimationsDown));
	
	attackAnimations.insert(std::pair<int, std::vector<Sprite*>>((int)Direction::LEFT , attackAnimationsLeft));
	attackAnimations.insert(std::pair<int, std::vector<Sprite*>>((int)Direction::RIGHT, attackAnimationsRight));
	attackAnimations.insert(std::pair<int, std::vector<Sprite*>>((int)Direction::UP   , attackAnimationsUp));
	attackAnimations.insert(std::pair<int, std::vector<Sprite*>>((int)Direction::DOWN , attackAnimationsDown));
	
	deathAnimations.insert(std::pair<int, std::vector<Sprite*>>((int)Direction::LEFT , deathAnimationsLeft));
	deathAnimations.insert(std::pair<int, std::vector<Sprite*>>((int)Direction::RIGHT, deathAnimationsRight));
	deathAnimations.insert(std::pair<int, std::vector<Sprite*>>((int)Direction::UP   , deathAnimationsUp));
	deathAnimations.insert(std::pair<int, std::vector<Sprite*>>((int)Direction::DOWN , deathAnimationsDown));

	position = Position(x, y);
	return true;
}

bool Entity::Update(Blit3D* blit3D, float seconds)
{
	internalTimer += seconds;

	if (internalTimer >= 0.11f) {
		internalTimer = 0.f;
		currentFrameNumber++;
		if (currentFrameNumber == maxFrameNumber) {
			currentFrameNumber = 0;
			isWalking = false;
			isGettingHit = false;
			isAttacking = false;
			isDying = false;
		}
		if (isBusy()) {
			//if (isGettingHit)
			//	currentSprite = hitAnimations.at((int) hitFromDirection).at(currentFrameNumber);
			if (isAttacking) {
				currentSprite = attackAnimations.at((int) lookDirection).at(currentFrameNumber);
				return true;
			}
			//else if (isWalking)
			else if (isWalking) {
				currentSprite = walkAnimations.at((int) lookDirection).at(currentFrameNumber);
				internalTimer = 0.02f;
				return true;
			}
			else if (isDying) {
				currentSprite = deathAnimations.at((int)lookDirection).at(currentFrameNumber);
				//internalTimer = 0.f;
				return true;
			}
		}
		else {
			currentSprite = idleAnimations.at((int)lookDirection).at(currentFrameNumber);
			position.Update(blit3D);
			//	return true;
		}
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

bool Entity::TurnUp()
{
	if (!hasNoAP() && !isBusy())
	{
		actionPoints--;
		lookDirection = Direction::UP;
		return true;
	}
	return false;
}

bool Entity::TurnDown()
{
	if (!hasNoAP() && !isBusy())
	{
		actionPoints--;
		lookDirection = Direction::DOWN;
		return true;
	}
	return false;
}

bool Entity::TurnLeft()
{
	if (!hasNoAP() && !isBusy())
	{
		actionPoints--;
		lookDirection = Direction::LEFT;
		return true;
	}
	return false;
}

bool Entity::TurnRight()
{
	if (!hasNoAP() && !isBusy())
	{
		actionPoints--;
		lookDirection = Direction::RIGHT;
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
	printf("\n\thp = %i", healthPoints);
	printf("\n\tap = %i", actionPoints);
	printf("\n");
	printf("\n\tisWalking = %i", isWalking);
	printf("\n\tisAttacking = %i", isAttacking);
	printf("\n\tisDying = %i", isDying);
	printf("\n\tisGettingHit = %i", isGettingHit);
	printf("\n");
	printf("\n\tx = %i, y = %i", position.gPosX, position.gPosY);
}

bool Entity::Attack()
{
	if (!hasNoAP() && !isBusy())
	{
		currentFrameNumber = 0;
		performAttack = true;
		isAttacking = true;
		actionPoints--;
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
	currentFrameNumber = 0;
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
	healthPoints = 3;
	maxActionPoints = 2;
	actionPoints = 2;

	spriteFilePath            = "Media\\bananaman_idle.png";

	idleLeftSpriteFilePath    = "Media\\bananaman_idle_left.png";
	idleRightSpriteFilePath   = "Media\\bananaman_idle_right.png";
	idleUpSpriteFilePath      = "Media\\bananaman_idle_up.png";
	idleDownSpriteFilePath    = "Media\\bananaman_idle_down.png";

	walkLeftSpriteFilePath    = "Media\\bananaman_step_left.png";
	walkRightSpriteFilePath   = "Media\\bananaman_step_right.png";
	walkUpSpriteFilePath      = "Media\\bananaman_step_up.png";
	walkDownSpriteFilePath    = "Media\\bananaman_step_down.png";

	attackLeftSpriteFilePath  = "Media\\bananaman_shoot_left.png";
	attackRightSpriteFilePath = "Media\\bananaman_shoot_right.png";
	attackUpSpriteFilePath    = "Media\\bananaman_shoot_up.png";
	attackDownSpriteFilePath  = "Media\\bananaman_shoot_down.png";

	hitLeftSpriteFilePath     = "Media\\bananaman_hit_left.png";
	hitRightSpriteFilePath    = "Media\\bananaman_hit_right.png";
	hitUpSpriteFilePath       = "Media\\bananaman_hit_up.png";
	hitDownSpriteFilePath     = "Media\\bananaman_hit_down.png";

	deathLeftSpriteFilePath   = "Media\\bananaman_idle_left.png";
	deathRightSpriteFilePath  = "Media\\bananaman_idle_right.png";
	deathUpSpriteFilePath     = "Media\\bananaman_idle_up.png";
	deathDownSpriteFilePath   = "Media\\bananaman_idle_down.png";
}

bool Player::Shoot()
{
	if (!hasNoAP() && !hasNoAmmo())
	{
		revolverRounds--;
		Attack();
		return true;
	}
	return false;
}

bool Player::Reload()
{
	if (!hasNoAP() && revolverRounds < maxRevolverRounds) {
		actionPoints--;
		revolverRounds = maxRevolverRounds;
		return true;
	}
	return false;
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

	// TODO: Finish the sprites already.....
	spriteFilePath            = "Media\\bananaman_idle.png";

	idleLeftSpriteFilePath    = "Media\\cactigoblin_idle_left.png";
	idleRightSpriteFilePath   = "Media\\cactigoblin_idle_right.png";
	idleUpSpriteFilePath      = "Media\\cactigoblin_idle_up.png";
	idleDownSpriteFilePath    = "Media\\cactigoblin_idle_down.png";

	walkLeftSpriteFilePath    = "Media\\cactigoblin_step_left.png";
	walkRightSpriteFilePath   = "Media\\cactigoblin_step_right.png";
	walkUpSpriteFilePath      = "Media\\cactigoblin_step_up.png";
	walkDownSpriteFilePath    = "Media\\cactigoblin_step_down.png";

	attackLeftSpriteFilePath  = "Media\\cactigoblin_idle_left.png";
	attackRightSpriteFilePath = "Media\\cactigoblin_idle_right.png";
	attackUpSpriteFilePath    = "Media\\cactigoblin_idle_up.png";
	attackDownSpriteFilePath  = "Media\\cactigoblin_idle_down.png";

	hitLeftSpriteFilePath     = "Media\\bananaman_hit_left.png";
	hitRightSpriteFilePath    = "Media\\bananaman_hit_right.png";
	hitUpSpriteFilePath       = "Media\\bananaman_hit_up.png";
	hitDownSpriteFilePath     = "Media\\bananaman_hit_down.png";

	deathLeftSpriteFilePath   = "Media\\cactigoblin_death_left.png";
	deathRightSpriteFilePath  = "Media\\cactigoblin_death_right.png";
	deathUpSpriteFilePath     = "Media\\cactigoblin_death_up.png";
	deathDownSpriteFilePath   = "Media\\cactigoblin_death_down.png";
}

bool Goblin::Act(Player player, std::vector<Goblin> goblins) {
	if (!position.isNextToTarget(player.position))
	{
		DiceRoller dice = DiceRoller();
		switch (FindOpenPath(position, player.position, goblins)) {
		case Direction::UP: {
			// TODO: Check for other obstacles?
			// No, because OpenPath is open, by definition.
			// WRONG, CHECK IT.
			bool isPathOpen = true;
			for each (auto goblin in goblins)
			{
				isPathOpen = goblin.position.getGridPosition() != position.getGridPosition() + glm::vec2(0, 1);
				if (!isPathOpen)
					break;
			}
			if (isPathOpen)
				WalkUp();
			else
				Attack();
			break;
		}

		case Direction::DOWN: {
			bool isPathOpen = true;
			for each (auto goblin in goblins)
			{
				isPathOpen = goblin.position.getGridPosition() != position.getGridPosition() + glm::vec2(0, -1);
				if (!isPathOpen)
					break;
			}
			if (isPathOpen)
				WalkDown();
			else
				Attack();
			break;
		}

		case Direction::LEFT: {
			bool isPathOpen = true;
			for each (auto goblin in goblins)
			{
				isPathOpen = goblin.position.getGridPosition() != position.getGridPosition() + glm::vec2(-1, 0);
				if (!isPathOpen)
					break;
			}
			if (isPathOpen)
				WalkLeft();
			else
				Attack();
			break;
		}

		case Direction::RIGHT: {
			bool isPathOpen = true;
			for each (auto goblin in goblins)
			{
				isPathOpen = goblin.position.getGridPosition() != position.getGridPosition() + glm::vec2(1, 0);
				if (!isPathOpen)
					break;
			}
			if (isPathOpen)
				WalkRight();
			else 
				Attack();
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
		if(position.isTargetInFornt(player.position, lookDirection))
			Attack();
		else
			switch (lookDirection)
			{
			case UP:
				TurnDown();
				break;
			case RIGHT:
				TurnLeft();
				break;
			case DOWN:
				TurnUp();
				break;
			case LEFT:
				TurnRight();
				break;
			case STATIC:
			default:
				Attack();
				break;
			}

		// Attack!
	}

	return true;
}