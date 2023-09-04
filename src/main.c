#include "game.h"

int main(int argc, char *argv[]){ //argv = tableau

    stGame* game = game_init();

    int quit = 0;
    while(quit != -1) {
        
        //Dessiner le jeu
        game_draw(game);

        //gerer les evenements
        quit = game_event(game);

        //attendre un peu en vif
        SDL_Delay(20); //sert a pas monopoliser le processeur dans la boucle = laisser ressources pour autres app
    }

    game_draw(game);

    game_destroy(game);

    return(EXIT_SUCCESS);

} 