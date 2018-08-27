
#include "Game.h"

#include <iostream>
#include <cstring>

#include "AutoPlayerAlgorithm.h"
#include "FilePlayerAlgorithm.h"

PlayerAlgorithm *algorithmFactory(const char *name, int player);

int main(int argc, char **argv){
	if (argc != 2){
		printf("invalid arguments\n");
		return -1;
	}

	char *arg = argv[1];

	PlayerAlgorithm *algorithms[2];
	algorithms[0] = algorithmFactory(strtok(arg,"-"),1);
	strtok(NULL,"-");
	algorithms[1] = algorithmFactory(strtok(NULL,"-"),2);

	Game game = Game(algorithms);
	game.runGame();
	for(int i=0; i<2; i++)
		delete algorithms[i];

	return 0;
}

PlayerAlgorithm *algorithmFactory(const char *name, int player){
	if(strcmp(name,"auto") == 0){
		return new AutoPlayerAlgorithm(player);
	}
	else{
		return new FilePlayerAlgorithm(player);
	}
}



