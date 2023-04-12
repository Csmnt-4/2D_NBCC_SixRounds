/*
	Simple example of loading/rotating/displaying sprites in Blit3D
*/
//memory leak detection
#define CRTDBG_MAP_ALLOC
#ifdef _DEBUG
#ifndef DBG_NEW
#define DBG_NEW new ( _NORMAL_BLOCK , __FILE__ , __LINE__ )
#define new DBG_NEW
#endif
#endif  // _DEBUG

#include <stdlib.h>
#include <crtdbg.h>

#include "Blit3D.h"
#include "Player.h"
#include "Rendering.h"
#include "DieRoller.h"

Blit3D* blit3D = NULL;

enum GameState { START = 0, PLAYING, OVER };

//GLOBAL DATA
GameState gameState = GameState::START;
Player player;
Grid drid;
DiceRoller dice;

std::vector<std::tuple<glm::vec2, Sprite*>> hugeBananas;
std::vector<Sprite*> pressStart;

int currentFrame = 0;
double elapsedTime = 0;
float timeSlice = 1.f / 120.f;

void Init()
{
	player = Player();
	player.Init(blit3D);
	player.Update(blit3D, 1);
	drid.player = player;

	drid.heartSprite = blit3D->MakeSprite(0, 0, 32, 32, "Media\\heart_banana.png");
	drid.bulletSprite = blit3D->MakeSprite(0, 0, 32, 32, "Media\\bullet_bullet.png");

	for (int i = 0; i < 12; i++)
		pressStart.push_back(blit3D->MakeSprite(i * 1920, 0, 1920, 1080, "Media\\press_start.png"));

	//drid.heartSprite = blit3D->MakeSprite(0, 0, 160, 160, "Media\\heart_banana.png");
	//drid.bulletSprite = blit3D->MakeSprite(0, 0, 160, 160, "Media\\bullet_bullet.png");

	drid.AddRandomGoblin(blit3D);

	//player.staticSprite = blit3D->MakeSprite(0, 0, 64, 64, player.getSpriteFilePath());
	//player.staticSprite = blit3D->MakeSprite(0, 0, 64, 64, "Media\\GreyChunk.png");
	//printf("%i", player.actionPoints);
}

void DeInit(void)
{
	//any sprites/fonts still allocated are freed automatically by the Blit3D object when we destroy it
}

void Update(double seconds)
{
	if (seconds < 0.15) {
		elapsedTime += seconds;
	}
	else {
		elapsedTime += 0.15;
	}

	//update by a full timeslice when it's time
	while (elapsedTime >= timeSlice)
	{
		if ((int)gameState == 0 || (int)gameState == 2) {
			if (hugeBananas.size() <= 1024) {
				for (int i = 0; i <= dice.Roll1DN(3); ++i) {
					Sprite* banana = blit3D->MakeSprite(0, 0, 160, 160, "Media\\huge_banana.png");
					banana->angle = dice.Roll1DN(360);
					hugeBananas.push_back(std::tuple<glm::vec2, Sprite*>{glm::vec2(dice.Roll1DN(1920), dice.Roll1DN(1080)), banana});
				}
			}
			if (currentFrame == 11)
				currentFrame = 0;
			else currentFrame++;
		}

		if ((int)gameState == 1) {
			timeSlice = 1.f / 120.f;
			if (!hugeBananas.empty()) {
				if (!hugeBananas.empty())
					hugeBananas.pop_back();
				if (!hugeBananas.empty())
					hugeBananas.pop_back();
				if (!hugeBananas.empty())
					hugeBananas.erase(hugeBananas.begin() + dice.Roll1DN(hugeBananas.size()) - 1);
			}
			drid.Update(blit3D, elapsedTime);
			if (drid.goblins.empty())
				drid.AddRandomGoblins(blit3D);
			if (drid.player.healthPoints == 0) {
				gameState = GameState::OVER;
			}
		}
		elapsedTime -= timeSlice;
	}
}

