sudo apt-get install libsdl2-dev && sudo apt-get install libsdl2-image-dev

gcc -Wall -I include src/main.c src/game.c -o myGame -lSDL2 -lSDL_Image
gcc -Wall -I include src/main.c src/game.c -o myGame -lSDL2