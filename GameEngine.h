#ifndef GAMEENGINE_H
#define GAMEENGINE_H

#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <SDL2/SDL_ttf.h>
#include <SDL2/SDL_mixer.h>
#include <string>

const int SCREEN_WIDTH = 640;
const int SCREEN_HEIGHT = 480;

extern SDL_Texture *load_rendered_text(std::string text, TTF_Font *font, SDL_Color textColor);
extern SDL_Texture *load_image_from_file(std::string path);
extern void render_image(int xPosition, int yPosition, int width, int height, float angle, SDL_Texture *texture,
                                SDL_Rect *sRect, SDL_Point *center, SDL_RendererFlip flip);

extern void render_sdl_geometry(SDL_Rect rectToRender, int red, int green, int blue, int alpha);

extern void run_engine();
extern bool init_engine();
extern bool load_engine();
extern void close_engine();

#endif // GAMEENGINE_H

