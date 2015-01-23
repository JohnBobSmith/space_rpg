#include "GameEngine.h"
#include "Game.h"
#include <iostream>

SDL_Window *engineWindow = nullptr;
SDL_Renderer *engineRenderer = nullptr;

SDL_Texture *load_rendered_text(std::string text, TTF_Font *font, SDL_Color textColor)
{
    SDL_Texture *newTexture = nullptr;
    SDL_Surface *textSurface = TTF_RenderText_Blended(font, text.c_str(), textColor);

    if(textSurface == nullptr){
        std::cout << "Failed to render text surface... SDL_ttf Error: " << TTF_GetError() << std::endl;
    } else {
        newTexture = SDL_CreateTextureFromSurface(engineRenderer, textSurface);
        if(newTexture == nullptr){
            std::cout << "Failed to create texture from surface... SDL Error: " << SDL_GetError() << std::endl;
        }
        SDL_FreeSurface(textSurface);
    }
    return newTexture;
}

SDL_Texture *load_image_from_file(std::string path)
{
	SDL_Texture* newTexture = nullptr;
	SDL_Surface* loadedSurface = IMG_Load(path.c_str());

	if(loadedSurface == nullptr) {
            std::cout << "Unable to load image " << path.c_str() << " SDL_image Error: " << IMG_GetError();
	} else {
        newTexture = SDL_CreateTextureFromSurface(engineRenderer, loadedSurface);
		if(newTexture == nullptr) {
            std::cout << "Unable to create texture from " << path.c_str() << " SDL Error: " << SDL_GetError();
		}
		SDL_FreeSurface(loadedSurface);
	}
	return newTexture;
}

void render_image(int xPosition, int yPosition, int width, int height, float angle, SDL_Texture *texture,
                                    SDL_Rect *sRect, SDL_Point *center, SDL_RendererFlip flip)
{
    SDL_Rect dRect = {xPosition, yPosition, width, height};

    if(sRect != nullptr){
        dRect.w = sRect->w;
        dRect.h = sRect->h;
    }

    SDL_RenderCopyEx(engineRenderer, texture, sRect, &dRect, angle, center, flip);
}

void render_sdl_geometry(SDL_Rect rectToRender, int red, int green, int blue, int alpha)
{
    SDL_SetRenderDrawColor(engineRenderer, red, green, blue, alpha);
    SDL_RenderFillRect(engineRenderer, &rectToRender);
}

bool init_engine()
{
    if(SDL_Init(SDL_INIT_VIDEO | SDL_INIT_AUDIO) < 0){
        std::cout << "Failed to init video... SDL Error: " << SDL_GetError() << std::endl;
        return false;
    }

    if(!SDL_SetHint(SDL_HINT_RENDER_SCALE_QUALITY, "1")){
        std::cout << "Linear texture filtering not enabled...." << std::endl;
    }

    engineWindow = SDL_CreateWindow("Spaceship RPG", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, SCREEN_WIDTH, SCREEN_HEIGHT, SDL_WINDOW_SHOWN);
    if(engineWindow == nullptr){
        std::cout << "Failed to create window... SDL Error: " << SDL_GetError() << std::endl;
        return false;
    }

    engineRenderer = SDL_CreateRenderer(engineWindow, -1, SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC);
    if(engineRenderer == nullptr){
        std::cout << "Failed to create renderer... SDL Error: " << SDL_GetError() << std::endl;
        return false;
    }

    int imageFlags = IMG_INIT_PNG;
    if(!(IMG_Init(imageFlags) &imageFlags)){
        std::cout << "SDL_image failed to initialize... SDL_image Error: " << IMG_GetError() << std::endl;
        return false;
    }

    if(Mix_OpenAudio(44100, MIX_DEFAULT_FORMAT, 2, 2048) < 0){
        std::cout << "SDL_mixer failed to initialize... SDL_mixer Error: " << Mix_GetError() << std::endl;
        return false;
    }

    if(TTF_Init() == -1){
        std::cout << "SDL_ttf failed to initialize... SDL_ttf Error: " << TTF_GetError() << std::endl;
        return false;
    }
    return true;
}

bool load_engine()
{
    std::cout << "Nothing to load in load_engine(), continuing." << std::endl;
    return true;
}

void close_engine()
{
    SDL_DestroyRenderer(engineRenderer);
    SDL_DestroyWindow(engineWindow);

    engineRenderer = nullptr;
    engineWindow = nullptr;
}

void run_engine()
{
    SDL_Event event;
    bool isRunning = true;

    if(!init_engine()){
        std::cout << "Failed to init engine..." << std::endl;
    }

    if(!load_engine()){
        std::cout << "Failed to load engine..." << std::endl;
    }

    if(!init_game()){
        std::cout << "Failed to init game...." << std::endl;
    }

    if(!load_game()){
        std::cout << "Failed to load game..." << std::endl;
    }

    create_bullet();

    while(isRunning){
        while(SDL_PollEvent(&event) != 0){
            if(event.type == SDL_QUIT){
                isRunning = false;
            }
            handle_player_events(event);
        }
        SDL_SetRenderDrawColor(engineRenderer, 0, 0, 0, 255);
        SDL_RenderClear(engineRenderer);

        move_player();
        move_bullet();
        move_camera();
        render_game();

        SDL_RenderPresent(engineRenderer);
    }
    close_game();
    close_engine();
}
