#ifndef GAME_H
#define GAME_H

#include "GameEngine.h"

typedef struct Bullet
{
    SDL_Texture *texture;

    bool isDead;

    int startX, startY;

    int xPosition;
    int yPosition;

    int xVelocity;
    int yVelocity;

    int width, height;
    int angle;

} Bullet;

const int LEVEL_WIDTH = 1024;
const int LEVEL_HEIGHT = 1024;

extern void create_bullet();
extern void move_bullet();
extern void render_bullet();
extern void check_bullet_collision(SDL_Rect bullet, SDL_Rect objectWeHit);

extern void handle_player_events(SDL_Event &event);
extern void move_player();
extern void render_player(int camX, int camY);

extern void move_camera();

extern bool check_collision(SDL_Rect a, SDL_Rect b);

extern void render_game();

extern bool init_game();
extern bool load_game();
extern void close_game();

#endif // GAME_H
