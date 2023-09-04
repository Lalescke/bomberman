#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <unistd.h>
#include <netdb.h>
#include <arpa/inet.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <pthread.h>


void *function(void *arg)
{
    int socket = *(int*)arg;
    char msg[] = "ok";
    char success[7];
    //SendServer Mclient;
    send(socket, msg, strlen(msg)+1, 0);
    recv(socket, success, sizeof(success), 0);
    printf("%s\n",success);
    close(socket);
    free(arg);
    pthread_exit(NULL);
}

int main(int argc, char *argv[])
{
    int socketServer = socket(AF_INET, SOCK_STREAM, 0);
    struct sockaddr_in addrServer;
    addrServer.sin_addr.s_addr = inet_addr("127.0.0.1");
    addrServer.sin_family = AF_INET;
    if(argc < 2){
      printf("arret du programme, veuillez ajouter le port !!\n");
      exit(EXIT_SUCCESS);
    }else {
      addrServer.sin_port = htons(atoi(argv[1]));
    }

    bind(socketServer, (const struct sockaddr *)&addrServer, sizeof(addrServer));
    printf("bind : %d\n", socketServer);

    listen(socketServer, 5);
    printf("listen\n");

    pthread_t threads[4];
    for (int i = 0; i < 4; i++)
    {
        struct sockaddr_in addrClient;
        socklen_t csize = sizeof(addrClient);
        int socketClient = accept(socketServer, (struct sockaddr *)&addrClient, &csize);
        printf("accept\n");

        printf("client: %d\n", socketClient);

        int *arg = malloc(sizeof(int));
        *arg = socketClient;
        pthread_create(&threads[i], NULL, function, arg);
    }
    
    for (int i = 0; i < 4; i++)
    {
        pthread_join(threads[i], NULL);
    }

    close(socketServer);
    printf("close\n");
    return 0;
    
}
