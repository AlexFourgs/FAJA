#include <iostream>
#include <SDL/SDL.h>

using namespace std;


int main(){
	
	int i=0;
	SDL_JoystickEventState(SDL_ENABLE);
	
printf("Programme de configuration du Joystick\n\n *******************\n\n\n"); 
    if(SDL_Init(SDL_INIT_JOYSTICK) < 0) // initialise juste le joystick (pas besoin d'une fenêtre pour nos tests)
        return EXIT_FAILURE;


        printf("Il y a %d joysticks. \n",SDL_NumJoysticks()); // on écrit combien il y a de joysticks
       
       for(i=0;i<SDL_NumJoysticks();i++)    {                                    // tant qu'il y a un joystick non-traité
            printf("Nom du joystick numero %d : %s",i,SDL_JoystickName(i)); // on écrit les noms des joysticks
    }
    
	if(i>1 || i<0){
printf("Merci de brancher que un Joystick.\n");
return 1;

}

    SDL_Joystick *pJoystick; // attention, c'est bien un pointeur !
	pJoystick = SDL_JoystickOpen(0); // prend en paramètre le numéro du joystick, ici 0
    printf("Le Joystick possède :\nBoutons : %d \nAxes : %d\n Chapeaux :%d \nTrackballs %d\n"
    ,SDL_JoystickNumButtons(pJoystick),SDL_JoystickNumAxes(pJoystick),SDL_JoystickNumHats(pJoystick)
    ,SDL_JoystickNumBalls(pJoystick));


    SDL_Quit();

  return 0;
}
