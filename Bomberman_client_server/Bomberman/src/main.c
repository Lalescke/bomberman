#include <SDL2/SDL_ttf.h>
#include <stdlib.h>
#include <stdio.h>
#include "game.h"

int main(int argc, char *argv[]){ //argv = tableau

  char *ip = "";
  int port = 0;

  if(argc < 3){
    printf("arret du programme, veuillez renseigner le port et l'IP du serveur");
    exit(EXIT_SUCCESS);
  }else {
    ip = argv[1];
    port = atoi(argv[2]);
  }
  stGame* game = game_init(ip, port);
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
