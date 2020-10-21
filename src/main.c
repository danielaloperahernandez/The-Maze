#include "header.h"

bool isGameRunning = false;
int ticksLastFrame;
player_t player;

/**
 * setup - initialize player variables and load wall textures
 *
*/

void setup(void)
{

	player.x = WINDOW_WIDTH / 2;
	player.y = WINDOW_HEIGHT / 2;
	player.width = 1;
	player.height = 1;
	player.turnDirection = 0;
	player.walkDirection = 0;
	player.rotationAngle = PI / 2;
	player.walkSpeed = 100;
	player.turnSpeed = 45 * (PI / 180);
	loadWallTextures();
}


/**
 * update - update delta time, the ticks last frame
 *          the player movement and the ray casting
 *
*/
void update(void)
{
	float deltaTime;
	int timeToWait = FRAME_TIME_LENGTH - (SDL_GetTicks() - ticksLastFrame);

	if (timeToWait > 0 && timeToWait <= FRAME_TIME_LENGTH)
	{
		SDL_Delay(timeToWait);
	}
	deltaTime = (SDL_GetTicks() - ticksLastFrame) / 1000.0f;

	ticksLastFrame = SDL_GetTicks();

	movePlayer(deltaTime);
	castAllRays();
}

/**
 * render - calls all functions needed for on-screen rendering
 *
*/

void render(void)
{
	clearColorBuffer(0xFF000000);

	renderWallProjection();

	renderMap();
	renderRays();
	renderPlayer();

	renderColorBuffer();
}

/**
 * Destroy - free wall textures and destroy window
 *
*/
void Destroy(void)
{

	destroyWindow();
}

/**
 * main - main function
 * Return: 0
*/

int main(void)
{
	isGameRunning = initializeWindow();

	setup();

	while (isGameRunning)
	{
		processInput();
		update();
		render();
	}

	Destroy();

	return (0);
}
