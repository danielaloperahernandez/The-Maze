#include "header.h"

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
 * renderWall - render wall projection
 *
*/

void renderWall(void)
{
	int x, y, texNum, texture_width, texture_height;
	int textureOffsetX, wallBottomPixel, wallStripHeight, wallTopPixel, distanceFromTop, textureOffsetY;
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
		/*for (y = 0; y < wallTopPixel; y++)
			drawPixel(x, y, 0xFFFF511B);*/
		if (rays[x].wasHitVertical)
			textureOffsetX = (int)rays[x].wallHitY % TILE_SIZE;
		else
			textureOffsetX = (int)rays[x].wallHitX % TILE_SIZE;

		texNum = rays[x].wallHitContent - 1;
		texture_width = wallTextures[texNum].width;
		texture_height = wallTextures[texNum].height;

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

		for (y = wallBottomPixel - 1; y < SCREEN_HEIGHT; y++)
		{

			float distance, ratio; 
		
			ratio = player.height / (y - SCREEN_HEIGHT/2);
			distance = (ratio * PROJ_PLANE) / cos(rays[x].rayAngle - player.rotationAngle);

			textureOffsetX = (int)((-distance * sin(rays[x].rayAngle)));
			textureOffsetY = (int)((distance * cos(rays[x].rayAngle)));

			textureOffsetX = abs(textureOffsetX);
			textureOffsetY = abs(textureOffsetY);

			/*textureOffsetX += abs(player.x);*/
			/*textureOffsetY += abs(player.y);*/

			textureOffsetX = (int)(textureOffsetX * texture_width/25)%texture_width;
			textureOffsetY = (int)(textureOffsetY * texture_height/25)%texture_height;

			texelColor = wallTextures[6].
									texture_buffer[(texture_width * textureOffsetY) + textureOffsetX];
			drawPixel(x, y, texelColor);

		}
		for (y = 0; y < wallTopPixel; y++)
		{
			float distance, ratio; 
		
			ratio = player.height / (SCREEN_HEIGHT/2 - y);
			distance =(float)(ratio * PROJ_PLANE) / cos(rays[x].rayAngle - player.rotationAngle);

			textureOffsetX = abs((int)(distance * sin(rays[x].rayAngle)));
			textureOffsetY = abs((int)(-distance * cos(rays[x].rayAngle)));


			/*textureOffsetY += abs(player.y);*/
			/*textureOffsetX += abs(player.x);*/


			textureOffsetX = (int)(textureOffsetX * texture_width/60)%texture_width;
			textureOffsetY = (int)(textureOffsetY * texture_width/60)%texture_width;

			texelColor = wallTextures[7].
									texture_buffer[(texture_width * textureOffsetX + textureOffsetY)];
			drawPixel(x, y, texelColor);
		}
	}
}
