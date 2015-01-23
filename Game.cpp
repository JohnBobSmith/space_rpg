#include "Game.h"
#include <vector>
#include <math.h>
#include <iostream>

SDL_Texture *playerTexture = nullptr;
SDL_Texture *backgroundTexture = nullptr;
SDL_Texture *bulletTexture = nullptr;

SDL_Rect playerCollider;
SDL_Rect camera = {0, 0, SCREEN_WIDTH, SCREEN_HEIGHT};

std::vector<Bullet*> bulletStorage;

const int PLAYER_VELOCITY = 2;
const int BULLET_VELOCITY = 5;
const int MAX_BULLETS = 5;

float mouseAngle;

int currentBullet = 0;

int playerVelocityX;
int playerVelocityY;

int playerPositionX;
int playerPositionY;

int playerWidth;
int playerHeight;

void create_bullet()
{
    for(int i = 0; i < MAX_BULLETS; i++){
        bulletStorage[i] = new Bullet;
        bulletStorage[i]->texture = bulletTexture;
        bulletStorage[i]->isDead = true;
        bulletStorage[i]->startX = playerPositionX;
        bulletStorage[i]->startY = playerPositionY;
        bulletStorage[i]->xPosition = playerPositionX;
        bulletStorage[i]->yPosition = playerPositionY;
        bulletStorage[i]->xVelocity = 0;
        bulletStorage[i]->yVelocity = 0;
        bulletStorage[i]->width = 25;
        bulletStorage[i]->height = 50;
        bulletStorage[i]->angle = 90;
    }
}

void move_bullet()
{

    for(int i = 0; i < MAX_BULLETS; i++){
        if(!bulletStorage[i]->isDead){
            bulletStorage[i]->xPosition += bulletStorage[i]->xVelocity;
            bulletStorage[i]->yPosition += bulletStorage[i]->yVelocity;
        }
        if(bulletStorage[i]->xPosition < 0 || bulletStorage[i]->xPosition > SCREEN_WIDTH ||
                    bulletStorage[i]->yPosition < 0 || bulletStorage[i]->yPosition > SCREEN_HEIGHT){

            bulletStorage[i]->isDead = true;
            bulletStorage[i]->xPosition = playerPositionX - camera.x;
            bulletStorage[i]->yPosition = playerPositionY - camera.y;
        }

        if(bulletStorage[i]->isDead){
            bulletStorage[i]->xPosition = playerPositionX - camera.x;
            bulletStorage[i]->yPosition = playerPositionY - camera.y;
        }
    }
}

void render_bullet()
{
    for(int i = 0; i < MAX_BULLETS; i++){
        if(!bulletStorage[i]->isDead){
            render_image(bulletStorage[i]->xPosition, bulletStorage[i]->yPosition, bulletStorage[i]->width, bulletStorage[i]->height,
                                bulletStorage[i]->angle, bulletStorage[i]->texture, nullptr, nullptr, SDL_FLIP_NONE);
        }
    }
}

void check_bullet_collision(SDL_Rect bullet, SDL_Rect objectWeHit)
{
    //add collision detection stuff here.
}

void handle_player_events(SDL_Event &event)
{
    int mouseX;
    int mouseY;

    SDL_Point mousePoint;

    if(event.type == SDL_KEYDOWN && event.key.repeat == 0)
    {
        switch(event.key.keysym.sym)
        {
            case SDLK_w:
            case SDLK_UP:
                playerVelocityY -= PLAYER_VELOCITY;
                break;

            case SDLK_s:
            case SDLK_DOWN:
                playerVelocityY += PLAYER_VELOCITY;
                break;

            case SDLK_a:
            case SDLK_LEFT:
                playerVelocityX -= PLAYER_VELOCITY;
                break;

            case SDLK_d:
            case SDLK_RIGHT:
                playerVelocityX += PLAYER_VELOCITY;
                break;
        }
    }

    else if(event.type == SDL_KEYUP && event.key.repeat == 0)
    {
        switch(event.key.keysym.sym)
        {
            case SDLK_w:
            case SDLK_UP:
                playerVelocityY += PLAYER_VELOCITY;
                break;

            case SDLK_s:
            case SDLK_DOWN:
                playerVelocityY -= PLAYER_VELOCITY;
                break;

            case SDLK_a:
            case SDLK_LEFT:
                playerVelocityX += PLAYER_VELOCITY;
                break;

            case SDLK_d:
            case SDLK_RIGHT:
                playerVelocityX -= PLAYER_VELOCITY;
                break;
        }
    }

    if(event.type == SDL_MOUSEMOTION){

        int xCenter;
        int yCenter;

        float result;

        SDL_GetMouseState(&mouseX, &mouseY);

        xCenter = playerPositionX - camera.x + playerWidth / 2;
        yCenter = playerPositionY - camera.y + playerHeight / 2;
        result = atan2(mouseY - yCenter, mouseX - xCenter) * 180 / 3.14;

        mouseAngle = result;
    }

    if(event.type == SDL_MOUSEBUTTONDOWN){
        currentBullet += 1;

        for(int i = 0; i < MAX_BULLETS; i++){
            bulletStorage[i]->isDead = false;
            bulletStorage[i]->angle = mouseAngle;
            bulletStorage[i]->xVelocity = BULLET_VELOCITY * (cos(mouseAngle * 3.14 / 180));
            bulletStorage[i]->yVelocity = BULLET_VELOCITY * (sin(mouseAngle * 3.14 / 180));
        }

        if(currentBullet == MAX_BULLETS){
            currentBullet = 0;
        }
    }
}

