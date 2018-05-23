#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <SDL2/SDL_mixer.h>
#include <SDL2/SDL_ttf.h>
#include <iostream>
#include <string>


using namespace std;

typedef SDL_Texture G_Texture;
typedef SDL_Rect G_Rect;
typedef TTF_Font G_Font;
typedef Mix_Music G_Music;
typedef Mix_Chunk G_Sound;

bool G_Running = true;

SDL_Window* window = NULL;
SDL_Renderer* renderer = NULL;
SDL_Event event;

//mouse and keyboard structs
struct GKey
{
	bool down;
	GKey()
	{
		down = false;
	}
}GK_Z, GK_X, GK_ESC, GK_UP, GK_DOWN, GK_RIGHT, GK_LEFT;
struct GMouse
{
	bool down;
	int X;
	int Y;
	int X_Motion;
	int Y_Motion;
	GMouse()
	{
		down = false;
	}

}GM_LEFT, GM_MIDDLE, GM_RIGHT;

//init all SDL
bool G_InitSDL()
{
	if ((SDL_Init(SDL_INIT_EVERYTHING) == 0) && (Mix_OpenAudio(44100, MIX_DEFAULT_FORMAT, 2, 2048) == 0) && (TTF_Init() == 0))
		return true;
	return false;
}
//creat window and set renderer
bool G_CreatWindow(char* title, G_Rect win, Uint8 r, Uint8 g, Uint8 b, int flag = 0)
{
	window = SDL_CreateWindow(title, win.x, win.y, win.w, win.h, flag);
	if (window != 0)
	{
		renderer = SDL_CreateRenderer(window, -1, 0);
		if (renderer != 0)
		{
			SDL_SetRenderDrawColor(renderer, r, g, b, 255);
			return true;
		}
	}
	return false;
}
//load image with any format and overloaded to remove rgb color from image
SDL_Texture* G_LoadImage(const char* file)
{
	SDL_Surface* surface = IMG_Load(file);
	if (surface != 0)
	{
		SDL_Texture* texture = SDL_CreateTextureFromSurface(renderer, surface);
		SDL_FreeSurface(surface);
		if (texture != 0)
		{
			return texture;
		}
	}
	return NULL;
}
SDL_Texture* G_LoadImage(const char *file, Uint8 r, Uint8 g, Uint8 b)
{
	SDL_Surface* surface = IMG_Load(file);
	if (surface != 0)
	{
		SDL_SetColorKey(surface, SDL_TRUE, SDL_MapRGB(surface->format, r, g, b));
		SDL_Texture* texture = SDL_CreateTextureFromSurface(renderer, surface);
		SDL_FreeSurface(surface);
		if (texture != 0)
		{
			return texture;
		}
	}
	return NULL;
}
//draw functions(over loaded) based on source and destination kinds (all 4 possible cases)
void G_Draw(G_Texture* texture, G_Rect *src, G_Rect *dst, bool fullscreen = false)
{
	if (!fullscreen)
		SDL_RenderCopy(renderer, texture, src, dst);
	else
		SDL_RenderCopy(renderer, texture, src, 0);
}
void G_Draw(G_Texture* texture, G_Rect *dst, bool fullscreen = false)
{
	if (!fullscreen)
		SDL_RenderCopy(renderer, texture, 0, dst);
	else
		SDL_RenderCopy(renderer, texture, 0, 0);
}
void G_DrawEx(G_Texture* texture, G_Rect *src, G_Rect *dst, SDL_RendererFlip flip, bool fullscreen = false)
{
	if (!fullscreen)
		SDL_RenderCopyEx(renderer, texture, src, dst, 0, 0, flip);
	else
		SDL_RenderCopyEx(renderer, texture, src, 0, 0, 0, flip);
}
void G_DrawEx(G_Texture* texture, G_Rect *dst, SDL_RendererFlip flip, bool fullscreen = false)
{
	if (!fullscreen)
		SDL_RenderCopyEx(renderer, texture, 0, dst, 0, 0, flip);
	else
		SDL_RenderCopyEx(renderer, texture, 0, 0, 0, 0, flip);
}

