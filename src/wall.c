#include "../headers/header.h"

/**
 * changeColorIntensity - change color intensity
 * based on a factor value between 0 and 1
 * @factor: intensity factor
 * @color: color for intensity
*/

void changeColorIntensity(color_t *color, float factor)
{
	color_t a = (*color & 0xFF000000);
	color_t r = (*color & 0x00FF0000) * factor;
	color_t g = (*color & 0x0000FF00) * factor;
	color_t b = (*color & 0x000000FF) * factor;

	*color = a | (r & 0x00FF0000) | (g & 0x0000FF00) | (b & 0x000000FF);
}

/**
 * renderFloor - render floor projection
 *
 * @WallBottomPixel: wall bottom pixel
 * @texelColor: texture color for current pixel
 * @x: current element in the rays array
*/

void renderFloor(int wallBottomPixel, color_t *texelColor, int x)
{
	int y, texture_height, texture_width, textureOffsetY, textureOffsetX;
	float distance, ratio;

	texture_width = wallTextures[3].width;
	texture_height = wallTextures[3].height;

	for (y = wallBottomPixel - 1; y < SCREEN_HEIGHT; y++)
	{
		ratio = player.height / (y - SCREEN_HEIGHT / 2);
		distance = (ratio * PROJ_PLANE)
					/ cos(rays[x].rayAngle - player.rotationAngle);

		textureOffsetY = (int)abs((distance * sin(rays[x].rayAngle)) + player.y);
		textureOffsetX = (int)abs((distance * cos(rays[x].rayAngle)) + player.x);

		textureOffsetX = (int)(abs(textureOffsetX * texture_width / 30)
								% texture_width);
		textureOffsetY = (int)(abs(textureOffsetY * texture_height / 30)
								% texture_height);

		*texelColor = wallTextures[4].
					  texture_buffer[(texture_width * textureOffsetY) + textureOffsetX];
		drawPixel(x, y, *texelColor);
	}
}

/**
 * renderCeil - render Ceil projection
 * @WallTopPixel: wall top pixel
 * @texelColor: texture color for current pixel
 * @x: current element in the rays array
*/

void renderCeil(int wallTopPixel, color_t *texelColor, int x)
{
	int y, texture_width, texture_height, textureOffsetY, textureOffsetX;

	texture_width = wallTextures[3].width;
	texture_height = wallTextures[3].height;

	for (y = 0; y < wallTopPixel; y++)
	{
		float distance, ratio;

		ratio = player.height / (y - SCREEN_HEIGHT / 2);
		distance = (ratio * PROJ_PLANE)
					/ cos(rays[x].rayAngle - player.rotationAngle);

		textureOffsetY = (int)abs((-distance * sin(rays[x].rayAngle)) + player.y);
		textureOffsetX = (int)abs((-distance * cos(rays[x].rayAngle)) + player.x);

		textureOffsetX = (int)(abs(textureOffsetX * texture_width / 40)
								% texture_width);
		textureOffsetY = (int)(abs(textureOffsetY * texture_height / 40)
								% texture_height);

		*texelColor = wallTextures[6].
					  texture_buffer[(texture_width * textureOffsetY) + textureOffsetX];
		drawPixel(x, y, *texelColor);

	}
}

/**
 * renderWall - render wall projection
 *
*/
void renderWall(void)
{
	int x, y, texNum, texture_width, texture_height,
		textureOffsetX, wallBottomPixel, wallStripHeight,
		wallTopPixel, distanceFromTop, textureOffsetY;
	float perpDistance, projectedWallHeight;
	color_t texelColor;

	for (x = 0; x < NUM_RAYS; x++)
	{
		perpDistance = rays[x].distance * cos(rays[x].rayAngle
							- player.rotationAngle);
		projectedWallHeight = (TILE_SIZE / perpDistance) * PROJ_PLANE;
		wallStripHeight = (int)projectedWallHeight;
		wallTopPixel = (SCREEN_HEIGHT / 2) - (wallStripHeight / 2);
		wallTopPixel = wallTopPixel < 0 ? 0 : wallTopPixel;
		wallBottomPixel = (SCREEN_HEIGHT / 2) + (wallStripHeight / 2);
		wallBottomPixel = wallBottomPixel > SCREEN_HEIGHT
							? SCREEN_HEIGHT : wallBottomPixel;
		texNum = rays[x].wallHitContent - 1;
		texture_width = wallTextures[texNum].width;
		texture_height = wallTextures[texNum].height;
		renderFloor(wallBottomPixel, &texelColor, x);
		renderCeil(wallTopPixel, &texelColor, x);

		if (rays[x].wasHitVertical)
			textureOffsetX = (int)rays[x].wallHitY % TILE_SIZE;
		else
			textureOffsetX = (int)rays[x].wallHitX % TILE_SIZE;

		for (y = wallTopPixel; y < wallBottomPixel; y++)
		{
			distanceFromTop = y + (wallStripHeight / 2) - (SCREEN_HEIGHT / 2);
			textureOffsetY = distanceFromTop *
								((float)texture_height / wallStripHeight);
			texelColor = wallTextures[texNum].
						 texture_buffer[(texture_width * textureOffsetY) + textureOffsetX];
			if (rays[x].wasHitVertical)
				changeColorIntensity(&texelColor, 0.5);
			drawPixel(x, y, texelColor);
		}
	}
}