void move_player()
{
    playerPositionX += playerVelocityX;
    playerCollider.x = playerPositionX;

    if(playerPositionX < 0 || (playerPositionX + playerWidth) > LEVEL_WIDTH){
        playerPositionX -= playerVelocityX;
        playerCollider.x = playerPositionX;
    }

    playerPositionY += playerVelocityY;
    playerCollider.y = playerPositionY;

    if(playerPositionY < 0 || (playerPositionY + playerHeight) > LEVEL_HEIGHT){
        playerPositionY -= playerVelocityY;
        playerCollider.y = playerPositionY;
    }
}

void render_player(int camX, int camY, float angle)
{
    render_image(playerPositionX - camX, playerPositionY - camY, playerWidth, playerHeight, angle,
                        playerTexture, nullptr, nullptr, SDL_FLIP_NONE);
}

void move_camera()
{
    camera.x = (playerPositionX + playerWidth / 2) - SCREEN_WIDTH / 2;
    camera.y = (playerPositionY + playerHeight / 2) - SCREEN_HEIGHT / 2;

    if(camera.x < 0) {
        camera.x = 0;
    }

    if(camera.y < 0){
        camera.y = 0;
    }

    if(camera.x > LEVEL_WIDTH - camera.w) {
        camera.x = LEVEL_WIDTH - camera.w;
    }

    if(camera.y > LEVEL_HEIGHT - camera.h) {
        camera.y = LEVEL_HEIGHT - camera.h;
    }
}

bool check_collision(SDL_Rect a, SDL_Rect b)
{
    int leftA, leftB;
    int rightA, rightB;
    int topA, topB;
    int bottomA, bottomB;

    leftA = a.x;
    rightA = a.x + a.w;
    topA = a.y;
    bottomA = a.y + a.h;

    leftB = b.x;
    rightB = b.x + b.w;
    topB = b.y;
    bottomB = b.y + b.h;

    if(bottomA <= topB) {
        return false;
    }

    if(topA >= bottomB) {
        return false;
    }

    if(rightA <= leftB) {
        return false;
    }

    if(leftA >= rightB) {
        return false;
    }

    return true;
}

void render_game()
{
    render_image(0, 0, LEVEL_WIDTH, LEVEL_HEIGHT, 0.0, backgroundTexture, &camera, nullptr, SDL_FLIP_NONE);
    render_player(camera.x, camera.y, mouseAngle);
    render_bullet();
}

bool init_game()
{
    playerHeight = 50;
    playerWidth = 50;
    playerPositionX = 10;
    playerPositionY = 10;
    playerVelocityX = 0;
    playerVelocityY = 0;

    playerCollider.w = playerWidth;
    playerCollider.h = playerHeight;

    bulletStorage.resize(100);

    return true;
}

bool load_game()
{
    playerTexture = load_image_from_file("images/player.png");
    if(playerTexture == nullptr){
        std::cout << "playerTexture is nullptr..." << std::endl;
        return false;
    }

    backgroundTexture = load_image_from_file("images/wall_bg.png");
    if(backgroundTexture == nullptr){
        std::cout << "backgroundTexture is nullptr..." << std::endl;
        return false;
    }

    bulletTexture = load_image_from_file("images/bullet.png");
    if(bulletTexture == nullptr){
        std::cout << "bulletTexture is nullptr..." << std::endl;
        return false;
    }
    return true;
}

void close_game()
{
    //NO MEMORY LEAKS ALLOWED!!!!!!!!!!
    SDL_DestroyTexture(playerTexture);
    SDL_DestroyTexture(backgroundTexture);

    playerTexture = nullptr;
    backgroundTexture = nullptr;

    for(int i = 0; i < MAX_BULLETS; i++){
        delete bulletStorage[i];
    }
}
