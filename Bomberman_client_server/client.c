#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <unistd.h>
#include <netdb.h>
#include <arpa/inet.h>
#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <SDL2/SDL.h>
#include <stdlib.h>
#include <stdio.h>
#include <SDL2/SDL_ttf.h>


int main(int argc, char *argv[])
{
    int socketClient = socket(AF_INET, SOCK_STREAM, 0);
    struct sockaddr_in addrClient;
    addrClient.sin_family = AF_INET;

    SDL_Window *window = NULL;
    SDL_Renderer *renderer = NULL;
    SDL_Event e;
    SDL_Surface *text = NULL;
    SDL_Color colors = {255, 255, 255};
    TTF_Font* font = NULL;
    SDL_Texture* texture = NULL;
    SDL_Rect dest_coo;

     if(argc < 3){
      printf("arret du programme, veuillez renseigner le port et l'IP du serveu\
r");
      exit(EXIT_SUCCESS);
    }else {
      addrClient.sin_addr.s_addr = inet_addr(argv[1]);
      addrClient.sin_port = htons(atoi(argv[2]));
    }

    
    if(SDL_Init(SDL_INIT_VIDEO) != 0){
      fprintf(stderr, "Erreur SDL_Init : %s", SDL_GetError());
      return EXIT_FAILURE;
    }
    window = SDL_CreateWindow("Test_Game", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, 640, 480, SDL_WINDOW_SHOWN);
    if(window == NULL){
      fprintf(stderr, "Erreur SDL_CreateWindow : %s", SDL_GetError());
      return EXIT_FAILURE;
    }
    renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);
    if(NULL == renderer){
      SDL_DestroyWindow(window);
      fprintf(stderr, "Erreur SDL_CreateRenderer : %s", SDL_GetError());
      SDL_Quit();
      return EXIT_FAILURE;
    }
    if (TTF_Init() < 0){
      fprintf(stderr, "Erreur SDL_TTF : %s", TTF_GetError());
      return EXIT_FAILURE;
    }
    font = TTF_OpenFont("arial.ttf", 18);
    if (font == NULL){
      fprintf(stderr, "Erreur SDL_TTF : %s", TTF_GetError());
    }
    text = TTF_RenderText_Blended(font, "Hello, World",  colors);
    if (text == NULL)
    {
      fprintf(stderr, "Erreur lors de l'affichage du texte : %s\n", TTF_GetError());
      exit(1);
    }
    texture = SDL_CreateTextureFromSurface(renderer , text);
    SDL_FreeSurface(text);
    TTF_CloseFont(font);

    connect(socketClient, (const struct sockaddr *)&addrClient, sizeof(addrClient));
    printf("connecté\n");
    char msg[2];
    char success[7] = "SUCCESS";
    recv(socketClient, msg, 2, 0);
    printf("%s\n", msg);
    send(socketClient, &success, sizeof(success), 0);

    while(1){
    if(SDL_PollEvent(&e)){
      if(e.type == SDL_QUIT) {
        break;
      }
    }
    dest_coo.x = 200;
    dest_coo.y = 150;
    dest_coo.w = text->w;
    dest_coo.h = text->h;
    SDL_RenderCopy(renderer, texture, NULL, &dest_coo);
    SDL_RenderPresent(renderer);

    }

    close(socketClient);

    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    SDL_Quit();
    TTF_Quit();
    return EXIT_SUCCESS;
}
