#ifndef GAME_H
#define GAME_H

#include <stdio.h> //déclarer fct conversion nombre + gestion memoire
#include <stdlib.h> //bibli basique du c = macros ou autre 
#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>

typedef struct {

    // Variables SDL + Screen
    SDL_Point screenSize;
    SDL_Window* pWindow;
    SDL_Renderer* pRenderer;

    //Texture
    SDL_Texture* pTextPlayer;
    SDL_Texture* pTextBomb;

    //jeu
    SDL_Rect playerPositionRect;
    SDL_Rect bombPositionRect;

} stGame;

stGame* game_init();
void game_destroy(stGame* game);
void game_draw(stGame* game);
int game_event(stGame* game);
void game_movePlayer(stGame* game, SDL_Keycode direction);
void game_placeBomb(stGame* game, SDL_Keycode poseBomb);


#endif /* GAME_H */