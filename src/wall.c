#include "header.h"

/* Change the color intensity based on a factor value between 0 and 1 */

/**
 * changeColorIntensity - change color intensity
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
 * renderWallProjection - render wall projection
 *
*/

void renderWallProjection(void)
{
	int x, y, texNum, texture_width, texture_height;
	int textureOffsetX, wallBottomPixel, wallStripHeight, wallTopPixel;
	float perpDistance, projectedWallHeight;

	for (x = 0; x < NUM_RAYS; x++)
	{
		perpDistance = rays[x].distance * cos(rays[x].rayAngle
							- player.rotationAngle);
		projectedWallHeight = (TILE_SIZE / perpDistance) * PROJ_PLANE;
		wallStripHeight = (int)projectedWallHeight;
		wallTopPixel = (WINDOW_HEIGHT / 2) - (wallStripHeight / 2);
		wallTopPixel = wallTopPixel < 0 ? 0 : wallTopPixel;
		wallBottomPixel = (WINDOW_HEIGHT / 2) + (wallStripHeight / 2);
		wallBottomPixel = wallBottomPixel > WINDOW_HEIGHT
							? WINDOW_HEIGHT : wallBottomPixel;
		for (y = 0; y < wallTopPixel; y++)
			drawPixel(x, y, 0xFFFF511B);
		if (rays[x].wasHitVertical)
			textureOffsetX = (int)rays[x].wallHitY % TILE_SIZE;
		else
			textureOffsetX = (int)rays[x].wallHitX % TILE_SIZE;

		texNum = rays[x].wallHitContent - 1;
		texture_width = wallTextures[texNum].width;
		texture_height = wallTextures[texNum].height;

		for (y = wallTopPixel; y < wallBottomPixel; y++)
		{
			int distanceFromTop = y + (wallStripHeight / 2) - (WINDOW_HEIGHT / 2);
			int textureOffsetY = distanceFromTop *
								((float)texture_height / wallStripHeight);
			color_t texelColor = wallTextures[texNum].
									texture_buffer[(texture_width * textureOffsetY) + textureOffsetX];
			if (rays[x].wasHitVertical)
				changeColorIntensity(&texelColor, 0.5);
			drawPixel(x, y, texelColor);
		}
		for (y = wallBottomPixel; y < WINDOW_HEIGHT; y++)
			drawPixel(x, y, 0xFF3C5C72);
	}
}
