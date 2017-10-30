//
#include "stdafx.h"
#include "draw.h"
#include "ctime"
#include "Windows.h"
#include "conio.h"


//Screen dimension constants
const int SCREEN_WIDTH = 640;
const int SCREEN_HEIGHT = 480;

bool init();
bool loadMedia();
void close();

SDL_Texture* loadTexture(std::string path);
SDL_Window* gWindow = NULL;
SDL_Renderer* gRenderer = NULL;
SDL_Texture* gTexture = NULL;
SDL_Surface* loadedSurface = NULL;

extern int dxLine, dyLine, dxWindow, dyWindow, primitive, area, positionX, positionY;
extern float angle;

bool init()
{
	bool success = true;
	if (SDL_Init(SDL_INIT_VIDEO) < 0)
	{
		printf("SDL could not initialize! SDL Error: %s\n", SDL_GetError());
		success = false;
	}
	else
	{
		/*if (!SDL_SetHint(SDL_HINT_RENDER_SCALE_QUALITY, "1"))
		{
		printf("Warning: Linear texture filtering not enabled!");
		}*/
		gWindow = SDL_CreateWindow("AOKG Lab 1", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, SCREEN_WIDTH, SCREEN_HEIGHT, SDL_WINDOW_SHOWN);
		if (gWindow == NULL)
		{
			printf("Window could not be created! SDL Error: %s\n", SDL_GetError());
			success = false;
		}
		else
		{
			gRenderer = SDL_CreateRenderer(gWindow, -1, SDL_RENDERER_ACCELERATED);
			if (gRenderer == NULL)
			{
				printf("Renderer could not be created! SDL Error: %s\n", SDL_GetError());
				success = false;
			}
			else
			{
				SDL_SetRenderDrawColor(gRenderer, 0xFF, 0xFF, 0xFF, 0xFF);
			}
		}
	}
	return success;
}

bool loadMedia()
{
	bool success = true;
	gTexture = loadTexture("07_texture_loading_and_rendering/texture.png");
	if (gTexture == NULL)
	{
		printf("Failed to load texture image!\n");
		success = false;
	}
	return success;
}

void close()
{
	SDL_DestroyTexture(gTexture);
	gTexture = NULL;
	SDL_DestroyRenderer(gRenderer);
	SDL_DestroyWindow(gWindow);
	gWindow = NULL;
	gRenderer = NULL;
	SDL_Quit();
}

SDL_Texture* loadTexture(std::string path)
{
	SDL_Texture* newTexture = NULL;
	loadedSurface = SDL_CreateRGBSurface(0, 640, 480, 32,
		0x00FF0000,
		0x0000FF00,
		0x000000FF,
		0x00000000);

	if (loadedSurface == NULL)
	{
		printf("Unable to load image %s! SDL_image Error: \n", path.c_str());
	}
	else
	{
		newTexture = SDL_CreateTextureFromSurface(gRenderer, loadedSurface);
		if (newTexture == NULL)
		{
			printf("Unable to create texture from %s! SDL Error: %s\n", path.c_str(), SDL_GetError());
		}
	}
	return newTexture;
}

int _tmain(int argc, _TCHAR* argv[])
{
	int windowX = 320, windowY = 240;
	int cursorX = 0, cursorY = 0;
	int cursorActivity = 0;
	POINT cursor;
	fflush(stdin);
	if (!init())
	{
		printf("Failed to initialize!\n");
	}
	else
	{
		if (!loadMedia())
		{
			printf("Failed to load media!\n");
		}
		else
		{
			bool quit = false;
			SDL_Event e;
			while (!quit)
			{
				while (SDL_PollEvent(&e) != 0)
				{
					if (e.type == SDL_QUIT)
					{
						quit = true;
					}
					if (area == 1) {
						if (cursorActivity == 1) {
							GetPhysicalCursorPos(&cursor);
							positionX = cursor.x - 640;
							positionY = cursor.y - 300;
							if (positionX>cursorX)
								dxWindow += 5;
							if (positionX<cursorX)
								dxWindow -= 5;
							if (positionY>cursorY)
								dyWindow += 5;
							if (positionY<cursorY)
								dyWindow -= 5;
							cursorX = positionX;
							cursorY = positionY;
						}
						else {
							positionX = 0;
							positionY = 0;
						}
					}
					GetPhysicalCursorPos(&cursor);
					positionX = cursor.x - 400;
					positionY = cursor.y - 200;
					if (e.type == SDL_MOUSEBUTTONDOWN)
						if (e.button.button == SDL_BUTTON_LEFT) {
							cursorActivity = 1;
						}
					if (e.type == SDL_MOUSEBUTTONUP)
						if (e.button.button == SDL_BUTTON_LEFT)
							cursorActivity = 0;
					if (e.type == SDL_KEYDOWN) {
						if (e.key.keysym.scancode == SDL_SCANCODE_ESCAPE) {
							quit = true;
						}
						if (e.key.keysym.scancode == SDL_SCANCODE_E) {
							angle += M_PI / 36;
						}
						if (e.key.keysym.scancode == SDL_SCANCODE_Q) {
							angle -= M_PI / 36;
						}
						if (e.key.keysym.scancode == SDL_SCANCODE_D) {
							windowX += 5;
						}
						if (e.key.keysym.scancode == SDL_SCANCODE_A) {
							windowX -= 5;
						}
						if (e.key.keysym.scancode == SDL_SCANCODE_W) {
							windowY += 5;
						}
						if (e.key.keysym.scancode == SDL_SCANCODE_S) {
							windowY -= 5;
						}
					}
				}
				SDL_RenderClear(gRenderer);
				draw(loadedSurface, windowX, windowY);
				SDL_UpdateTexture(gTexture, NULL, loadedSurface->pixels, loadedSurface->pitch);
				SDL_RenderCopy(gRenderer, gTexture, NULL, NULL);
				SDL_RenderPresent(gRenderer);
			}
		}
	}
	close();
	return 0;
}
