#include "game.h"

/* Fct qui initialise SDL + Jeu
@Return */

stGame* game_init(char* ip, int port){

  //SDL_Event e;
    stGame * game = NULL;
    game = malloc(sizeof(stGame));
    game->screenSize.x = 1280;
    game->screenSize.y = 720;
    game->pWindow = NULL;
    game->pRenderer = NULL;
    game->pTextPlayer = NULL;
    game->playerPositionRect.x = game->screenSize.x / 2;
    game->playerPositionRect.y = game->screenSize.y / 2;
    game->playerPositionRect.w = 75;
    game->playerPositionRect.h = 105;

    int socketClient = socket(AF_INET, SOCK_STREAM, 0);
    struct sockaddr_in addrClient;
    addrClient.sin_addr.s_addr = inet_addr(ip);
    addrClient.sin_family = AF_INET;
    addrClient.sin_port = htons(port);

    //init SDL

    if (SDL_Init(SDL_INIT_VIDEO) !=0) {
        fprintf(stderr, "no %s\n", SDL_GetError());
        game_destroy(game);
        return NULL;
    }

    // fenetre ecran
    game->pWindow = SDL_CreateWindow("Bomberman",
        SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED,
        game->screenSize.x, game->screenSize.y, SDL_WINDOW_SHOWN);

    if (game->pWindow) {
        // renderer
        game->pRenderer = SDL_CreateRenderer(game->pWindow, -1, SDL_RENDERER_ACCELERATED);
        if (!game->pRenderer){
            fprintf(stderr, "error de renderer %s\n", SDL_GetError());
            game_destroy(game);
             return NULL;
        }

    } else {
        fprintf(stderr, "error create fenetre SDL %s\n", SDL_GetError());
        game_destroy(game);
        return NULL;
    }

    // Chargement texture joueur
    SDL_Surface* surfaceBomberman = IMG_Load("./ressources/perso_front.png");
    if (!surfaceBomberman) {
        fprintf(stderr, "error de charger image perso pkmn %s\n", IMG_GetError());
        game_destroy(game);
        return NULL;
    } else {
        game->pTextPlayer = SDL_CreateTextureFromSurface(game->pRenderer,surfaceBomberman);
        if (!game->pTextPlayer){
            fprintf(stderr, "error traitement texture joueur %s\n", SDL_GetError());
            game_destroy(game);
            return NULL;
        }
        SDL_FreeSurface(surfaceBomberman);
    }

    // Chargement texture bomb
    SDL_Surface* surfaceBomb = IMG_Load("./ressources/voltorbomb.png");
    if (!surfaceBomb) {
        fprintf(stderr, "error de charger image bomb pkmn %s\n", IMG_GetError());
        game_destroy(game);
        return NULL;
    } else {
        game->pTextBomb = SDL_CreateTextureFromSurface(game->pRenderer,surfaceBomb);
        if (!game->pTextBomb){
            fprintf(stderr, "error traitement texture bomb %s\n", SDL_GetError());
            game_destroy(game);
            return NULL;
        }
        SDL_FreeSurface(surfaceBomb);
    }

    connect(socketClient, (const struct sockaddr *)&addrClient, sizeof(addrClient));
    printf("connecté\n");
    char msg[2];
    char success[7] = "SUCCESS";
    recv(socketClient, msg, 2, 0);
    printf("%s\n", msg);
    send(socketClient, &success, sizeof(success), 0);
    
    close(socketClient);

    return game;

}

/* Détruit jeu puis SDL*/

void game_destroy(stGame* game){

    if (game) {

        if (game->pTextBomb) {
            SDL_DestroyTexture(game->pTextBomb);
        }

        if (game->pTextPlayer) {
            SDL_DestroyTexture(game->pTextPlayer);
        }

        if (game->pRenderer) {
            SDL_DestroyRenderer(game->pRenderer);
        }

        if(game->pWindow) {
            SDL_DestroyWindow(game->pWindow);
        }

        SDL_Quit();

        free(game);

    }

}

//Affichage jeu @param jeu

void game_draw(stGame* game) {
    //Nettoyer ecran
    SDL_SetRenderDrawColor(game->pRenderer, 0, 0, 0, 255);
    SDL_RenderClear(game->pRenderer);

    //afficher joueur
    SDL_RenderCopy(game->pRenderer, game->pTextPlayer, NULL, &game->playerPositionRect);

    //afficher bomb
    SDL_RenderCopy(game->pRenderer, game->pTextBomb, NULL, &game->bombPositionRect);

    //présenter le rendu
    SDL_RenderPresent(game->pRenderer);
}

//gestions des evenements @param game @return -1 pour quitter
int game_event(stGame* game) {
    int result = 0;

    SDL_Event e;
    //recup evenement appui touche
    if (SDL_PollEvent(&e)){
        if (e.type == SDL_QUIT){
            // quitter appli
            result = -1;
        } else if (e.type == SDL_KEYDOWN) 

            //gerer touches clavier
            switch(e.key.keysym.sym) {
                case SDLK_ESCAPE :
                    // quitter appli
                    result = -1;
                    break;

                case SDLK_UP:
                case SDLK_DOWN:
                case SDLK_LEFT:
                case SDLK_RIGHT:
                    // se déplacer
                    game_movePlayer(game, e.key.keysym.sym);
                    break;

                case SDLK_SPACE :
                    //poser une bombe
                    game_placeBomb(game, e.key.keysym.sym);
                    fprintf(stderr, "Une bombe a été placée\n");
                    break;


                default :
                    fprintf(stderr, "Touche inconnue %d\n", e.key.keysym.sym);
                    break;
            }
        
    }   

    return result;
}


//deplace joueur
void game_movePlayer(stGame* game, SDL_Keycode direction) {

    if (direction == SDLK_UP) {
        if(game->playerPositionRect.y > 0) {
            game->playerPositionRect.y -= 10;
        }
    } else if (direction == SDLK_DOWN) {
        if (game->playerPositionRect.y < (game->screenSize.y - game->playerPositionRect.h)) {
            game->playerPositionRect.y += 10;
        } 
    } else if (direction == SDLK_LEFT) {
        if (game->playerPositionRect.x > 0 ) {
            game->playerPositionRect.x -= 10;
        } 
    } else if (direction == SDLK_RIGHT) {
        if (game->playerPositionRect.x < (game->screenSize.x - game->playerPositionRect.w)) {
            game->playerPositionRect.x += 10;
        } 
    } else {
        fprintf(stderr, "Direction inconnue \n");
        }
    // close(socketClient);
} 

//placer bombe
void game_placeBomb(stGame* game, SDL_Keycode poseBomb) {
    if (poseBomb == SDLK_SPACE) {
        //game->bombPositionRect.x = (game->playerPositionRect.x + game->playerPositionRect.h) / 2; mettre la bombe pile au milieu du perso = marche pas = piste
        //game->bombPositionRect.y = (game->playerPositionRect.y + game->playerPositionRect.w) / 2; mettre la bombe pile au milieu du perso = marche pas = piste
        game->bombPositionRect.x = game->playerPositionRect.x;
        game->bombPositionRect.y = game->playerPositionRect.y;
        game->bombPositionRect.w = 60;
        game->bombPositionRect.h = 60;
    } else {
        fprintf(stderr, "erreur pose bombe\n");
        }
}