void Draw(void)
{
	glClearColor(0.197f / 0.256f, 0.207f / 0.256f, 0.196f / 0.256f, 1.f);	//clear colour: r,g,b,a
	// wipe the drawing surface clear
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	//draw stuff here

	//draw the background in the middle of the screen
	//the arguments to Blit(0 are the x, y pixel coords to draw the center of the sprite at,
	//starting as 0,0 in the bottom-left corner.
	//backgroundSprite->Blit(1920.f / 2, 1080.f / 2);

	if ((int)gameState >= 1) {
		drid.Draw(blit3D);
	}

	if (!hugeBananas.empty()) {
		for (auto& tuple : hugeBananas) {
			std::get<1>(tuple)->Blit(std::get<0>(tuple).x, std::get<0>(tuple).y);
		}
		if (hugeBananas.size() >= 512 && (int)gameState < 1) {
			pressStart[currentFrame]->Blit(1920.f / 2, 1080.f / 2);
			timeSlice = 0.07f;
		}
	}
	//////////////////////////////////////////////////

	//player.Draw(blit3D);
	//rotate the heart:
	//sprites have a public var called angle that determines the rotation in degrees.
}

//the key codes/actions/mods for DoInput are from GLFW: check its documentation for their values
void DoInput(int key, int scancode, int action, int mods)
{
	if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS)
		blit3D->Quit(); //start the shutdown sequence

	//////////////////////////////////////////////////

	if ((int)gameState == 0 /*|| (int)gameState == 2 TODO*/)
		if (key == GLFW_KEY_ENTER && action == GLFW_PRESS)
			gameState = GameState::PLAYING;
	//////////////////////////////////////////////////

	if ((int)gameState == 1 && hugeBananas.empty()) {
		if (key == GLFW_KEY_A && action == GLFW_PRESS)
			drid.player.WalkLeft();

		if (key == GLFW_KEY_D && action == GLFW_PRESS)
			drid.player.WalkRight();

		if (key == GLFW_KEY_W && action == GLFW_PRESS)
			drid.player.WalkUp();

		if (key == GLFW_KEY_S && action == GLFW_PRESS)
			drid.player.WalkDown();

		//////////////////////////////////////////////////

		if (key == GLFW_KEY_LEFT && action == GLFW_PRESS)
			drid.player.WalkLeft();

		if (key == GLFW_KEY_RIGHT && action == GLFW_PRESS)
			drid.player.WalkRight();

		if (key == GLFW_KEY_UP && action == GLFW_PRESS)
			drid.player.WalkUp();

		if (key == GLFW_KEY_DOWN && action == GLFW_PRESS)
			drid.player.WalkDown();

		//////////////////////////////////////////////////

		if (key == GLFW_KEY_SPACE && action == GLFW_PRESS)
			drid.player.Shoot();

		if (key == GLFW_KEY_R && action == GLFW_PRESS)
			drid.player.Reload();

		//////////////////////////////////////////////////

		if (key == GLFW_KEY_SLASH && action == GLFW_PRESS)
			drid.player.Recover();
	}
}

//called whenever the user resizes the window
void DoResize(int width, int height)
{
	blit3D->trueScreenWidth = blit3D->screenWidth = static_cast<float>(width);
	blit3D->trueScreenHeight = blit3D->screenHeight = static_cast<float>(height);
	blit3D->Reshape(blit3D->shader2d);
}

int main(int argc, char* argv[])
{
	//memory leak detection
	_CrtSetDbgFlag(_CRTDBG_ALLOC_MEM_DF | _CRTDBG_LEAK_CHECK_DF);

	//set X to the memory allocation number in order to force a break on the allocation:
	//useful for debugging memory leaks, as long as your memory allocations are deterministic.
	//_crtBreakAlloc = X;

	blit3D = new Blit3D(Blit3DWindowModel::BORDERLESSFULLSCREEN_1080P, 640, 400);

	//set our callback funcs
	blit3D->SetInit(Init);
	blit3D->SetDeInit(DeInit);
	blit3D->SetUpdate(Update);
	blit3D->SetDraw(Draw);
	blit3D->SetDoInput(DoInput);
	blit3D->SetDoResize(DoResize);

	//Run() blocks until the window is closed
	blit3D->Run(Blit3DThreadModel::SINGLETHREADED);
	if (blit3D) delete blit3D;
}