//update sceen and keyboard and mouse structs
void G_Update()
{
	if (SDL_PollEvent(&event) != 0)
	{
		switch (event.type)
		{
		case SDL_QUIT:
			G_Running = false;
			break;
		case SDL_KEYDOWN:
			switch (event.key.keysym.sym)
			{
			case SDLK_z:
				GK_Z.down = true;
				break;
			case SDLK_x:
				GK_X.down = true;
				break;
			case SDLK_ESCAPE:
				GK_ESC.down = true;
				break;
			case SDLK_UP:
				GK_UP.down = true;
				break;
			case SDLK_DOWN:
				GK_DOWN.down = true;
				break;
			case SDLK_LEFT:
				GK_LEFT.down = true;
				break;
			case SDLK_RIGHT:
				GK_RIGHT.down = true;
				break;
			}
			break;
		case SDL_KEYUP:
			switch (event.key.keysym.sym)
			{
			case SDLK_z:
				GK_Z.down = false;
				break;
			case SDLK_x:
				GK_X.down = false;
				break;
			case SDLK_ESCAPE:
				GK_ESC.down = false;
				break;
			case SDLK_UP:
				GK_UP.down = false;
				break;
			case SDLK_DOWN:
				GK_DOWN.down = false;
				break;
			case SDLK_LEFT:
				GK_LEFT.down = false;
				break;
			case SDLK_RIGHT:
				GK_RIGHT.down = false;
				break;
			}
			break;
		case SDL_MOUSEBUTTONDOWN:
			switch (event.button.button)
			{
			case SDL_BUTTON_LEFT:
				GM_LEFT.down = true;
				break;
			case SDL_BUTTON_RIGHT:
				GM_RIGHT.down = true;
				break;
			case SDL_BUTTON_MIDDLE:
				GM_MIDDLE.down = true;
				break;
			}
			GM_LEFT.X = event.button.x;
			GM_LEFT.Y = event.button.y;

			GM_MIDDLE.X = event.button.x;
			GM_MIDDLE.Y = event.button.y;

			GM_RIGHT.X = event.button.x;
			GM_RIGHT.Y = event.button.y;

			break;
		case SDL_MOUSEBUTTONUP:{
			switch (event.button.button)
			{
			case SDL_BUTTON_LEFT:
				GM_LEFT.down = false;
				break;
			case SDL_BUTTON_RIGHT:
				GM_RIGHT.down = false;
				break;
			case SDL_BUTTON_MIDDLE:
				GM_MIDDLE.down = false;
				break;
			}
		}
			break;
		case SDL_MOUSEMOTION: {

			GM_LEFT.X_Motion = event.motion.x;
			GM_LEFT.Y_Motion = event.motion.y;

			GM_MIDDLE.X_Motion = event.motion.x;
			GM_MIDDLE.Y_Motion = event.motion.y;

			GM_RIGHT.X_Motion = event.motion.x;
			GM_RIGHT.Y_Motion = event.motion.y;
		}

		}
	}

	SDL_RenderPresent(renderer);
	SDL_RenderClear(renderer);
}

//font
TTF_Font* G_OpenFont(char* file, int fontSize)
{
	return TTF_OpenFont(file, fontSize);
}
SDL_Texture* G_LoadFont(G_Font* font, char* title, Uint8 r, Uint8 g, Uint8 b)
{
	SDL_Color color;
	color.r = r;
	color.g = g;
	color.b = b;
	color.a = 255;
	SDL_Surface* surface = TTF_RenderText_Solid(font, title, color);
	SDL_Texture* texture = SDL_CreateTextureFromSurface(renderer, surface);
	SDL_FreeSurface(surface);
	return texture;
}

//music
Mix_Music* G_LoadMusic(char* file)
{
	return Mix_LoadMUS(file);
}
void G_PlayMusic(G_Music* music, int loop = -1)
{
	Mix_PlayMusic(music, loop);
}
void G_PauseMusic()
{
	Mix_PauseMusic();
}
void G_ResumeMusic()
{
	Mix_ResumeMusic();
}
void G_StopMusic()
{
	Mix_HaltMusic();
}
void G_FreeMusic(G_Music* music)
{
	Mix_FreeMusic(music);
}

//sound
Mix_Chunk* G_LoadSound(char* file)
{
	return Mix_LoadWAV(file);
}
void G_PlaySound(G_Sound* sound, int loop)
{
	Mix_PlayChannel(-1, sound, loop);
}
void G_PauseSound()
{
	Mix_Pause(-1);
}
void G_FreeSound(G_Sound* sound)
{
	Mix_FreeChunk(sound);
}

//delay and getticks
void G_Delay(Uint32 second)
{
	SDL_Delay(second);
}
Uint32 G_GetTicks()
{
	return SDL_GetTicks();
}

//quit
void G_DestroyTexture(G_Texture *texture)
{
	SDL_DestroyTexture(texture);
}
void G_Quit()
{
	IMG_Quit();
	SDL_DestroyRenderer(renderer);
	SDL_DestroyWindow(window);
	SDL_Quit();
}
