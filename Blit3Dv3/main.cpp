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

Blit3D* blit3D = NULL;

//GLOBAL DATA
Player player;
Grid drid;
double elapsedTime = 0;
float timeSlice = 1.f /30.f ;

void Init()
{
	player = Player();
	player.Init(blit3D);
	player.Update(blit3D);
	drid.player = player;

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
		elapsedTime -= timeSlice;
		drid.Update(blit3D);
		if (drid.goblins.empty())
			drid.AddRandomGoblins(blit3D);
	}
 }

void Draw(void)
{
	glClearColor(1.0f, 1.0f, 1.0f, 1.0f);	//clear colour: r,g,b,a
	// wipe the drawing surface clear
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	//draw stuff here

	//draw the background in the middle of the screen
	//the arguments to Blit(0 are the x, y pixel coords to draw the center of the sprite at,
	//starting as 0,0 in the bottom-left corner.
	//backgroundSprite->Blit(1920.f / 2, 1080.f / 2);
	drid.Draw(blit3D);
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